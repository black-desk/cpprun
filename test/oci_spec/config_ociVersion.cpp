#include "config_test.hpp"

TEST(OCIConfigOciVersion, supportedRange)
{
        WITH_CONFIG("minimal-for-start-config");

        {
                VERIFY_PASS({});

                ASSERT_EQ(verified_config->ociVersion, "1.0.0");
        }

        config_json.at("ociVersion") = "1.1.0";

        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
                VERIFY_FAIL({ .stop_when_unsupported_version_detected = true });
#pragma GCC diagnostic pop
        }
}
