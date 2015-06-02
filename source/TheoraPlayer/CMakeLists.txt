
cmake_minimum_required(VERSION 2.8)

add_subdirectory("TheoraOgg/" "TheoraOgg/build/")
add_subdirectory("TheoraTheora/" "TheoraTheora/build/")
add_subdirectory("TheoraVorbis/" "TheoraVorbis/build/")



set (SOURCE_DIR "src/")
set (SOURCE_FILES
	"${SOURCE_DIR}/TheoraAsync.cpp"
	"${SOURCE_DIR}/TheoraAudioInterface.cpp"
	"${SOURCE_DIR}/TheoraDataSource.cpp"
	"${SOURCE_DIR}/TheoraException.cpp"
	"${SOURCE_DIR}/TheoraFrameQueue.cpp"
	"${SOURCE_DIR}/TheoraTimer.cpp"
	"${SOURCE_DIR}/TheoraUtil.cpp"
	"${SOURCE_DIR}/TheoraVideoClip.cpp"
	"${SOURCE_DIR}/TheoraVideoFrame.cpp"
	"${SOURCE_DIR}/TheoraVideoManager.cpp"
	"${SOURCE_DIR}/TheoraWorkerThread.cpp"
	)


set (OGG_INCLUDE_DIR "TheoraOgg/include/")
set (VORBIS_INCLUDE_DIR "TheoraVorbis/include/")
set (THEORA_INCLUDE_DIR "TheoraTheora/include/")

set (INCLUDE_DIR 
	"include/"
	"${VORBIS_INCLUDE_DIR}"
	"${THEORA_INCLUDE_DIR}"
	"${OGG_INCLUDE_DIR}"
	"${OGG_INCLUDE_DIR}ogg/"
	)

set (LIB_DIR
	${THEORA_OGG_BINARY_DIR}
	${THEORA_THEORA_BINARY_DIR}
	${THEORA_VORBIS_BINARY_DIR}	
	)


include_directories(${INCLUDE_DIR})
link_directories(${LIB_DIR})
add_definitions(-DTHEORAVIDEO_EXPORTS)

if(APPLE)
	set(XCODE_ATTRIBUTE_SDKROOT macosx)
	set(CMAKE_OSX_SYSROOT macosx)
	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.4)
	set(CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD_32_BIT))
endif()

	
project(THEORA_PLAYER)

#if(THEORA_STATIC)
	add_library(THEORA_PLAYER STATIC ${SOURCE_FILES})	
#else()
#	add_library(THEORA_PLAYER SHARED ${SOURCE_FILES})	
#endif()

set_target_properties(THEORA_PLAYER PROPERTIES DEBUG_OUTPUT_NAME "libtheoraplayer_d")
set_target_properties(THEORA_PLAYER PROPERTIES RELEASE_OUTPUT_NAME "libtheoraplayer")
#target_link_libraries(THEORA_PLAYER debug "libogg_static_d")
#target_link_libraries(THEORA_PLAYER optimized "libogg_static")
#target_link_libraries(THEORA_PLAYER debug "libvorbis_static_d")
#target_link_libraries(THEORA_PLAYER optimized "libvorbis_static")
#target_link_libraries(THEORA_PLAYER debug "libtheora_static_d")
#target_link_libraries(THEORA_PLAYER optimized "libtheora_static")

target_link_libraries(THEORA_PLAYER optimized "THEORA_OGG")
target_link_libraries(THEORA_PLAYER optimized "THEORA_THEORA")
target_link_libraries(THEORA_PLAYER optimized "THEORA_VORBIS")

add_dependencies(THEORA_PLAYER THEORA_OGG THEORA_THEORA THEORA_VORBIS)


