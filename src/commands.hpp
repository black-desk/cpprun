#pragma once

#include <map>
#include <string>

#include <docopt.h>

namespace cpprun
{

int cmd_help(std::map<std::string, docopt::value> &args);
int cmd_create(std::map<std::string, docopt::value> &args);
int cmd_delete(std::map<std::string, docopt::value> &args);
int cmd_exec(std::map<std::string, docopt::value> &args);
int cmd_kill(std::map<std::string, docopt::value> &args);
int cmd_list(std::map<std::string, docopt::value> &args);
int cmd_start(std::map<std::string, docopt::value> &args);
int cmd_state(std::map<std::string, docopt::value> &args);

}
