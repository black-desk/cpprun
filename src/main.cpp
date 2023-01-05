#include <docopt.h>

#include "black_desk/cpplib/Linux/utils.hpp"
#include "commands.hpp"
#include "config.hpp"
#include "private_common.hpp"
#include "reexec.hpp"

static const std::string_view USAGE = R"(cpprun: a toy OCI runtime.

Usage:
  cpprun create <id> <path_to_bundle>
  cpprun start  [ ( -i | --console-socket=<address> ) --extra-fds=<nfd> ] <id>
  cpprun exec   [ ( -i | --console-socket=<address> ) --extra-fds=<nfd> ] <id>
                ( -p <path_to_json> | -- <command>... )
  cpprun kill   <id> <signal>
  cpprun state  <id>
  cpprun list
  cpprun delete <id>
  cpprun help   <sub_command>
  cpprun -h | --help
  cpprun -v | --version

Options:
  -h --help                   Show this screen.
  -v --version                Show version.
  -i                          Execute command interactively.
  --console-socket=<address>  Socket address to recive tty fd of process
                              [default: ].
  --extra-fds=<nfds>          Number of extra fds should be passed to
                              process [default: 0].)";

auto main(int argc, const char **argv) -> int
{
        try {
                {
                        bool should_reexec = false;
                        {
                                const std::filesystem::path exe =
                                        argv[0]; // NOLINT
                                if (std::filesystem::exists(exe)) {
                                        auto real_exe =
                                                std::filesystem::canonical(exe);
                                        if (real_exe.has_filename() &&
                                            real_exe.filename() != "cpprun") {
                                                should_reexec = true;
                                        }
                                }
                        }
                        if (should_reexec) {
                                return cpprun::reexec(argc, argv); // NOLINT
                        }
                }

                auto args = docopt::docopt(std::string(USAGE),
                                           { argv + 1, argv + argc }, // NOLINT
                                           true, std::string(cpprun::VERSION));

                if (args.find("create")->second.asBool()) {
                        return cpprun::cmd_create(args);
                }
                if (args.find("start")->second.asBool()) {
                        return cpprun::cmd_start(args);
                }
                if (args.find("exec")->second.asBool()) {
                        return cpprun::cmd_exec(args);
                }
                if (args.find("kill")->second.asBool()) {
                        return cpprun::cmd_kill(args);
                }
                if (args.find("state")->second.asBool()) {
                        return cpprun::cmd_state(args);
                }
                if (args.find("list")->second.asBool()) {
                        return cpprun::cmd_list(args);
                }
                if (args.find("delete")->second.asBool()) {
                        return cpprun::cmd_delete(args);
                }
                if (args.find("help")->second.asBool()) {
                        return cpprun::cmd_help(args);
                }
        } catch (const std::exception &exception) {
                SPDLOG_ERROR("cpprun failed:\n{}", exception);
                return -1;
        }
}
