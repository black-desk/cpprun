#include "./verify_helper.hpp"

#include <algorithm>
#include <regex>
#include <set>
#include <type_traits>
#include <unordered_set>

#include <black_desk/cpplib/utils.hpp>
#include <libmount.h>
#include <semver.hpp>

#include "capabilities.hpp"
#include "private_common.hpp"
#include "rlimits.hpp"
namespace cxx_oci_spec
{
// NOLINTNEXTLINE
#define WARNING_OR_EXCEPTION(opt, ...)         \
        if (!(opt)) {                          \
                SPDLOG_WARN(__VA_ARGS__);      \
        } else {                               \
                NESTED_EXCEPTION(__VA_ARGS__); \
        }

// NOLINTNEXTLINE
#define UNSUPPORTED(...)                                                      \
        WARNING_OR_EXCEPTION(!option.continue_when_unsupported_feature_found, \
                             __VA_ARGS__)

#ifdef CXX_OCI_SPEC_PRECHECK
constexpr static const auto mount_context_deleter =
        [](struct libmnt_context *context) { mnt_free_context(context); };
#endif

void Verifyhelper::verify() const
{
        SPDLOG_TRACE("start verfiy config");
        verify_ociVersion();
        verify_root();
        verify_mounts();
        verify_process();
        verify_hostname();
        verify_domainname();
        verify_hooks();
        verify_annotations();
        verify_namespaces();
        verify_devices();
        verify_resources();
}

// NOLINTNEXTLINE
#define DO_OCI_CONFIG_VERIFY_START(FIELD, url)         \
        void Verifyhelper::verify_##FIELD() const      \
        {                                              \
                const std::string_view help_url = url; \
                try {
// NOLINTNEXTLINE
#define DO_OCI_CONFIG_VERIFY_END(FIELD)                                    \
        }                                                                  \
        catch (...)                                                        \
        {                                                                  \
                NESTED_EXCEPTION("oci config verification field \"" #FIELD \
                                 "\" failed, check {}",                    \
                                 help_url);                                \
        }                                                                  \
        }

DO_OCI_CONFIG_VERIFY_START(
        ociVersion,
        "https://github.com/opencontainers/runtime-spec/blob/main/config.md#specification-version");
{
        constexpr std::string_view version_range = ">=1.0.0 <1.0.2";
        semver::version config_version(config.ociVersion);
        if (!semver::range::satisfies(config_version, version_range)) {
                UNSUPPORTED("oci version {} unsupported:\n{}",
                            config.ociVersion);
        }
}
DO_OCI_CONFIG_VERIFY_END(ociVersion);

DO_OCI_CONFIG_VERIFY_START(
        root,
        "https://github.com/opencontainers/runtime-spec/blob/main/config.md#root");
{
        if (config.root.path.is_relative()) {
                config.root.path = config.path_to_bundle / config.root.path;
        }

        if (!std::filesystem::exists(config.root.path) ||
            !std::filesystem::is_directory(config.root.path)) {
                NESTED_EXCEPTION(
                        "\"path\" invalid: must be an existed directory [root: {}]",
                        JSON(config.root));
        }
}
DO_OCI_CONFIG_VERIFY_END(root);

DO_OCI_CONFIG_VERIFY_START(
        mounts,
        "https://github.com/opencontainers/runtime-spec/blob/main/config.md#mounts");
{
        if (!config.mounts.has_value()) {
                return;
        }

        for (auto &mount : config.mounts.value()) {
                verify_mount(mount);
        }
}
DO_OCI_CONFIG_VERIFY_END(mounts);

void Verifyhelper::verify_mount(Mount const &mount) const
{
        if (mount.destination.is_relative()) {
                NESTED_EXCEPTION(
                        "\"destination\" invalid: should be absolute path [mount: {}]",
                        JSON(mount));
        }

#ifdef CXX_OCI_SPEC_PRECHECK
        auto mnt_ctx = std::shared_ptr<struct libmnt_context>(
                mnt_new_context(), mount_context_deleter);

        // NOTE(black_desk): libmount will dup the c-style string.

        if (mount.source.has_value()) {
                auto ret = mnt_context_set_source(mnt_ctx.get(),
                                                  mount.source->c_str());
                if (ret != 0) {
                        NESTED_EXCEPTION(
                                "\"source\" invalid: mnt_context_set_source returns {} [mount: {}]",
                                ret, JSON(mount));
                }
        }

        if (mount.options.has_value()) {
                auto ret = mnt_context_append_options(
                        mnt_ctx.get(), black_desk::cpplib::strings::join(
                                               mount.options.value(), ",")
                                               .c_str());
                if (ret != 0) {
                        NESTED_EXCEPTION(
                                "\"options\" invalid: mnt_context_append_options returns {} [mount: {}]",
                                ret, JSON(mount));
                }
        }

        if (mount.type.has_value()) {
                auto ret = mnt_context_set_fstype(mnt_ctx.get(),
                                                  mount.type->c_str());
                if (ret != 0) {
                        NESTED_EXCEPTION(
                                "\"type\" invalid: mnt_context_set_fstype returns {} [mount: {}]",
                                ret, JSON(mount));
                }
        }

        if (mount.uidMappings.has_value()) {
                UNSUPPORTED("\"uidMappings\" not supported [mount: {}]",
                            JSON(mount));
        }

        if (mount.gidMappings.has_value()) {
                UNSUPPORTED("\"gidMappings\" not supported [mount: {}]",
                            JSON(mount));
        }
#endif
}

DO_OCI_CONFIG_VERIFY_START(
        process,
        "https://github.com/opencontainers/runtime-spec/blob/main/config.md#process");
{
        if (!config.process.has_value()) {
                WARNING_OR_EXCEPTION(option.stop_when_processes_not_found,
                                     "\"process\" not found");
                return;
        }

        if (config.process->args.empty()) {
                NESTED_EXCEPTION(
                        "at least one entry is REQUIRED for \"process.args\"");
        }

        verify_args(config.process->args.begin() + 1,
                    config.process->args.end());

        if (config.process->env.has_value()) {
                verify_env(config.process->env.value());
        }

        if (config.process->rlimits.has_value()) {
                verify_rlimits(config.process->rlimits.value());
        }

        if (config.process->apparmorProfile.has_value()) {
                UNSUPPORTED("\"apparmorProfile\" not supported");
        }

        if (config.process->capabilities.has_value()) {
                verify_capabilities(config.process->capabilities.value());
        }

        if (config.process->selinuxLabel.has_value()) {
                UNSUPPORTED("\"selinuxLabel\" not supported");
        }
}
DO_OCI_CONFIG_VERIFY_END(process);

void Verifyhelper::verify_args(Args::const_iterator args_begin,
                               Args::const_iterator args_end)
{
        // TODO(black_desk): implement
        (void)args_begin;
        (void)args_end;
}

void Verifyhelper::verify_env(Env const &env) const
{
        std::set<std::string> keys;
        for (const auto &key_and_value : env) {
                auto euqal_pos = key_and_value.find_first_of('=');
                if (euqal_pos == std::string::npos) {
                        NESTED_EXCEPTION(
                                "entry in env should be like KEY=VALUE, but we get \"{}\"",
                                key_and_value);
                }

                auto key = key_and_value.substr(0, euqal_pos);
                if (keys.find(key) != keys.end()) {
                        auto msg = fmt::format(
                                "duplicate key \"{}\" found in environ", key);
                        if (this->option
                                    .stop_when_duplicate_key_found_in_environ) {
                                NESTED_EXCEPTION("{}", msg);
                        } else {
                                SPDLOG_WARN(msg);
                        }
                }
                keys.insert(key);
                verify_env_key(key);
        }
}

void Verifyhelper::verify_env_key(EnvKey const &key) const
{
        for (const auto &character : key) {
                if ((std::isupper(character) == 0) && character != '_' &&
                    (std::isdigit(character) == 0)) {
                        auto msg = fmt::format(
                                "environment varliable name should contain only upper case letters, digits, and '_', but we get '{}' in \"{}\"",
                                character, key);
                        if (option.stop_when_unrecommended_env_detect) {
                                NESTED_EXCEPTION("{}", msg);
                        } else {
                                SPDLOG_WARN(msg);
                        }
                }
        }
}

void Verifyhelper::verify_rlimits(Rlimits const &rlimits)
{
        std::unordered_set<int> verified;

        for (const auto &rlimit : rlimits) {
                auto type = rlimit_string_to_int(rlimit.type);
                if (verified.find(type) != verified.end()) {
                        NESTED_EXCEPTION(
                                "duplicated entries with same type \"{}\"",
                                rlimit.type);
                }
                verified.insert(type);

#ifdef CXX_OCI_SPEC_PRECHECK
                black_desk::cpplib::Linux::getrlimit(type);
                if (rlimit.soft > rlimit.hard) {
                        NESTED_EXCEPTION(
                                "\"soft\" should less or equal to \"hard\" [rlimit: {}]",
                                JSON(rlimit));
                }
#endif
        }
}

void Verifyhelper::verify_capabilities(Capabilities const &capabilities)
{
#ifdef CXX_OCI_SPEC_PRECHECK

        static std::vector<decltype(capabilities.effective)> list = {
                capabilities.effective,   capabilities.bounding,
                capabilities.inheritable, capabilities.permitted,
                capabilities.ambient,
        };

        for (auto const &capability : list) {
                if (!capability.has_value()) {
                        continue;
                }

                for (auto const &cap : *capability) {
                        capability_string_to_int(cap);
                }
        }

#endif
}

DO_OCI_CONFIG_VERIFY_START(
        hostname,
        "https://github.com/opencontainers/runtime-spec/blob/main/config.md#hostname");
{
        if (config.hostname.has_value()) {
                UNSUPPORTED("\"hostname\" not supported");
        }
}
DO_OCI_CONFIG_VERIFY_END(hostname);

DO_OCI_CONFIG_VERIFY_START(
        domainname,
        "https://github.com/opencontainers/runtime-spec/blob/main/config.md#domainname");
{
        if (config.domainname.has_value()) {
                UNSUPPORTED("\"domainname\" not supported");
        }
}
DO_OCI_CONFIG_VERIFY_END(domainname);

DO_OCI_CONFIG_VERIFY_START(
        hooks,
        "https://github.com/opencontainers/runtime-spec/blob/main/config.md#posix-platform-hooks");
{
        if (!config.hooks.has_value()) {
                return;
        }

        const int hook_kind = 6;

        std::array<std::reference_wrapper<const std::optional<
                           std::vector<Config::Hooks::Hook>>>,
                   hook_kind>
                hook_kinds = {
                        config.hooks->createRuntime,
                        config.hooks->createContainer,
                        config.hooks->startContainer,
                        config.hooks->prestart,
                        config.hooks->poststart,
                        config.hooks->poststop,
                };

        bool empty = true;

        for (auto &hook_ref : hook_kinds) {
                const auto &hook = hook_ref.get();
                bool this_empty = (!hook.has_value()) || (hook->empty());
                if (this_empty) {
                        continue;
                }

                empty = false;
                verify_hook(hook.value());
        }

        if (empty && option.stop_when_no_hook_found_in_hooks) {
                NESTED_EXCEPTION("no hook in \"hooks\"");
        }
}
DO_OCI_CONFIG_VERIFY_END(hooks);

void Verifyhelper::verify_hook(Hooks const &hooks) const
{
        for (const auto &hook : hooks) {
                if (!hook.path.is_absolute()) {
                        NESTED_EXCEPTION(
                                "\"hooks\" invalid: \"path\" should be absolute [hook: {}]",
                                JSON(hook));
                }

                if (hook.args.has_value()) {
                        verify_args(hook.args.value());
                }

                if (hook.env.has_value()) {
                        verify_env(hook.env.value());
                }
        }
}

DO_OCI_CONFIG_VERIFY_START(
        annotations,
        "https://github.com/opencontainers/runtime-spec/blob/main/config.md#annotations");
{
        if (!config.annotations.has_value()) {
                return;
        }

        for (const auto &[key, value] : config.annotations.value()) {
                verify_annotations_key(key);
        }
}
DO_OCI_CONFIG_VERIFY_END(hooks);

void Verifyhelper::verify_annotations_key(AnnoKey const &key) const
{
        auto components = black_desk::cpplib::strings::split(key, '.');

        std::reverse(components.begin(), components.end());

        auto reversed = black_desk::cpplib::strings::join(components, '.');

        // https://regexr.com/3au3g
        static std::regex regex_match_domain(
                R"((?:[a-z0-9](?:[a-z0-9-]{0,61}[a-z0-9])?\.)+[a-z0-9][a-z0-9-]{0,61}[a-z0-9])");

        if (std::regex_match(reversed, regex_match_domain)) {
                return;
        }

        auto msg = fmt::format(
                "Keys in annotations should be named using a reverse domain notation, but we get \"{}\"",
                key);

        if (option.stop_when_unrecommended_keyname_in_annotations_detected) {
                NESTED_EXCEPTION("{}", msg);
        } else {
                SPDLOG_WARN(msg);
        }
}

DO_OCI_CONFIG_VERIFY_START(
        namespaces,
        "https://github.com/opencontainers/runtime-spec/blob/main/config-linux.md#namespaces");
{
        if (!config.namespaces.has_value()) {
                return;
        }

        std::map<std::string, bool> namespaces{
                { "pid", false },   { "network", false }, { "mount", false },
                { "ipc", false },   { "uts", false },     { "user", false },
                { "cgroup", false }
        };

        for (const auto &namespace_ : config.namespaces.value()) {
                auto iter = namespaces.find(namespace_.type);
                if (iter == namespaces.end()) {
                        NESTED_EXCEPTION("unknown namespace type \"{}\"",
                                         namespace_.type);
                }

                if (iter->second) {
                        NESTED_EXCEPTION(
                                "\"namespaces\" field contains duplicated namespaces with same \"type\" (\"{}\")",
                                namespace_.type);
                }

                iter->second = true;

                if (namespace_.path.has_value() &&
                    !namespace_.path->is_absolute()) {
                        NESTED_EXCEPTION(
                                "\"path\" MUST be an absolute path [\"path\"= \"{}\"]",
                                namespace_.path->string());
                }
        }
}
DO_OCI_CONFIG_VERIFY_END(namespaces);

DO_OCI_CONFIG_VERIFY_START(
        devices,
        "https://github.com/opencontainers/runtime-spec/blob/main/config-linux.md#devices");
{
        if (!config.devices.has_value()) {
                return;
        }

        static const std::set<std::string> types{ "c", "b", "u", "p" };

        for (const auto &device : config.devices.value()) {
                if (types.find(device.type) == types.end()) {
                        NESTED_EXCEPTION("unknown device type [deivce: {}]",
                                         JSON(device));
                }

                if (!device.path.is_absolute()) {
                        NESTED_EXCEPTION(
                                "path of deivce should be absolute [device: {}]",
                                JSON(device));
                }

                // TODO(black_desk): Should we check uid and gid is mapped or not?
        }
}
DO_OCI_CONFIG_VERIFY_END(devices);

DO_OCI_CONFIG_VERIFY_START(
        resources,
        "https://github.com/opencontainers/runtime-spec/blob/main/config-linux.md#control-groups");
{
        if (!config.resources.has_value()) {
                return;
        }

        if (config.resources->devices.has_value()) {
                verify_allowed_device_list(config.resources->devices.value());
        }

        if (config.resources->memory.has_value()) {
                verify_memory(config.resources->memory.value());
        }

        if (config.resources->cpu.has_value()) {
                verify_cpu(config.resources->cpu.value());
        }
}
DO_OCI_CONFIG_VERIFY_END(resources);

void Verifyhelper::verify_allowed_device_list(AllowedDeviceList const &devices)
{
        for (auto const &device : devices) {
                try {
                        verify_allowed_device_list_type(device.type);

                        if (device.access.has_value()) {
                                verify_allowed_device_list_access(
                                        device.access.value());
                        }

                } catch (const std::exception &e) {
                        NESTED_EXCEPTION("invalid device [{}]", JSON(device));
                }
        }
}

void Verifyhelper::verify_allowed_device_list_type(AllowedDeviceType const &type)
{
        static const std::set<std::string> types{ "c", "b", "a" };
        if (types.find(type) == types.end()) {
                NESTED_EXCEPTION("invalid type");
        }
}

void Verifyhelper::verify_allowed_device_list_access(
        AllowedDeviceAccess const &access)
{
        static const std::set<char> accesses{ 'r', 'w', 'm' };
        for (auto const &character : access) {
                if (accesses.find(character) == accesses.end() ||
                    std::count(access.begin(), access.end(), character) != 1) {
                        NESTED_EXCEPTION("invalid access");
                }
        }
}

void Verifyhelper::verify_memory(Memory const &memory)
{
        auto limits = std::vector<decltype(Memory::limit)::value_type>{
                memory.limit.value_or(-1),     memory.reservation.value_or(-1),
                memory.swap.value_or(-1),      memory.kernel.value_or(-1),
                memory.kernelTCP.value_or(-1),
        };

        for (auto const &limit : limits) {
                if (limit < -1) {
                        NESTED_EXCEPTION(
                                "invalid memory limit value, should be greater than -1 [memory: {}]",
                                JSON(memory))
                }
        }
}

void Verifyhelper::verify_cpu(CPU const &cpu)
{
        // https://regex101.com/r/VVvdhU/1
        static std::regex const regex_match_cpus(R"((?:\d+(?:-\d+)?,?)+)");
        auto const &regex_match_mems = regex_match_cpus;

        if (cpu.cpus.has_value() &&
            !std::regex_match(cpu.cpus.value(), regex_match_cpus)) {
                NESTED_EXCEPTION("invalid cpus [cpu: {}]", JSON(cpu));
        }

        if (cpu.mems.has_value() &&
            !std::regex_match(cpu.mems.value(), regex_match_mems)) {
                NESTED_EXCEPTION("invalid mems [cpu: {}]", JSON(cpu));
        }

        // TODO(black_desk): more verification
}

#undef DO_OCI_CONFIG_VERIFY_START
#undef DO_OCI_CONFIG_VERIFY_END

} // namespace cxx_oci_spec
