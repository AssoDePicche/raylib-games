namespace raylib {
  #include <raylib.h>
}

int main(void) {
  raylib::InitWindow(640, 480, "Dinos Take Over The World");

  raylib::SetTargetFPS(60);

  raylib::SetExitKey(raylib::KEY_Q);

  while (!raylib::WindowShouldClose()) {
    raylib::BeginDrawing();

    raylib::ClearBackground(raylib::WHITE);

    raylib::EndDrawing();
  }

  return 0;
}
