# cpprun

`cpprun` is a toy OCI runtime implementation written in cpp.

    Usage:
      cpprun --version
      cpprun --help
      cpprun [options] features [<subcommand_args>...]
      cpprun [options] state [<subcommand_args>...]
      cpprun [options] create [<subcommand_args>...]
      cpprun [options] start [<subcommand_args>...]
      cpprun [options] kill [<subcommand_args>...]
      cpprun [options] delete [<subcommand_args>...]
      cpprun [options] list [<subcommand_args>...]
      cpprun [options] exec [<subcommand_args>...]
      cpprun [options] run [<subcommand_args>...]

    Options:
      --json  Output result (if any) in json format.
      --root  Specify the directory to store container state.

You can get document of subcommand by running `cpprun <subcommand> --help`.
