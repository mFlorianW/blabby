################################################################################
# Build
################################################################################
add_executable(test_plugin_core_multimedia_plugin)

target_sources(test_plugin_core_multimedia_plugin
    PRIVATE
        src/MultimediaPluginShould.cpp
)

#target_include_directories(test_media_server_plugin_media_server_model
#)

target_link_libraries(test_plugin_core_multimedia_plugin
    Blabby::PluginCore
    Qt5::Test
)

add_test(test_plugin_core_multimedia_plugin ${EXECUTABLE_OUTPUT_PATH}/test_plugin_core_multimedia_plugin)
