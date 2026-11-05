find_package(Qt6Test 6.5 REQUIRED)

###############################################################################
# Test Helper
###############################################################################
add_library(test_artwork_test_helper STATIC)
add_library(Blabby::ArtworkTestHelper ALIAS test_artwork_test_helper)
target_sources(test_artwork_test_helper
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/SchedulerDouble.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SchedulerDouble.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/HttpBackendDouble.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/HttpBackendDouble.hpp
)
target_include_directories(test_artwork_test_helper
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(test_artwork_test_helper
    PUBLIC
        Blabby::Artwork
)

###############################################################################
# Test OnlineLookup
###############################################################################
add_executable(test_artwork_online_lookup)
target_sources(test_artwork_online_lookup
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/OnlineLookupShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OnlineLookupShould.hpp
)
target_link_libraries(test_artwork_online_lookup
    Qt6::Test
    Blabby::ArtworkTestHelper
)
add_test(test_artwork_online_lookup ${EXECUTABLE_OUTPUT_PATH}/test_artwork_online_lookup)
