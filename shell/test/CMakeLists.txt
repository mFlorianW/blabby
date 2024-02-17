# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later
###############################################################################
# Dependencies
###############################################################################
find_package(Qt6Test 6.5 REQUIRED)

###############################################################################
# Test QmlTests
###############################################################################
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/AbstractInteractiveControlShould.qml
    ${CMAKE_CURRENT_BINARY_DIR}/tst_AbstractInteractiveControlShould.qml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/AbstractListEntryShould.qml
    ${CMAKE_CURRENT_BINARY_DIR}/tst_AbstractListEntryShould.qml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/AbstractSliderShould.qml
    ${CMAKE_CURRENT_BINARY_DIR}/tst_AbstractSliderShould.qml COPYONLY)

add_executable(test_shell_qmltests)
target_sources(test_shell_qmltests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/QmlTests.cpp
)
target_link_libraries(test_shell_qmltests
    PRIVATE
        Qt6::Test
        Qt6::QuickTest
        BlabbyShell::Object
)
add_test(test_shell_qmltests ${EXECUTABLE_OUTPUT_PATH}/test_shell_qmltests)
###############################################################################
# Test MediaPlayer
###############################################################################
add_executable(test_shell_mediaplayer)
target_sources(test_shell_mediaplayer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaPlayerShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaPlayerShould.hpp
)
target_link_libraries(test_shell_mediaplayer
    PRIVATE
        Qt6::Test
        BlabbyShell::Object
        Blabby::MultimediaTestHelper
        Blabby::UPnPAVTestHelper
)
add_test(test_shell_mediaplayer ${EXECUTABLE_OUTPUT_PATH}/test_shell_mediaplayer)

###############################################################################
# Test MediaRendererModel
###############################################################################
add_executable(test_shell_mediarenderer_model)
target_sources(test_shell_mediarenderer_model
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaRendererModelShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaRendererModelShould.hpp
)
target_link_libraries(test_shell_mediarenderer_model
    PRIVATE
        Qt6::Test
        BlabbyShell::Object
        Blabby::MultimediaTestHelper
        Blabby::UPnPAVTestHelper
)
add_test(test_shell_mediarenderer_model ${EXECUTABLE_OUTPUT_PATH}/test_shell_mediarenderer_model)

###############################################################################
# Test MediaItemModel
###############################################################################
add_executable(test_shell_mediaitem_model)
target_sources(test_shell_mediaitem_model
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaItemModelShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaItemModelShould.hpp
)
target_include_directories(test_shell_mediaitem_model
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/data>
)
target_link_libraries(test_shell_mediaitem_model
    PRIVATE
        Qt6::Test
        BlabbyShell::Object
        Blabby::MultimediaTestHelper
)
add_test(test_shell_mediaitem_model ${EXECUTABLE_OUTPUT_PATH}/test_shell_mediaitem_model)

###############################################################################
# Test MediaSourceModel
###############################################################################
add_executable(test_shell_mediasource_model)
target_sources(test_shell_mediasource_model
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaSourceModelShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaSourceModelShould.hpp
)
target_include_directories(test_shell_mediasource_model
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/data>
)
target_link_libraries(test_shell_mediasource_model
    PRIVATE
        Qt6::Test
        BlabbyShell::Object
        Blabby::MultimediaTestHelper
)
add_test(test_shell_mediasource_model ${EXECUTABLE_OUTPUT_PATH}/test_shell_mediasource_model)
