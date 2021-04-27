/* Copyright (C) 2021 G'k
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <functional>
#include <utility>

namespace libbsarch {
template<typename F>
class scope_fail
{
private:
    F f;
    int uncaught_exception_count = std::uncaught_exceptions();

public:
    explicit scope_fail(const F &f)
        : f(f)
    {}

    scope_fail(const scope_fail &) = delete;
    scope_fail &operator=(const scope_fail &) = delete;

    // f() should not throw, else std::terminate is called.
    ~scope_fail()
    {
        if (uncaught_exception_count != std::uncaught_exceptions())
            f();
    }
};
} // namespace libbsarch
