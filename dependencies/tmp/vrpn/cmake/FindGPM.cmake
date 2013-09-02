# - try to find GPM library
#
# Cache Variables: (probably not for direct use in your scripts)
#  GPM_INCLUDE_DIR
#  GPM_LIBRARY
#
# Non-cache variables you might use in your CMakeLists.txt:
#  GPM_FOUND
#  GPM_INCLUDE_DIRS
#  GPM_LIBRARIES
#  GPM_MARK_AS_ADVANCED - whether to mark our vars as advanced even
#    if we don't find this library.
#
# Requires these CMake modules:
#  FindPackageHandleStandardArgs (known included with CMake >=2.6.2)
#
# Original Author:
# 2009 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com
# Iowa State University HCI Graduate Program/VRAC

find_library(GPM_LIBRARY
	NAMES gpm)

find_path(GPM_INCLUDE_DIR
	NAMES gpm.h)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GPM DEFAULT_MSG
	GPM_LIBRARY
	GPM_INCLUDE_DIR)

if(GPM_FOUND)
	set(GPM_LIBRARIES "${GPM_LIBRARY}")

	set(GPM_INCLUDE_DIRS "${GPM_INCLUDE_DIR}")
endif()

if(GPM_FOUND OR GPM_MARK_AS_ADVANCED)
	foreach(_cachevar
		GPM_INCLUDE_DIR
		GPM_LIBRARY)

		mark_as_advanced(${_cachevar})
	endforeach()
endif()
