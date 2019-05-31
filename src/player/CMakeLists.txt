set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt4 REQUIRED QtCore)

SET(SRCS
	Player.cpp
)

SET(INCS
	../../include/qomxwidget/player/Player.h
)

ADD_LIBRARY(qomxwidgetplayer STATIC ${SRCS} ${INCS} )

target_link_libraries(qomxwidgetplayer 
	Qt4::QtCore
	QDBusDaemon::QDBusDaemon
)
