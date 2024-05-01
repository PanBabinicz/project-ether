# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/babinicz/esp/esp-idf/components/bootloader/subproject"
  "/Users/babinicz/Documents/workspace/project-ether/app/ether/build/bootloader"
  "/Users/babinicz/Documents/workspace/project-ether/app/ether/build/bootloader-prefix"
  "/Users/babinicz/Documents/workspace/project-ether/app/ether/build/bootloader-prefix/tmp"
  "/Users/babinicz/Documents/workspace/project-ether/app/ether/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/babinicz/Documents/workspace/project-ether/app/ether/build/bootloader-prefix/src"
  "/Users/babinicz/Documents/workspace/project-ether/app/ether/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/babinicz/Documents/workspace/project-ether/app/ether/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/babinicz/Documents/workspace/project-ether/app/ether/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
