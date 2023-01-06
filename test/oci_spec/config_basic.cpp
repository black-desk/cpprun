#include "config_test.hpp"

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

TEST(OCIConfig, devices)
{
        WITH_CONFIG("minimal-for-start-config");
        config_json["devices"] = R"([
                {
                        "type": "b",
                        "path": "/dev/sda",
                        "major": 8,
                        "minor": 0
                }
        ])"_json;
        {
                VERIFY_PASS({});
        }

        config_json["devices"] = R"([
                {
                        "type": "x",
                        "path": "/dev/sda",
                        "major": 8,
                        "minor": 0
                }
        ])"_json;
        {
                VERIFY_FAIL({});
        }

        config_json["devices"] = R"([
                {
                        "type": "b",
                        "path": "dev/sda",
                        "major": 8,
                        "minor": 0
                }
        ])"_json;
        {
                VERIFY_FAIL({});
        }
}
