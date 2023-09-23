# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

find_package(Qt5Test 5.13 REQUIRED)

###############################################################################
# Subdirectories
###############################################################################
add_subdirectory(doubles)
add_subdirectory(media_server_model)
add_subdirectory(main_window)
add_subdirectory(server_item_model)
add_subdirectory(server_item_view)
