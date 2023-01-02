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
}

void Verifyhelper::verify_mount(const Config::Mount &mount)
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
                NESTED_EXCEPTION("\"uidMappings\" not supported [mount: {}]",
                                 JSON(mount));
        }

        if (mount.gidMappings.has_value()) {
                NESTED_EXCEPTION("\"gidMappings\" not supported [mount: {}]",
                                 JSON(mount));
        }
#endif
}

void Verifyhelper::verify_rlimits(decltype(Config::process->rlimits) &rlimits)
{
        if (!rlimits.has_value()) {
                return;
        }

        std::unordered_set<int> verified;

        for (const auto &rlimit : *rlimits) {
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

void Verifyhelper::verify_capabilities(
        decltype(Config::process->capabilities) &capabilities)
{
#ifdef CXX_OCI_SPEC_PRECHECK

        if (!capabilities.has_value()) {
                return;
        }

        static std::vector<decltype(capabilities->effective)> list = {
                capabilities->effective,   capabilities->bounding,
                capabilities->inheritable, capabilities->permitted,
                capabilities->ambient,
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
void Verifyhelper::verify_args(
        std::vector<std::string>::const_iterator args_begin,
        std::vector<std::string>::const_iterator args_end)
{
        // TODO(black_desk): implement
        (void)args_begin;
        (void)args_end;
}

void Verifyhelper::verify_env_key(const std::string &key) const
{
        for (const auto &character : key) {
                if ((std::isupper(character) == 0) && character != '_') {
                        auto msg = fmt::format(
                                "environment varliable name SHOULD contain only upper case letter and '_', but we get '{}' in \"{}\"",
                                character, key);
                        if (option.stop_when_unrecommended_env_detect) {
                                NESTED_EXCEPTION("{}", msg);
                        } else {
                                SPDLOG_WARN(msg);
                        }
                }
        }
}

void Verifyhelper::verify_env(const std::vector<std::string> &env) const
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

void Verifyhelper::verify_hook(
        const std::vector<Config::Hooks::Hook> &hooks) const
// FIXME: use decltype
{
        for (const auto &hook : hooks) {
                if (hook.path.is_absolute()) {
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

void Verifyhelper::verify_annotations_key(const std::string &key) const
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
                auto msg = fmt::format("oci version {} unsupported:\n{}",
                                       config.ociVersion);
                if (option.stop_when_unsupported_version_detected) {
                        NESTED_EXCEPTION("{}", msg);
                } else {
                        SPDLOG_WARN(msg);
                }
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

DO_OCI_CONFIG_VERIFY_START(
        process,
        "https://github.com/opencontainers/runtime-spec/blob/main/config.md#process");
{
        if (!config.process.has_value()) {
                if (option.stop_when_processes_not_found) {
                        NESTED_EXCEPTION("\"process\" not found");
                } else {
                        SPDLOG_WARN(
                                "\"process\" not found, this configuration will failed when \"start\" called");
                        return;
                }
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

        verify_rlimits(config.process->rlimits);

        if (config.process->apparmorProfile.has_value()) {
                NESTED_EXCEPTION("\"apparmorProfile\" not supported");
        }

        verify_capabilities(config.process->capabilities);

        if (config.process->selinuxLabel.has_value()) {
                NESTED_EXCEPTION("\"selinuxLabel\" not supported");
        }
}
DO_OCI_CONFIG_VERIFY_END(process);

DO_OCI_CONFIG_VERIFY_START(
        hostname,
        "https://github.com/opencontainers/runtime-spec/blob/main/config.md#hostname");
{
        if (config.hostname) {
                NESTED_EXCEPTION("\"hostname\" not supported");
        }
}
DO_OCI_CONFIG_VERIFY_END(hostname);

DO_OCI_CONFIG_VERIFY_START(
        domainname,
        "https://github.com/opencontainers/runtime-spec/blob/main/config.md#domainname");
{
        if (config.domainname) {
                NESTED_EXCEPTION("\"domainname\" not supported");
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

#undef DO_OCI_CONFIG_VERIFY_START
#undef DO_OCI_CONFIG_VERIFY_END

} // namespace cxx_oci_spec
