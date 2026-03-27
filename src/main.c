#include "raylib.h"

#define BRICK_ROWS 5
#define BRICK_COLUMNS 10

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Block Buster");
    SetTargetFPS(60);

    Rectangle paddle = {screenWidth/2 - 50, screenHeight - 40, 100, 20};

    Vector2 ballPosition = {screenWidth/2, screenHeight - 60};
    Vector2 ballSpeed = {0,0};
    int ballRadius = 10;
    bool ballActive = false;
    Color ballColor = BLUE;

    Rectangle bricks[BRICK_ROWS][BRICK_COLUMNS];
    bool brickAlive[BRICK_ROWS][BRICK_COLUMNS];

    int brickWidth = screenWidth / BRICK_COLUMNS;
    int brickHeight = 30;

    int lives = 3;
    int level = 1;
    int score = 0;
    bool gameOver = false;
    
    for(int row = 0; row < BRICK_ROWS; row++)
    for(int column = 0; column < BRICK_COLUMNS; column++){
        bricks[row][column] = (Rectangle){column * brickWidth, 50 + row * brickHeight, brickWidth - 2, brickHeight - 2};
        brickAlive[row][column] = true;
    }

    while(!WindowShouldClose())
    {
        if(!gameOver)
        {
            // Paddle
            if(IsKeyDown(KEY_LEFT)) paddle.x -= 6;
            if(IsKeyDown(KEY_RIGHT)) paddle.x += 6;
            if(paddle.x < 0) paddle.x = 0;
            if(paddle.x + paddle.width > screenWidth) paddle.x = screenWidth - paddle.width;

            // Ball start
            if(!ballActive){
                ballPosition = (Vector2){paddle.x + paddle.width/2, paddle.y - ballRadius};
                if(IsKeyPressed(KEY_SPACE)){
                    ballActive = true;
                    ballSpeed = (Vector2){4 + level, -4 - level};
                }
            }

            // Ball move
            if(ballActive){
                ballPosition.x += ballSpeed.x;
                ballPosition.y += ballSpeed.y;
            }

            // Walls
            if(ballPosition.x <= ballRadius || ballPosition.x >= screenWidth - ballRadius) ballSpeed.x *= -1;
            if(ballPosition.y <= ballRadius) ballSpeed.y *= -1;

            // Miss
            if(ballPosition.y > screenHeight){
                ballActive = false;
                lives--;
                if(lives <= 0) gameOver = true;
            }

            // Paddle hit
            if(CheckCollisionCircleRec(ballPosition, ballRadius, paddle)){
                ballSpeed.y *= -1;
                ballSpeed.x = (ballPosition.x - (paddle.x + paddle.width/2)) * 0.1f;
            }

            // Bricks
            bool allGone = true;
            for(int row = 0; row < BRICK_ROWS; row++)
            for(int column = 0; column < BRICK_COLUMNS; column++)
            {
                if(brickAlive[row][column])
                {
                    allGone = false;

                    if(CheckCollisionCircleRec(ballPosition, ballRadius, bricks[row][column])){
                        brickAlive[row][column] = false;
                        ballSpeed.y *= -1;

                        // change colour
                        ballColor = (Color){GetRandomValue(50,255), GetRandomValue(50,255), GetRandomValue(50,255), 255};
                        score++;
                    }
                }
            }

            // Next level
            if(allGone){
                level++;
                ballActive = false;

                for(int r = 0; r < BRICK_ROWS; r++)
                for(int c = 0; c < BRICK_COLUMNS; c++)
                    brickAlive[r][c] = true;
            }
        }

        // Restart
        if(gameOver && IsKeyPressed(KEY_ENTER)){
            lives = 3;
            level = 1;
            gameOver = false;
            ballActive = false;

            for(int r = 0; r < BRICK_ROWS; r++)
            for(int c = 0; c < BRICK_COLUMNS; c++)
                brickAlive[r][c] = true;
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if(!gameOver)
        {
            DrawRectangleRec(paddle, BLACK);
            DrawCircleV(ballPosition, ballRadius, ballColor);

            for(int row = 0; row < BRICK_ROWS; row++)
            for(int column = 0; column < BRICK_COLUMNS; column++)
                if(brickAlive[row][column])
                    DrawRectangleRec(bricks[row][column], BLACK);

            DrawText(TextFormat("Lives: %d", lives), 10, 10, 20, BLACK);
            DrawText(TextFormat("Level: %d", level), 680, 10, 20, BLACK);
            DrawText(TextFormat("Score: %d", score), 400, 10, 20, BLACK);
        }
        else
        {
            DrawText("BETTER LUCK NEXT TIME", 175, 180, 40, RED);
            DrawText("Press ENTER to restart", 270, 240, 20, BLACK);
            DrawText("YOUR SCORE IS", 270, 260, 20, BLACK);
            DrawText(TextFormat(" : %d", score), 450, 260, 20, BLACK);
        }   

        EndDrawing();
    }

    CloseWindow();
}

//Make the ball change colour when it hits a brick ✔
//Add a game over screen so that the game can reset and start again ✔
//Add extra levels to the game and add lives so that the game ends ✔