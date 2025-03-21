use rand::random_range;
use raylib::{
    ffi::{CheckCollisionCircleRec, Rectangle, Vector2},
    prelude::*,
};

#[derive(Debug)]
struct Ball {
    pos: Vector2,
    mov_vector: Vector2,
    radius: f32,
}

#[derive(Debug)]
struct Player {
    rec: Rectangle,
    score: i32,
    hit_top: bool,
    hit_bot: bool,
}

struct Game {
    ball: Ball,
    players: [Player; 2],
    scoreboard: String,
    window_width: i32,
    window_height: i32,
    move_speed: f32,
}

impl Game {
    fn new() -> Game {
        let window_width = 1280.0;
        let window_height = 720.0;
        let move_speed = window_height * 0.005;

        Game {
            ball: Ball {
                pos: Vector2 {
                    x: window_width / 2.0,
                    y: window_height / 2.0,
                },
                mov_vector: Vector2 { x: 2.0, y: 2.0 },
                radius: 5.5,
            },
            players: [
                Player {
                    rec: Rectangle {
                        x: 0.0,
                        y: 0.0,
                        width: 16.0,
                        height: 130.0,
                    },
                    score: 0,
                    hit_top: false,
                    hit_bot: false,
                },
                Player {
                    rec: Rectangle {
                        x: window_width - 16.0,
                        y: window_height - 130.0,
                        width: 16.0,
                        height: 130.0,
                    },
                    score: 0,
                    hit_top: false,
                    hit_bot: false,
                },
            ],
            scoreboard: "0 - 0".to_string(),
            window_width: window_width as i32,
            window_height: window_height as i32,
            move_speed,
        }
    }

    // Handle player input and movement
    fn handle_input(&mut self, draw_handle: &mut RaylibDrawHandle) {
        let p1 = &mut self.players[0];
        if draw_handle.is_key_down(KeyboardKey::KEY_W) && !p1.hit_top {
            p1.rec.y -= self.move_speed;
        }
        if draw_handle.is_key_down(KeyboardKey::KEY_S) && !p1.hit_bot {
            p1.rec.y += self.move_speed;
        }

        let p2 = &mut self.players[1];
        if draw_handle.is_key_down(KeyboardKey::KEY_UP) && !p2.hit_top {
            p2.rec.y -= self.move_speed;
        }
        if draw_handle.is_key_down(KeyboardKey::KEY_DOWN) && !p2.hit_bot {
            p2.rec.y += self.move_speed;
        }
    }

    // Update the physics (ball movement, collisions)
    fn handle_physics(&mut self) {
        self.ball.pos.y += self.ball.mov_vector.y;
        self.ball.pos.x += self.ball.mov_vector.x;

        // Handle ball hitting top or bottom
        if self.ball.pos.y - self.ball.radius <= 0.0
            || self.ball.pos.y + self.ball.radius >= self.window_height as f32
        {
            self.ball.mov_vector.y = -self.ball.mov_vector.y;
        }

        if unsafe { CheckCollisionCircleRec(self.ball.pos, self.ball.radius, self.players[0].rec) }
            || unsafe {
                CheckCollisionCircleRec(self.ball.pos, self.ball.radius, self.players[1].rec)
            }
        {
            self.ball.mov_vector.x = -self.ball.mov_vector.x;
        }

        // Ball out of bounds logic (scoring)
        if self.ball.pos.x <= 0.0 {
            self.players[1].score += 1;
            self.reset_ball();
        } else if self.ball.pos.x >= self.window_width as f32 {
            self.players[0].score += 1;
            self.reset_ball();
        }

        // Update score UI string
        self.scoreboard = format!("{} - {}", self.players[0].score, self.players[1].score);
    }

    // Reset the ball position after scoring
    fn reset_ball(&mut self) {
        self.ball.pos = Vector2 {
            x: self.window_width as f32 / 2.0,
            y: self.window_height as f32 / 2.0,
        };
        //TODO: randomize launch
        self.ball.mov_vector = Vector2 {
            x: random_range(-3.0..=3.0),
            y: random_range(-3.0..=3.0),
        };
    }

    // Draw the game objects
    fn draw(&mut self, draw_handle: &mut RaylibDrawHandle) {
        draw_handle.clear_background(Color::BLACK);
        draw_handle.draw_text(
            &self.scoreboard,
            self.window_width as i32 / 2 - 50,
            20,
            30,
            Color::WHITE,
        );
        draw_handle.draw_circle_v(self.ball.pos, self.ball.radius, Color::GREEN);
        draw_handle.draw_rectangle_rec(self.players[0].rec, Color::RED);
        draw_handle.draw_rectangle_rec(self.players[1].rec, Color::BLUE);
    }
}

fn main() {
    let w_width = 1280;
    let w_height = 720;

    let (mut rl, thread) = raylib::init()
        .size(w_width, w_height)
        .title("Pong - Raylib.rs")
        .build();
    rl.set_target_fps(240);

    let mut game = Game::new();
    while !rl.window_should_close() {
        let mut draw_handle = rl.begin_drawing(&thread);
        game.handle_input(&mut draw_handle);
        game.handle_physics();
        game.draw(&mut draw_handle);
    }
}
