# Install script for directory: /Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf

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
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libIlmImf.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/lib" TYPE STATIC_LIBRARY FILES "/Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/openexr/OpenEXR/IlmImf/libIlmImf.a")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/libIlmImf.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/libIlmImf.a")
    execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/ranlib" "$ENV{DESTDIR}/usr/local/lib/libIlmImf.a")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/OpenEXR/ImfForward.h;/usr/local/include/OpenEXR/ImfExport.h;/usr/local/include/OpenEXR/ImfAttribute.h;/usr/local/include/OpenEXR/ImfBoxAttribute.h;/usr/local/include/OpenEXR/ImfCRgbaFile.h;/usr/local/include/OpenEXR/ImfChannelList.h;/usr/local/include/OpenEXR/ImfChannelListAttribute.h;/usr/local/include/OpenEXR/ImfCompressionAttribute.h;/usr/local/include/OpenEXR/ImfDoubleAttribute.h;/usr/local/include/OpenEXR/ImfFloatAttribute.h;/usr/local/include/OpenEXR/ImfFrameBuffer.h;/usr/local/include/OpenEXR/ImfHeader.h;/usr/local/include/OpenEXR/ImfIO.h;/usr/local/include/OpenEXR/ImfInputFile.h;/usr/local/include/OpenEXR/ImfIntAttribute.h;/usr/local/include/OpenEXR/ImfLineOrderAttribute.h;/usr/local/include/OpenEXR/ImfMatrixAttribute.h;/usr/local/include/OpenEXR/ImfOpaqueAttribute.h;/usr/local/include/OpenEXR/ImfOutputFile.h;/usr/local/include/OpenEXR/ImfRgbaFile.h;/usr/local/include/OpenEXR/ImfStringAttribute.h;/usr/local/include/OpenEXR/ImfVecAttribute.h;/usr/local/include/OpenEXR/ImfHuf.h;/usr/local/include/OpenEXR/ImfWav.h;/usr/local/include/OpenEXR/ImfLut.h;/usr/local/include/OpenEXR/ImfArray.h;/usr/local/include/OpenEXR/ImfCompression.h;/usr/local/include/OpenEXR/ImfLineOrder.h;/usr/local/include/OpenEXR/ImfName.h;/usr/local/include/OpenEXR/ImfPixelType.h;/usr/local/include/OpenEXR/ImfVersion.h;/usr/local/include/OpenEXR/ImfXdr.h;/usr/local/include/OpenEXR/ImfConvert.h;/usr/local/include/OpenEXR/ImfPreviewImage.h;/usr/local/include/OpenEXR/ImfPreviewImageAttribute.h;/usr/local/include/OpenEXR/ImfChromaticities.h;/usr/local/include/OpenEXR/ImfChromaticitiesAttribute.h;/usr/local/include/OpenEXR/ImfKeyCode.h;/usr/local/include/OpenEXR/ImfKeyCodeAttribute.h;/usr/local/include/OpenEXR/ImfTimeCode.h;/usr/local/include/OpenEXR/ImfTimeCodeAttribute.h;/usr/local/include/OpenEXR/ImfRational.h;/usr/local/include/OpenEXR/ImfRationalAttribute.h;/usr/local/include/OpenEXR/ImfFramesPerSecond.h;/usr/local/include/OpenEXR/ImfStandardAttributes.h;/usr/local/include/OpenEXR/ImfEnvmap.h;/usr/local/include/OpenEXR/ImfEnvmapAttribute.h;/usr/local/include/OpenEXR/ImfInt64.h;/usr/local/include/OpenEXR/ImfRgba.h;/usr/local/include/OpenEXR/ImfTileDescription.h;/usr/local/include/OpenEXR/ImfTileDescriptionAttribute.h;/usr/local/include/OpenEXR/ImfTiledInputFile.h;/usr/local/include/OpenEXR/ImfTiledOutputFile.h;/usr/local/include/OpenEXR/ImfTiledRgbaFile.h;/usr/local/include/OpenEXR/ImfRgbaYca.h;/usr/local/include/OpenEXR/ImfTestFile.h;/usr/local/include/OpenEXR/ImfThreading.h;/usr/local/include/OpenEXR/ImfB44Compressor.h;/usr/local/include/OpenEXR/ImfStringVectorAttribute.h;/usr/local/include/OpenEXR/ImfMultiView.h;/usr/local/include/OpenEXR/ImfAcesFile.h;/usr/local/include/OpenEXR/ImfMultiPartOutputFile.h;/usr/local/include/OpenEXR/ImfGenericOutputFile.h;/usr/local/include/OpenEXR/ImfMultiPartInputFile.h;/usr/local/include/OpenEXR/ImfGenericInputFile.h;/usr/local/include/OpenEXR/ImfPartType.h;/usr/local/include/OpenEXR/ImfPartHelper.h;/usr/local/include/OpenEXR/ImfOutputPart.h;/usr/local/include/OpenEXR/ImfTiledOutputPart.h;/usr/local/include/OpenEXR/ImfInputPart.h;/usr/local/include/OpenEXR/ImfTiledInputPart.h;/usr/local/include/OpenEXR/ImfDeepScanLineOutputFile.h;/usr/local/include/OpenEXR/ImfDeepScanLineOutputPart.h;/usr/local/include/OpenEXR/ImfDeepScanLineInputFile.h;/usr/local/include/OpenEXR/ImfDeepScanLineInputPart.h;/usr/local/include/OpenEXR/ImfDeepTiledInputFile.h;/usr/local/include/OpenEXR/ImfDeepTiledInputPart.h;/usr/local/include/OpenEXR/ImfDeepTiledOutputFile.h;/usr/local/include/OpenEXR/ImfDeepTiledOutputPart.h;/usr/local/include/OpenEXR/ImfDeepFrameBuffer.h;/usr/local/include/OpenEXR/ImfDeepCompositing.h;/usr/local/include/OpenEXR/ImfCompositeDeepScanLine.h;/usr/local/include/OpenEXR/ImfNamespace.h;/usr/local/include/OpenEXR/ImfMisc.h;/usr/local/include/OpenEXR/ImfDeepImageState.h;/usr/local/include/OpenEXR/ImfDeepImageStateAttribute.h;/usr/local/include/OpenEXR/ImfFloatVectorAttribute.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/include/OpenEXR" TYPE FILE FILES
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfForward.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfExport.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfBoxAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfCRgbaFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfChannelList.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfChannelListAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfCompressionAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfDoubleAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfFloatAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfFrameBuffer.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfHeader.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfIO.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfInputFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfIntAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfLineOrderAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfMatrixAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfOpaqueAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfOutputFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfRgbaFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfStringAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfVecAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfHuf.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfWav.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfLut.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfArray.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfCompression.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfLineOrder.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfName.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfPixelType.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfVersion.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfXdr.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfConvert.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfPreviewImage.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfPreviewImageAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfChromaticities.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfChromaticitiesAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfKeyCode.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfKeyCodeAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfTimeCode.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfTimeCodeAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfRational.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfRationalAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfFramesPerSecond.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfStandardAttributes.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfEnvmap.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfEnvmapAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfInt64.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfRgba.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfTileDescription.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfTileDescriptionAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfTiledInputFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfTiledOutputFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfTiledRgbaFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfRgbaYca.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfTestFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfThreading.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfB44Compressor.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfStringVectorAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfMultiView.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfAcesFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfMultiPartOutputFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfGenericOutputFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfMultiPartInputFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfGenericInputFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfPartType.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfPartHelper.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfOutputPart.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfTiledOutputPart.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfInputPart.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfTiledInputPart.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfDeepScanLineOutputFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfDeepScanLineOutputPart.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfDeepScanLineInputFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfDeepScanLineInputPart.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfDeepTiledInputFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfDeepTiledInputPart.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfDeepTiledOutputFile.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfDeepTiledOutputPart.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfDeepFrameBuffer.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfDeepCompositing.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfCompositeDeepScanLine.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfNamespace.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfMisc.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfDeepImageState.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfDeepImageStateAttribute.h"
    "/Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/openexr/OpenEXR/IlmImf/ImfFloatVectorAttribute.h"
    )
endif()

