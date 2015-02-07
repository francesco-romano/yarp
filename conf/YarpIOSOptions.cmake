# Currently I'm not able to use CMake to properly generate an Xcode iOS project. These macros are only for reference
macro(yarp_iOS_set_SDKRoot)
    #starting from OSX_SDKROOT which is automatically found by cmake
    #The value of this variable should be set prior to the first project() or enable_language() ????
    set(IOS_SDK_ROOT ${CMAKE_OSX_SYSROOT})
    message("------- Xcode root is ${CMAKE_OSX_SYSROOT}")
    set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk")    
endmacro()


macro(yarp_ios_set_options)
#disable shared libraries
set(CREATE_SHARED_LIBRARY FALSE)

set(SKIP_ACE TRUE)
set(YARP_USE_READLINE FALSE)
set(YARP_WRAP_STL_STRING FALSE)

endmacro()
