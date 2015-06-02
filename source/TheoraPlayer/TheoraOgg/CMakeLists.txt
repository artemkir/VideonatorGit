
cmake_minimum_required(VERSION 2.8)

PROJECT(THEORA_OGG)

set (SOURCE_DIR "src/")
set (SOURCE_FILES
	"${SOURCE_DIR}/bitwise.c"
	"${SOURCE_DIR}/framing.c"
	)

if(APPLE)
	set(XCODE_ATTRIBUTE_SDKROOT macosx)
	set(CMAKE_OSX_SYSROOT macosx)
	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.4)
	set(CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD_32_BIT))
endif()


include_directories ("include/")
include_directories ("include/ogg/")

add_library(THEORA_OGG STATIC ${SOURCE_FILES})	

set_target_properties(THEORA_OGG PROPERTIES DEBUG_OUTPUT_NAME "libogg_static_d")
set_target_properties(THEORA_OGG PROPERTIES RELEASE_OUTPUT_NAME "libogg_static")

