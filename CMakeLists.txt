cmake_minimum_required(VERSION 3.0.0)
project(urlRegex VERSION 0.1.0)

include(CTest)
enable_testing()

# See if boost is installed
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIRS}")
    message(STATUS "Boost_LIBRARIES ${Boost_LIBRARIES}")
    include_directories(
        ${Boost_INCLUDE_DIRS}
        ${Boost_LIBRARY_DIRS})
    # Compile options for Boost
    add_compile_options(-lunit_test_framework)
endif()

add_executable(urlRegex main.cpp RegexTool.cpp RegexTool.h RegexToolTest.cpp)

target_link_libraries(urlRegex
PUBLIC ${Boost_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Add tests, COMMAND can be the target
add_test(
    NAME urlRegex
    COMMAND urlRegex --log_level=message --color_output=yes
)