#pragma once

#include "cxx_oci_spec/config.hpp"

namespace cxx_oci_spec
{

class VerifyedConfig : public Config {
    public:
        struct Options {
                bool stop_when_duplicate_key_found_in_environ;
                bool stop_when_no_hook_found_in_hooks;
                bool stop_when_processes_not_found;
                bool stop_when_unrecommended_env_detect;
                bool stop_when_unsupported_version_detected;
                bool stop_when_unrecommended_keyname_in_annotations_detected;
        };

        static constexpr Options DefaultVerifyConfig = Options{
                .stop_when_duplicate_key_found_in_environ = false,
                .stop_when_no_hook_found_in_hooks = false,
                .stop_when_processes_not_found = false,
                .stop_when_unrecommended_env_detect = false,
                .stop_when_unsupported_version_detected = false,
                .stop_when_unrecommended_keyname_in_annotations_detected =
                        false,
        };

        static auto verfiy(const std::filesystem::path &path_to_bundle,
                           const Config &config,
                           const Options &opt = DefaultVerifyConfig)
                -> std::shared_ptr<VerifyedConfig>;

        const std::filesystem::path path_to_bundle;

    private:
        explicit VerifyedConfig(std::filesystem::path path_to_bundle,
                                const Config &config);
};

} // namespace cxx_oci_spec
