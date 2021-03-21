from PyQt5 import QtCore
import time
import logging


class ConnectionThread(QtCore.QThread):
    # int - status. 0 - connected, 1 - waiting for device, 2 - failed
    # str - optional string parameter
    connection_update_signal = QtCore.pyqtSignal(int, str)

    def __init__(self, device):
        QtCore.QThread.__init__(self)
        self.log = logging.getLogger('ConnectionThread')
        self.device = device

    def check_ping(self, line):
        line = str(line).strip()
        self.log.info("Received [{}]".format(line))

        if line == "pong":
            self.log.info("Received pong, asking for configuration")
            self.device.send("conf\n")

        if line.startswith("conf"):
            self.log.info("Received config")
            self.device.line_received_signal.disconnect(self.check_ping)
            self.device.finished.disconnect(self.device_disconnected)

            self.log.info("Stopping thread")

            self.finished.disconnect(self.at_failed)
            self.terminate()
            self.wait()

            self.log.info("Thread terminated, emiting signal")
            self.connection_update_signal.emit(0, line)

    def device_disconnected(self):
        self.connection_update_signal.emit(2, "Device disconnected")

    def run(self):
        try:
            self.finished.connect(self.at_failed)

            self.log.error("Thread started - connecting to device")
            self.device.line_received_signal.connect(self.check_ping)
            self.device.finished.connect(self.device_disconnected)

            self.device.start()
            self.connection_update_signal.emit(1, "")

            for _ in range(4):
                self.device.send("ping\n")
                time.sleep(5)

            self.log.error("No answer for ping command")
            self.device.finished.disconnect(self.device_disconnected)
            self.connection_update_signal.emit(2, "No response from device")
        except IOError:
            self.connection_update_signal.emit(2, "Can't open device port")
        self.log.info("Thread done")

    def at_failed(self):
        self.log.info("at termination - stopping device")
        # self.device.line_received_signal.disconnect(self.check_ping)
        self.device.stop()

