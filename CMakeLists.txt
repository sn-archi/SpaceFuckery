cmake_minimum_required(VERSION 2.6)

set(APP "SpaceFuckery")

set(HDRS
    ./src/SpaceFuckery.h
)

set(SRCS
    ./src/SpaceFuckery.cpp
)

project(${APP})

if(EXISTS "/usr/local/lib/OGRE/cmake")
    # We could just *assume* that developers uses this basepath : /usr/local
    set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
    set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/Samples/Common/include/")
elseif(EXISTS "/usr/lib/OGRE/cmake")
    # Otherwise, this one
    set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
    set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/Samples/Common/include/")
elseif(EXISTS "/usr/share/OGRE/cmake/modules")
    set(CMAKE_MODULE_PATH "/usr/share/OGRE/cmake/modules/;${CMAKE_MODULE_PATH}")
    set(OGRE_SAMPLES_INCLUDEPATH "$ENV{OGRE_HOME}/Samples/Common/include/")
else ()
    message(SEND_ERROR "Failed to find module path.")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "")
    # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
    # differentiation between debug and release builds.
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set(CMAKE_DEBUG_POSTFIX "_d")

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/")

find_package(OGRE REQUIRED)
find_package(OIS REQUIRED)

if(NOT OIS_FOUND)
    message(SEND_ERROR "Failed to find OIS.")
endif()

# Find Boost
# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})

set(Boost_ADDITIONAL_VERSIONS "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )

# Components that need linking
# (NB does not include header-only components like bind)
set(OGRE_BOOST_COMPONENTS thread date_time)
find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)

if (NOT Boost_FOUND)
    # Try again with the other type of libs
    set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
    find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
endif()

find_package(Boost QUIET)

# Set up referencing of Boost
include_directories(${Boost_INCLUDE_DIR})

# With 1-8 debian packages, there is a linker error with boost_system.
# The solution were to add BOOST_SYSTEM_NO_DEPRECATED at compile:
# http://stackoverflow.com/a/30877725/2180332
add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_SYSTEM_NO_DEPRECATED)
set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})

include_directories(
    ${OIS_INCLUDE_DIRS}
    ${OGRE_INCLUDE_DIRS}
    ${OGRE_SAMPLES_INCLUDEPATH}
    ${OGRE_Overlay_INCLUDE_DIRS}
    ${OGRE_Terrain_INCLUDE_DIRS}
)

add_executable(${APP} WIN32 ${HDRS} ${SRCS})

set_target_properties(${APP} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(
    ${APP}
    ${OGRE_LIBRARIES}
    ${OIS_LIBRARIES}
    ${OGRE_Overlay_LIBRARIES}
    ${OGRE_Terrain_LIBRARIES}
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/media)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

install(TARGETS ${APP}
        RUNTIME DESTINATION bin
        CONFIGURATIONS All
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/media
        DESTINATION ./
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/etc
        DESTINATION ./
)
