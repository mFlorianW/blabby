// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#include "AbstractDialog.hpp"

namespace Shell
{

AbstractDialog::~AbstractDialog() = default;

qreal AbstractDialog::headerHeight() const noexcept
{
    return mHeaderHeight;
}

const QString &AbstractDialog::title() const noexcept
{
    return mTitle;
}

void AbstractDialog::setTitle(QString const &title) noexcept
{
    if (mTitle != title) {
        mTitle = title;
        Q_EMIT titleChanged();
    }
}

QQuickItem *AbstractDialog::contentItem() const noexcept
{
    return mContentItem;
}

void AbstractDialog::setContentItem(QQuickItem *contentItem) noexcept
{
    if (contentItem == nullptr) {
        return;
    }

    if (mContentItem != contentItem) {
        if (contentItem->implicitHeight() == 0) {
            contentItem->setImplicitHeight(implicitHeight() - mHeaderHeight);
        }
        if (contentItem->height() == 0) {
            contentItem->setHeight(height() - mHeaderHeight);
        }
        if (contentItem->implicitWidth() == 0) {
            contentItem->setImplicitWidth(implicitWidth());
        }
        if (contentItem->width() == 0) {
            contentItem->setWidth(width());
        }
        mContentItem = contentItem;
        mContentItem->setParentItem(this);
        mContentItem->setY(mHeaderHeight);
        Q_EMIT contentItemChanged();
    }
}

} // namespace Shell
