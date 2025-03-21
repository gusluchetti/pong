#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <random>
#include <raylib.h>
#include <string>

struct Ball {
  Vector2 pos;
  Vector2 movementVector;
  float radius = 5.5f;
};

struct Player {
  Rectangle rec;
  int score = 0;
  bool hitTop = false;
  bool hitBottom = false;
};

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(5.0, 7.0);
float randomX = dis(gen);
float randomY = dis(gen);

float w_Width = 1280;
float w_Height = 720;
const float moveSpeed = w_Height * 0.01;

Ball ball;
Player p1, p2;
std::string scoreUI;

void DrawObjects() {
  std::cout << "\np1 (left) " << p1.rec.x << " " << p1.rec.y;
  std::cout << " | p2 (right) " << p2.rec.x << " " << p2.rec.y;
  ClearBackground(BLACK);
  DrawText(scoreUI.c_str(), w_Width / 2, 20, 30, WHITE);
  DrawCircle(ball.pos.x, ball.pos.y, ball.radius, GREEN);
  DrawRectangleRec(p1.rec, BLUE);
  DrawRectangleRec(p2.rec, RED);
}

void BlockPlayers(Player *pList[2]) {
  for (size_t i = 0; i <= 1; ++i) {
    if (pList[i] != nullptr) {
      Player *p = pList[i];
      bool hitTop = CheckCollisionPointLine({p->rec.x, p->rec.y}, {0.0f, 0.0f},
                                            {w_Width, 0.0f}, 1);
      if (hitTop) {
        p->hitTop = true;
      } else {
        p->hitTop = false;
      }
      bool hitBottom =
          CheckCollisionPointLine({(p->rec.x), (p->rec.y + p->rec.height)},
                                  {0.0f, w_Height}, {w_Width, w_Height}, 1);
      if (hitBottom) {
        p->hitBottom = true;
      } else {
        p->hitBottom = false;
      }
    }
  }
}

void HandleEvents() {
  Player *playerList[2] = {&p1, &p2};
  BlockPlayers(playerList);

  if (IsKeyDown(KEY_W) && p1.hitTop == false) {
    p1.rec.y -= moveSpeed;
  }
  if (IsKeyDown(KEY_S) && p1.hitBottom == false) {
    p1.rec.y += moveSpeed;
  }

  if (IsKeyDown(KEY_UP) && p2.hitTop == false) {
    p2.rec.y -= moveSpeed;
  }
  if (IsKeyDown(KEY_DOWN) && p2.hitBottom == false) {
    p2.rec.y += moveSpeed;
  }

  if (IsKeyDown(KEY_ESCAPE)) {
    CloseWindow();
  }
}

void HandlePhysics() {
  ball.pos.y += ball.movementVector.y;
  ball.pos.x += ball.movementVector.x;

  bool hitHorizontal =
      CheckCollisionCircleLine(ball.pos, ball.radius, {0.0f, 0.0f},
                               {w_Width, 0.0f}) ||
      CheckCollisionCircleLine(ball.pos, ball.radius, {0.0f, w_Height},
                               {w_Width, w_Height});
  if (hitHorizontal) {
    ball.movementVector.y = -ball.movementVector.y;
  }

  bool hitAnyPlayer = CheckCollisionCircleRec(ball.pos, ball.radius, p1.rec) ||
                      CheckCollisionCircleRec(ball.pos, ball.radius, p2.rec);
  if (hitAnyPlayer) {
    ball.movementVector.x =
        (std::abs(ball.movementVector.x) + 1) *
        -(ball.movementVector.x / std::abs(ball.movementVector.x));
    std::cout << "\nhit player! " << ball.pos.x << " " << ball.pos.y;
  }

  bool isBallInsideWindow = CheckCollisionCircleRec(
      ball.pos, ball.radius, {0.0f, 0.0f, w_Width, w_Height});
  if (!isBallInsideWindow) {
    if (ball.pos.x <= 0) {
      p2.score += 1;
    }
    if (ball.pos.x >= w_Width) {
      p1.score += 1;
    }
    scoreUI = std::to_string(p1.score) + " - " + std::to_string(p2.score);

    ball.pos = {(float)w_Width / 2, (float)w_Height / 2};
    randomX = dis(gen);
    randomY = dis(gen);
    ball.movementVector = {randomX, randomY};
  }
}

int main() {
  SetTargetFPS(60);
  InitWindow(w_Width, w_Height, "pong");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  // SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
  SetWindowState(FLAG_WINDOW_MAXIMIZED);

  w_Width = GetScreenWidth();
  w_Height = GetScreenHeight();

  ball = {{(float)w_Width / 2, (float)w_Height / 2}, {randomX, randomY}};
  p1 = {{0.0f, 0.0f, 12, 108}};
  p2 = {{w_Width - p1.rec.width, w_Height - p1.rec.height, p1.rec.width,
         p1.rec.height}};
  scoreUI = std::to_string(p1.score) + " - " + std::to_string(p2.score);

  std::cout << "\nstart!\n";
  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      DrawObjects();
      HandleEvents();
      HandlePhysics();
    }
    EndDrawing();
  }
  std::cout << "\n!finish\n";

  CloseWindow();

  return 0;
}
