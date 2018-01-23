cmake_minimum_required(VERSION 2.8.11)

######## project
project(xnmea)

set(TOOLSET cc)

if(MSVC)
	set(TOOLSET msvc-${MSVC_VERSION})
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
	${CMAKE_CURRENT_LIST_DIR}/lib/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${TOOLSET})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
	${CMAKE_CURRENT_LIST_DIR}/bin/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${TOOLSET})

add_subdirectory(samples)


######## source files
set(source_files
	src/context.c
	src/generate.c
	src/generator.c
	src/gmath.c
	src/info.c
	src/parse.c
	src/parser.c
	src/sentence.c
	src/time.c
	src/tok.c
)


######## dependencies
set(LINK_LIBS)

include_directories(
	./include
)


######## output
add_library(${PROJECT_NAME} ${source_files})
