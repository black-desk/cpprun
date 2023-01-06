#include "cxx_oci_spec/verified_config.hpp"
#define CXX_OCI_SPEC_PRECHECK

#ifdef CXX_OCI_SPEC_PRECHECK
#include <black_desk/cpplib/Linux/rlimit.hpp>
#endif

namespace cxx_oci_spec
{

class Verifyhelper {
    public:
        using Options = VerifyedConfig::Options;
        const Options &option;
        VerifyedConfig &config;

        Verifyhelper(const Options &opt, VerifyedConfig &cfg)
                : option(opt)
                , config(cfg)
        {
        }

        void verify() const;

        using Mount = decltype(Config::mounts)::value_type::value_type;
        void verify_mount(Mount const &mount) const;

        using Rlimits = decltype(Config::process->rlimits)::value_type;
        static void verify_rlimits(Rlimits const &rlimits);

        using Capabilities =
                decltype(Config::process->capabilities)::value_type;
        static void verify_capabilities(Capabilities const &capabilities);

        using Args = decltype(Config::process->args);
        static void verify_args(Args const &args)
        {
                verify_args(args.begin(), args.end());
        }
        static void verify_args(Args::const_iterator args_begin,
                                Args::const_iterator args_end);

        using Env = decltype(Config::process->env)::value_type;
        void verify_env(Env const &env) const;
        using EnvKey = Env::value_type;
        void verify_env_key(EnvKey const &key) const;

        using Hooks = decltype(Config::hooks->poststart)::value_type;
        void verify_hook(Hooks const &hooks) const;

        using AnnoKey = decltype(Config::annotations)::value_type::key_type;
        void verify_annotations_key(AnnoKey const &key) const;

        using AllowedDeviceList =
                decltype(Config::Resources::devices)::value_type;
        static void
        verify_allowed_device_list(AllowedDeviceList const &devices);

        using AllowedDeviceType = decltype(AllowedDeviceList::value_type::type);
        static void
        verify_allowed_device_list_type(AllowedDeviceType const &type);

        using AllowedDeviceAccess =
                decltype(AllowedDeviceList::value_type::access)::value_type;
        static void
        verify_allowed_device_list_access(AllowedDeviceAccess const &access);

        using Memory = decltype(Config::Resources::memory)::value_type;
        static void verify_memory(Memory const &memory);

        using CPU = decltype(Config::Resources::cpu)::value_type;
        static void verify_cpu(CPU const &cpu);

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
        void verify_resources() const;
};

} // namespace cxx_oci_spec
