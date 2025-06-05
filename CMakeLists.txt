# refactor cmake.
cmake_minimum_required(VERSION 2.8.12...3.13)

set(CMAKE_CXX_STANDARD 17)
set(CXX_FILESYSTEM_LIBRARIES "stdc++fs")

project(FlowIndicatorTool)

file(
     GLOB COMMON_LOGIC
     common_logic/src/*.cpp
     common_logic/include/*.h
     )
	 
file(
      GLOB FLOW_INDICATOR_CONSOLE_FILES
      flow_indicator/src/*.cpp
      flow_indicator/include/*.h
     )
include_directories(common_logic/include)
include_directories(flow_indicator/include)

if(WIN32)
    add_library(common_logic SHARED ${COMMON_LOGIC})
else()
    add_library(common_logic STATIC ${COMMON_LOGIC})
endif()

target_compile_definitions(common_logic PUBLIC _CRT_SECURE_NO_WARNINGS)

# indicator  
add_executable(flow_indicator_app ${FLOW_INDICATOR_CONSOLE_FILES})
target_link_libraries(flow_indicator_app )


message("Building flow indicator tool")