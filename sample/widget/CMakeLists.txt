set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (VQTVERSION STREQUAL QT5)
	find_package(Qt5 COMPONENTS Widgets REQUIRED)
elseif (VQTVERSION STREQUAL QT4)
	find_package(Qt4 REQUIRED QtGui)
endif()
SET( TARGET_BASE_NAME qdomxwidget${VQTVERSION_SUFFIX}_sample )

add_executable(${TARGET_BASE_NAME}
	main.cpp
	MainWindow.h
	MainWindow.cpp
)

target_link_libraries(${TARGET_BASE_NAME}
	qomxwidget${VQTVERSION_SUFFIX}
	qomxwidgetplayer${VQTVERSION_SUFFIX}
	QDBusDaemon::QDBusDaemon
)

if (VQTVERSION STREQUAL QT5)
	target_link_libraries(${TARGET_BASE_NAME} Qt5::Widgets)
elseif (VQTVERSION STREQUAL QT4)
	target_link_libraries(${TARGET_BASE_NAME} Qt4::QtGui)
endif()
