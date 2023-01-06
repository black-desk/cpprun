#include "config_test.hpp"

TEST(OCIConfigResources, devices)
{
        WITH_CONFIG("minimal-for-start-config");
        config_json["resources"] = R"({
                "devices": [
                        {
                                "allow": false,
                                "access": "rwm"
                        },
                        {
                                "allow": true,
                                "type": "c",
                                "major": 10,
                                "minor": 229,
                                "access": "rw"
                        },
                        {
                                "allow": true,
                                "type": "b",
                                "major": 8,
                                "minor": 0,
                                "access": "r"
                        }
                ]
        })"_json;
        {
                VERIFY_PASS({});
        }

        config_json["resources"] = R"({
                "devices": [
                        {
                                "allow": false,
                                "access": "a"
                        }
                ]
        })"_json;
        {
                VERIFY_FAIL({});
        }

        config_json["resources"] = R"({
                "devices": [
                        {
                                "allow": false,
                                "access": "rr"
                        }
                ]
        })"_json;
        {
                VERIFY_FAIL({});
        }

        config_json["resources"] = R"({
                "devices": [
                        {
                                "allow": false,
                                "type": "B"
                        }
                ]
        })"_json;
        {
                VERIFY_FAIL({});
        }
}

TEST(OCIConfigResources, memory)
{
        WITH_CONFIG("minimal-for-start-config");
        config_json["resources"] = R"({
                "memory": {
                        "limit": -1
                }
        })"_json;
        {
                VERIFY_PASS({});
                ASSERT_EQ(config.resources.has_value() &&
                                  config.resources->memory.has_value(),
                          true);
                ASSERT_EQ(config.resources->memory->limit, -1);
        }

        config_json["resources"] = R"({
                "memory": {
                        "limit": -2
                }
        })"_json;
        {
                VERIFY_FAIL({});
        }
}
