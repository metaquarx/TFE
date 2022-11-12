# SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
# SPDX-License-Identifier: GPL-3.0-only

# Setup

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(TFE VERSION 0.1.0 LANGUAGES CXX)

# Build type

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE "RELEASE" CACHE
      STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Subdirectories

add_subdirectory(src)
