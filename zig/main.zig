const rl = @cImport({
    @cInclude("raylib.h");
});

pub fn main() void {
    rl.InitWindow(800, 600, "Test");
    defer rl.CloseWindow();

    rl.SetTargetFPS(60);

    while (!rl.WindowShouldClose()) {
        rl.BeginDrawing();
        defer rl.EndDrawing();
        rl.ClearBackground(rl.RAYWHITE);
        rl.DrawText("Hello Raylib!", 800/2-@divTrunc(rl.MeasureText("Hello Raylib!", 20), 2), 100, 20, rl.RED);
        rl.DrawFPS(10, 10);
    }
}
