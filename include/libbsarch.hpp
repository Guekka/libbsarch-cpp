#pragma once

#include "base_types.hpp"
#include "detail/string_convert.hpp"

#include <iostream>

namespace libbsarch {
[[maybe_unused]] constexpr int max_string_buffer_size = 1024;

inline void checkResult(const bsa_result_message_s &result)
{
    if (result.code == BSA_RESULT_EXCEPTION)
    {
        const std::string &error = detail::to_string(result.text);
        throw exception(error);
    }
}

inline void checkResult(const bsa_result_message_buffer_s &result)
{
    checkResult(result.message);
}
} // namespace libbsarch
