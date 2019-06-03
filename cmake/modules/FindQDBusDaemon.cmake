#.rst:
# FindQDBusDaemon
# -------
#
# Finds the QDBusDaemon library
#
# This will define the following variables:
#
#   QDBusDaemon_FOUND    - True if the system has the QDBusDaemon library
#   QDBusDaemon_VERSION  - The version of the QDBusDaemon library which was found
#
# and the following imported targets:
#
#   QDBusDaemon::QDBusDaemon   - The QDBusDaemon library
#

set (AVAIL_QDBusDaemon_QTVERSION QT4 QT5)
set(QDBusDaemon_QTVERSION "" CACHE STRING "Qt Version (${AVAIL_QDBusDaemon_QTVERSION})")
set_property(CACHE QDBusDaemon_QTVERSION PROPERTY STRINGS ${AVAIL_QDBusDaemon_QTVERSION})

if (QDBusDaemon_QTVERSION STREQUAL "")
	message(FATAL_ERROR "QDBusDaemon_QTVERSION not set")
endif ()

if (QDBusDaemon_QTVERSION STREQUAL QT5)
  SET (QDBusDaemon_QTVERSION_SUFFIX 5)
elseif (QDBusDaemon_QTVERSION STREQUAL QT4)
  SET (QDBusDaemon_QTVERSION_SUFFIX 4)
endif()

# find the include path
find_path(QDBusDaemon_INCLUDE_DIR
  NAMES Base.h
  PATH_SUFFIXES qdbusdaemon
)

include(FindPackageHandleStandardArgs)

# determine if should search separate debug and release libraries
set(QDBusDaemon_DEBUG_AND_RELEASE OFF)
if (WIN32)
	set(QDBusDaemon_DEBUG_AND_RELEASE ON)
endif()

if (QDBusDaemon_DEBUG_AND_RELEASE)
	# find release library
	find_library(QDBusDaemon_LIBRARY_RELEASE
	  NAMES qdbusdaemon${QDBusDaemon_QTVERSION_SUFFIX}
	)

	# find debug library
	find_library(QDBusDaemon_LIBRARY_DEBUG
	  NAMES qdbusdaemon${QDBusDaemon_QTVERSION_SUFFIX}
	)

	# only release is required
	find_package_handle_standard_args(QDBusDaemon
	  FOUND_VAR QDBusDaemon_FOUND
	  REQUIRED_VARS
		QDBusDaemon_LIBRARY_RELEASE
		QDBusDaemon_INCLUDE_DIR
	)

else()
	# find library
	find_library(QDBusDaemon_LIBRARY
	  NAMES qdbusdaemon${QDBusDaemon_QTVERSION_SUFFIX}
	)

	find_package_handle_standard_args(QDBusDaemon
	  FOUND_VAR QDBusDaemon_FOUND
	  REQUIRED_VARS
		QDBusDaemon_LIBRARY
		QDBusDaemon_INCLUDE_DIR
	)
endif()

if(QDBusDaemon_FOUND AND NOT TARGET QDBusDaemon::QDBusDaemon)
  add_library(QDBusDaemon::QDBusDaemon UNKNOWN IMPORTED)
  set_target_properties(QDBusDaemon::QDBusDaemon PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${QDBusDaemon_INCLUDE_DIR}"
#    INTERFACE_COMPILE_OPTIONS "${QDBusDaemon_CFLAGS_OTHER}"
  )

  if (QDBusDaemon_DEBUG_AND_RELEASE)
	# release (must be first, so it is the default)
    set_property(TARGET QDBusDaemon::QDBusDaemon APPEND PROPERTY
      IMPORTED_CONFIGURATIONS RELEASE
    )
    set_target_properties(QDBusDaemon::QDBusDaemon PROPERTIES
      IMPORTED_LOCATION_RELEASE "${QDBusDaemon_LIBRARY_RELEASE}"
    )

	# debug (optional)
	if (QDBusDaemon_LIBRARY_DEBUG)
		set_property(TARGET QDBusDaemon::QDBusDaemon APPEND PROPERTY
		  IMPORTED_CONFIGURATIONS DEBUG
		)
		set_target_properties(QDBusDaemon::QDBusDaemon PROPERTIES
		  IMPORTED_LOCATION_DEBUG "${QDBusDaemon_LIBRARY_DEBUG}"
		)
	endif()

  else()
	# only release
	set_target_properties(QDBusDaemon::QDBusDaemon PROPERTIES
		IMPORTED_LOCATION "${QDBusDaemon_LIBRARY}"
    )	
  endif()
endif()

mark_as_advanced(
  QDBusDaemon_INCLUDE_DIR
  QDBusDaemon_LIBRARY
  QDBusDaemon_LIBRARY_RELEASE
  QDBusDaemon_LIBRARY_DEBUG
)