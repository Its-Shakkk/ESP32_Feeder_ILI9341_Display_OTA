# =========================================================================
# Feeder + OTA — project-level CMakeLists.txt
# =========================================================================
cmake_minimum_required(VERSION 3.16)

# Pull in the IDF build system. This defines `project()`, `idf_component_register()`, etc.
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Project name — free choice, becomes the .elf/.bin filename base
project(ESP32_Feeder_OTA)