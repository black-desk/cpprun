#pragma once

#include <fstream>

#include "black_desk/cpplib/error.hpp"
#include "black_desk/cpplib/test.hpp"
#include "cxx_oci_spec/verified_config.hpp"

static auto data_path() -> std::filesystem::path
{
        static std::filesystem::path ret =
                std::filesystem::path("..") / ".." / "test" / "data";
        return ret;
}

// NOLINTNEXTLINE
#define WITH_CONFIG(config)                                                   \
        std::ifstream config_fs(std::filesystem::path("..") / ".." / "test" / \
                                "data" / config ".json");                     \
        ASSERT_EQ(config_fs.is_open(), true) << "failed to open json file";   \
        nlohmann::json config_json;                                           \
        ASSERT_NO_THROW(/*NOLINT*/                                            \
                        config_json = nlohmann::json::parse(config_fs);)      \
                << "json parse failed";

// NOLINTNEXTLINE
#define VERIFY_PASS(...)                                                       \
        cxx_oci_spec::Config config;                                           \
        config_json.get_to(config);                                            \
        std::shared_ptr<cxx_oci_spec::VerifyedConfig> verified_config;         \
        ASSERT_NO_THROW(/*NOLINT*/                                             \
                        try {                                                  \
                                verified_config =                              \
                                        cxx_oci_spec::VerifyedConfig::verfiy(  \
                                                data_path(), config,           \
                                                cxx_oci_spec::VerifyedConfig:: \
                                                        Options __VA_ARGS__);  \
                        } catch (const std::exception &e) {                    \
                                std::cout << fmt::format("{}", e);             \
                                throw;                                         \
                        });                                                    \
        ASSERT_NE(verified_config, nullptr)                                    \
                << "verified_config should never be nullptr";

// NOLINTNEXTLINE
#define VERIFY_FAIL(opt)                                                         \
        cxx_oci_spec::Config config;                                             \
        config_json.get_to(config);                                              \
        std::shared_ptr<cxx_oci_spec::VerifyedConfig> verified_config;           \
        ASSERT_ANY_THROW(/*NOLINT*/                                              \
                         verified_config = cxx_oci_spec::VerifyedConfig::verfiy( \
                                 data_path(), config,                            \
                                 cxx_oci_spec::VerifyedConfig::Options opt););   \
        ASSERT_EQ(verified_config, nullptr)                                      \
                << "verified_config should be nullptr";
