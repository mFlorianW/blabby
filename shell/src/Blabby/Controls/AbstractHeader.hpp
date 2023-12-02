// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QQuickItem>

namespace Shell
{
/**
 * The @ref AbstractHeader has all the logic for the header control without any visual aspects.
 */
class AbstractHeader : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

    /**
     * The text for the header
     */
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    /**
     * Inherited Constructors
     */
    using QQuickItem::QQuickItem;

    /**
     * Default destructor
     */
    ~AbstractHeader() override;

    /**
     * Disable copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(AbstractHeader)

    /**
     * Gives the text for the header.
     * @return The text for header
     */
    QString text() const noexcept;

    /**
     * Sets the text for the header.
     * @param text The new text for the header.
     */
    void setText(QString const &text) noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the text of the header is changed.
     */
    void textChanged();

private:
    QString mText;
};

} // namespace Shell
