
cmake_minimum_required(VERSION 2.8)

PROJECT(THEORA_THEORA)

set (SOURCE_DIR "lib/")
set (SOURCE_FILES
	"${SOURCE_DIR}/analyze.c"
	"${SOURCE_DIR}/apiwrapper.c"
	"${SOURCE_DIR}/bitpack.c"
	"${SOURCE_DIR}/decapiwrapper.c"
	"${SOURCE_DIR}/decinfo.c"
	"${SOURCE_DIR}/decode.c"
	"${SOURCE_DIR}/dequant.c"
	"${SOURCE_DIR}/encapiwrapper.c"
	"${SOURCE_DIR}/encfrag.c"
	"${SOURCE_DIR}/encinfo.c"
	"${SOURCE_DIR}/encode.c"
	"${SOURCE_DIR}/enquant.c"
	"${SOURCE_DIR}/fragment.c"
	"${SOURCE_DIR}/huffdec.c"
	"${SOURCE_DIR}/huffenc.c"
	"${SOURCE_DIR}/idct.c"
	"${SOURCE_DIR}/info.c"
	"${SOURCE_DIR}/internal.c"
	"${SOURCE_DIR}/mathops.c"
	"${SOURCE_DIR}/mcenc.c"
	"${SOURCE_DIR}/x86_vc/mmxencfrag.c"
	"${SOURCE_DIR}/x86_vc/mmxfdct.c"
	"${SOURCE_DIR}/x86_vc/mmxfrag.c"
	"${SOURCE_DIR}/x86_vc/mmxidct.c"
	"${SOURCE_DIR}/x86_vc/mmxstate.c"
	"${SOURCE_DIR}/quant.c"
	"${SOURCE_DIR}/rate.c"
	"${SOURCE_DIR}/state.c"
	"${SOURCE_DIR}/tokenize.c"
	"${SOURCE_DIR}/x86_vc/x86cpu.c"
	"${SOURCE_DIR}/x86_vc/x86enc.c"
	"${SOURCE_DIR}/x86_vc/x86state.c"
	)

set (OGG_INCLUDE_DIR "../TheoraOgg/include/")
set (INCLUDE_DIR 
	"include/"
	"include/theora/"
	"${SOURCE_DIR}"
	"${OGG_INCLUDE_DIR}"
	"${OGG_INCLUDE_DIR}ogg/"
	)

if(APPLE)
	set(XCODE_ATTRIBUTE_SDKROOT macosx)
	set(CMAKE_OSX_SYSROOT macosx)
	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.4)
	set(CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD_32_BIT))
endif()

	
include_directories(${INCLUDE_DIR})

add_library(THEORA_THEORA STATIC ${SOURCE_FILES})	

set_target_properties(THEORA_THEORA PROPERTIES DEBUG_OUTPUT_NAME "libtheora_static_d")
set_target_properties(THEORA_THEORA PROPERTIES RELEASE_OUTPUT_NAME "libtheora_static")

