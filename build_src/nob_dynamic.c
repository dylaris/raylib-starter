#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

#define RAYLIB_SRC "../raylib_src"
#define MAKE "make"
#define CC "clang"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    Cmd cmd = {0};

    cmd_append(&cmd, MAKE, "-C", RAYLIB_SRC, "clean");
    if (!cmd_run_sync_and_reset(&cmd)) {
        nob_log(NOB_ERROR, "failed to clean\n");
        return 1;
    }

    cmd_append(&cmd, MAKE, "-C", RAYLIB_SRC, "PLATFORM=PLATFORM_DESKTOP", "RAYLIB_LIBTYPE=SHARED");
    if (!cmd_run_sync_and_reset(&cmd)) {
        nob_log(NOB_ERROR, "failed to build raylib\n");
        return 1;
    }

#ifdef _WIN32
    cmd_append(&cmd, CC, "-I"RAYLIB_SRC, "-o", "../main", "../main.c", "-L"RAYLIB_SRC, "-lraylib");
#else
    cmd_append(&cmd, CC, "-I"RAYLIB_SRC, "-o", "../main", "../main.c", "-Wl,-rpath="RAYLIB_SRC, "-L"RAYLIB_SRC, "-lraylib");
#endif
    if (!cmd_run_sync_and_reset(&cmd)) {
        nob_log(NOB_ERROR, "failed to build project\n");
        return 1;
    }

    return 0;
}
