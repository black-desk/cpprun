#include <fstream>

#include "black_desk/cpplib/error.hpp"
#include "black_desk/cpplib/test.hpp"
#include "cxx_oci_spec/verified_config.hpp"

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
#define VERIFY_PASS(...)                                                      \
        cxx_oci_spec::Config config;                                          \
        config_json.get_to(config);                                           \
        std::shared_ptr<cxx_oci_spec::VerifyedConfig> verified_config;        \
        ASSERT_NO_THROW(/*NOLINT*/                                            \
                        verified_config =                                     \
                                cxx_oci_spec::VerifyedConfig::verfiy(         \
                                        data_path(), config,                  \
                                        cxx_oci_spec::VerifyedConfig::Options \
                                                __VA_ARGS__););               \
        ASSERT_NE(verified_config, nullptr)                                   \
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

static auto data_path() -> std::filesystem::path
{
        static std::filesystem::path ret =
                std::filesystem::path("..") / ".." / "test" / "data";
        return ret;
}

TEST(OCIConfig, ociVersionSupported)
{
        WITH_CONFIG("minimal-for-start-config");

        VERIFY_PASS({});

        ASSERT_EQ(verified_config->ociVersion, "1.0.0");
}

TEST(OCIConfig, ociVersionNotSupported)
{
        WITH_CONFIG("minimal-for-start-config");
        config_json.at("ociVersion") = "1.1.0";
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
        VERIFY_FAIL({ .stop_when_unsupported_version_detected = true });
#pragma GCC diagnostic pop
}

TEST(OCIConfig, rootReadonlyDefaultValue)
{
        {
                WITH_CONFIG("minimal-for-start-config");

                VERIFY_PASS({});

                ASSERT_EQ(verified_config->root.readonly, false)
                        << "optional field readonly default to false";
        }

        {
                WITH_CONFIG("minimal-for-start-config");
                config_json["root"]["readonly"] = true;

                VERIFY_PASS({});

                ASSERT_EQ(verified_config->root.readonly, true)
                        << "readonly should be true now";
        }
}

TEST(OCIConfig, mountsRelativeDestination)
{
        WITH_CONFIG("minimal-for-start-config");
        config_json["mounts"] =
                R"([{"destination":"some/relative/path"}])"_json;
        VERIFY_FAIL({});
}

TEST(OCIConfig, processNotFound)
{
        WITH_CONFIG("minimal-for-start-config");

        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
                VERIFY_PASS({ .stop_when_processes_not_found = true });
#pragma GCC diagnostic pop
        }

        config_json.erase("process");

        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
                VERIFY_FAIL({ .stop_when_processes_not_found = true });
#pragma GCC diagnostic pop
        }
        {
                VERIFY_PASS({});
        }
}

TEST(OCIConfig, invalidRlimits)
{
        WITH_CONFIG("minimal-for-start-config");

        config_json["process"]["rlimits"] = R"( [
                {
                        "type": "RLIMIT_CORE",
                        "hard": 1024,
                        "soft": 1024
                },
                {
                        "type": "RLIMIT_NOFILE",
                        "hard": 1024,
                        "soft": 1024
                }
        ] )"_json;
        {
                VERIFY_PASS({});
        }

        config_json["process"]["rlimits"] = R"( [
                {
                        "type": "RLIMIT_CORE",
                        "hard": 1024,
                        "soft": 2048
                },
                {
                        "type": "RLIMIT_NOFILE",
                        "hard": 1024,
                        "soft": 1024
                }
        ] )"_json;
        {
                VERIFY_FAIL({});
        }

        config_json["process"]["rlimits"] = R"( [
                {
                        "type": "RLIMIT_ERROR_LIMIT",
                        "hard": 1024,
                        "soft": 1024
                },
                {
                        "type": "RLIMIT_NOFILE",
                        "hard": 1024,
                        "soft": 1024
                }
        ] )"_json;
        {
                VERIFY_FAIL({});
        }
}

TEST(OCIConfig, processEmptryArgs)
{
        WITH_CONFIG("minimal-for-start-config");

        config_json["process"]["args"] = R"( [ ] )"_json;

        VERIFY_FAIL({});
}

TEST(OCIConfig, processDuplicateEnv)
{
        WITH_CONFIG("minimal-for-start-config");

        config_json["process"]["env"] = R"( [
                "A=B",
                "C=D",
                "A=D"
        ] )"_json;
        {
                VERIFY_PASS({});
        }
        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
                VERIFY_FAIL(
                        { .stop_when_duplicate_key_found_in_environ = true });
#pragma GCC diagnostic pop
        }
}

TEST(OCIConfig, annotations)
{
        WITH_CONFIG("minimal-for-start-config");
        config_json["some_unknow_key"]["some_other_unknow_key"] = "some string";
        {
                VERIFY_PASS({});
        }

        config_json["annotations"]["some_unknow_key"] = "some string";
        {
                VERIFY_PASS({});
        }
        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
                VERIFY_FAIL(
                        { .stop_when_unrecommended_keyname_in_annotations_detected =
                                  true });
#pragma GCC diagnostic pop
        }
}

TEST(OCIConfig, example)
{
        WITH_CONFIG("example");
        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
                VERIFY_PASS({
                        .continue_when_unsupported_feature_found = true,
                });
#pragma GCC diagnostic pop
        }
        {
                VERIFY_PASS({
                        .continue_when_unsupported_feature_found = true,
                        .stop_when_duplicate_key_found_in_environ = true,
                        .stop_when_no_hook_found_in_hooks = true,
                        .stop_when_processes_not_found = true,
                        .stop_when_unrecommended_env_detect = true,
                        .stop_when_unsupported_version_detected = true,
                        .stop_when_unrecommended_keyname_in_annotations_detected =
                                true,
                });
        }
}

TEST(OCIConfig, namespaces)
{
        WITH_CONFIG("minimal-for-start-config");
        config_json["namespaces"] = R"([
                {
                        "type": "pid"
                }
        ])"_json;
        {
                VERIFY_PASS({});
        }

        config_json["namespaces"] = R"([
                {
                        "type": "pid"
                },
                {
                        "type": "pid"
                }
        ])"_json;
        {
                VERIFY_FAIL({});
        }

        config_json["namespaces"] = R"([
                {
                        "type": "pid",
                        "path": "/path/to/pid_ns"
                }
        ])"_json;
        {
                VERIFY_PASS({});
        }

        config_json["namespaces"] = R"([
                {
                        "type": "pid",
                        "path": "path/to/pid_ns"
                }
        ])"_json;
        {
                VERIFY_FAIL({});
        }
}
