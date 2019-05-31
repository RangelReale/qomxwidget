set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt4 REQUIRED QtGui)

SET(SRCS
	Widget.cpp
)

SET(INCS
	../../include/qomxwidget/widget/Widget.h
)

ADD_LIBRARY(qomxwidget STATIC ${SRCS} ${INCS} )

target_link_libraries(qomxwidget
	qomxwidgetplayer
	Qt4::QtGui
)
