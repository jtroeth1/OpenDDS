if(NOT DEFINED _OPENDDS_CMAKE_DIR)
  set(_OPENDDS_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")
endif()
if(NOT "${_OPENDDS_CMAKE_DIR}" IN_LIST CMAKE_MODULE_PATH)
  list(APPEND CMAKE_MODULE_PATH "${_OPENDDS_CMAKE_DIR}")
endif()

if(NOT DEFINED OPENDDS_VERSION)
  set(_version_file "${_OPENDDS_CMAKE_DIR}/../VERSION.txt")
  if(NOT EXISTS "${_version_file}")
    set(_version_file "${_OPENDDS_CMAKE_DIR}/../../dds/VERSION.txt")
    if(NOT EXISTS "${_version_file}")
      message(FATAL_ERROR "Can't find OpenDDS VERSION.txt file")
    endif()
  endif()
  file(READ "${_version_file}" _version_file_contents)
  string(REGEX MATCH "OpenDDS version ([0-9]+.[0-9]+.[0-9]+)" _ "${_version_file_contents}")
  set(OPENDDS_VERSION "${CMAKE_MATCH_1}")
  if(NOT OPENDDS_VERSION)
    message(FATAL_ERROR "Couldn't get OpenDDS version from ${_version_file}")
  endif()
endif()
