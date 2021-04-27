add_executable(libbsarch_tests
    main.test.cpp

    transform_archive.test.cpp
    save_and_load.test.cpp

    utils.hpp
    utils.cpp)

target_link_libraries(libbsarch_tests PRIVATE libbsarch-cpp)
target_compile_features(libbsarch_tests PRIVATE cxx_std_17)

target_compile_options(libbsarch_tests PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wno-c++98-compat -Wno-c++98-compat-pedantic>
    )
