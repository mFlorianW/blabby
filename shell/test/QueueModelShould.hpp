// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "MediaRendererDoubleFactory.hpp"
#include "MediaRendererModel.hpp"
#include "QueueModel.hpp"
#include "ServiceProviderDouble.hpp"
#include <QObject>
#include <memory>

namespace Shell
{
class QueueModelShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~QueueModelShould() override;
    Q_DISABLE_COPY_MOVE(QueueModelShould)

private:
    void activate(QString const& name);
    UPnPAV::Doubles::MediaRendererDouble* device(QString const& name) const noexcept;

    UPnPAV::Doubles::ServiceProviderDouble* mServiceProvider = nullptr;
    UPnPAV::Doubles::MediaRendererDoubleFactory* mRendererFactory = nullptr;
    std::unique_ptr<MediaRendererModel> mRendererModel = nullptr;
    std::unique_ptr<QueueModel> mModel = nullptr;

private Q_SLOTS:
    void init();
    void cleanup();
    void be_empty_and_not_running_at_start();
    void give_the_role_names();
    void give_the_title_artist_and_artwork_of_the_entries_and_mark_the_current_entry();
    void reset_when_the_queue_is_replaced();
    void play_on_the_active_renderer_and_run_after_a_replace();
    void follow_a_switch_of_the_active_renderer();
    void only_hold_the_entries_without_an_active_renderer();
    void mark_the_next_entry_as_current_when_the_queue_advances();
};

} // namespace Shell
