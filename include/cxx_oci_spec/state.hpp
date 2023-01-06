#pragma once

#include "serializer.hpp"

namespace cxx_oci_spec
{

// https://github.com/opencontainers/runtime-spec/blob/main/runtime.md#state
struct State {
        struct Annotations {
                int monitorPid;
        };

        enum Status { Creating, Created, Running, Stopped };

        std::string ociVersion;
        std::string id;
        Status status;
        int pid;
        std::string bundle;
        std::optional<Annotations> annotations;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(State::Annotations, monitorPid);
NLOHMANN_JSON_SERIALIZE_ENUM(State::Status,
                             {
                                     { State::Status::Creating, "creating" },
                                     { State::Status::Created, "created" },
                                     { State::Status::Running, "running" },
                                     { State::Status::Stopped, "stopped" },
                             });
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(State, ociVersion, id, status,
                                                pid, bundle, annotations);

}
