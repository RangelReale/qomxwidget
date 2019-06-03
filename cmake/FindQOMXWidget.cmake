#.rst:
# FindQOMXWidget
# -------
#
# Finds the QOMXWidget library
#
# This will define the following variables:
#
#   QOMXWidget_FOUND    - True if the system has the QOMXWidget library
#   QOMXWidget_VERSION  - The version of the QOMXWidget library which was found
#
# and the following imported targets:
#
#   QOMXWidget::QOMXWidget   - The QOMXWidget library
#

set (AVAIL_QOMXWidget_QTVERSION QT4 QT5)
set(QOMXWidget_QTVERSION "" CACHE STRING "Qt Version (${AVAIL_QOMXWidget_QTVERSION})")
set_property(CACHE QOMXWidget_QTVERSION PROPERTY STRINGS ${AVAIL_QOMXWidget_QTVERSION})

if (QOMXWidget_QTVERSION STREQUAL "")
	message(FATAL_ERROR "QOMXWidget_QTVERSION not set")
endif ()

if (QOMXWidget_QTVERSION STREQUAL QT5)
  SET (QOMXWidget_QTVERSION_SUFFIX 5)
elseif (QOMXWidget_QTVERSION STREQUAL QT4)
  SET (QOMXWidget_QTVERSION_SUFFIX 4)
endif()


set(QOMXWidget_ENABLE_WIDGET OFF CACHE BOOL "Enable OMX widget")

# find the include path
find_path(QOMXWidget_INCLUDE_DIR
  NAMES player/Player.h
  PATH_SUFFIXES qomxwidget
)

include(FindPackageHandleStandardArgs)

# determine if should search separate debug and release libraries
set(QOMXWidget_DEBUG_AND_RELEASE OFF)
if (WIN32)
	set(QOMXWidget_DEBUG_AND_RELEASE ON)
endif()

if (QOMXWidget_DEBUG_AND_RELEASE)
	# find release library
	find_library(QOMXWidgetPlayer_LIBRARY_RELEASE
	  NAMES qomxwidgetplayer${QOMXWidget_QTVERSION_SUFFIX}
	)

	# find debug library
	find_library(QOMXWidgetPlayer_LIBRARY_DEBUG
	  NAMES qomxwidgetplayer${QOMXWidget_QTVERSION_SUFFIX}
	)

	# only release is required
	find_package_handle_standard_args(QOMXWidgetPlayer
	  FOUND_VAR QOMXWidgetPlayer_FOUND
	  REQUIRED_VARS
		QOMXWidgetPlayer_LIBRARY_RELEASE
		QOMXWidget_INCLUDE_DIR
	)
	if (QOMXWidget_ENABLE_WIDGET)
		# find release library
		find_library(QOMXWidget_LIBRARY_RELEASE
		  NAMES qomxwidget${QOMXWidget_QTVERSION_SUFFIX}
		)
	
		# find debug library
		find_library(QOMXWidget_LIBRARY_DEBUG
		  NAMES qomxwidget${QOMXWidget_QTVERSION_SUFFIX}
		)
	
		# only release is required
		find_package_handle_standard_args(QOMXWidget
		  FOUND_VAR QOMXWidget_FOUND
		  REQUIRED_VARS
			QOMXWidget_LIBRARY_RELEASE
			QOMXWidget_INCLUDE_DIR
		)
	endif()
else()
	# find library
	find_library(QOMXWidgetPlayer_LIBRARY
	  NAMES qomxwidgetplayer${QOMXWidget_QTVERSION_SUFFIX}
	)

	find_package_handle_standard_args(QOMXWidgetPlayer
	  FOUND_VAR QOMXWidgetPlayer_FOUND
	  REQUIRED_VARS
		QOMXWidgetPlayer_LIBRARY
		QOMXWidget_INCLUDE_DIR
	)

	if (QOMXWidget_ENABLE_WIDGET)
		# find library
		find_library(QOMXWidget_LIBRARY
		  NAMES qomxwidget${QOMXWidget_QTVERSION_SUFFIX}
		)
	
		find_package_handle_standard_args(QOMXWidget
		  FOUND_VAR QOMXWidget_FOUND
		  REQUIRED_VARS
			QOMXWidget_LIBRARY
			QOMXWidget_INCLUDE_DIR
		)
	endif()
endif()

if(QOMXWidgetPlayer_FOUND AND NOT TARGET QOMXWidget::Player)
  add_library(QOMXWidget::Player UNKNOWN IMPORTED)
  set_target_properties(QOMXWidget::Player PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${QOMXWidget_INCLUDE_DIR}"
#    INTERFACE_COMPILE_OPTIONS "${QOMXWidgetPlayer_CFLAGS_OTHER}"
  )

  if (QOMXWidget_DEBUG_AND_RELEASE)
	# release (must be first, so it is the default)
    set_property(TARGET QOMXWidget::Player APPEND PROPERTY
      IMPORTED_CONFIGURATIONS RELEASE
    )
    set_target_properties(QOMXWidget::Player PROPERTIES
      IMPORTED_LOCATION_RELEASE "${QOMXWidgetPlayer_LIBRARY_RELEASE}"
    )

	# debug (optional)
	if (QOMXWidget_LIBRARY_DEBUG)
		set_property(TARGET QOMXWidget::Player APPEND PROPERTY
		  IMPORTED_CONFIGURATIONS DEBUG
		)
		set_target_properties(QOMXWidget::Player PROPERTIES
		  IMPORTED_LOCATION_DEBUG "${QOMXWidgetPlayer_LIBRARY_DEBUG}"
		)
	endif()

  else()
	# only release
	set_target_properties(QOMXWidget::Player PROPERTIES
		IMPORTED_LOCATION "${QOMXWidgetPlayer_LIBRARY}"
    )	
  endif()
endif()

if (QOMXWidget_ENABLE_WIDGET)
	if(QOMXWidget_FOUND AND NOT TARGET QOMXWidget::Widget)
	  add_library(QOMXWidget::Widget UNKNOWN IMPORTED)
	  set_target_properties(QOMXWidget::Widget PROPERTIES
	    INTERFACE_INCLUDE_DIRECTORIES "${QOMXWidget_INCLUDE_DIR}"
	#    INTERFACE_COMPILE_OPTIONS "${QOMXWidget_CFLAGS_OTHER}"
	  )
	
	  if (QOMXWidget_DEBUG_AND_RELEASE)
		# release (must be first, so it is the default)
	    set_property(TARGET QOMXWidget::Widget APPEND PROPERTY
	      IMPORTED_CONFIGURATIONS RELEASE
	    )
	    set_target_properties(QOMXWidget::Widget PROPERTIES
	      IMPORTED_LOCATION_RELEASE "${QOMXWidget_LIBRARY_RELEASE}"
	    )
	
		# debug (optional)
		if (QOMXWidget_LIBRARY_DEBUG)
			set_property(TARGET QOMXWidget::Widget APPEND PROPERTY
			  IMPORTED_CONFIGURATIONS DEBUG
			)
			set_target_properties(QOMXWidget::Widget PROPERTIES
			  IMPORTED_LOCATION_DEBUG "${QOMXWidget_LIBRARY_DEBUG}"
			)
		endif()
	
	  else()
		# only release
		set_target_properties(QOMXWidget::Widget PROPERTIES
			IMPORTED_LOCATION "${QOMXWidget_LIBRARY}"
	    )	
	  endif()
	endif()
endif()


mark_as_advanced(
  QOMXWidget_INCLUDE_DIR
  QOMXWidgetPlayer_LIBRARY
  QOMXWidgetPlayer_LIBRARY_RELEASE
  QOMXWidgetPlayer_LIBRARY_DEBUG
)

if (QOMXWidget_ENABLE_WIDGET)
	mark_as_advanced(
	  QOMXWidget_LIBRARY
	  QOMXWidget_LIBRARY_RELEASE
	  QOMXWidget_LIBRARY_DEBUG
	)
endif()