#include "config_test.hpp"

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
