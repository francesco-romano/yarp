# Copyright (C) 2011 Duarte Aragao
# Author: Duarte Aragao
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

# Search for source code.
file(GLOB folder_source *.cpp *.cc *.c)
file(GLOB folder_header *.h)
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})

# Automatically add include directories if needed.
foreach(header_file ${folder_header})
  get_filename_component(p ${header_file} PATH)
endforeach(header_file ${folder_header})

yarp_add_plugin(KinectYarpDeviceClientLib ${folder_source} ${folder_header})
target_link_libraries(KinectYarpDeviceClientLib ${YARP_LIBRARIES})
