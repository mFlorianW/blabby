// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "SCPDStateVariable.hpp"

namespace UPnPAV
{

SCPDStateVariable createTransportStateVariable() noexcept;

SCPDStateVariable createTransportStatusVariable() noexcept;

SCPDStateVariable createPlaybackStorageMediumVariable() noexcept;

SCPDStateVariable createRecordStorageMediumVariable() noexcept;

SCPDStateVariable createPossiblePlaybackStorageMediaVariable() noexcept;

SCPDStateVariable createPossibleRecordStorageMediumVariable() noexcept;

SCPDStateVariable createCurrentPlayModeVariable() noexcept;

SCPDStateVariable createTransportPlaySpeedVariable() noexcept;

SCPDStateVariable createRecordMediumWriteStatusVariable() noexcept;

SCPDStateVariable createCurrentRecordQualityModeVariable() noexcept;

SCPDStateVariable createPossibleRecordQualityModesVariable() noexcept;

SCPDStateVariable createNumberOfTracksVariable() noexcept;

SCPDStateVariable createCurrentTrackVariable() noexcept;

SCPDStateVariable createCurrentTrackDurationVariable() noexcept;

SCPDStateVariable createCurrentMediaDurationVariable() noexcept;

SCPDStateVariable createCurrentTrackMetaDataVariable() noexcept;

SCPDStateVariable createCurrentTrackURIVariable() noexcept;

SCPDStateVariable createAVTransportURIVariable() noexcept;

SCPDStateVariable createAVTransportURIMetaDataVariable() noexcept;

SCPDStateVariable createNextAVTransportURIVariable() noexcept;

SCPDStateVariable createNextAVTransportURIMetaDataVariable() noexcept;

SCPDStateVariable createRelativeTimePositionVariable() noexcept;

SCPDStateVariable createAbsoluteTimePositionVariable() noexcept;

SCPDStateVariable createRelativeCounterPositionVariable() noexcept;

SCPDStateVariable createAbsoluteCounterPositionVariable() noexcept;

SCPDStateVariable createLastChangeVariable() noexcept;

SCPDStateVariable createA_ARG_TYPE_SeekModeVariable() noexcept;

SCPDStateVariable createA_ARG_TYPE_SeekTargetVariable() noexcept;

SCPDStateVariable createA_ARG_TYPE_InstanceIDVariable() noexcept;

} // namespace UPnPAV
