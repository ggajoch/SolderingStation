# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'main_window.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(800, 600)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.centralwidget)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.sendText = QtGui.QLineEdit(self.centralwidget)
        self.sendText.setObjectName(_fromUtf8("sendText"))
        self.horizontalLayout.addWidget(self.sendText)
        self.sendButton = QtGui.QPushButton(self.centralwidget)
        self.sendButton.setDefault(True)
        self.sendButton.setObjectName(_fromUtf8("sendButton"))
        self.horizontalLayout.addWidget(self.sendButton)
        self.verticalLayout_2.addLayout(self.horizontalLayout)
        self.widget = PlotWidget(self.centralwidget)
        self.widget.setObjectName(_fromUtf8("widget"))
        self.verticalLayout_2.addWidget(self.widget)
        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        MainWindow.setStatusBar(self.statusbar)
        self.menubar = QtGui.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 20))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self.menuConnect = QtGui.QMenu(self.menubar)
        self.menuConnect.setObjectName(_fromUtf8("menuConnect"))
        self.menuControl = QtGui.QMenu(self.menubar)
        self.menuControl.setObjectName(_fromUtf8("menuControl"))
        MainWindow.setMenuBar(self.menubar)
        self.actionSimulator = QtGui.QAction(MainWindow)
        self.actionSimulator.setObjectName(_fromUtf8("actionSimulator"))
        self.actionDisconnect = QtGui.QAction(MainWindow)
        self.actionDisconnect.setObjectName(_fromUtf8("actionDisconnect"))
        self.menuConnect.addAction(self.actionSimulator)
        self.menuConnect.addSeparator()
        self.menuConnect.addAction(self.actionDisconnect)
        self.menubar.addAction(self.menuConnect.menuAction())
        self.menubar.addAction(self.menuControl.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow", None))
        self.sendButton.setText(_translate("MainWindow", "Send", None))
        self.menuConnect.setTitle(_translate("MainWindow", "Connection", None))
        self.menuControl.setTitle(_translate("MainWindow", "Control", None))
        self.actionSimulator.setText(_translate("MainWindow", "Simulator", None))
        self.actionDisconnect.setText(_translate("MainWindow", "Disconnect", None))

from pyqtgraph import PlotWidget
