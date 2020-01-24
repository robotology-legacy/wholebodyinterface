# Copyright: (C) 2013 CoDyCo Consortium
# Authors: Andrea Del Prete
# CopyPolicy: Released under the terms of the GNU GPL v2.0 (or any later version).
cmake_minimum_required(VERSION 2.8.11)

SET(PROJECTNAME wholeBodyInterface)
SET(LIBRARYNAME whole-body-interface)
SET(HEADERSDIRECTORY wbi)

PROJECT(${PROJECTNAME})

set (${PROJECTNAME}_VERSION_MAJOR 0)
set (${PROJECTNAME}_VERSION_MINOR 2)
set (${PROJECTNAME}_VERSION_PATCH 6)

set (${PROJECTNAME}_VERSION ${${PROJECTNAME}_VERSION_MAJOR}.${${PROJECTNAME}_VERSION_MINOR}.${${PROJECTNAME}_VERSION_PATCH})

message (STATUS "${PROJECT_NAME} version ${${PROJECTNAME}_VERSION}")

option(WBI_ENABLE_RPATH "Enable RPATH for the library" TRUE)
mark_as_advanced(WBI_ENABLE_RPATH)

SET( PROJ_SOURCE_DIR ${${PROJECTNAME}_SOURCE_DIR} )
SET( PROJ_BINARY_DIR ${${PROJECTNAME}_BINARY_DIR} )

# set the default build type
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# This shouldn't be necessary, but there has been trouble
# with MSVC being set off, but MSVCXX ON.
if(MSVC OR MSVC90 OR MSVC10)
  set(MSVC ON)
endif (MSVC OR MSVC90 OR MSVC10)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
endif(MSVC)

SET(folder_source src/wbiUtil.cpp
                  src/wbiID.cpp
                  src/wbiConstants.cpp
                  src/iWholeBodyModel.cpp
                  src/iWholeBodyStates.cpp
                  src/iWholeBodySensors.cpp
                  src/iWholeBodyActuators.cpp
                  src/wholeBodyInterface.cpp
                  src/Error.cpp)

SET(folder_header include/wbi/wbi.h
                  include/wbi/wbiConstants.h
                  include/wbi/wbiUtil.h
                  include/wbi/wbiID.h
                  include/wbi/wbiUtil.inl
                  include/wbi/iWholeBodyModel.h
                  include/wbi/iWholeBodyStates.h
                  include/wbi/iWholeBodySensors.h
                  include/wbi/iWholeBodyActuators.h
                  include/wbi/wholeBodyInterface.h
                  include/wbi/Error.h)

include_directories(include/wbi)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

if(MSVC)
    set(LIB_TYPE STATIC)
else()
    set(LIB_TYPE SHARED)
endif()

find_package(YCM REQUIRED)

include(AddInstallRPATHSupport)
add_install_rpath_support(BIN_DIRS "${CMAKE_INSTALL_PREFIX}/bin"
                          LIB_DIRS "${CMAKE_INSTALL_PREFIX}/lib"
                          DEPENDS WBI_ENABLE_RPATH
                          USE_LINK_PATH)


add_library(${LIBRARYNAME} ${LIB_TYPE} ${folder_source} ${folder_header})

SET_TARGET_PROPERTIES(${LIBRARYNAME} PROPERTIES
  SOVERSION "${${PROJECTNAME}_VERSION_MAJOR}.${${PROJECTNAME}_VERSION_MINOR}"
  VERSION "${${PROJECTNAME}_VERSION}"
  COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD}"
  PUBLIC_HEADER "${folder_header}"
  )


if(${CMAKE_MINIMUM_REQUIRED_VERSION} VERSION_GREATER "2.8.12")
    message(AUTHOR_WARNING "CMAKE_MINIMUM_REQUIRED_VERSION is now ${CMAKE_MINIMUM_REQUIRED_VERSION}. This check can be removed.")
endif()
if(CMAKE_VERSION VERSION_LESS 2.8.12)
    if(NOT WBI_DISABLE_RPATH)
        set_target_properties(${LIBRARYNAME} PROPERTIES
            INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
        )
    endif(NOT WBI_DISABLE_RPATH)
endif(CMAKE_VERSION VERSION_LESS 2.8.12)

target_include_directories(${LIBRARYNAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
                                                 "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${${VARS_PREFIX}_INSTALL_INCLUDEDIR}>")
INSTALL(TARGETS ${LIBRARYNAME}
  EXPORT ${PROJECTNAME}-target
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/${HEADERSDIRECTORY}
)

INSTALL(DIRECTORY include/${HEADERSDIRECTORY} DESTINATION include/${HEADERSDIRECTORY})

EXPORT(TARGETS ${LIBRARYNAME} FILE "${PROJECT_BINARY_DIR}/${PROJECTNAME}-target.cmake")

#EXPORT(PACKAGE ${PROJECTNAME})

CONFIGURE_FILE(cmake/${PROJECTNAME}Config.cmake.in ${PROJECT_BINARY_DIR}/${PROJECTNAME}Config.cmake @ONLY)
CONFIGURE_FILE(cmake/${PROJECTNAME}ConfigVersion.cmake.in ${PROJECT_BINARY_DIR}/${PROJECTNAME}ConfigVersion.cmake @ONLY)

INSTALL(FILES ${PROJECT_BINARY_DIR}/${PROJECTNAME}Config.cmake DESTINATION share/${PROJECTNAME})
INSTALL(FILES ${PROJECT_BINARY_DIR}/${PROJECTNAME}ConfigVersion.cmake DESTINATION share/${PROJECTNAME})
INSTALL(EXPORT ${PROJECTNAME}-target  DESTINATION share/${PROJECTNAME})

include(AddUninstallTarget)
