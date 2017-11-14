import sys
from PyQt4 import QtCore, QtGui
from main_window import Ui_MainWindow
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

        ui = menu.ctrl[0]

        ui.timeScaleLabel = QtGui.QLabel(menu.widgetGroups[0])
        ui.timeScaleLabel.setObjectName("timeScaleLabel")
        ui.gridLayout.addWidget(ui.timeScaleLabel, 8, 0, 1, 2)
        ui.timeScaleLabel.setText("Last seconds:")

        ui.timeScale = QtGui.QSpinBox(menu.widgetGroups[0])
        ui.timeScale.setEnabled(True)
        ui.timeScale.setMinimum(1)
        ui.timeScale.setMaximum(1e8)
        ui.timeScale.setSingleStep(1)
        ui.timeScale.setSuffix(" s")
        ui.timeScale.setProperty("value", 1e8)
        ui.timeScale.setObjectName("timeScale")
        ui.gridLayout.addWidget(ui.timeScale, 8, 2, 1, 2)

        QtCore.QObject.connect(ui.timeScale, QtCore.SIGNAL("valueChanged(int)"), self.set_history)
        QtCore.QObject.connect(ui.autoRadio, QtCore.SIGNAL("clicked()"), self.update_graph)

        self.plot_update_timer = QtCore.QTimer(self)
        self.connect(self.plot_update_timer, QtCore.SIGNAL("timeout()"), self.update_graph)
        self.plot_update_timer.start(50)

        self.changed = False
        self.start_time = time.time()
        self.x = []
        self.y = []

        self.clear()

        self.serial_thread = self.SocketThread()
        self.connect_socket()

    def set_history(self, seconds):
        self.history_seconds = seconds

    def clear(self):
        self.start_time = time.time()
        self.x = []
        self.y = []
        self.curve.setData(self.x, self.y)

    def add_point(self, y):
        now = time.time() - self.start_time

        now = round(now, 2)
        self.x.append(now)
        self.y.append(y)

        self.changed = True

    def update_graph(self):
        now = time.time() - self.start_time

        remove_until = 0
        for index, value in enumerate(self.x):
            if now - value < self.history_seconds:
                remove_until = index
                break

        # if X is autorange
        if self.curve.getViewBox().state['autoRange'][0]:
            if self.changed:
                self.changed = False
                x = self.x[remove_until:]
                y = self.y[remove_until:]
                self.curve.setData(x, y)
        else:
            # X manual - set all acquired data
            self.curve.setData(self.x, self.y)

    class SocketThread(QtCore.QThread):
        def __init__(self):
            QtCore.QThread.__init__(self)
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

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