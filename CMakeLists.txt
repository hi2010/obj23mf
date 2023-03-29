cmake_minimum_required (VERSION 3.2)

project(obj23mf)
#region import all needed for lib3mf(core) (CopySharedLibrary call on the target(s) is still needed afterwards)
#set(CMAKE_CXX_STANDARD 11)

if (WIN32)
	set(LSUFFIX "dll")
	set(LSUFFIXOUT ".dll")
elseif(UNIX AND NOT APPLE)
	set(LSUFFIX "so")
	set(LSUFFIXOUT ".so.2")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2")
else()
	set(LSUFFIX "dylib")
	set(LSUFFIXOUT ".2.dylib")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2")
endif()

#add_definitions( -DTEXTURESPATH="${CMAKE_CURRENT_SOURCE_DIR}/../Files/Textures/" )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/lib3mf/Bindings/Cpp)


if (WIN32)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/lib3mf/Lib)
	link_libraries(lib3mf)
	SET(3mfName ${lib3mf})
else()
	# Unix prefixes the name of the library with "lib" anyway
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/lib3mf/Bin)
	link_libraries(3mf)
	link_directories(${CMAKE_CURRENT_BINARY_DIR})
	SET(3mfName ${3mf})
endif()

function(CopySharedLibrary MyTarget)
	add_custom_command(TARGET ${MyTarget} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/external/lib3mf/Bin/lib3mf.${LSUFFIX}" $<TARGET_FILE_DIR:${MyTarget}>/lib3mf${LSUFFIXOUT})
endfunction(CopySharedLibrary)

if (${MSVC})
	IF(${CMAKE_VERSION} VERSION_LESS 3.6.3)
		MESSAGE ("Note: You need to manually select a StartUp-project in Visual Studio.")
	ELSE()
		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Example_Cube)
	ENDIF()
endif()

set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")

#endregion
set(OBJLOADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/OBJ-Loader-master/Source)
set(STLLOADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/stl-loader/src)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cpp "external/stl-loader/src/stl_binary_loader.h" "src/SimpleLogger.h")
# add lib3mf.so.2
CopySharedLibrary(${PROJECT_NAME})
#add_custom_command(TARGET testex POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${VHACD_DIR}/build/libvhacd.a" $<TARGET_FILE_DIR:testex>/libvhacd.a)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/inc ${OBJLOADER_DIR} ${STLLOADER_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
SET(GCC_COVERAGE_COMPILE_FLAGS "-g -pthread -O0")
set(CMAKE_CXX_COMPILER_ID "g++")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

#install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#install(FILES lib3mf.so.2 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/external/lib3mf/Bin/lib3mf${LSUFFIXOUT} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#install(TARGETS ${3mf} LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#install(TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/build/lib3mf.so.2 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#install(TARGETS ${3mfName}
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib/myproject)


#include(CPack)