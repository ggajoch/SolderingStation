from PyQt4 import QtCore, QtGui
import socket
import logging


class SocketConnection(QtCore.QThread):
    line_received_signal = QtCore.pyqtSignal(str)

    def __init__(self, parent):
        QtCore.QThread.__init__(self)
        self.log = logging.getLogger('tcp_socket')
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.line_received_signal.connect(parent.line_parse)

    def start(self):
        self.log.info("Connecting: ('127.0.0.1', 12345)")
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        try:
            self.socket.connect(('127.0.0.1', 12345))
        except socket.error:
            self.log.error("Connection FAIL")
            return False

        self.log.info("Connection OK")
        self.log.debug("Starting socket thread")
        QtCore.QThread.start(self)

    def run(self):
        while True:
            try:
                s = self.socket.recv(1000)
                s = s.strip()
                self.line_received_signal.emit(s)
            except socket.error as e:
                self.log.info("Socket error {}".format(e))
                break

    def send(self, text):
        self.log.debug("Sending {}".format(text))
        print self.socket.send(str(text))

    def active(self):
        return self.isRunning()

    def stop(self):
        if not self.active():
            self.log.info("Connection already closed")
            return

        self.log.debug("Closing socket")
        self.socket.shutdown(socket.SHUT_WR)
        self.socket.close()

        self.log.debug("Waiting for thread to complete")
        self.wait()
        self.log.debug("Disconnected")
