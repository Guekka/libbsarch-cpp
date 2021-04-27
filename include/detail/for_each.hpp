/* Copyright (C) 2021 G'k
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <algorithm>

#ifdef LIBBSARCH_SINGLE_THREAD

template<typename Cont, typename Func>
auto for_each(const Cont &cont, const Func &func)
{
    using std::begin, std::end;
    return std::for_each(begin(cont), end(cont), func);
}

#else

#include <execution>

template<typename Cont, typename Func>
auto for_each(const Cont &cont, const Func &func)
{
    using std::begin, std::end;
    return std::for_each(std::execution::par, begin(cont), end(cont), func);
}

#endif
