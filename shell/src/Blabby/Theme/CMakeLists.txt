# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later
################################################################################
# QML Module
################################################################################
set_source_files_properties(Theme.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(${PROJECT_NAME}Theme
    URI
        "Blabby.Theme"
    VERSION
        1.0
    STATIC
    QML_FILES
        Theme.qml
)
add_library(BlabbyShell::Theme ALIAS ${PROJECT_NAME}Theme)
add_library(BlabbyShell::ThemePlugin ALIAS ${PROJECT_NAME}Themeplugin)
