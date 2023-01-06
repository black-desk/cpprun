#include "cxx_oci_spec/verified_config.hpp"

#include "./verify_helper.hpp"
#include "cxx_oci_spec/config.hpp"
#include "private_common.hpp"

namespace cxx_oci_spec
{

VerifyedConfig::VerifyedConfig(std::filesystem::path path_to_bundle,
                               const Config &config)
        : Config(config)
        , path_to_bundle(std::move(path_to_bundle))
{
}

auto VerifyedConfig::verfiy(const std::filesystem::path &path_to_bundle,
                            const Config &config, const Options &opt)
        -> std::shared_ptr<VerifyedConfig>
{
        try {
                auto verifyed_config_ptr = std::shared_ptr<VerifyedConfig>(
                        new VerifyedConfig(path_to_bundle, config));

                Verifyhelper helper(opt, *verifyed_config_ptr);
                helper.verify();

                return verifyed_config_ptr;
        } catch (...) {
                NESTED_EXCEPTION("oci config verification failed");
        };
}

} // namespace cxx_oci_spec
