/* Copyright (C) 2021 G'k
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "transform_archive.hpp"
#include "bsa_saver.hpp"
#include "for_each.hpp"
#include "scope_fail.hpp"

namespace libbsarch {

void transform_archive(const bsa &source,
                       const fs::path &target_path,
                       const transform_callback &callback,
                       bsa_archive_type_t type)
{
    const auto &files = source.list_files();

    bsa_entry_list entries;
    std::for_each(files.cbegin(), files.cend(), [&entries](auto &&file) { entries.add(file); });

    if (type == baNone)
        type = source.get_type();

    bsa target;
    bsa_saver_complex target_saver(std::move(target));
    target_saver.prepare(target_path, std::move(entries), type);

    target_saver.get_bsa().set_compressed(source.get_compressed());

    if (auto flags = source.get_archive_flags(); flags != 0)
        target_saver.get_bsa().set_archive_flags(flags);

    target_saver.get_bsa().set_file_flags(source.get_file_flags());
    target_saver.get_bsa().set_share_data(source.get_share_data());

    scope_fail guard([target_path] { fs::remove(target_path); });

    for_each(files, [&source, &callback, &target_saver](auto &&relative_path) {
        extracted_data input_blob = source.extract_to_memory(relative_path);
        auto output = callback(relative_path, std::move(input_blob));
        target_saver.add_file(relative_path, std::move(output));
    });

    target_saver.save();
}

} // namespace libbsarch
