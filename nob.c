#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#ifdef _WIN32
    #define PLATFORM "win"
    #define RAYLIB_DIR "raylib/raylib-5.5_win64_mingw-w64/"
    #define RAYLIB_DLL "raylib.dll"
    #define CC "gcc"
#else
    #define PLATFORM "linux"
    #define RAYLIB_DIR "raylib/raylib-5.5_linux_amd64/"
    #define RAYLIB_DLL "libraylib.so"
    #define CC "gcc"
#endif

static Cmd cmd = {0};

static void link_static(void)
{
    cmd_append(&cmd, RAYLIB_DIR"lib/libraylib.a");
#ifdef _WIN32
    cmd_append(&cmd, "-lgdi32", "-lwinmm", "-lmsvcrt", "-lucrtbase");
#else
    cmd_append(&cmd, "-lm");
#endif
}

static void link_dynamic(void)
{
#ifdef _WIN32
    if (!nob_copy_file(RAYLIB_DIR"lib/raylib.dll", "./raylib.dll")) {
        nob_log(NOB_ERROR, "failed to copy 'raylib.dll' to current direcory");
        exit(EXIT_FAILURE);
    }
    cmd_append(&cmd, RAYLIB_DIR"lib/libraylibdll.a");
#else
    cmd_append(&cmd, "-Wl,-rpath="RAYLIB_DIR"lib");
    cmd_append(&cmd, "-L"RAYLIB_DIR"lib", "-l:libraylib.so");
#endif
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    cmd_append(&cmd, CC);
    cmd_append(&cmd, "-Wall", "-Wextra");
    cmd_append(&cmd, "-ggdb");
    cmd_append(&cmd, "-I"RAYLIB_DIR"include");
    cmd_append(&cmd, "-o", "main", "main.c");
#ifdef LINK_STATIC
    link_static();
#else
    link_dynamic();
#endif

    if (!cmd_run(&cmd)) return 1;
    return 0;
}

