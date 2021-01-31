# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

find_package(Qt5Test 5.13 REQUIRED)

###############################################################################
# Test Projects
###############################################################################
add_subdirectory(test_data)
add_subdirectory(service_provider)
add_subdirectory(media_server)
add_subdirectory(pending_soap_call)
add_subdirectory(browse_result)
add_subdirectory(media_server_object)
add_subdirectory(media_device)
