from PyQt4 import QtCore, QtGui
from serial import Serial
import logging


class SerialConnection(QtCore.QThread):
    line_received_signal = QtCore.pyqtSignal(str)

    def __init__(self, device):
        QtCore.QThread.__init__(self)
        self.log = logging.getLogger('serial')
        self.device = device

    def start(self):
        self.log.info("Connecting: {}".format(self.device))

        try:
            self.serial = Serial(self.device, 115200, timeout=1.0)
        except Exception as e:
            self.log.error("Connection FAIL: {}".format(e))
            raise

        self.log.info("Connection OK")
        self.log.debug("Starting serial thread")
        QtCore.QThread.start(self)

    def run(self):
        while True:
            try:
                s = self.serial.read_until(b'\n')
                s = s.decode("utf-8")
                s = s.strip()

                self.line_received_signal.emit(s)
            except Exception as e:
                self.log.info("Serial error {}".format(e))
                break

    def send(self, text):
        self.log.debug("Sending {}".format(text))
        self.serial.write(str(text).encode())

    def active(self):
        return self.isRunning()

    def stop(self):
        if not self.active():
            self.log.info("Connection already closed")
            return

        self.log.debug("Closing socket")
        self.serial.close()

        self.log.debug("Waiting for thread to complete")
        self.wait()
        self.log.debug("Disconnected")
