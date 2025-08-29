
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define APP_NAME "nosh"

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "cc");
    nob_cmd_append(&cmd, "-Wall");
    nob_cmd_append(&cmd, "main.c");
    nob_cmd_append(&cmd, "-o");
    nob_cmd_append(&cmd, APP_NAME);
    nob_cmd_append(&cmd, "-lreadline");

    if (!nob_cmd_run(&cmd)) {
        return 1;
    }

    return 0;
}