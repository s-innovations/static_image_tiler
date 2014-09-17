cmake_minimum_required(VERSION 3.0)

project( StaticImageTiler )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( StaticImageTiler src/static_image_tiler.cpp )
target_link_libraries( StaticImageTiler ${OpenCV_LIBS} )

set_target_properties(StaticImageTiler PROPERTIES VS_DOTNET_REFERENCES "System;System.Core")
set_target_properties(StaticImageTiler PROPERTIES COMPILE_FLAGS "/clr /EHa")
set_target_properties(StaticImageTiler PROPERTIES DEBUG_POSTFIX "d")


if(CMAKE_CXX_FLAGS_DEBUG MATCHES "/RTC1")
   string(REPLACE "/RTC1" " " CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()

if(CMAKE_CXX_FLAGS MATCHES "/EHsc")
   string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()