#include "config_test.hpp"

TEST(OCIConfig, resources)
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
