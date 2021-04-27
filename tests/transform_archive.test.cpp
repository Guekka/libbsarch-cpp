/* Copyright (C) 2021 G'k
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "doctest.h"

#include "bsa_saver.hpp"
#include "transform_archive.hpp"

#include "Utils.hpp"

using namespace libbsarch;

TEST_CASE("Transform archive")
{
    const fs::path temp_file_path = "interface/temp_file.txt";
    const fs::path temp_absolute_file_path = get_absolute_file_path(temp_file_path);

    REQUIRE(make_file(temp_file_path, "some data"));

    for (const auto &type : standard_types)
    {
        const fs::path input_archive_path = get_absolute_file_path(type.str + " - input.bsa");
        const fs::path output_archive_path = get_absolute_file_path(type.str + " - output.bsa");

        //Preparing input archive
        bsa archive;
        bsa_saver_simple saver(std::move(archive));
        REQUIRE_NOTHROW(saver.set_save_type(type.type));
        REQUIRE_NOTHROW(saver.set_save_path(input_archive_path));
        saver.set_root_path(root_directory());

        REQUIRE_NOTHROW(saver.add_file_absolute(temp_absolute_file_path));
        REQUIRE_NOTHROW(saver.save());

        REQUIRE_NOTHROW(saver.close());

        //Transforming it
        bsa input;
        REQUIRE_NOTHROW(input.load(input_archive_path));

        transform_archive(input,
                          output_archive_path,
                          []([[maybe_unused]] const fs::path &relative_path, extracted_data &&blob) {
                              return to_vector(std::move(blob)); // Do nothing
                          });

        //Check that input and output are the same
        CHECK(compare_files(input_archive_path, output_archive_path));
    }
}
