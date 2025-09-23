#include "ball.h"

#include <math.h>
#include <raymath.h>
#include <stddef.h>

#ifndef __WORLD_GRAVITY__
#define __WORLD_GRAVITY__ 9.81f
#endif

struct Ball {
  Vector2 center;
  Vector2 previousCenter;
  Vector2 velocity;
  Color color;
  float radius;
  float friction;
  float elasticity;
  float mass;
  bool floating;
};

static Color GetRandomColor(void) {
  return (Color){
      .r = GetRandomValue(0, 255),
      .g = GetRandomValue(0, 255),
      .b = GetRandomValue(0, 255),
      .a = 255,
  };
}

struct Ball *Ball(void) {
  struct Ball *this = (struct Ball *)MemAlloc(sizeof(struct Ball));

  if (NULL == this) {
    return NULL;
  }

  if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    this->center = (Vector2){
        .x = GetMouseX() % GetScreenWidth(),
        .y = GetMouseY() % GetScreenHeight(),
    };
  } else {
    this->center = (Vector2){
        .x = GetScreenWidth() / 2.0f,
        .y = GetScreenHeight() / 2.0f,
    };
  }

  this->previousCenter = this->center;

  this->radius = GetRandomValue(20, 35);

  this->velocity = (Vector2){
      .x = 0.0f,
      .y = GetRandomValue(100.0f, 200.0f),
  };

  this->friction = 0.99f;

  this->elasticity = 0.9f;

  this->mass = GetRandomValue(1.0f, 5.0f);

  this->color = GetRandomColor();

  return this;
}

void FreeBall(struct Ball *this) {
  if (NULL == this) {
    return;
  }

  MemFree(this);
}

bool CollideWithPoint(const struct Ball *this, const Vector2 point) {
  const float dx = point.x - this->center.x;

  const float dy = point.y - this->center.y;

  return dx * dx + dy * dy <= this->radius * this->radius;
}

void Collide(struct Ball *this, struct Ball *other) {
  Vector2 r = Vector2Subtract(other->center, this->center);

  float distance = Vector2Length(r);

  float tangent = this->radius + other->radius;

  if (tangent < distance) {
    return;
  }

  Vector2 t = Vector2Scale(r, 0.5f * (distance - tangent) / distance);

  this->center = Vector2Add(this->center, t);

  other->center = Vector2Subtract(other->center, t);

  r = Vector2Scale(r, tangent / distance);

  Vector2 v = Vector2Subtract(other->velocity, this->velocity);

  float k = 2.0f * Vector2DotProduct(v, r);

  float z = (this->mass + other->mass) * tangent * tangent;

  k /= z;

  this->velocity = Vector2Add(this->velocity, Vector2Scale(r, k * other->mass));

  other->velocity =
      Vector2Add(other->velocity, Vector2Scale(r, -k * this->mass));
}

void DrawBall(const struct Ball *this) {
  DrawCircle((int)this->center.x, (int)this->center.y, this->radius,
             this->color);
}

void UpdateBall(struct Ball *this, const float dt) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
      CollideWithPoint(this, GetMousePosition())) {
    this->floating = true;
  }

  if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
    this->floating = false;
  }

  if (this->floating) {
    this->center = GetMousePosition();

    this->velocity = Vector2Scale(
        Vector2Subtract(this->center, this->previousCenter), 1 / dt);

    this->previousCenter = this->center;

    return;
  }

  this->center = Vector2Add(this->center, Vector2Scale(this->velocity, dt));

  if ((this->center.x - this->radius <= 0.0f) ||
      (GetScreenWidth() < this->center.x + this->radius)) {
    this->velocity.x = -this->velocity.x * this->elasticity;
  }

  if ((GetScreenHeight() < this->center.y + this->radius) ||
      (this->center.y - this->radius <= 0.0f)) {
    this->velocity.y = -this->velocity.y * this->elasticity;
  }

  if (GetScreenWidth() < this->center.x + this->radius) {
    this->center.x = GetScreenWidth() - this->radius;
  }

  if (this->center.x - this->radius <= 0.0f) {
    this->center.x = this->radius;
  }

  if (GetScreenHeight() < this->center.y + this->radius) {
    this->center.y = GetScreenHeight() - this->radius;
  }

  if (this->center.y - this->radius <= 0.0f) {
    this->center.y = this->radius;
  }

  this->velocity.x = this->velocity.x * this->friction;

  this->velocity.y =
      this->velocity.y * this->friction + __WORLD_GRAVITY__ * this->mass;
}
