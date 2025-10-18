#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

#ifdef _WIN32
#define RAYLIB_DIR "raylib/raylib-5.5_win64_mingw-w64/"
#else
#define RAYLIB_DIR "raylib/raylib-5.5_linux_amd64/"
#endif
#define BIN_DIR  "bin/"
#define SRC_FILE "main.c"
#define TARGET   BIN_DIR"main"
#define CC       "gcc"

bool build_static(void)
{
    Cmd cmd = {0};
    if (!nob_mkdir_if_not_exists(BIN_DIR)) return false;
#ifdef _WIN32
    cmd_append(&cmd, CC,
               "-I"RAYLIB_DIR"include",
               "-o", TARGET, SRC_FILE, RAYLIB_DIR"lib/libraylib.a",
               "-lgdi32", "-lwinmm", "-lmsvcrt", "-lucrtbase");
#else
    cmd_append(&cmd, CC,
               "-I"RAYLIB_DIR"include",
               "-o", TARGET, SRC_FILE, RAYLIB_DIR"lib/libraylib.a",
               "-lm");
#endif
    if (!cmd_run(&cmd)) return false;
    return true;
}

bool build_dynamic(void)
{
    Cmd cmd = {0};
    if (!nob_mkdir_if_not_exists(BIN_DIR)) return false;
#ifdef _WIN32
    if (!nob_copy_file(RAYLIB_DIR"lib/raylib.dll", BIN_DIR"raylib.dll")) return false;
    cmd_append(&cmd, CC,
               "-I"RAYLIB_DIR"include",
               "-o", TARGET, SRC_FILE, RAYLIB_DIR"lib/libraylibdll.a",
               "-L"RAYLIB_DIR"lib", "-lraylib");
#else
    cmd_append(&cmd, CC,
               "-I"RAYLIB_DIR"include",
               "-o", TARGET, SRC_FILE,
               "-Wl,-rpath="RAYLIB_DIR"lib", "-L"RAYLIB_DIR"lib", "-lraylib");
#endif
    if (!cmd_run(&cmd)) return false;
    return true;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (argc != 2) {
        nob_log(NOB_INFO, "usage: ./nob [static|dynamic]");
        return 1;
    }

    const char *type = argv[1];
    if (strcmp(type, "static") == 0) {
        if (!build_static()) return 1;
    } else if (strcmp(type, "dynamic") == 0) {
        if (!build_dynamic()) return 1;
    } else {
        nob_log(NOB_INFO, "usage: ./nob [static|dynamic]");
        return 1;
    }

    return 0;
}
