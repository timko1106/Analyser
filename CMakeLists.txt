cmake_minimum_required (VERSION 3.29)
option (BUILD_ALL OFF)

project (
	Analyser
	LANGUAGES CXX)
set (CMAKE_CXX_EXTENSIONS OFF)
set (ROOT_PATH .)
set (SOURCES_PATH ${ROOT_PATH}/src)
set (INCLUDE_PATH ${ROOT_PATH}/include/analyser)
set (TEST_PATH ${ROOT_PATH}/test)
#file (GLOB SOURCES ${SOURCES_PATH}/*.cpp)
#file (GLOB HEADERS ${INCLUDE_PATH}/*.hpp)
#file (GLOB TESTS ${TEST_PATH}/*.hpp ${TEST_PATH}/*.cpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH
	${ROOT_PATH}/cmake
	${CMAKE_MODULE_PATH}
)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_PATH}/bin)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_compile_options (/W4)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options (-O3 -pedantic -Wall -Wextra -Werror -Wuninitialized -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -pedantic-errors -Wextra -Wcast-align -Wcast-qual -Wconversion -Werror -Wfloat-equal -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimport  -Winit-self  -Winvalid-pch  -Wmissing-field-initializers -Wmissing-format-attribute  -Wmissing-include-dirs -Wmissing-noreturn -Wpacked  -Wpointer-arith -Wredundant-decls -Wshadow -Wstack-protector -Wswitch-default -Wswitch-enum -Wunreachable-code -Wvariadic-macros -Wwrite-strings -fno-rtti)
else ()
	add_compile_options (-Wall -Wextra -Wpedantic -O3)
endif()
#message (STATUS ${CMAKE_CXX_COMPILER_ID})
#message (STATUS ${SOURCES})
#message (STATUS ${HEADERS})
#message (STATUS ${GMP_INCLUDES} ${GMP_LIBRARIES})

message (STATUS "Loading libraries")

find_package(GMP REQUIRED)
message (STATUS "GMP loaded")
add_subdirectory (src)
add_subdirectory (test)
add_custom_target(test
    COMMAND bin/testlib
    DEPENDS testlib
    WORKING_DIRECTORY ${ROOT_PATH}
)
#add_library (analyser STATIC ${SOURCES} ${HEADERS})

#add_executable (tests ${TESTS})
#target_link_libraries (tests analyser ${GMP_LIBRARIES})
