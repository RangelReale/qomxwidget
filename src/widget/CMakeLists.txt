set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (VQTVERSION STREQUAL QT5)
	find_package(Qt5 COMPONENTS Widgets REQUIRED)
elseif (VQTVERSION STREQUAL QT4)
	find_package(Qt4 REQUIRED QtGui)
endif()
SET( TARGET_BASE_NAME qomxwidget${VQTVERSION_SUFFIX} )

SET(SRCS
	Widget.cpp
)

SET(INCS
	../../include/qomxwidget/widget/Widget.h
)

ADD_LIBRARY(${TARGET_BASE_NAME} STATIC ${SRCS} ${INCS} )

target_link_libraries(${TARGET_BASE_NAME}
	qomxwidgetplayer${VQTVERSION_SUFFIX}
)

if (VQTVERSION STREQUAL QT5)
	target_link_libraries(${TARGET_BASE_NAME} Qt5::Widgets)
elseif (VQTVERSION STREQUAL QT4)
	target_link_libraries(${TARGET_BASE_NAME} Qt4::QtGui)
endif()
