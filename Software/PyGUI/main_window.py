# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'main_window2.ui'
##
## Created by: Qt User Interface Compiler version 5.15.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

from pyqtgraph import PlotWidget


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(1049, 450)
        self.actionSimulator = QAction(MainWindow)
        self.actionSimulator.setObjectName(u"actionSimulator")
        self.actionDisconnect = QAction(MainWindow)
        self.actionDisconnect.setObjectName(u"actionDisconnect")
        self.actionTip_Calibration = QAction(MainWindow)
        self.actionTip_Calibration.setObjectName(u"actionTip_Calibration")
        self.actionPID_Calibration = QAction(MainWindow)
        self.actionPID_Calibration.setObjectName(u"actionPID_Calibration")
        self.actionSetpoint = QAction(MainWindow)
        self.actionSetpoint.setObjectName(u"actionSetpoint")
        self.actionDisplay = QAction(MainWindow)
        self.actionDisplay.setObjectName(u"actionDisplay")
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.centralwidget.setLayoutDirection(Qt.LeftToRight)
        self.verticalLayout_2 = QVBoxLayout(self.centralwidget)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.gridLayout = QGridLayout()
        self.gridLayout.setObjectName(u"gridLayout")
        self.label_5 = QLabel(self.centralwidget)
        self.label_5.setObjectName(u"label_5")

        self.gridLayout.addWidget(self.label_5, 2, 8, 1, 1)

        self.offsetSpinBox = QDoubleSpinBox(self.centralwidget)
        self.offsetSpinBox.setObjectName(u"offsetSpinBox")
        sizePolicy = QSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.offsetSpinBox.sizePolicy().hasHeightForWidth())
        self.offsetSpinBox.setSizePolicy(sizePolicy)
        self.offsetSpinBox.setMinimumSize(QSize(0, 0))
        self.offsetSpinBox.setMaximumSize(QSize(16777215, 16777215))

        self.gridLayout.addWidget(self.offsetSpinBox, 1, 9, 1, 1)

        self.label_4 = QLabel(self.centralwidget)
        self.label_4.setObjectName(u"label_4")
        self.label_4.setMinimumSize(QSize(40, 0))
        self.label_4.setMaximumSize(QSize(40, 16777215))

        self.gridLayout.addWidget(self.label_4, 1, 8, 1, 1)

        self.label_8 = QLabel(self.centralwidget)
        self.label_8.setObjectName(u"label_8")
        sizePolicy1 = QSizePolicy(QSizePolicy.Minimum, QSizePolicy.Preferred)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.label_8.sizePolicy().hasHeightForWidth())
        self.label_8.setSizePolicy(sizePolicy1)

        self.gridLayout.addWidget(self.label_8, 3, 6, 1, 1)

        self.KiSpinBox = QDoubleSpinBox(self.centralwidget)
        self.KiSpinBox.setObjectName(u"KiSpinBox")

        self.gridLayout.addWidget(self.KiSpinBox, 2, 7, 1, 1)

        self.gainSpinBox = QDoubleSpinBox(self.centralwidget)
        self.gainSpinBox.setObjectName(u"gainSpinBox")

        self.gridLayout.addWidget(self.gainSpinBox, 2, 9, 1, 1)

        self.KpSpinBox = QDoubleSpinBox(self.centralwidget)
        self.KpSpinBox.setObjectName(u"KpSpinBox")
        self.KpSpinBox.setMinimumSize(QSize(0, 0))
        self.KpSpinBox.setMaximumSize(QSize(16777215, 16777215))

        self.gridLayout.addWidget(self.KpSpinBox, 1, 7, 1, 1)

        self.label_7 = QLabel(self.centralwidget)
        self.label_7.setObjectName(u"label_7")
        sizePolicy1.setHeightForWidth(self.label_7.sizePolicy().hasHeightForWidth())
        self.label_7.setSizePolicy(sizePolicy1)

        self.gridLayout.addWidget(self.label_7, 2, 6, 1, 1)

        self.label_6 = QLabel(self.centralwidget)
        self.label_6.setObjectName(u"label_6")
        sizePolicy2 = QSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.label_6.sizePolicy().hasHeightForWidth())
        self.label_6.setSizePolicy(sizePolicy2)
        self.label_6.setMinimumSize(QSize(25, 0))
        self.label_6.setMaximumSize(QSize(25, 16777215))

        self.gridLayout.addWidget(self.label_6, 1, 6, 1, 1)

        self.KdSpinBox = QDoubleSpinBox(self.centralwidget)
        self.KdSpinBox.setObjectName(u"KdSpinBox")

        self.gridLayout.addWidget(self.KdSpinBox, 3, 7, 1, 1)

        self.setpointSpinBox = QDoubleSpinBox(self.centralwidget)
        self.setpointSpinBox.setObjectName(u"setpointSpinBox")
        self.setpointSpinBox.setMaximumSize(QSize(70, 16777215))
        self.setpointSpinBox.setMaximum(999.990000000000009)
        self.setpointSpinBox.setSingleStep(5.000000000000000)

        self.gridLayout.addWidget(self.setpointSpinBox, 1, 5, 1, 1)

        self.disconnectButton = QPushButton(self.centralwidget)
        self.disconnectButton.setObjectName(u"disconnectButton")

        self.gridLayout.addWidget(self.disconnectButton, 3, 3, 1, 1)

        self.label_2 = QLabel(self.centralwidget)
        self.label_2.setObjectName(u"label_2")

        self.gridLayout.addWidget(self.label_2, 2, 10, 1, 1)

        self.label = QLabel(self.centralwidget)
        self.label.setObjectName(u"label")
        self.label.setMinimumSize(QSize(70, 0))
        self.label.setMaximumSize(QSize(70, 16777215))

        self.gridLayout.addWidget(self.label, 1, 10, 1, 1)

        self.deviceTextEdit = QLineEdit(self.centralwidget)
        self.deviceTextEdit.setObjectName(u"deviceTextEdit")

        self.gridLayout.addWidget(self.deviceTextEdit, 1, 3, 1, 1)

        self.label_9 = QLabel(self.centralwidget)
        self.label_9.setObjectName(u"label_9")

        self.gridLayout.addWidget(self.label_9, 2, 4, 1, 1)

        self.label_3 = QLabel(self.centralwidget)
        self.label_3.setObjectName(u"label_3")
        self.label_3.setMaximumSize(QSize(55, 16777215))

        self.gridLayout.addWidget(self.label_3, 1, 4, 1, 1)

        self.label_11 = QLabel(self.centralwidget)
        self.label_11.setObjectName(u"label_11")

        self.gridLayout.addWidget(self.label_11, 1, 12, 1, 1)

        self.standTemperatureSpinBox = QSpinBox(self.centralwidget)
        self.standTemperatureSpinBox.setObjectName(u"standTemperatureSpinBox")
        self.standTemperatureSpinBox.setMaximum(999)
        self.standTemperatureSpinBox.setSingleStep(5)

        self.gridLayout.addWidget(self.standTemperatureSpinBox, 2, 13, 1, 1)

        self.sleepTemperatureSpinBox = QSpinBox(self.centralwidget)
        self.sleepTemperatureSpinBox.setObjectName(u"sleepTemperatureSpinBox")
        self.sleepTemperatureSpinBox.setMaximum(999)
        self.sleepTemperatureSpinBox.setSingleStep(5)

        self.gridLayout.addWidget(self.sleepTemperatureSpinBox, 1, 13, 1, 1)

        self.label_12 = QLabel(self.centralwidget)
        self.label_12.setObjectName(u"label_12")

        self.gridLayout.addWidget(self.label_12, 2, 14, 1, 1)

        self.label_13 = QLabel(self.centralwidget)
        self.label_13.setObjectName(u"label_13")

        self.gridLayout.addWidget(self.label_13, 1, 14, 1, 1)

        self.offTimeoutSpinBox = QSpinBox(self.centralwidget)
        self.offTimeoutSpinBox.setObjectName(u"offTimeoutSpinBox")
        self.offTimeoutSpinBox.setMaximum(255)

        self.gridLayout.addWidget(self.offTimeoutSpinBox, 2, 15, 1, 1)

        self.sleepTimeoutSpinBox = QSpinBox(self.centralwidget)
        self.sleepTimeoutSpinBox.setObjectName(u"sleepTimeoutSpinBox")
        self.sleepTimeoutSpinBox.setMaximum(255)

        self.gridLayout.addWidget(self.sleepTimeoutSpinBox, 1, 15, 1, 1)

        self.label_14 = QLabel(self.centralwidget)
        self.label_14.setObjectName(u"label_14")
        self.label_14.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.gridLayout.addWidget(self.label_14, 3, 14, 1, 1)

        self.maxPowerSpinBox = QSpinBox(self.centralwidget)
        self.maxPowerSpinBox.setObjectName(u"maxPowerSpinBox")
        self.maxPowerSpinBox.setMaximum(100)

        self.gridLayout.addWidget(self.maxPowerSpinBox, 2, 5, 1, 1)

        self.connectSimulatorButton = QPushButton(self.centralwidget)
        self.connectSimulatorButton.setObjectName(u"connectSimulatorButton")

        self.gridLayout.addWidget(self.connectSimulatorButton, 3, 4, 1, 1)

        self.connectSerialButton = QPushButton(self.centralwidget)
        self.connectSerialButton.setObjectName(u"connectSerialButton")
        sizePolicy3 = QSizePolicy(QSizePolicy.Expanding, QSizePolicy.Minimum)
        sizePolicy3.setHorizontalStretch(0)
        sizePolicy3.setVerticalStretch(0)
        sizePolicy3.setHeightForWidth(self.connectSerialButton.sizePolicy().hasHeightForWidth())
        self.connectSerialButton.setSizePolicy(sizePolicy3)
        self.connectSerialButton.setMaximumSize(QSize(16777215, 16777215))

        self.gridLayout.addWidget(self.connectSerialButton, 2, 3, 1, 1)

        self.brightnessSpinBox = QSpinBox(self.centralwidget)
        self.brightnessSpinBox.setObjectName(u"brightnessSpinBox")
        self.brightnessSpinBox.setMaximum(100)

        self.gridLayout.addWidget(self.brightnessSpinBox, 1, 11, 1, 1)

        self.contrastSpinBox = QSpinBox(self.centralwidget)
        self.contrastSpinBox.setObjectName(u"contrastSpinBox")
        self.contrastSpinBox.setMaximum(100)

        self.gridLayout.addWidget(self.contrastSpinBox, 2, 11, 1, 1)

        self.label_10 = QLabel(self.centralwidget)
        self.label_10.setObjectName(u"label_10")

        self.gridLayout.addWidget(self.label_10, 2, 12, 1, 1)

        self.label_15 = QLabel(self.centralwidget)
        self.label_15.setObjectName(u"label_15")

        self.gridLayout.addWidget(self.label_15, 3, 12, 1, 1)

        self.maxTemperatureSpinBox = QSpinBox(self.centralwidget)
        self.maxTemperatureSpinBox.setObjectName(u"maxTemperatureSpinBox")
        self.maxTemperatureSpinBox.setMaximum(999)

        self.gridLayout.addWidget(self.maxTemperatureSpinBox, 3, 13, 1, 1)


        self.verticalLayout_2.addLayout(self.gridLayout)

        self.widget = PlotWidget(self.centralwidget)
        self.widget.setObjectName(u"widget")

        self.verticalLayout_2.addWidget(self.widget)

        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)
        QWidget.setTabOrder(self.disconnectButton, self.setpointSpinBox)
        QWidget.setTabOrder(self.setpointSpinBox, self.KpSpinBox)
        QWidget.setTabOrder(self.KpSpinBox, self.KiSpinBox)
        QWidget.setTabOrder(self.KiSpinBox, self.KdSpinBox)
        QWidget.setTabOrder(self.KdSpinBox, self.offsetSpinBox)
        QWidget.setTabOrder(self.offsetSpinBox, self.gainSpinBox)
        QWidget.setTabOrder(self.gainSpinBox, self.widget)

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        self.actionSimulator.setText(QCoreApplication.translate("MainWindow", u"Simulator", None))
        self.actionDisconnect.setText(QCoreApplication.translate("MainWindow", u"Disconnect", None))
        self.actionTip_Calibration.setText(QCoreApplication.translate("MainWindow", u"Tip Calibration", None))
        self.actionPID_Calibration.setText(QCoreApplication.translate("MainWindow", u"PID Calibration", None))
        self.actionSetpoint.setText(QCoreApplication.translate("MainWindow", u"Temperature", None))
        self.actionDisplay.setText(QCoreApplication.translate("MainWindow", u"Display", None))
        self.label_5.setText(QCoreApplication.translate("MainWindow", u"Gain:", None))
        self.label_4.setText(QCoreApplication.translate("MainWindow", u"Offset:", None))
        self.label_8.setText(QCoreApplication.translate("MainWindow", u"Kd:", None))
        self.label_7.setText(QCoreApplication.translate("MainWindow", u"Ki:", None))
        self.label_6.setText(QCoreApplication.translate("MainWindow", u"Kp:", None))
        self.disconnectButton.setText(QCoreApplication.translate("MainWindow", u"Disconnect", None))
        self.label_2.setText(QCoreApplication.translate("MainWindow", u"Contrast:", None))
        self.label.setText(QCoreApplication.translate("MainWindow", u"Brightness:", None))
        self.deviceTextEdit.setPlaceholderText(QCoreApplication.translate("MainWindow", u"Serial port", None))
        self.label_9.setText(QCoreApplication.translate("MainWindow", u"Max power:", None))
        self.label_3.setText(QCoreApplication.translate("MainWindow", u"Setpoint:", None))
        self.label_11.setText(QCoreApplication.translate("MainWindow", u"Sleep temp:", None))
        self.label_12.setText(QCoreApplication.translate("MainWindow", u"Off timeout [min]:", None))
        self.label_13.setText(QCoreApplication.translate("MainWindow", u"Sleep timeout [min]:", None))
#if QT_CONFIG(tooltip)
        self.offTimeoutSpinBox.setToolTip("")
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        self.sleepTimeoutSpinBox.setToolTip("")
#endif // QT_CONFIG(tooltip)
        self.label_14.setText(QCoreApplication.translate("MainWindow", u"(0 min - disable)", None))
        self.connectSimulatorButton.setText(QCoreApplication.translate("MainWindow", u"Simulator", None))
        self.connectSerialButton.setText(QCoreApplication.translate("MainWindow", u"Connect", None))
        self.label_10.setText(QCoreApplication.translate("MainWindow", u"Stand temp:", None))
        self.label_15.setText(QCoreApplication.translate("MainWindow", u"Max temp:", None))
    # retranslateUi

