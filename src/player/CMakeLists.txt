set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (VQTVERSION STREQUAL QT5)
	find_package(Qt5 COMPONENTS Widgets DBus REQUIRED)
elseif (VQTVERSION STREQUAL QT4)
	find_package(Qt4 REQUIRED QtGui QtDBus)
endif()
SET( TARGET_BASE_NAME qomxwidgetplayer${VQTVERSION_SUFFIX} )

SET(SRCS
	Player.cpp
)

SET(INCS
	../../include/qomxwidget/player/Player.h
)

ADD_LIBRARY(${TARGET_BASE_NAME} STATIC ${SRCS} ${INCS} )

target_link_libraries(${TARGET_BASE_NAME}
	QDBusDaemon::QDBusDaemon
)

if (VQTVERSION STREQUAL QT5)
	target_link_libraries(${TARGET_BASE_NAME} Qt5::Widgets Qt5::DBus)
elseif (VQTVERSION STREQUAL QT4)
	target_link_libraries(${TARGET_BASE_NAME} Qt4::QtGui Qt4::QtDBus)
endif()
