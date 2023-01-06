#include "config_test.hpp"

TEST(OCIConfigProcess, notFound)
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

TEST(OCIConfigProcess, invalidRlimits)
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

TEST(OCIConfigProcess, emptryArgs)
{
        WITH_CONFIG("minimal-for-start-config");

        config_json["process"]["args"] = R"( [ ] )"_json;

        VERIFY_FAIL({});
}

TEST(OCIConfigProcess, duplicateEnv)
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
