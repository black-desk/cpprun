#include "cxx_oci_spec/verified_config.hpp"
#define CXX_OCI_SPEC_PRECHECK

#ifdef CXX_OCI_SPEC_PRECHECK
#include <black_desk/cpplib/Linux/rlimit.hpp>
#endif

namespace cxx_oci_spec
{

class Verifyhelper {
    public:
        const VerifyedConfig::Options &option;
        VerifyedConfig &config;

        Verifyhelper(const VerifyedConfig::Options &opt, VerifyedConfig &cfg)
                : option(opt)
                , config(cfg)
        {
        }

        void verify() const;

        void verify_mount(const Config::Mount &mount) const;
        static void verify_rlimits(decltype(Config::process->rlimits) &rlimits);
        static void verify_capabilities(
                decltype(Config::process->capabilities) &capabilities);
        static void
        verify_args(std::vector<std::string>::const_iterator args_begin,
                    std::vector<std::string>::const_iterator args_end);
        static void verify_args(const std::vector<std::string> &args)
        {
                verify_args(args.begin(), args.end());
        }
        void verify_annotations_key(const std::string &key) const;

        void verify_env_key(const std::string &key) const;
        void verify_env(const std::vector<std::string> &env) const;
        void verify_hook(const std::vector<Config::Hooks::Hook> &hooks) const;
        void verify_ociVersion() const;
        void verify_root() const;
        void verify_mounts() const;
        void verify_process() const;
        void verify_hostname() const;
        void verify_domainname() const;
        void verify_hooks() const;
        void verify_annotations() const;
        void verify_namespaces() const;
        void verify_devices() const;
};

} // namespace cxx_oci_spec
