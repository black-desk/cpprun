#include "config_test.hpp"

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
                        "type": "unknow ns"
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
