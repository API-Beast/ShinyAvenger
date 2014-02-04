# - Find Springbok
# Find the native Springbok includes and libraries
#
#  SPRINGBOK_INCLUDE_DIR - include files for Springbok
#  SPRINGBOK_LIBRARIES   - List of libraries when using libSpringbok.
#  SPRINGBOK_FOUND       - True if libSpringbok found.

if(SPRINGBOK_INCLUDE_DIR)
    # Already in cache, be silent
    set(SPRINGBOK_FIND_QUIETLY TRUE)
endif(SPRINGBOK_INCLUDE_DIR)

find_path(SPRINGBOK_INCLUDE_DIR Springbok/Config.h)
find_library(SPRINGBOK_LIBRARY NAMES libSpringbok.so springbok)

# Handle the QUIETLY and REQUIRED arguments and set SPRINGBOK_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Springbok DEFAULT_MSG SPRINGBOK_LIBRARY SPRINGBOK_INCLUDE_DIR)

if(SPRINGBOK_FOUND)
  set(SPRINGBOK_LIBRARIES ${SPRINGBOK_LIBRARY})
else(SPRINGBOK_FOUND)
  set(SPRINGBOK_LIBRARIES)
endif(SPRINGBOK_FOUND)

mark_as_advanced(SPRINGBOK_INCLUDE_DIR SPRINGBOK_LIBRARY)