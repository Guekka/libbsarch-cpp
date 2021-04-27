/* Copyright (C) 2021 G'k
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "bsa.hpp"

namespace libbsarch {
class bsa_saver_simple
{
public:
    bsa_saver_simple(bsa &&bsa);

    ~bsa_saver_simple() = default;

    bsa_saver_simple(const bsa_saver_simple &) = delete;
    bsa_saver_simple(bsa_saver_simple &&) = default;

    bsa_saver_simple &operator=(const bsa_saver_simple &) = delete;
    bsa_saver_simple &operator=(bsa_saver_simple &&) = default;

    void add_file_relative(const fs::path &relative_path);
    void add_file_absolute(const fs::path &absolute_path);

    const fs::path &get_save_path() const;
    void set_save_path(fs::path path);

    const fs::path &get_root_path() const;
    void set_root_path(fs::path path);

    bsa_archive_type_t get_save_type() const;
    void set_save_type(bsa_archive_type_t type);

    void save(const fs::path &archive_path, const fs::path &root_dir, bsa_archive_type_t type) const;
    void save() const;
    void close();

    bsa &get_bsa() { return bsa_; }
    const bsa_entry_list &get_file_list() const;

private:
    bsa bsa_;

    bsa_entry_list files_;
    fs::path save_path_;
    fs::path root_dir_;
    bsa_archive_type_t save_type_ = baNone;
};

class bsa_saver_complex
{
public:
    bsa_saver_complex(bsa &&bsa)
        : bsa_(std::move(bsa))
    {}

    bsa_saver_complex(const bsa_saver_complex &) = delete;
    bsa_saver_complex(bsa_saver_complex &&) = default;

    ~bsa_saver_complex() = default;

    void prepare(const fs::path &archive_path, bsa_entry_list entries, bsa_archive_type_t type);

    void add_file(const fs::path &path_in_archive, std::vector<std::byte> &&memory_data);
    void add_file(disk_blob &&blob);

    void save() const;
    void close();

    bsa &get_bsa() { return bsa_; }

private:
    bsa bsa_;
};

namespace detail {
class bsa_saver_variant
{
public:
private:
};

} // namespace detail
} // namespace libbsarch
