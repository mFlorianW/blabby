// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QObject>
#include <QTemporaryDir>

namespace Multimedia
{

class JsonRendererStoreShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~JsonRendererStoreShould() override;
    Q_DISABLE_COPY_MOVE(JsonRendererStoreShould)

private:
    QString storePath() const;

    std::unique_ptr<QTemporaryDir> mDir;

private Q_SLOTS:
    void init();
    void give_the_saved_renderers_after_a_restart();
    void replace_the_remembered_renderers_on_save();
    void give_no_renderers_when_the_file_is_missing();
    void give_no_renderers_when_the_file_is_corrupt();
    void create_the_missing_directory_on_save();
};

} // namespace Multimedia
