#include <raylib.h>
#include <math.h>  // For sqrt function

#define NUM_WALLS 10  // Increase number of walls

typedef struct {
    Rectangle rect;  // Wall position and size
} Wall;

int main() {
    int width = 800;
    int height = 800;
    int ball_size = 30;

    // Initialize window and audio
    InitWindow(width + 100, height + 100, "MAZE GAME");
    InitAudioDevice(); // Enable sound
    SetTargetFPS(60);

    // Load Sounds
    Sound moveSound = LoadSound("move.wav");
    Sound winSound = LoadSound("win.wav");
    Sound gameOverSound = LoadSound("gameover.wav");

    // Player's Ball Position
    float playerX = 100, playerY = 100, playerSpeed = 5.0;

    // Enemy Ball Position
    float enemyX = 120, enemyY = 40, enemySpeed = 2.0;

    // Goal Position
    Rectangle goal = {720, 720, 50, 50};

    // More Complex Maze Walls
    Wall walls[NUM_WALLS] = {
        {{200, 100, 400, 20}},  {{300, 300, 20, 300}},  {{500, 500, 200, 20}},
        {{100, 600, 20, 200}},  {{600, 200, 20, 200}},  {{250, 400, 300, 20}},
        {{400, 150, 20, 250}},  {{100, 200, 100, 20}},  {{500, 700, 200, 20}},
        {{700, 100, 20, 400}}};

    bool gameOver = false, gameWon = false;

    // Game Loop
    while (!WindowShouldClose()) {
        if (!gameOver && !gameWon) {
            // Handle Player Movement
            float newPlayerX = playerX, newPlayerY = playerY;
            bool moved = false;

            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) { newPlayerX += playerSpeed; moved = true; }
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) { newPlayerX -= playerSpeed; moved = true; }
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) { newPlayerY += playerSpeed; moved = true; }
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) { newPlayerY -= playerSpeed; moved = true; }

            // Check for collisions
            bool collision = false;
            for (int i = 0; i < NUM_WALLS; i++) {
                if (CheckCollisionCircleRec((Vector2){newPlayerX, newPlayerY}, ball_size, walls[i].rect)) {
                    collision = true;
                    break;
                }
            }

            if (!collision) {
                playerX = newPlayerX;
                playerY = newPlayerY;
                if (moved) PlaySound(moveSound);
            }

            // Move Enemy Towards Player with Smarter AI (avoids walls)
            float diffX = playerX - enemyX;
            float diffY = playerY - enemyY;
            float distance = sqrt(diffX * diffX + diffY * diffY);

            if (distance > 0) {
                float newEnemyX = enemyX + (diffX / distance) * enemySpeed;
                float newEnemyY = enemyY + (diffY / distance) * enemySpeed;

                // Check if enemy collides with walls
                bool enemyCollision = false;
                for (int i = 0; i < NUM_WALLS; i++) {
                    if (CheckCollisionCircleRec((Vector2){newEnemyX, newEnemyY}, ball_size, walls[i].rect)) {
                        enemyCollision = true;
                        break;
                    }
                }

                if (!enemyCollision) {
                    enemyX = newEnemyX;
                    enemyY = newEnemyY;
                }
            }

            // Check Collision (Game Over Condition)
            if (distance <= ball_size * 1.2) {
                gameOver = true;
                PlaySound(gameOverSound);
            }

            // Check if Player Reaches Goal
            if (CheckCollisionCircleRec((Vector2){playerX, playerY}, ball_size, goal)) {
                gameWon = true;
                PlaySound(winSound);
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // Draw Maze Walls
        for (int i = 0; i < NUM_WALLS; i++) {
            DrawRectangleRec(walls[i].rect, BLACK);
        }

        // Draw Goal
        DrawRectangleRec(goal, GREEN);

        if (!gameOver && !gameWon) {
            DrawCircle(playerX, playerY, ball_size, BLUE);
            DrawCircle(enemyX, enemyY, ball_size, RED);
        } else if (gameOver) {
            DrawText("GAME OVER!", 350, 400, 40, RED);
            DrawText("Press R to Restart", 340, 450, 20, BLACK);
        } else if (gameWon) {
            DrawText("YOU WIN!", 350, 400, 40, GREEN);
            DrawText("Press R to Restart", 340, 450, 20, BLACK);
        }

        // Restart if 'R' is Pressed
        if (IsKeyPressed(KEY_R)) {
            playerX = 100;
            playerY = 100;
            enemyX = 120;
            enemyY = 40;
            gameOver = false;
            gameWon = false;
        }

        EndDrawing();
    }

    // Cleanup
    UnloadSound(moveSound);
    UnloadSound(winSound);
    UnloadSound(gameOverSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
