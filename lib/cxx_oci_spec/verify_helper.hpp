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

        void verify_mount(decltype(Config::mounts)::value_type::value_type const
                                  &mount) const;
        static void verify_rlimits(
                decltype(Config::process->rlimits)::value_type const &rlimits);
        static void verify_capabilities(
                const decltype(Config::process->capabilities)::value_type
                        &capabilities);
        static void
        verify_args(decltype(Config::process->args)::const_iterator args_begin,
                    decltype(Config::process->args)::const_iterator args_end);
        static void verify_args(decltype(Config::process->args) const &args)
        {
                verify_args(args.begin(), args.end());
        }
        void
        verify_env(decltype(Config::process->env)::value_type const &env) const;
        void verify_env_key(
                decltype(Config::process->env)::value_type::value_type const
                        &key) const;
        void verify_hook(decltype(Config::hooks->poststart)::value_type const
                                 &hooks) const;
        void verify_annotations_key(
                decltype(Config::annotations)::value_type::key_type const &key)
                const;
        static void verify_allowed_device_list(
                decltype(Config::Resources::devices)::value_type const &devices);
        static void verify_allowed_device_list_type(
                decltype(decltype(Config::Resources::devices)::value_type::
                                 value_type::type) const &type);
        static void verify_allowed_device_list_access(
                decltype(decltype(Config::Resources::devices)::value_type::
                                 value_type::access)::value_type const &access);

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
