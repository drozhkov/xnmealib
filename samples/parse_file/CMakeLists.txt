cmake_minimum_required(VERSION 2.8.11)

######## project
project(parse-file)


######## source files
set(source_files
	main.c
)


######## dependencies
set(LINK_LIBS xnmea)

include_directories(
	../../include
)


######## output
add_executable(${PROJECT_NAME} ${source_files})


######## link
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
