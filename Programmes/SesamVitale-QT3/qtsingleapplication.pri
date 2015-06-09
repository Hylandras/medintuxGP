INCLUDEPATH	+= $$PWD
HEADERS		= $$PWD/qtsingleapplication.h
SOURCES		= $$PWD/qtsingleapplication.cpp
win32:SOURCES	+= $$PWD/qtsingleapplication_win.cpp
mac:SOURCES     += $$PWD/qtsingleapplication_mac.cpp
mac:LIBS += -framework CoreFoundation
unix:!mac {
	SOURCES	+= $$PWD/qtsingleapplication_x11.cpp
	CONFIG	+= x11
}

