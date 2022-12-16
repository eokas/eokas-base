
#include "../engine/main.h"
using namespace eokas;

_eokas_test_case(cli)
{
    int argc = 8;
    char* argv[] = {"docker", "run", "--name=eokas", "-p", "80:80", "-v", "/data:/data", "eokas/eokas" };

    cli::Command program(argv[0]);
    program.action([&](const cli::Command& cmd)->void {
        printf(cmd.toString().cstr());
    });

    program.subCommand("run", "")
        .option("--name,-n", "", "")
        .option("--publish,-p", "", "")
        .option("--volume,-v", "", "")
        .action([&](const cli::Command& cmd)->void{
            printf(cmd.toString().cstr());
        });

    program.exec(argc, argv);

    return 0;
}
