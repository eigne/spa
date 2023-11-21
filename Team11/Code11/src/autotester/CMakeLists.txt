file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(autotester ${srcs})
target_link_libraries(autotester spa ${AUTOTESTER_LIBRARIES})

if(UNIX AND NOT APPLE)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
endif(UNIX AND NOT APPLE)

if (NOT WIN32)
    target_link_libraries(autotester pthread)
endif()
