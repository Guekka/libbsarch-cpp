#pragma once

#include "bsa_entry_list.hpp"
#include "libbsarch.hpp"
#include <functional>

#include <mutex>

namespace libbsarch {
class bsa
{
public:
    /* Properties */
    fs::path get_filename() const;
    bsa_archive_type_t get_type() const;
    uint32_t get_version() const;
    fs::path get_format_name() const;
    uint32_t get_file_count() const;

    uint32_t get_archive_flags() const;
    void set_archive_flags(uint32_t flags);

    uint32_t get_file_flags() const;
    void set_file_flags(uint32_t flags);

    bool get_compressed() const;
    void set_compressed(bool value);

    bool get_share_data() const;
    void set_share_data(bool value);

    /* Selectors */
    file_record get_file_record(uint32_t index) const;
    file_record find_file_record(const fs::path &filename) const;
    fs::path get_relative_path(uint32_t index) const;
    std::vector<fs::path> list_files() const;

    /* IO */

    void load(const fs::path &archive_path);
    void close();

    extracted_data extract_to_memory(file_record record) const;
    extracted_data extract_to_memory(const fs::path &relative_path) const;

    static constexpr bool overwrite_current_files = true;
    static constexpr bool preserve_current_files = false;

    void extract_to_disk(const fs::path &relative_path,
                         const fs::path &absolute_path,
                         bool overwrite_existing) const;

    void extract_to_disk(file_record record, const fs::path &absolute_path, bool overwrite_existing) const;

    void extract_all_to_disk(const fs::path &directory, bool overwrite_existing) const;

    /* Other */
    /*! \brief Sets the DDS callback. It is required to use Fallout 4 DDS archives.
     *  \note You do not have to manage the lifetime of the provided context
     *  \example
    void dds_callback([[maybe_unused]] bsa_archive_t archive,
                                   const wchar_t *file_path,
                                   bsa_dds_info_t *dds_info,
                                   void *context)
    {
        const auto &path = *static_cast<fs::path *>(context) / fs::path(file_path);
    
        auto image = std::make_unique<DirectX::ScratchImage>();
        DirectX::TexMetadata info;
        
        const auto hr = LoadFromDDSFile(path.wstring(), DirectX::DDS_FLAGS_BAD_DXTN_TAILS, &info, *image);
        if (FAILED(hr))
        {
            //Do not throw here. Exceptions will be ignored
            return;
        }
        
        dds_info->width = info.width;
        dds_info->height = info.height;
        dds_info->mipmaps = info.mipLevels;
    }
    */
    void set_dds_callback(bsa_file_dds_info_proc_t func, fs::path root_dir)
    {
        callback_context_ = std::move(root_dir);
        bsa_file_dds_info_callback_set(archive_.get(), func, &callback_context_);
    }

    void set_dds_callback(bsa_file_dds_info_proc_t dds_function)
    {
        bsa_file_dds_info_callback_set(archive_.get(), dds_function, nullptr);
    }

    void iterate_files(bsa_file_iteration_proc_t file_iteration_proc, void *context) const;

    const detail::bsa_wrapper &get_unchecked() const;
    detail::bsa_wrapper &get_unchecked();

private:
    detail::bsa_wrapper archive_;
    fs::path callback_context_;
};
} // namespace libbsarch
