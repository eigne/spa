file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(integration_testing ${srcs})
add_custom_command(
        TARGET integration_testing POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/integration_testing/src/PKBSP/inputs
        ${CMAKE_CURRENT_BINARY_DIR}/src/PKBSP/inputs)

target_link_libraries(integration_testing spa)