import sys
from PyQt4 import QtCore, QtGui
from main_window import Ui_MainWindow
import pyqtgraph as pg
import socket
import time


class StartQT4(QtGui.QMainWindow):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)

        self.history_seconds = 1e10

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        QtCore.QObject.connect(self.ui.sendButton, QtCore.SIGNAL("clicked()"), self.send)
        QtCore.QObject.connect(self.ui.actionSimulator, QtCore.SIGNAL("triggered()"), self.connect_socket)
        QtCore.QObject.connect(self.ui.actionDisconnect, QtCore.SIGNAL("triggered()"), self.disconnect_socket)

        self.curve = self.ui.widget.getPlotItem().plot()

        menu = self.ui.widget.getPlotItem().getViewBox().menu
        clear = QtGui.QAction("Clear", menu)
        QtCore.QObject.connect(clear, QtCore.SIGNAL("triggered()"), self.clear)
        menu.addAction(clear)

        x = menu.ctrl
        x = x[0]


        ui = menu.ctrl[0]

        ui.autoPercentSpin.valueChanged.disconnect()

        ui.autoPercentSpin.setMaximum(10000)
        ui.autoPercentSpin.setSuffix(" s")

        QtCore.QObject.connect(ui.autoPercentSpin, QtCore.SIGNAL("valueChanged(int)"), self.set_history)
        ui.autoPercentSpin.setValue(10000)

        self.clear()
        self.connect_socket()

    def set_history(self, seconds):
        self.history_seconds = seconds

    def clear(self):
        self.start_time = time.time()
        self.x = []
        self.y = []
        self.curve.setData(self.x, self.y)

    def getMenu(self):
        """
        Create the menu
        """
        self.menu = None
        if self.menu is None:
            self.menu = QtGui.QMenu("Clear")
            self.viewAll = QtGui.QAction("Dupa", self.menu)
            self.menu.addAction(self.viewAll)
        return self.menu

    def add_point(self, y):
        now = time.time() - self.start_time

        remove_until = 0
        for index, value in enumerate(self.x):
            if now - value < self.history_seconds:
                remove_until = index
                break

        self.x[:remove_until] = []
        self.y[:remove_until] = []

        now = round(now, 2)
        self.x.append(now)
        self.y.append(y)
        self.curve.setData(self.x, self.y)

    class UpdatePlotThread(QtCore.QThread):
        def __init__(self, add_point):
            QtCore.QThread.__init__(self)
            self.add_point = add_point

        def run(self):
            print "Connectting to TCP"
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect(('127.0.0.1', 12345))

            print "Connected"
            while True:
                s = self.socket.recv(1000)
                s = s.strip()
                if s.startswith("TICK"):
                    table = s.split(' ')
                    tip = table[1]
                    self.emit(QtCore.SIGNAL('add_point(float)'), float(tip))


        def send(self, text):
            print self.socket.send(str(text))

        def close(self):
            self.socket.shutdown(socket.SHUT_WR)
            self.socket.close()

    def connect_socket(self):
        print "Connect"
        self.serial_thread = self.UpdatePlotThread(self.add_point)
        self.connect(self.serial_thread, QtCore.SIGNAL("add_point(float)"), self.add_point)
        self.serial_thread.start()

    def disconnect_socket(self):
        print "Disconnect"
        self.serial_thread.terminate()
        self.serial_thread.wait()
        self.serial_thread.close()

    def send(self):
        text = self.ui.sendText.text()
        print "Sending: ", text
        self.serial_thread.send(text + "\n")


if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    myapp = StartQT4()
    myapp.show()
    sys.exit(app.exec_())