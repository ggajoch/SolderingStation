from PyQt4 import QtCore, QtGui
import time


class ConnectionThread(QtCore.QThread):
    # int - status. 0 - connected, 1 - waiting for device, 2 - failed
    # str - optional error message
    connection_update_signal = QtCore.pyqtSignal(int, str)

    def __init__(self, device):
        QtCore.QThread.__init__(self)
        self.device = device

    def check_ping(self, line):
        line = str(line).strip()
        if line.find("pong") > -1:
            self.device.send("conf\n")
        if line.find("conf") > -1:
            self.device.line_received_signal.disconnect(self.check_ping)
            self.device.finished.disconnect(self.device_disconnected)

            self.connection_update_signal.emit(0, line)
            self.terminate()

    def device_disconnected(self):
        self.connection_update_signal.emit(2, "Device disconnected")

    def run(self):
        try:
            self.device.start()
            self.connection_update_signal.emit(1, "")

            self.device.line_received_signal.connect(self.check_ping)
            self.device.finished.connect(self.device_disconnected)

            self.got_ping = False
            for _ in xrange(4):
                self.device.send("ping\n")
                time.sleep(3)

            self.device.line_received_signal.disconnect(self.check_ping)
            self.device.finished.disconnect(self.device_disconnected)
            self.connection_update_signal.emit(2, "No response for ping command")
            self.device.stop()
        except IOError:
            self.connection_update_signal.emit(2, "Can't connect")
            self.device.stop()
