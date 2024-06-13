message (STATUS "Test compilation")

set (TEST_PATH ".")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

file (GLOB_RECURSE TESTS ${TEST_PATH}/*.cpp ${TEST_PATH}/*.hpp)
list(FILTER TESTS EXCLUDE REGEX ".*${SOURCES_PATH}/main.cpp" )
include_directories (../include/analyser)
link_directories (../src)
add_executable (testlib ${TESTS})
target_compile_features(testlib PUBLIC cxx_std_17)
target_link_libraries (testlib analyser ${GMP_LIBRARIES})
