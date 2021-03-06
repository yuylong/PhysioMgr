#-
# GNU GENERAL PUBLIC LICENSE, version 3
# See LICENSE file for detail.
#
# Author: Yulong Yu, Jun 29th, 2017
# Copyright(c) 2015-2017 Yulong Yu. All rights reserved.
#

#-------------------------------------------------
#
# Project created by QtCreator 2017-06-29T14:05:53
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhysioMgr
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        MainWindow.cpp \
    PsmDatabase.cpp \
    PsmService.cpp \
    PsmDlgDepartment.cpp \
    PsmDlgPhysioItem.cpp \
    PsmDlgDoctor.cpp \
    PsmDlgDepartSel.cpp \
    PsmDlgPatient.cpp \
    PsmDlgHospiRec.cpp \
    PsmDlgDoctorSel.cpp \
    PsmDlgHospiPhysio.cpp \
    PsmDlgHospiPhysioReg.cpp \
    PsmDlgPhysioSel.cpp \
    PsmDlgPhysioList.cpp \
    PsmDlgPatientSel.cpp

HEADERS  += MainWindow.h \
    PsmDatabase.h \
    PsmService.h \
    PsmDlgDepartment.h \
    PsmDlgPhysioItem.h \
    PsmDlgDoctor.h \
    PsmDlgDepartSel.h \
    PsmDlgPatient.h \
    PsmDlgHospiRec.h \
    PsmDlgDoctorSel.h \
    PsmDlgHospiPhysio.h \
    PsmDlgHospiPhysioReg.h \
    PsmDlgPhysioSel.h \
    PsmDlgPhysioList.h \
    PsmDlgPatientSel.h

FORMS    += MainWindow.ui \
    PsmDlgDepartment.ui \
    PsmDlgPhysioItem.ui \
    PsmDlgDoctor.ui \
    PsmDlgDepartSel.ui \
    PsmDlgPatient.ui \
    PsmDlgHospiRec.ui \
    PsmDlgDoctorSel.ui \
    PsmDlgHospiPhysio.ui \
    PsmDlgHospiPhysioReg.ui \
    PsmDlgPhysioSel.ui \
    PsmDlgPhysioList.ui \
    PsmDlgPatientSel.ui

DISTFILES += \
    PsmDbConfig.xml
