set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt4 REQUIRED QtGui)

add_executable(qdomxwidget_sample
	main.cpp
	MainWindow.h
	MainWindow.cpp
)

target_link_libraries(qdomxwidget_sample
	qomxwidget
	qomxwidgetplayer
	QDBusDaemon::QDBusDaemon
	Qt4::QtGui
)
