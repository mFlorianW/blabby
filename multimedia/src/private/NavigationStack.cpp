// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "NavigationStack.hpp"

namespace Multimedia
{

NavigationStack::NavigationStack(Source& source)
    : mSource{source}
{
    QObject::connect(&mSource, &Source::navigationFinished, &mSource, [this](QString const& path) {
        if (not mNavigationAction) {
            // Remove older navigation history and replace with the new history.
            if (not mPathStack.isEmpty() and mPathStack.size() - 1 > mNavigationIndex) {
                mPathStack.erase(mPathStack.cbegin() + mNavigationIndex + 1, mPathStack.cend());
            }
            // append new history on the stack.
            mPathStack.append(path);
            // Increase the navigation index only after inserting the
            // second element.
            if (mPathStack.size() > 1) {
                ++mNavigationIndex;
            }
        }
        mNavigationAction = false;
    });
}

NavigationStack::~NavigationStack() = default;

QString const& NavigationStack::path()
{
    return mPath;
}

void NavigationStack::navigateBack()
{
    if (mPathStack.empty()) {
        return;
    }

    if (mNavigationIndex > qsizetype{0} and mPathStack.size() > qsizetype{0}) {
        --mNavigationIndex;
    }
    auto const path = mPathStack.at(mNavigationIndex);
    mNavigationAction = true;
    mSource.navigateTo(path);
}

void NavigationStack::navigateForward()
{
    if (mPathStack.empty() or (mNavigationIndex + 1) >= mPathStack.size()) {
        return;
    }

    ++mNavigationIndex;
    if (mNavigationIndex < mPathStack.size()) {
        auto const path = mPathStack.at(mNavigationIndex);
        mNavigationAction = true;
        mSource.navigateTo(path);
    } else {
        --mNavigationIndex;
    }
}

} // namespace Multimedia
