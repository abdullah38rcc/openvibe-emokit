# - Find hidapi
# This module defines
# Mcrypt_INCLUDE_DIR, where to find Mcrypt headers
# Mcrypt_LIBS, Mcrypt libraries
# HIDAPI_FOUND, If false, do not try to use Mcrypt
 
FIND_PATH(HIDAPI_INCLUDE_DIR hidapi/hidapi.h PATHS /usr/local/include /opt/local/include)
FIND_LIBRARY(HIDAPI_LIB NAMES hidapi-libusb PATHS /usr/local/lib /opt/local/lib /usr/lib64)
 
if (HIDAPI_LIB AND HIDAPI_INCLUDE_DIR)
  set(HIDAPI_FOUND TRUE)
  set(HIDAPI_LIBS ${HIDAPI_LIB})
else ()
  set(HIDAPI_FOUND FALSE)
endif ()
