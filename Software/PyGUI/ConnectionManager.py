from PyQt4 import QtGui
from ConnectToDevice import ConnectionThread
import time


class ConnectionManager(object):
    def __init__(self, main):
        self.main = main
        self.device = None
        self.connection_thread = ConnectionThread(None)

        self.main.ui.actionDisconnect.setDisabled(True)
        self.main.ui.sendButton.setDisabled(True)
        self.main.change_status(self.main.Status.Disconnected)

    def connection_update(self, status, error_msg):
        if status == 0:
            self.main.change_status(self.main.Status.Connected)
            self.device.line_received_signal.connect(self.main.line_parse)

            self.main.ui.actionSimulator.setDisabled(True)
            self.main.ui.actionDisconnect.setDisabled(False)
            self.main.ui.sendButton.setDisabled(False)

            self.device.finished.connect(self.device_disconnected)
        else:
            if status == 1:
                self.main.change_status(self.main.Status.WaitingForDevice)
            if status == 2:
                self.device = None
                self.main.change_status(self.main.Status.Disconnected)
                QtGui.QMessageBox.warning(self.main, "Connection failed", error_msg)

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
        self.main.change_status(self.main.Status.Disconnected)
        self.main.ui.actionSimulator.setDisabled(False)
        self.main.ui.actionDisconnect.setDisabled(True)
        self.main.ui.sendButton.setDisabled(True)

    def disconnect(self):
        assert (self.device is not None)
        self.device.stop()
