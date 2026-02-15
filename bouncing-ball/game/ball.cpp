#include "ball.h"

#include <math.h>
#include <raymath.h>
#include <stddef.h>

#ifndef __WORLD_GRAVITY__
#define __WORLD_GRAVITY__ 9.81f
#endif

static Color GetRandomColor(void) {
  return (Color){
      .r = GetRandomValue(0, 255),
      .g = GetRandomValue(0, 255),
      .b = GetRandomValue(0, 255),
      .a = 255,
  };
}

Ball *CreateBall(void) {
  Ball *ball = (Ball *)MemAlloc(sizeof(Ball));

  if (NULL == ball) {
    return NULL;
  }

  if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    ball->center = (Vector2){
        .x = GetMouseX() % GetScreenWidth(),
        .y = GetMouseY() % GetScreenHeight(),
    };
  } else {
    ball->center = (Vector2){
        .x = GetScreenWidth() / 2.0f,
        .y = GetScreenHeight() / 2.0f,
    };
  }

  ball->previousCenter = ball->center;

  ball->radius = GetRandomValue(20, 35);

  ball->velocity = (Vector2){
      .x = 0.0f,
      .y = GetRandomValue(100.0f, 200.0f),
  };

  ball->friction = 0.99f;

  ball->elasticity = 0.9f;

  ball->mass = GetRandomValue(1.0f, 5.0f);

  ball->color = GetRandomColor();

  return ball;
}

void FreeBall(Ball *ball) {
  if (NULL == ball) {
    return;
  }

  MemFree(ball);
}

bool Ball::CollideWithPoint(const Vector2 point) {
  const float dx = point.x - center.x;

  const float dy = point.y - center.y;

  return dx * dx + dy * dy <= radius * radius;
}

void Ball::Collide(Ball *other) {
  Vector2 r = Vector2Subtract(other->center, center);

  float distance = Vector2Length(r);

  float tangent = radius + other->radius;

  if (tangent < distance) {
    return;
  }

  Vector2 t = Vector2Scale(r, 0.5f * (distance - tangent) / distance);

  center = Vector2Add(center, t);

  other->center = Vector2Subtract(other->center, t);

  r = Vector2Scale(r, tangent / distance);

  Vector2 v = Vector2Subtract(other->velocity, velocity);

  float k = 2.0f * Vector2DotProduct(v, r);

  float z = (mass + other->mass) * tangent * tangent;

  k /= z;

  velocity = Vector2Add(velocity, Vector2Scale(r, k * other->mass));

  other->velocity =
      Vector2Add(other->velocity, Vector2Scale(r, -k * mass));
}

void Ball::Draw() {
  DrawCircle((int)center.x, (int)center.y, radius,
             color);
}

void Ball::Update(const float dt) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
      CollideWithPoint(GetMousePosition())) {
    floating = true;
  }

  if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
    floating = false;
  }

  if (floating) {
    center = GetMousePosition();

    velocity = Vector2Scale(Vector2Subtract(center, previousCenter), 1 / dt);

    previousCenter = center;

    return;
  }

  center = Vector2Add(center, Vector2Scale(velocity, dt));

  if ((center.x - radius <= 0.0f) ||
      (GetScreenWidth() < center.x + radius)) {
    velocity.x = -velocity.x * elasticity;
  }

  if ((GetScreenHeight() < center.y + radius) ||
      (center.y - radius <= 0.0f)) {
    velocity.y = -velocity.y * elasticity;
  }

  if (GetScreenWidth() < center.x + radius) {
    center.x = GetScreenWidth() - radius;
  }

  if (center.x - radius <= 0.0f) {
    center.x = radius;
  }

  if (GetScreenHeight() < center.y + radius) {
    center.y = GetScreenHeight() - radius;
  }

  if (center.y - radius <= 0.0f) {
    center.y = radius;
  }

  velocity.x = velocity.x * friction;

  velocity.y = velocity.y * friction + __WORLD_GRAVITY__ * mass;
}
