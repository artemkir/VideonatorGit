
cmake_minimum_required(VERSION 2.8)




set (OGG_INCLUDE_DIR "TheoraPlayer/TheoraOgg/include/")
set (VORBIS_INCLUDE_DIR "TheoraPlayer/TheoraVorbis/include/")
set (THEORA_INCLUDE_DIR "TheoraPlayer/TheoraTheora/include/")
set (PLAYER_INCLUDE_DIR "TheoraPlayer/include/")



# -----------------------------------------------------------------
add_subdirectory("TheoraPlayer/" "TheoraPlayer/build")

set (SOURCE_DIR "src/")
set (SOURCE_FILES
	"${SOURCE_DIR}/JetdogsTheoraDataStream.cpp"
	"${SOURCE_DIR}/JetdogsVideoDLLmain.cpp"
	"${SOURCE_DIR}/JetdogsVideoManager.cpp"
	)
	
include_directories(
	"include/"
	"${PLAYER_INCLUDE_DIR}"
	)
link_directories(
	${THEORA_PLAYER_BINARY_DIR}
	${THEORA_OGG_BINARY_DIR}
	${THEORA_THEORA_BINARY_DIR}
	${THEORA_VORBIS_BINARY_DIR}	
	)
	
	
add_definitions(-DTHEORAPLUGIN_DLL_EXPORTS)
add_definitions(-DTHEORAVIDEO_STATIC)
add_definitions(-DTHEORAVIDEO_PLUGIN_EXPORTS)


if(APPLE)
	set(XCODE_ATTRIBUTE_SDKROOT macosx)
	set(CMAKE_OSX_SYSROOT macosx)
	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.4)
	set(CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD_32_BIT))
endif()

	
project(THEORA_PLUGIN)

#if(THEORA_STATIC)
#	add_library(THEORA_PLUGIN STATIC ${SOURCE_FILES})
#else()
	add_library(THEORA_PLUGIN SHARED ${SOURCE_FILES})
#endif()

set_target_properties(THEORA_PLUGIN PROPERTIES DEBUG_OUTPUT_NAME "Plugin_JetdogsVideo_d")
set_target_properties(THEORA_PLUGIN PROPERTIES RELEASE_OUTPUT_NAME "Plugin_JetdogsVideo")
target_link_libraries(THEORA_PLUGIN debug "libtheoraplayer_d")

if(NOT WIN32)
	target_link_libraries(THEORA_PLUGIN optimized "GL")
	target_link_libraries(THEORA_PLUGIN optimized "pthread")
endif()

target_link_libraries(THEORA_PLUGIN optimized "THEORA_OGG")
target_link_libraries(THEORA_PLUGIN optimized "THEORA_THEORA")
target_link_libraries(THEORA_PLUGIN optimized "THEORA_VORBIS")
target_link_libraries(THEORA_PLUGIN optimized "THEORA_PLAYER")


add_dependencies(THEORA_PLUGIN THEORA_PLAYER)

