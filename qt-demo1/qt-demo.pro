QT       += core gui
QT       += network
QT       += widgets
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    qss/QDarkStyleSheet/Hmovetoolbar.png \
    qss/QDarkStyleSheet/Hsepartoolbar.png \
    qss/QDarkStyleSheet/Vmovetoolbar.png \
    qss/QDarkStyleSheet/Vsepartoolbar.png \
    qss/QDarkStyleSheet/branch_closed-on.png \
    qss/QDarkStyleSheet/branch_closed.png \
    qss/QDarkStyleSheet/branch_open-on.png \
    qss/QDarkStyleSheet/branch_open.png \
    qss/QDarkStyleSheet/checkbox_checked.png \
    qss/QDarkStyleSheet/checkbox_checked_disabled.png \
    qss/QDarkStyleSheet/checkbox_checked_focus.png \
    qss/QDarkStyleSheet/checkbox_indeterminate.png \
    qss/QDarkStyleSheet/checkbox_indeterminate_disabled.png \
    qss/QDarkStyleSheet/checkbox_indeterminate_focus.png \
    qss/QDarkStyleSheet/checkbox_unchecked.png \
    qss/QDarkStyleSheet/checkbox_unchecked_disabled.png \
    qss/QDarkStyleSheet/checkbox_unchecked_focus.png \
    qss/QDarkStyleSheet/close-hover.png \
    qss/QDarkStyleSheet/close-pressed.png \
    qss/QDarkStyleSheet/close.png \
    qss/QDarkStyleSheet/down_arrow.png \
    qss/QDarkStyleSheet/down_arrow_disabled.png \
    qss/QDarkStyleSheet/left_arrow.png \
    qss/QDarkStyleSheet/left_arrow_disabled.png \
    qss/QDarkStyleSheet/radio_checked.png \
    qss/QDarkStyleSheet/radio_checked_disabled.png \
    qss/QDarkStyleSheet/radio_checked_focus.png \
    qss/QDarkStyleSheet/radio_unchecked.png \
    qss/QDarkStyleSheet/radio_unchecked_disabled.png \
    qss/QDarkStyleSheet/radio_unchecked_focus.png \
    qss/QDarkStyleSheet/right_arrow.png \
    qss/QDarkStyleSheet/right_arrow_disabled.png \
    qss/QDarkStyleSheet/sizegrip.png \
    qss/QDarkStyleSheet/stylesheet-branch-end.png \
    qss/QDarkStyleSheet/stylesheet-branch-more.png \
    qss/QDarkStyleSheet/stylesheet-vline.png \
    qss/QDarkStyleSheet/transparent.png \
    qss/QDarkStyleSheet/undock.png \
    qss/QDarkStyleSheet/up_arrow.png \
    qss/QDarkStyleSheet/up_arrow_disabled.png \
    qss/images/checkbox.png \
    qss/images/down_arrow.png \
    qss/images/handle.png \
    qss/param.ini \
    qss/styleBlue1.qss \
    qss/styleBlue2.qss \
    qss/styleBlue3.qss \
    qss/styleDark1.qss \
    qss/styleDark2.qss \
    qss/styleDark3.qss \
    qss/styleDark4.qss \
    qss/styleDefault.qss
