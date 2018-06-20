cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 14)
#------------------- Core --------------------
project(Interpreter-Core)

add_subdirectory(src/core)
add_library(${PROJECT_NAME} ${core_src})
target_include_directories(${PROJECT_NAME} PUBLIC src/core)

if(${CMAKE_VERSION} VERSION_GREATER "3.8.0")
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src/core 
		PREFIX "core"
		FILES ${core_src})
endif()


#------------------- Tests -------------------
set(INTERPRETER_TESTS FALSE CACHE BOOL "Enable/Disable build of test executable")
if(INTERPRETER_TESTS)
	message(STATUS "------------- Building tests.... ------------")
	include(googletest-download.CMakeLists.txt)
	
	project(Interpreter-Tests)
	add_subdirectory(src/tests)
	
	add_executable(${PROJECT_NAME} ${tests_src})
	target_include_directories(${PROJECT_NAME} PUBLIC src/tests)
	target_link_libraries(${PROJECT_NAME} gtest_main Interpreter-Core)
	
	if(${CMAKE_VERSION} VERSION_GREATER "3.8.0")
		source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src/tests 
			PREFIX "tests" 
			FILES ${tests_src})
	endif()
endif()


#----------------- nan Adapter -------------
set(INTERPRETER_NAN FALSE CACHE BOOL "Only enable, if built via cmake-js")
if(INTERPRETER_NAN)
	message(STATUS "------------ Building nan adapter... ---------")
	project(InterpreterNan)
	
	add_subdirectory(src/nan)
	add_library(${PROJECT_NAME} SHARED ${nan_src})
	target_include_directories(${PROJECT_NAME} PUBLIC src/nan)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src/nan
		PREFIX "nan"
		FILES ${nan_src})
	
	# Gives our library file a .node extension without any "lib" prefix
	set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
	
	# Essential include files to build a node addon,
	# You should add this line in every CMake.js based project
	target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
	target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})
	
	target_link_libraries(${PROJECT_NAME} Interpreter-Core)
endif()