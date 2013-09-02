# ---------------------------------
# Finds BLiFF
# Adds library to target
# Adds include path
# ---------------------------------
IF(UNIX)
	FIND_PATH(PATH_BLiFF include/Trial.h PATHS ${OV_CUSTOM_DEPENDENCIES_PATH})
	IF(PATH_BLiFF)
		MESSAGE(STATUS "  Found BLiFF++...")
		INCLUDE_DIRECTORIES(${PATH_BLiFF}/include)
		INCLUDE_DIRECTORIES(${OV_CUSTOM_DEPENDENCIES_PATH}/include/itpp)              # for itpp, BLiFF++ does not include files correctly
		INCLUDE_DIRECTORIES(${OV_CUSTOM_DEPENDENCIES_PATH}/include/itpp/base)         # for itpp, BLiFF++ does not include files correctly
		INCLUDE_DIRECTORIES(${OV_CUSTOM_DEPENDENCIES_PATH}/include/itpp/base/algebra) # for itpp, BLiFF++ does not include files correctly
		INCLUDE_DIRECTORIES(${OV_CUSTOM_DEPENDENCIES_PATH}/include/itpp/comm)         # for itpp, BLiFF++ does not include files correctly
		INCLUDE_DIRECTORIES(${OV_CUSTOM_DEPENDENCIES_PATH}/include/itpp/fixedpoint)   # for itpp, BLiFF++ does not include files correctly
		INCLUDE_DIRECTORIES(${OV_CUSTOM_DEPENDENCIES_PATH}/include/itpp/protocol)     # for itpp, BLiFF++ does not include files correctly
		INCLUDE_DIRECTORIES(${OV_CUSTOM_DEPENDENCIES_PATH}/include/itpp/srccode)      # for itpp, BLiFF++ does not include files correctly
		FIND_LIBRARY(LIB_BLiFF bliff PATHS ${PATH_BLiFF}/lib )
		IF(LIB_BLiFF)
			MESSAGE(STATUS "    [  OK  ] lib ${LIB_BLiFF}")
			TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_BLiFF} )
		ELSE(LIB_BLiFF)
			MESSAGE(STATUS "    [FAILED] lib bliff")
		ENDIF(LIB_BLiFF)

		ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyBLiFF)
		ADD_DEFINITIONS(-Wno-deprecated)
	ELSE(PATH_BLiFF)
		MESSAGE(STATUS "  FAILED to find BLiFF++")
	ENDIF(PATH_BLiFF)
ENDIF(UNIX)
