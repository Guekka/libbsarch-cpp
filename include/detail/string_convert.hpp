/* Copyright (C) 2021 G'k
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#pragma once

#include <string>

namespace libbsarch::detail {
std::string to_string(const std::wstring &str);
std::wstring to_wstring(const std::string &str);
} // namespace libbsarch::detail
