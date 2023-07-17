#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ge211" for configuration "Release"
set_property(TARGET ge211 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ge211 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/ge211.lib"
  )

list(APPEND _cmake_import_check_targets ge211 )
list(APPEND _cmake_import_check_files_for_ge211 "${_IMPORT_PREFIX}/lib/ge211.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
