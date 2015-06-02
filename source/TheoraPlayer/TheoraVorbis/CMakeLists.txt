
cmake_minimum_required(VERSION 2.8)


set (SOURCE_DIR "lib/")
set (SOURCE_FILES
	"${SOURCE_DIR}/analysis.c"
	"${SOURCE_DIR}/bitrate.c"
	"${SOURCE_DIR}/block.c"
	"${SOURCE_DIR}/codebook.c"
	"${SOURCE_DIR}/envelope.c"
	"${SOURCE_DIR}/floor0.c"
	"${SOURCE_DIR}/floor1.c"
	"${SOURCE_DIR}/info.c"
	"${SOURCE_DIR}/lookup.c"
	"${SOURCE_DIR}/lpc.c"
	"${SOURCE_DIR}/lsp.c"
	"${SOURCE_DIR}/mapping0.c"
	"${SOURCE_DIR}/mdct.c"
	"${SOURCE_DIR}/psy.c"
	"${SOURCE_DIR}/registry.c"
	"${SOURCE_DIR}/res0.c"
	"${SOURCE_DIR}/sharedbook.c"
	"${SOURCE_DIR}/smallft.c"
	"${SOURCE_DIR}/synthesis.c"
	"${SOURCE_DIR}/vorbisenc.c"
	"${SOURCE_DIR}/window.c"
	)

set (OGG_INCLUDE_DIR "../TheoraOgg/include/")
set (INCLUDE_DIR 
	"include/"
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

	
PROJECT(THEORA_VORBIS)
include_directories(${INCLUDE_DIR})
add_library(THEORA_VORBIS STATIC ${SOURCE_FILES})	
set_target_properties(THEORA_VORBIS PROPERTIES DEBUG_OUTPUT_NAME "libvorbis_static_d")
set_target_properties(THEORA_VORBIS PROPERTIES RELEASE_OUTPUT_NAME "libvorbis_static")

target_link_libraries(THEORA_VORBIS optimized "THEORA_OGG")
target_link_libraries(THEORA_VORBIS optimized "THEORA_THEORA")