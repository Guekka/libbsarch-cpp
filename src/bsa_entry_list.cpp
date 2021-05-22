#include "bsa_entry_list.hpp"

#include <array>

namespace libbsarch {

void bsa_entry_list::add(const fs::path &file_path)
{
    const auto &result = bsa_entry_list_add(entries_.get(), file_path.wstring().c_str());
    checkResult(result);
}

uint32_t bsa_entry_list::count() const
{
    return bsa_entry_list_count(entries_.get());
}

std::vector<fs::path> bsa_entry_list::list() const
{
    std::vector<fs::path> list;
    uint32_t count = this->count();
    list.reserve(count);

    for (uint32_t i = 0; i < count; ++i)
        list.emplace_back(get(i));

    return list;
}

std::filesystem::path bsa_entry_list::get(uint32_t index) const
{
    std::array<wchar_t, max_string_buffer_size> buffer{};
    bsa_entry_list_get(entries_.get(), index, max_string_buffer_size, buffer.data());
    return buffer.data();
}

const detail::bsa_entry_list_wrapper &bsa_entry_list::get_unchecked() const
{
    return entries_;
}

detail::bsa_entry_list_wrapper &bsa_entry_list::get_unchecked()
{
    return entries_;
}
} // namespace libbsarch
