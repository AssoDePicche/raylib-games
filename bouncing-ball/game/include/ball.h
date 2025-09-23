#ifndef __BALL__
#define __BALL__

#include <raylib.h>
#include <stdbool.h>

struct Ball;

struct Ball *Ball(void);

void FreeBall(struct Ball *);

bool CollideWithPoint(const struct Ball *, const Vector2);

void Collide(struct Ball *, struct Ball *);

void DrawBall(const struct Ball *);

void UpdateBall(struct Ball *, const float);

#endif
