import sys
from PyQt4 import QtCore, QtGui
from main_window import Ui_MainWindow
import pyqtgraph as pg
import time
import logging

from ConnectionManager import ConnectionManager
from connection_socket import SocketConnection
from connection_serial import SerialConnection


class StartQT4(QtGui.QMainWindow):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)

        self.log = logging.getLogger('main')
        self.log.info("Starting main window")

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.ui.setpointSpinBox.valueChanged.connect(self.update_setpoint)

        self.ui.offsetSpinBox.valueChanged.connect(self.update_tip)
        self.ui.gainSpinBox.valueChanged.connect(self.update_tip)
        self.ui.maxTemperatureSpinBox.valueChanged.connect(self.update_tip)

        self.ui.KpSpinBox.valueChanged.connect(self.update_pid)
        self.ui.KiSpinBox.valueChanged.connect(self.update_pid)
        self.ui.KdSpinBox.valueChanged.connect(self.update_pid)
        self.ui.maxPowerSpinBox.valueChanged.connect(self.update_pid)

        self.ui.brightnessSpinBox.valueChanged.connect(self.update_display)
        self.ui.contrastSpinBox.valueChanged.connect(self.update_display)

        self.ui.sleepTemperatureSpinBox.valueChanged.connect(self.update_standby_temperatures)
        self.ui.standTemperatureSpinBox.valueChanged.connect(self.update_standby_temperatures)

        self.ui.sleepTimeoutSpinBox.valueChanged.connect(self.update_timeouts)
        self.ui.offTimeoutSpinBox.valueChanged.connect(self.update_timeouts)

        self.ui.connectSerialButton.pressed.connect(self.connect_serial)
        self.ui.connectSimulatorButton.pressed.connect(self.connect_simulator)
        self.ui.disconnectButton.pressed.connect(self.disconnect)

        self.disabled_when_disconnected = (
            self.ui.setpointSpinBox,
            self.ui.offsetSpinBox, self.ui.gainSpinBox, self.ui.maxTemperatureSpinBox,
            self.ui.KpSpinBox, self.ui.KiSpinBox, self.ui.KdSpinBox, self.ui.maxPowerSpinBox,
            self.ui.brightnessSpinBox, self.ui.contrastSpinBox,
            self.ui.sleepTemperatureSpinBox,self.ui.standTemperatureSpinBox,
            self.ui.sleepTimeoutSpinBox, self.ui.offTimeoutSpinBox,
            self.ui.disconnectButton
        )
        self.disabled_when_connected = (
            self.ui.connectSerialButton,
            self.ui.connectSimulatorButton
        )

        self.setup_graph_menu()
        self.setup_graph_curves()

        self.plot_update_timer = QtCore.QTimer(self)
        self.plot_update_timer.timeout.connect(self.update_graph)
        self.plot_update_timer.start(20)

        self.status_update_timer = QtCore.QTimer(self)
        self.status_update_timer.timeout.connect(self.update_status)
        self.status_update_timer.setSingleShot(True)
        self.status_update_timer.start(0)

        self.status_label = QtGui.QLabel("")
        self.statusBar().addWidget(self.status_label)
        self.statusBar().addWidget(QtGui.QLabel(""), 1)
        self.mode_label = QtGui.QLabel("")
        self.statusBar().addWidget(self.mode_label, 0)

        self.connection_manager = ConnectionManager(self)

    class Status:
        Disconnected = ["Disconnected", "red"]
        WaitingForDevice = ["Waiting For Device", "blue"]
        Connected = ["Connected", "green"]

    class Mode:
        NoData = ["No data", "red"]
        InStand = ["In stand", "green"]
        Sleeping = ["Sleeping", "green"]
        Heating = ["Heating", "blue"]

    def change_status(self, status):
        self.status_label.setText(status[0])
        self.status_label.setStyleSheet(' QLabel {color: %s}' % status[1])

    def change_mode(self, mode):
        self.mode_label.setText(mode[0])
        self.mode_label.setStyleSheet(' QLabel {color: %s}' % mode[1])

    def update_status(self):
        # if timer expired that means there is no data from device
        if not self.status_update_timer.isActive():
            self.change_mode(self.Mode.NoData)

        self.status_update_timer.stop()
        self.status_update_timer.start(1000)

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
        self.update_status()

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
        if string.startswith("DISP"):
            if string.find("SLEEP") > -1:
                self.change_mode(self.Mode.Sleeping)
            elif string.find("STAND") > -1:
                self.change_mode(self.Mode.InStand)
            else:
                self.change_mode(self.Mode.Heating)

    def connect_simulator(self):
        self.log.info("Connect to Simulator")
        self.connection_manager.connect(SocketConnection())

    def connect_serial(self):
        self.log.info("Connect to serial " + self.ui.deviceTextEdit.text())
        self.connection_manager.connect(SerialConnection(str(self.ui.deviceTextEdit.text())))

    def disconnect(self):
        self.connection_manager.disconnect()

    def send(self, text):
        assert self.connection_manager.device is not None
        self.log.debug("Command {}".format(text))
        self.connection_manager.device.send(text + "\n")

    def update_setpoint(self, _):
        self.send("temp {}".format(self.ui.setpointSpinBox.value()))

    def update_tip(self, _):
        self.send("tip {} {} {}".format(self.ui.offsetSpinBox.value(),
                                        self.ui.gainSpinBox.value(),
                                        self.ui.maxTemperatureSpinBox.value()))

    def update_pid(self, _):
        self.send("pid {} {} {} {}".format(self.ui.KpSpinBox.value(),
                                           self.ui.KiSpinBox.value(),
                                           self.ui.KdSpinBox.value(),
                                           self.ui.maxPowerSpinBox.value()))

    def update_display(self, _):
        self.send("disp {} {}".format(self.ui.brightnessSpinBox.value(),
                                      self.ui.contrastSpinBox.value()))

    def update_standby_temperatures(self, _):
        self.send("stdby {} {}".format(self.ui.sleepTemperatureSpinBox.value(),
                                       self.ui.standTemperatureSpinBox.value()))

    def update_timeouts(self, _):
        self.send("timeouts {} {}".format(self.ui.sleepTimeoutSpinBox.value(),
                                          self.ui.offTimeoutSpinBox.value()))


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)

    app = QtGui.QApplication(sys.argv)
    myapp = StartQT4()
    myapp.show()
    sys.exit(app.exec_())
