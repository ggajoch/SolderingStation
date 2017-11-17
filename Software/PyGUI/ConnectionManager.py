from PyQt4 import QtGui
from ConnectToDevice import ConnectionThread
import time


class ConnectionManager(object):
    def __init__(self, main):
        self.main = main
        self.device = None
        self.connection_thread = ConnectionThread(None)

        self._ui_device_disconnected()


    def parse_config(self, string):
            arr = str(string).split()

            self.main.ui.setpointSpinBox.setValue(int(arr[1]))
            self.main.ui.KpSpinBox.setValue(float(arr[2]))
            self.main.ui.KiSpinBox.setValue(float(arr[3]))
            self.main.ui.KdSpinBox.setValue(float(arr[4]))
            self.main.ui.offsetSpinBox.setValue(float(arr[5]))
            self.main.ui.gainSpinBox.setValue(float(arr[6]))
            self.main.ui.brightnessSpinBox.setValue(float(arr[7]))
            self.main.ui.contrastSpinBox.setValue(float(arr[8]))

            time.sleep(0.5)
            self.main.ui.KpSpinBox.valueChanged.emit(float(arr[2]))

            time.sleep(0.5)
            self.main.ui.offsetSpinBox.valueChanged.emit(float(arr[5]))

            time.sleep(0.5)
            self.main.ui.brightnessSpinBox.valueChanged.emit(float(arr[7]))


    def connection_update(self, status, msg):
        if status == 0:
            self.main.change_status(self.main.Status.Connected)
            self.device.line_received_signal.connect(self.main.line_parse)

            self.device.finished.connect(self.device_disconnected)

            self.parse_config(msg)

            for enable in self.main.disabled_when_disconnected:
                enable.setDisabled(False)

            for disable in self.main.disabled_when_connected:
                disable.setDisabled(True)

        elif status == 1:
            self.main.change_status(self.main.Status.WaitingForDevice)

        elif status == 2:
            self.device = None
            self.main.change_status(self.main.Status.Disconnected)
            QtGui.QMessageBox.warning(self.main, "Connection failed", msg)

    def connect(self, device):
        assert(self.device is None)

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
