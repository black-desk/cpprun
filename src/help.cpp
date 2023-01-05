#include "help.hpp"

#include <iostream>

#include "commands.hpp"

namespace cpprun
{

auto cmd_help(std::map<std::string, docopt::value> &args) -> int
{
        auto sub_command = args.find("<sub_command>")->second.asString();

        if (sub_command == "create") {
                std::cout << CREATE_DOC;
                return 0;
        }
        if (sub_command == "start") {
                std::cout << START_DOC;
                return 0;
        }
        if (sub_command == "exec") {
                std::cout << EXEC_DOC;
                return 0;
        }
        if (sub_command == "kill") {
                std::cout << KILL_DOC;
                return 0;
        }
        if (sub_command == "state") {
                std::cout << STATE_DOC;
                return 0;
        }
        if (sub_command == "list") {
                std::cout << LIST_DOC;
                return 0;
        }
        if (sub_command == "delete") {
                std::cout << DELETE_DOC;
                return 0;
        }
        static_assert(true, "docopt bug");

        return 0;
}
} // namespace cpprun
