/* Copyright (C) 2021 G'k
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "bsa_saver.hpp"
#include "for_each.hpp"

namespace libbsarch {

bsa_saver_simple::bsa_saver_simple(bsa &&bsa)
    : bsa_(std::move(bsa))
{}

void bsa_saver_simple::add_file_relative(const fs::path &relative_path)
{
    files_.add(relative_path);
}

void bsa_saver_simple::add_file_absolute(const fs::path &absolute_path)
{
    files_.add(absolute_path.lexically_relative(root_dir_));
}

const fs::path &bsa_saver_simple::get_save_path() const
{
    return save_path_;
}

void bsa_saver_simple::set_save_path(fs::path path)
{
    save_path_ = std::move(path);
}

const fs::path &bsa_saver_simple::get_root_path() const
{
    return root_dir_;
}

void bsa_saver_simple::set_root_path(fs::path path)
{
    root_dir_ = std::move(path);
}

bsa_archive_type_t bsa_saver_simple::get_save_type() const
{
    return save_type_;
}

void bsa_saver_simple::set_save_type(bsa_archive_type_t type)
{
    save_type_ = type;
}

void bsa_saver_simple::save(const fs::path &archive_path,
                            const fs::path &root_dir,
                            bsa_archive_type_t type) const
{
    if (archive_path.empty())
        throw exception("Trying to save an archive with an empty path");

    if (root_dir.empty())
        throw exception("Trying to save an archive with an empty root dir");

    if (type == baNone)
        throw exception("Trying to save an archive with a None type");

    auto create_result = bsa_create_archive(bsa_.get_unchecked().get(),
                                            archive_path.wstring().c_str(),
                                            type,
                                            files_.get_unchecked().get());
    checkResult(create_result);

    const std::vector<fs::path> files = files_.list();
    for_each(files, [this, root_dir](auto &&f) {
        const auto result = bsa_add_file_from_disk(bsa_.get_unchecked().get(),
                                                   f.wstring().c_str(),
                                                   (root_dir / f).wstring().c_str());

        checkResult(result);
    });

    const auto &result = bsa_save(bsa_.get_unchecked().get());
    checkResult(result);
}

void bsa_saver_simple::save() const
{
    save(save_path_, root_dir_, save_type_);
}

void bsa_saver_simple::close()
{
    bsa_.close();
}

const bsa_entry_list &bsa_saver_simple::get_file_list() const
{
    return files_;
}

void bsa_saver_complex::prepare(const fs::path &archive_path, bsa_entry_list entries, bsa_archive_type_t type)
{
    const auto &result = bsa_create_archive(bsa_.get_unchecked().get(),
                                            archive_path.wstring().c_str(),
                                            type,
                                            entries.get_unchecked().get());

    checkResult(result);
}

void bsa_saver_complex::add_file(const fs::path &path_in_archive, std::vector<std::byte> &&memory_data)
{
    const auto &result = bsa_add_file_from_memory(bsa_.get_unchecked().get(),
                                                  path_in_archive.wstring().c_str(),
                                                  static_cast<uint32_t>(memory_data.size()),
                                                  memory_data.data());
    checkResult(result);
}

void bsa_saver_complex::save() const
{
    const auto &result = bsa_save(bsa_.get_unchecked().get());
    checkResult(result);
}

void bsa_saver_complex::close()
{
    bsa_.close();
}

} // namespace libbsarch
