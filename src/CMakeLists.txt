message (STATUS "library")
set (ROOT_PATH "..")
set (INCLUDE_PATH ${ROOT_PATH}/include/analyser)
set (SOURCES_PATH ".")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_PATH}/bin)
file (GLOB_RECURSE HEADERS ${INCLUDE_PATH}/*.hpp)
file (GLOB_RECURSE SOURCES ${SOURCES_PATH}/*.cpp)
#message (STATUS ${HEADERS})
#message (STATUS ${SOURCES})
list(FILTER SOURCES EXCLUDE REGEX ".*${SOURCES_PATH}/base/streams/.*" )
add_library (analyser STATIC ${SOURCES} ${HEADERS})
target_include_directories(analyser PUBLIC ../include)
target_link_libraries(analyser ${GMP_LIBRARIES})
target_compile_features(analyser PUBLIC cxx_std_17)
