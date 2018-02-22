from PyQt4 import QtGui
from ConnectToDevice import ConnectionThread
import time
import logging


class ConnectionManager(object):
    def __init__(self, main):
        self.log = logging.getLogger('ConnectionManager')
        self.main = main
        self.device = None
        self.connection_thread = ConnectionThread(None)

        self._ui_device_disconnected()

    def parse_config(self, string):
            self.log.info("Parsing config [{}]".format(str(string)))
            arr = str(string).split()

            self.main.ui.setpointSpinBox.setValue(int(arr[1]))
            self.main.ui.KpSpinBox.setValue(float(arr[2]))
            self.main.ui.KiSpinBox.setValue(float(arr[3]))
            self.main.ui.KdSpinBox.setValue(float(arr[4]))
            self.main.ui.maxPowerSpinBox.setValue(float(arr[5]))
            self.main.ui.offsetSpinBox.setValue(float(arr[6]))
            self.main.ui.gainSpinBox.setValue(float(arr[7]))
            self.main.ui.brightnessSpinBox.setValue(float(arr[8]))
            self.main.ui.contrastSpinBox.setValue(float(arr[9]))
            self.main.ui.sleepTemperatureSpinBox.setValue(int(arr[10]))
            self.main.ui.standTemperatureSpinBox.setValue(int(arr[11]))
            self.main.ui.sleepTimeoutSpinBox.setValue(int(arr[12]))
            self.main.ui.offTimeoutSpinBox.setValue(int(arr[13]))

            time.sleep(0.2)
            self.main.ui.KpSpinBox.valueChanged.emit(self.main.ui.KpSpinBox.value())

            time.sleep(0.2)
            self.main.ui.offsetSpinBox.valueChanged.emit(self.main.ui.offsetSpinBox.value())

            time.sleep(0.2)
            self.main.ui.brightnessSpinBox.valueChanged.emit(self.main.ui.brightnessSpinBox.value())

            time.sleep(0.2)
            self.main.ui.sleepTemperatureSpinBox.valueChanged.emit(self.main.ui.sleepTemperatureSpinBox.value())

            time.sleep(0.2)
            self.main.ui.sleepTimeoutSpinBox.valueChanged.emit(self.main.ui.sleepTimeoutSpinBox.value())

            self.log.info("Parsing config successful")

    def connection_update(self, status, msg):
        self.log.info("Connection update: {} {}".format(status, msg))
        if status == 0:
            try:
                self.parse_config(msg)
            except Exception as e:
                self.log.error("Exception {} during parsing config [{}]".format(e, msg))
                self.connection_thread.at_failed()
                self.connection_update(2, "Incorrect response from device! [" + msg + "]")
                return

            self.main.change_status(self.main.Status.Connected)
            self.device.line_received_signal.connect(self.main.line_parse)

            self.device.finished.connect(self.device_disconnected)

            for enable in self.main.disabled_when_disconnected:
                enable.setDisabled(False)

            for disable in self.main.disabled_when_connected:
                disable.setDisabled(True)

            self.log.info("Device connected")

        elif status == 1:
            self.log.info("Waiting for device")
            self.main.change_status(self.main.Status.WaitingForDevice)

        elif status == 2:
            self.log.error("Connection failed {}".format(msg))
            self.device = None
            self.main.change_status(self.main.Status.Disconnected)
            QtGui.QMessageBox.warning(self.main, "Connection failed", msg)
            self._ui_device_disconnected()

    def connect(self, device):
        assert(self.device is None)

        for enable in self.main.disabled_when_connected:
            enable.setDisabled(True)

        self.device = device

        self.connection_thread = ConnectionThread(device)
        self.connection_thread.connection_update_signal.connect(self.connection_update)
        self.connection_thread.start()

    def device_disconnected(self):
        self.device.line_received_signal.disconnect(self.main.line_parse)
        self.device = None

        time.sleep(0.1)

        self._ui_device_disconnected()

    def _ui_device_disconnected(self):
        self.main.change_status(self.main.Status.Disconnected)

        for enable in self.main.disabled_when_connected:
            enable.setDisabled(False)

        for disable in self.main.disabled_when_disconnected:
            disable.setDisabled(True)

    def disconnect(self):
        assert (self.device is not None)
        self.device.stop()
