# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

find_package(Qt6 6.5 REQUIRED COMPONENTS Test)

###############################################################################
# Test Projects
###############################################################################
add_subdirectory(doubles)
add_subdirectory(multi_media_plugin_model)
add_subdirectory(main_window)
