include(../common_top.pri)
TARGET = ut_quicklaunchbarview

STYLE_HEADERS += $$SRCDIR/quicklaunchbarstyle.h
MODEL_HEADERS += $$SRCDIR/quicklaunchbarmodel.h

# unit test and unit
SOURCES += \
    ut_quicklaunchbarview.cpp \
    $$SRCDIR/quicklaunchbarview.cpp \
    $$SRCDIR/quicklaunchbar.cpp

# base classes
SOURCES += ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_quicklaunchbarview.h \
    $$SRCDIR/quicklaunchbarview.h \

# service classes
HEADERS += \
    $$SRCDIR/quicklaunchbarmodel.h \
    $$SRCDIR/quicklaunchbarstyle.h \
    $$SRCDIR/quicklaunchbar.h \
    $$SRCDIR/launcher.h \
    $$SRCDIR/launcherbutton.h

include(../common_bot.pri)
