#include "config_test.hpp"

TEST(OCIConfigMount, relativeDestination)
{
        WITH_CONFIG("minimal-for-start-config");
        config_json["mounts"] =
                R"([{"destination":"some/relative/path"}])"_json;
        VERIFY_FAIL({});
}
