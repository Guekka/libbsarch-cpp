#include "bsa.hpp"
#include "for_each.hpp"

#include <fstream>
#include <numeric>

namespace libbsarch {

/* Properties */
fs::path bsa::get_filename() const
{
    wchar_t name[max_string_buffer_size];
    bsa_filename_get(archive_.get(), max_string_buffer_size, name);
    return name;
}

bsa_archive_type_t bsa::get_type() const
{
    return bsa_archive_type_get(archive_.get());
}

uint32_t bsa::get_version() const
{
    return bsa_version_get(archive_.get());
}

fs::path bsa::get_format_name() const
{
    wchar_t format_name[max_string_buffer_size];
    bsa_format_name_get(archive_.get(), max_string_buffer_size, format_name);
    return format_name;
}

uint32_t bsa::get_file_count() const
{
    return bsa_file_count_get(archive_.get());
}

uint32_t bsa::get_archive_flags() const
{
    return bsa_archive_flags_get(archive_.get());
}

void bsa::set_archive_flags(uint32_t flags)
{
    bsa_archive_flags_set(archive_.get(), flags);
}

uint32_t bsa::get_file_flags() const
{
    return bsa_file_flags_get(archive_.get());
}

void bsa::set_file_flags(uint32_t flags)
{
    bsa_file_flags_set(archive_.get(), flags);
}

void bsa::set_compressed(bool value)
{
    bsa_compress_set(archive_.get(), value);
}

void bsa::set_share_data(bool value)
{
    bsa_share_data_set(archive_.get(), value);
}

bool bsa::get_compressed() const
{
    return bsa_compress_get(archive_.get());
}

bool bsa::get_share_data() const
{
    return bsa_share_data_get(archive_.get());
}

void bsa::load(const fs::path &archive_path)
{
    const auto &result = bsa_load_from_file(archive_.get(), archive_path.wstring().c_str());
    checkResult(result);
}

void bsa::close()
{
    bsa_close(archive_.get());
}

extracted_data bsa::extract_to_memory(file_record record) const
{
    const auto &result = bsa_extract_file_data_by_record(archive_.get(), record.value);
    checkResult(result);
    return extracted_data(result.buffer, archive_.get());
}

extracted_data bsa::extract_to_memory(const fs::path &relative_path) const
{
    const auto &result = bsa_extract_file_data_by_filename(archive_.get(), relative_path.wstring().c_str());
    checkResult(result);
    return extracted_data(result.buffer, archive_.get());
}

void bsa::extract_to_disk(const fs::path &relative_path,
                          const fs::path &absolute_path,
                          bool overwrite_existing) const
{
    auto record = find_file_record(relative_path);
    extract_to_disk(record, absolute_path, overwrite_existing);
}

void bsa::extract_to_disk(file_record record,
                          const std::filesystem::path &absolute_path,
                          bool overwrite_existing) const
{
    {
        static std::mutex mut;
        std::lock_guard g(mut);

        const bool exist = fs::exists(absolute_path);
        if (exist && !overwrite_existing)
            return;

        if (exist)
            fs::remove(absolute_path);
    }

    auto data = extract_to_memory(record);

    std::fstream file(absolute_path, std::ios::out | std::ios::binary);
    file.write(static_cast<const char *>(data.get_buffer()), data.get_size());
}

void bsa::extract_all_to_disk(const fs::path &directory, bool overwrite_current_files) const
{
    std::vector<uint32_t> indexes(get_file_count());
    std::iota(indexes.begin(), indexes.end(), 0);

    for_each(indexes.cbegin(), indexes.cend(), [this, overwrite_current_files, directory](auto &&idx) {
	      auto record = get_file_record(idx);
	      auto relative_path = get_relative_path(idx);
	
	      fs::path absolute_path = directory / relative_path;
	      fs::path absolute_directory = fs::path(absolute_path).remove_filename();
	
	      {
	          static std::mutex mut;
	          std::lock_guard g(mut);
	          fs::create_directories(absolute_directory);
	      }
	      try
	      {
	          extract_to_disk(record, absolute_path, overwrite_current_files);
	      }
	      catch (const std::exception &e)
	      {
	          std::cerr << e.what() << std::endl;
	      }
	  });
}

void bsa::iterate_files(bsa_file_iteration_proc_t file_iteration_proc, void *context) const
{
    bsa_iterate_files(archive_.get(), file_iteration_proc, context);
}

file_record bsa::get_file_record(uint32_t index) const
{
    return {bsa_get_file_record(archive_.get(), index)};
}

file_record bsa::find_file_record(const fs::path &filename) const
{
    return {bsa_find_file_record(archive_.get(), filename.wstring().c_str())};
}

std::filesystem::path bsa::get_relative_path(uint32_t index) const
{
    bsa_entry_list list;
    const auto &result = bsa_get_resource_list(archive_.get(), list.get_unchecked().get(), L"");

    checkResult(result);

    return list.get(index);
}

std::vector<fs::path> bsa::list_files() const
{
    bsa_entry_list list;
    const auto &result = bsa_get_resource_list(archive_.get(), list.get_unchecked().get(), L"");

    checkResult(result);

    return list.list();
}

const detail::bsa_wrapper &bsa::get_unchecked() const
{
    return archive_;
}

detail::bsa_wrapper &bsa::get_unchecked()
{
    return archive_;
}

} // namespace libbsarch
