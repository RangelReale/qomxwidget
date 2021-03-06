set (AVAIL_VQTVERSION QT4 QT5)
set(VQTVERSION "" CACHE STRING "Qt Version (${AVAIL_VQTVERSION})")
set_property(CACHE VQTVERSION PROPERTY STRINGS ${AVAIL_VQTVERSION})

if (VQTVERSION STREQUAL "")
  if (MSVC10)
    set(VQTVERSION "QT4")
  elseif(MSVC14)
    set(VQTVERSION "QT5")
  endif()
endif ()

if (VQTVERSION STREQUAL "")
	message(FATAL_ERROR "VQTVERSION not set")
endif ()

if (VQTVERSION STREQUAL QT5)
  SET (VQTVERSION_SUFFIX 5)
elseif (VQTVERSION STREQUAL QT4)
  SET (VQTVERSION_SUFFIX 4)
endif()
