PROJECT(MYTEST)

AUX_SOURCE_DIRECTORY(./ SRC_LIST)
SET(EXECUTABLE_OUTPUT_PATH ../bin)

ADD_DEFINITIONS(-g -W -Wall -Wno-deprecated -DLINUX -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DAC_HAS_INFO -DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DTIXML_USE_STL -DHAVE_CXX_STDHEADERS ${CMAKE_CXX_FLAGS})

INCLUDE_DIRECTORIES(
./
/usr/local/include/commonlib/syncwriteclient
/usr/local/include/commonlib/svrutil2
)
LINK_DIRECTORIES(
./
/usr/local/lib
/usr/local/lib/commonlib
)

ADD_EXECUTABLE(mytest ${SRC_LIST})

TARGET_LINK_LIBRARIES(mytest pthread netutil svrutil  leveldb syncwriteclient dl z)
