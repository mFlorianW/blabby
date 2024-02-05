// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

namespace UPnPAV
{
// clang-format off
constexpr char const* renderingControlNotifiy = R"(
<e:propertyset xmlns:e="urn:schemas-upnp-org:event-1-0">
    <e:property>
        <LastChange>
            &lt;Event xmlns=&quot;urn:schemas-upnp-org:metadata-1-0/RCS/&quot;&gt;&lt;InstanceID val=&quot;0&quot;&gt;&lt;Volume channel=&quot;Master&quot; val=&quot;89&quot;/&gt;&lt;Volume channel=&quot;LF&quot; val=&quot;100&quot;/&gt;&lt;Volume channel=&quot;RF&quot; val=&quot;100&quot;/&gt;&lt;Mute channel=&quot;Master&quot; val=&quot;0&quot;/&gt;&lt;Mute channel=&quot;LF&quot; val=&quot;0&quot;/&gt;&lt;Mute channel=&quot;RF&quot; val=&quot;0&quot;/&gt;&lt;Bass val=&quot;7&quot;/&gt;&lt;Treble val=&quot;4&quot;/&gt;&lt;Loudness channel=&quot;Master&quot; val=&quot;1&quot;/&gt;&lt;OutputFixed val=&quot;0&quot;/&gt;&lt;HeadphoneConnected val=&quot;0&quot;/&gt;&lt;SpeakerSize val=&quot;-1&quot;/&gt;&lt;SubGain val=&quot;0&quot;/&gt;&lt;SubCrossover val=&quot;0&quot;/&gt;&lt;SubPolarity val=&quot;0&quot;/&gt;&lt;SubEnabled val=&quot;1&quot;/&gt;&lt;SonarEnabled val=&quot;0&quot;/&gt;&lt;SonarCalibrationAvailable val=&quot;0&quot;/&gt;&lt;PresetNameList val=&quot;FactoryDefaults&quot;/&gt;&lt;/InstanceID&gt;&lt;/Event&gt;
        </LastChange>
    </e:property>
</e:propertyset>
)";
// clang-format on

} // namespace UPnPAV
