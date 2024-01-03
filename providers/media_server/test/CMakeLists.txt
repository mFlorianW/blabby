find_package(Qt6 6.5 REQUIRED COMPONENTS Test)

###############################################################################
# Test MediaServerSource
###############################################################################
add_executable(test_provider_mediaserversource)
target_sources(test_provider_mediaserversource
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerSourceShould.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerSourceShould.hpp
)
target_link_libraries(test_provider_mediaserversource
    PRIVATE
        Qt6::Test
        Blabby::MediaServerProvider
        Blabby::UPnPAVTestHelper
)
add_test(test_provider_mediaserversource ${EXECUTABLE_OUTPUT_PATH}/test_provider_mediaserversource)

###############################################################################
# Test MediaServerProvider
###############################################################################
add_executable(test_provider_mediaserverprovider)
target_sources(test_provider_mediaserverprovider
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerProviderShould.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerProviderShould.hpp
)
target_link_libraries(test_provider_mediaserverprovider
    PRIVATE
        Qt6::Test
        Blabby::MediaServerProvider
        Blabby::UPnPAVTestHelper
)
add_test(test_provider_mediaserverprovider ${EXECUTABLE_OUTPUT_PATH}/test_provider_mediaserverprovider)
