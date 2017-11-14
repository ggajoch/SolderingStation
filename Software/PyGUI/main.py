import sys
from PyQt4 import QtCore, QtGui
from main_window import Ui_MainWindow
import pyqtgraph as pg
import time
import logging

from connection_socket import SocketConnection


class StartQT4(QtGui.QMainWindow):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)

        self.log = logging.getLogger('main')
        self.log.info("Starting main window")

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.ui.sendButton.clicked.connect(self.send)
        self.ui.actionSimulator.triggered.connect(self.connect_simulator)
        self.ui.actionDisconnect.triggered.connect(self.disconnect)

        self.setup_graph_menu()
        self.setup_graph_curves()

        self.plot_update_timer = QtCore.QTimer(self)
        self.plot_update_timer.timeout.connect(self.update_graph)
        self.plot_update_timer.start(50)

        self.ui.actionSimulator.setDisabled(False)
        self.ui.actionDisconnect.setDisabled(True)
        self.connection = None

        self.connect_simulator()

    def setup_graph_menu(self):
        self.log.info("Creating graph context menu")

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
        self.log.info("Creating graph curves")
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
        self.log.debug("Setting history to {} seconds".format(seconds))
        self.history_seconds = seconds

    def clear(self):
        self.log.info("Clearing graph")
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

    def line_parse(self, string):
        string = str(string)
        if string.startswith("TICK"):
            table = string.split(' ')
            tip = float(table[1])
            setpoint = float(table[2]) - 0.5
            pwr = float(table[3])
            self.add_point([tip, setpoint, pwr])

    def connect_simulator(self):
        self.log.info("Connect to Simulator")
        self.connect_device(SocketConnection(self))

    def connect_device(self, device):
        if self.connection is None:
            self.connection = device
            self.log.info("Starting connection to {}".format(self.connection))
            self.connection.start()

            self.ui.actionSimulator.setDisabled(True)
            self.ui.actionDisconnect.setDisabled(False)
        else:
            self.log.warning("Cannot connect to {}, connection {} already present".format(device, self.connection))

    def disconnect(self):
        self.log.info("Disconnect")
        if self.connection is not None:
            self.connection.stop()
            self.log.info("Device {} disconnected".format(self.connection))
            self.connection = None

            self.ui.actionSimulator.setDisabled(False)
            self.ui.actionDisconnect.setDisabled(True)
        else:
            self.log.info("No connected device")

    def send(self):
        text = self.ui.sendText.text()
        self.log.debug("Command {}".format(text))
        self.connection.send(text + "\n")


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)

    app = QtGui.QApplication(sys.argv)
    myapp = StartQT4()
    myapp.show()
    sys.exit(app.exec_())
