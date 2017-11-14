import sys
from PyQt4 import QtCore, QtGui
from main_window import Ui_MainWindow
import socket
import pyqtgraph as pg
import time


class StartQT4(QtGui.QMainWindow):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)

        self.history_seconds = 1000

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.ui.sendButton.clicked.connect(self.send)
        self.ui.actionSimulator.triggered.connect(self.connect_socket)
        self.ui.actionDisconnect.triggered.connect(self.disconnect_socket)

        self.setup_graph_menu()
        self.setup_graph_curves()

        self.plot_update_timer = QtCore.QTimer(self)
        self.plot_update_timer.timeout.connect(self.update_graph)
        self.plot_update_timer.start(50)

        self.serial_thread = self.SocketThread()
        self.connect_socket()

    def setup_graph_menu(self):
        menu = self.ui.widget.getPlotItem().getViewBox().menu
        clear = QtGui.QAction("Clear", menu)
        clear.triggered.connect(self.clear)
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
        ui.timeScale.setObjectName("timeScale")
        ui.gridLayout.addWidget(ui.timeScale, 8, 2, 1, 2)

        ui.timeScale.valueChanged.connect(self.set_history)
        ui.timeScale.setValue(1000)

    def setup_graph_curves(self):
        # actual temperature
        self.curve_actual = self.ui.widget.getPlotItem().plot()
        self.curve_actual.setPen(pen=pg.mkPen(color='#ffffff', width=2))

        # setpoint temperature
        self.curve_setpoint = pg.PlotCurveItem(pen=pg.mkPen(color='#0fff00', width=1))
        self.ui.widget.getPlotItem().addItem(self.curve_setpoint)

        # second axis - power
        p = self.ui.widget.plotItem
        p.showAxis('right')

        p2 = pg.ViewBox()
        p.scene().addItem(p2)
        p.getAxis('right').linkToView(p2)
        p2.setXLink(p)

        self.curve_power = pg.PlotCurveItem(pen=pg.mkPen(color='#ff0000', width=1))
        p2.addItem(self.curve_power)

        def updateViews():
            p2.setGeometry(p.getViewBox().sceneBoundingRect())
            p2.linkedViewChanged(p.getViewBox(), p2.XAxis)

        p.getViewBox().sigResized.connect(updateViews)

        p2.setRange(yRange=(0, 100))

        # clear and reset values
        self.clear()

    def set_history(self, seconds):
        self.history_seconds = seconds

    def clear(self):
        self.start_time = time.time()
        self.x = []
        self.curves = (
            {"curve": self.curve_actual, "y": []},
            {"curve": self.curve_setpoint, "y": []},
            {"curve": self.curve_power, "y": []},
        )
        self.changed = True
        self.update_graph()

    def add_point(self, list_y):
        now = time.time() - self.start_time
        now = round(now, 2)
        self.x.append(now)

        for index, val in enumerate(list_y):
            self.curves[index]['y'].append(val)

        self.changed = True

    def update_graph(self):
        now = time.time() - self.start_time

        remove_until = 0
        for index, value in enumerate(self.x):
            if now - value < self.history_seconds:
                remove_until = index
                break

        for c in self.curves:
            # if X is autorange
            if self.curve_actual .getViewBox().state['autoRange'][0]:
                if self.changed:
                    x = self.x[remove_until:]
                    y = c['y'][remove_until:]
                    c['curve'].setData(x, y)
            else:
                # X manual - set all acquired data
                c['curve'].setData(self.x, c['y'])

        if self.changed:
            self.changed = False

    class SocketThread(QtCore.QThread):
        point_received_signal = QtCore.pyqtSignal(list)

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
                    tip = float(table[1])
                    setpoint = float(table[2])-0.5
                    pwr = float(table[3])
                    self.point_received_signal.emit([tip, setpoint, pwr])

        def send(self, text):
            print self.socket.send(str(text))

        def close(self):
            self.socket.shutdown(socket.SHUT_WR)
            self.socket.close()

    def connect_socket(self):
        print "Connect"
        self.serial_thread.point_received_signal.connect(self.add_point)
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