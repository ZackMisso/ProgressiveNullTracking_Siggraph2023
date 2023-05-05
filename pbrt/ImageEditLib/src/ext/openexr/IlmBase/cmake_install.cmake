# Install script for directory: /Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/IlmBase

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Library/Developer/CommandLineTools/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES "/Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/openexr/IlmBase/config/IlmBaseConfig.h")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/openexr/IlmBase/Half/cmake_install.cmake")
  include("/Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/openexr/IlmBase/Iex/cmake_install.cmake")
  include("/Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/openexr/IlmBase/IexMath/cmake_install.cmake")
  include("/Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/openexr/IlmBase/Imath/cmake_install.cmake")
  include("/Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/openexr/IlmBase/IlmThread/cmake_install.cmake")

endif()

