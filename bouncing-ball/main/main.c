#include <raylib.h>
#include <raymath.h>
#include <stddef.h>

#include "ball.h"

#define MAX_BALLS 32

__attribute((constructor)) static void setup(void) {
  InitWindow(640, 640, "Bouncing Balls");

  SetTargetFPS(60);

  SetExitKey(KEY_Q);

  SetRandomSeed(0);
}

__attribute((destructor)) static void teardown(void) { CloseWindow(); }

int main(void) {
  struct Ball *balls[MAX_BALLS];

  size_t size = 0;

  balls[size++] = Ball();

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(WHITE);

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && size < MAX_BALLS) {
      balls[size++] = Ball();
    }

    if (IsKeyPressed(KEY_R)) {
      for (size_t index = 0; index < size; ++index) {
        FreeBall(balls[index]);
      }

      size = 0;

      balls[size++] = Ball();
    }

    for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < size; ++j) {
        if (i == j) {
          continue;
        }

        Collide(balls[i], balls[j]);
      }

      UpdateBall(balls[i], GetFrameTime());
    }

    for (size_t index = 0; index < size; ++index) {
      DrawBall(balls[index]);
    }

    const char *copyright = "AssoDePicche © 2025";

    const int fontSize = 20;

    DrawText(copyright, 20, 20, fontSize, DARKGRAY);

    const char *hud = TextFormat("Particles: %zu", size);

    const int size = MeasureText(hud, fontSize);

    DrawText(hud, GetScreenWidth() - size - 20, 20, fontSize, DARKGRAY);

    EndDrawing();
  }

  for (size_t index = 0; index < size; ++index) {
    FreeBall(balls[index]);
  }

  return 0;
}
