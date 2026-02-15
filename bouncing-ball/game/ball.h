#pragma once

#include <raylib.h>

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

  bool CollideWithPoint(const Vector2);

  void Collide(Ball *);

  void Draw();

  void Update(const float);
};

Ball *CreateBall(void);

void FreeBall(struct Ball *);
