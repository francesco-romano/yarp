# Copyright: (C) 2009, 2010 RobotCub Consortium
# Authors: Paul Fitzpatrick, Giorgio Metta, Lorenzo Natale
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.8.9)

# CMake Policy CMP0020 (Automatically link Qt executables to qtmain
# target on Windows). Can be removed with
# CMAKE_MINIMUM_REQUIRED_VERSION 2.8.11 or later.
if(NOT ${CMAKE_MINIMUM_REQUIRED_VERSION} VERSION_LESS 2.8.11)
  message(AUTHOR_WARNING "CMake Policy CMP0020 is now NEW by default. You can remove this.")
endif()
if(NOT ${CMAKE_VERSION} VERSION_LESS 2.8.11)
  cmake_policy(SET CMP0020 NEW)
endif()

project(YARP)

# Pick up our CMake scripts - they are all in the conf subdirectory.
set(YARP_MODULE_DIR ${PROJECT_SOURCE_DIR}/conf)
set(YARP_MODULE_PATH ${YARP_MODULE_DIR})
if(${CMAKE_VERSION} VERSION_LESS 3.1)
  list(APPEND YARP_MODULE_PATH ${PROJECT_SOURCE_DIR}/conf/cmake-3.1/Modules)
endif()
set(CMAKE_MODULE_PATH ${YARP_MODULE_PATH})

# The YARP build is fairly flexible, so we need to accumulate some
# information about what libraries are being built (and their dependencies),
# along with the include path.  We'll be passing that along to clients
# using YARP without a global install.
set_property(GLOBAL PROPERTY YARP_TREE_BUILD TRUE)
set_property(GLOBAL PROPERTY YARP_TREE_INCLUDE_DIRS)
set_property(GLOBAL PROPERTY YARP_TREE_LINK_DIRS)
set_property(GLOBAL PROPERTY YARP_LIBS)
set_property(GLOBAL PROPERTY YARP_DEFS)

# Get the current YARP version.
# See conf/YarpVersion.cmake.
include(YarpVersion)

# Check system configuration.
# See conf/YarpSystemCheck.cmake.
include(YarpSystemCheck)

# Set up some compilation options.
# See conf/YarpOptions.cmake.
include(YarpOptions)

# Include some workarounds for older cmake versions
# See conf/YarpCMakeWorkarounds.cmake
include(YarpCMakeWorkarounds)

# Check YARP dependencies, find necessary libraries.
# See conf/YarpFindDependencies.cmake.
include(YarpFindDependencies)

# Make some custom header files containing compile settings.
add_subdirectory(conf)

# Build required 3rd party libraries
add_subdirectory(extern)

# Set up the main libraries and utilities.
add_subdirectory(src)

# Offer to compile bindings
add_subdirectory(bindings)

# Tests
add_subdirectory(tests)

# add rule for uninstalling YARP
# see conf/YARPUninstall.cmake
include(YARPUninstall)

# Output configuration files describing how YARP was built, and how
# its libraries may be linked against and used.  This uses
# information accumulated in the global property variables set up earlier.
# See conf/YarpDescribe.cmake
include(YarpDescribe)

# Set up some packaging properties (for generating tarballs etc).
# See conf/YarpPackage.cmake
include(YarpPackage)

# Make documentation target, documentation goes in doc subdirectory
# of build.  See conf/YarpDoc.cmake
include(YarpDoc)

# Make sure yarp.i and related source files get installed, to allow
# bindings for other languages to be compiled from the build material.
include(YarpBindingsSource)

# Modify this file when you add/remove files in directories that
# are scanned for sources.  This will make sure that cmake gets rerun
# for everybody.
# Touch: 1 2 3 4 5
