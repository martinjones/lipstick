include(../common_top.pri)
TARGET = ut_home
INCLUDEPATH += $$SRCDIR/notifications
PKGCONFIG += ContentManagerSearchIf

MODEL_HEADERS += $$SRCDIR/desktopmodel.h

# unit test and unit
SOURCES += \
    ut_home.cpp \
    $$SRCDIR/home.cpp

# base classes
SOURCES += \

# unit test and unit
HEADERS += \
    ut_home.h \
    $$SRCDIR/home.h \

# base classes
HEADERS += \

# service classes
SOURCES += ../stubs/stubbase.cpp

# service classes
HEADERS += \
    $$SRCDIR/desktop.h \
    $$SRCDIR/desktopmodel.h \
    $$SRCDIR/launcher.h \
    $$SRCDIR/statusarea.h \
    $$SRCDIR/statusareawindow.h \
    $$SRCDIR/notificationarea.h \
    $$SRCDIR/mainwindow.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/notifications/homenotificationsink.h \
    $$SRCDIR/notifications/widgetnotificationsink.h \
    $$SRCDIR/notifications/notificationsink.h

include(../common_bot.pri)
