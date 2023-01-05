#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include <black_desk/cpplib/json/json_helper.hpp>
#include <black_desk/cpplib/json/std_optional.hpp>
#include <black_desk/cpplib/macro.hpp>

namespace cxx_oci_spec
{

// https://github.com/opencontainers/runtime-spec/blob/main/config.md#configuration
struct Config {
        struct LinuxIDMapping {
                uint32_t containerID;
                uint32_t hostID;
                uint32_t size;
        };

        std::string ociVersion;

        struct Root {
                std::filesystem::path path;
                bool readonly = false;
        };
        Root root;

        struct Mount {
                std::filesystem::path destination;
                std::optional<std::string> source;
                std::optional<std::vector<std::string>> options;

                std::optional<std::string> type;
                std::optional<LinuxIDMapping> uidMappings, gidMappings;
        };
        std::optional<std::vector<Mount>> mounts;

        struct Process {
                bool terminal;

                struct ConsoleSize {
                        uint height, width;
                };
                std::optional<ConsoleSize> consoleSize;

                std::filesystem::path cwd;

                std::optional<std::vector<std::string>> env;

                std::vector<std::string> args;

                struct Rlimit {
                        std::string type;
                        uint64_t soft, hard;
                };

                std::optional<std::vector<Rlimit>> rlimits;

                std::optional<std::string> apparmorProfile;

                struct Capabilities {
                        std::optional<std::vector<std::string>> effective,
                                bounding, inheritable, permitted, ambient;
                };
                std::optional<Capabilities> capabilities;

                bool noNewPrivileges;

                std::optional<int> oomScoreAdj;

                std::optional<std::string> selinuxLabel;

                struct User {
                        int uid, gid;
                        std::optional<int> umask;
                        std::optional<std::vector<int>> additionalGids;
                } user;
        };
        std::optional<Process> process;

        std::optional<std::string> hostname;
        std::optional<std::string> domainname;

        struct Hooks {
                struct Hook {
                        std::filesystem::path path;
                        std::optional<std::vector<std::string>> args;
                        std::optional<std::vector<std::string>> env;
                        std::optional<int> timeout;
                };

                std::optional<std::vector<Hook>> prestart, createRuntime,
                        createContainer, startContainer, poststart, poststop;
        };
        std::optional<Hooks> hooks;
        std::optional<std::map<std::string, nlohmann::json>> annotations;

        struct Namespace {
                std::string type;
                std::optional<std::filesystem::path> path;
        };

        std::optional<std::vector<Namespace>> namespaces;

        std::optional<std::vector<LinuxIDMapping>> uidMappings, gidMappings;

        struct Device {
                std::string type;
                std::filesystem::path path;
                int64_t major, minor;
                std::optional<uint32_t> fileMode, uid, gid;
        };

        std::optional<std::vector<Device>> devices;

        std::optional<std::filesystem::path> cgroupPath;

        struct Resources {
                struct Device {
                        bool allow;
                        std::string type = "a";
                        std::optional<int64_t> major, minor;
                        std::optional<std::string> access;
                };

                std::optional<std::vector<Device>> devices;

                struct Memory {
                        std::optional<int64_t> limit, reservation, swap, kernel,
                                kernelTCP;
                        std::optional<uint64_t> swappiness;
                        std::optional<bool> disableOOMKiller, useHierarchy,
                                checkBeforeUpdate;
                };

                std::optional<Memory> memory;

                struct CPU {
                        std::optional<uint64_t> shares, period, realtimePeriod;
                        std::optional<int64_t> quota, realtimeRuntime, idle;
                        std::optional<std::string> cpus, mems;
                };

                std::optional<CPU> cpu;

                struct BlockIO {
                        std::optional<uint16_t> weight, leafWeight;

                        struct WeightDevice {
                                int64_t major, minor;
                                std::optional<uint16_t> weight, leafWeight;
                        };

                        std::optional<std::vector<WeightDevice>> weightDevice;

                        struct ThrottleDevice {
                                int64_t major, minor;
                                uint64_t rate;
                        };

                        std::optional<std::vector<ThrottleDevice>>
                                throttleReadBpsDevice, throttleWriteBpsDevice;
                };

                std::optional<BlockIO> blockIO;

                struct HugepageLimit {
                        std::string pageSize;
                        uint64_t limit;
                };

                std::optional<std::vector<HugepageLimit>> hugepageLimits;

                struct Network {
                        std::optional<uint32_t> classID;
                        struct Priority {
                                std::string name;
                                uint32_t priority;
                        };
                        std::optional<std::vector<Priority>> priorities;
                };

                std::optional<Network> network;

                struct PIDs {
                        int64_t limit;
                };

                std::optional<PIDs> pids;

                struct RDMA {
                        std::optional<uint32_t> hcaHanles, hcaObjects;
                };

                std::optional<RDMA> rdma;
        };

        std::optional<Resources> resources;

        static auto from(const std::filesystem::path &file)
                -> std::shared_ptr<Config>;

        static auto from(const nlohmann::json &json) -> std::shared_ptr<Config>;
};

#define JSON_DEFINE BLACKDESK_CPPLIB_JSON_NON_INTRUSIVE
#define OPTIONAL BLACKDESK_CPPLIB_LIST
#define REQUIRED BLACKDESK_CPPLIB_LIST

#define JSON_DEFINE_REQUIRED NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE
#define JSON_DEFINE_OPTIONAL NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT

JSON_DEFINE_REQUIRED(Config::LinuxIDMapping, containerID, hostID, size);
JSON_DEFINE(Config::Root, REQUIRED(path), OPTIONAL(readonly));
JSON_DEFINE(Config::Mount, REQUIRED(destination),
            OPTIONAL(source, options, type, uidMappings, gidMappings));
JSON_DEFINE_REQUIRED(Config::Process::ConsoleSize, height, width);
JSON_DEFINE_REQUIRED(Config::Process::Rlimit, type, soft, hard);
JSON_DEFINE_OPTIONAL(Config::Process::Capabilities, effective, bounding,
                     inheritable, permitted, ambient);
JSON_DEFINE(Config::Process::User, REQUIRED(uid, gid),
            OPTIONAL(umask, additionalGids));
JSON_DEFINE(Config::Hooks::Hook, REQUIRED(path), OPTIONAL(args, env, timeout));
JSON_DEFINE_OPTIONAL(Config::Hooks, prestart, createRuntime, createContainer,
                     startContainer, poststart, poststop);
JSON_DEFINE(Config::Process, REQUIRED(cwd, args, user),
            OPTIONAL(terminal, consoleSize, rlimits, env, apparmorProfile,
                     capabilities, noNewPrivileges, oomScoreAdj, selinuxLabel));
JSON_DEFINE(Config::Namespace, REQUIRED(type), OPTIONAL(path));
JSON_DEFINE(Config::Device, REQUIRED(type, path, major, minor),
            OPTIONAL(fileMode, uid, gid));
JSON_DEFINE(Config::Resources::Device, REQUIRED(allow),
            OPTIONAL(type, major, minor, access));
JSON_DEFINE_OPTIONAL(Config::Resources::Memory, limit, reservation, swap,
                     kernel, kernelTCP, swappiness, disableOOMKiller,
                     useHierarchy, checkBeforeUpdate);
JSON_DEFINE_OPTIONAL(Config::Resources::CPU, shares, period, realtimePeriod,
                     quota, realtimeRuntime, idle, cpus, mems);
JSON_DEFINE(Config::Resources::BlockIO::WeightDevice, REQUIRED(major, minor),
            OPTIONAL(weight, leafWeight));
JSON_DEFINE_REQUIRED(Config::Resources::BlockIO::ThrottleDevice, major, minor,
                     rate);
JSON_DEFINE_OPTIONAL(Config::Resources::BlockIO, weight, leafWeight,
                     weightDevice, throttleReadBpsDevice,
                     throttleWriteBpsDevice);
JSON_DEFINE_REQUIRED(Config::Resources::HugepageLimit, pageSize, limit);
JSON_DEFINE_REQUIRED(Config::Resources::Network::Priority, name, priority);
JSON_DEFINE_OPTIONAL(Config::Resources::Network, classID, priorities);
JSON_DEFINE_REQUIRED(Config::Resources::PIDs, limit);
JSON_DEFINE_OPTIONAL(Config::Resources::RDMA, hcaHanles, hcaObjects);
JSON_DEFINE_OPTIONAL(Config::Resources, devices, memory, cpu, blockIO,
                     hugepageLimits, network, pids, rdma);
JSON_DEFINE(Config, REQUIRED(ociVersion, root),
            OPTIONAL(mounts, process, hooks, annotations, namespaces,
                     uidMappings, gidMappings, devices, resources));

#undef JSON_DEFINE
#undef OPTIONAL
#undef REQUIRED
#undef JSON_DEFINE_OPTIONAL
#undef JSON_DEFINE_REQUIRED

} // namespace cxx_oci_spec
