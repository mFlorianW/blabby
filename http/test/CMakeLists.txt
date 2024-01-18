find_package(Qt6Test 6.5 REQUIRED)

###############################################################################
# Test Provider
###############################################################################
add_executable(test_http_server_should)
target_sources(test_http_server_should
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ServerShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ServerShould.hpp
)
target_link_libraries(test_http_server_should
    Qt6::Test
    Blabby::Http
)
add_test(test_http_server_should ${EXECUTABLE_OUTPUT_PATH}/test_http_server_should)

