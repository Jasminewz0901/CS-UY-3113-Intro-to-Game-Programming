#include "CS3113/cs3113.h"
#include <math.h>

/** 
* Author: Jasmine Zhang
* Assignment: Pong Clone
* Date Due: 10/16/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on 
* Academic Misconduct.
**/

// Global Constants
constexpr int SCREEN_WIDTH  = 1600 / 2,
              SCREEN_HEIGHT = 900 / 2,
              FPS           = 60;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr char LEFTGOALIE_FP[] = "assets/LeftGoalie.png";
constexpr char RIGHTGOALIE_FP[] = "assets/RightGoalie.png";
constexpr char BALL_FP[] = "assets/SoccerBall.png";
constexpr char FIELD_FP[] = "assets/SoccerField.png";
constexpr char BGM_FP[] = "assets/Background.mp3";
/*
"Sergio's Magic Dustbin" Kevin MacLeod (incompetech.com)
Licensed under Creative Commons: By Attribution 4.0 License
http://creativecommons.org/licenses/by/4.0/
*/
constexpr char CHEER_FP[] = "assets/Cheering.mp3";
constexpr char GOAL_FP[] = "assets/Goal2.mp3";
constexpr char ZERO_FP[] = "assets/numbers/zero.png";
constexpr char ONE_FP[] = "assets/numbers/one.png";
constexpr char TWO_FP[] = "assets/numbers/two.png";
constexpr char THREE_FP[] = "assets/numbers/three.png";
constexpr char FOUR_FP[] = "assets/numbers/four.png";
constexpr char FIVE_FP[] = "assets/numbers/five.png";
constexpr char SIX_FP[] = "assets/numbers/six.png";
constexpr char SEVEN_FP[]= "assets/numbers/seven.png";
constexpr char EIGHT_FP[] = "assets/numbers/eight.png";
constexpr char NINE_FP[] = "assets/numbers/nine.png";
constexpr char WIN_FP[] = "assets/Win.png";
constexpr char WIN2_FP[] = "assets/Win2.png";
constexpr char WIN3_FP[] = "assets/Win3.png";
constexpr char LOSE_FP[] = "assets/Lose.png";
constexpr char LOSE2_FP[] = "assets/Lose2.png";
constexpr char LOSE3_FP[] = "assets/Lose3.png";
constexpr char START_FP[] = "assets/starterscreen.png";
constexpr int GOALIE_SPEED = 200;
constexpr float BALL_SPEED = 200.0f;
constexpr int WINNING_SCORE = 11;
constexpr int MAX_BALLS = 9;

float gPreviousTicks = 0.0f;
int gLeftScore = 0;
int gRightScore = 0;
bool gAIMode = false;
int gActiveBalls = 1;
float gAIDirection = 1.0f;
bool gGameStarted = false;
float gBallRotations[MAX_BALLS] = {};
float gBallRotationDirections[MAX_BALLS] = {};
float gResultAnimationTime = 0.0f;

AppStatus gAppStatus = RUNNING;

Vector2 gLeftGoaliePosition = { 40.0f, 225.0f };
Vector2 gRightGoaliePosition = { 760.0f, 225.0f };
Vector2 gBallPositions[MAX_BALLS];
Vector2 gLeftGoalieMovement = { 0.0f, 0.0f };
Vector2 gRightGoalieMovement = { 0.0f, 0.0f };
Vector2 gBallMovements[MAX_BALLS];

Vector2 gLeftGoalieScale = { 71.0f, 140.0f };
Vector2 gRightGoalieScale = { 71.0f, 140.0f };
Vector2 gBallScale = { 25.0f, 25.0f };

Texture2D gLeftGoalieTexture;
Texture2D gRightGoalieTexture;
Texture2D gBallTexture; 
Texture2D gFieldTexture;
Texture2D gNumberTextures[10];
Texture2D gWinTextures[3];
Texture2D gLoseTextures[3];
Texture2D gStartScreenTexture;

Music gBGM;
Sound gCheer;
Sound gGoal;

void initialise();
void processInput();
void update();
void render();
void shutdown();
void resetGame();

void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 2: Pong Variations");

    InitAudioDevice();

    gLeftGoalieTexture = LoadTexture(LEFTGOALIE_FP);
    gRightGoalieTexture = LoadTexture(RIGHTGOALIE_FP);
    gBallTexture = LoadTexture(BALL_FP);
    gFieldTexture = LoadTexture(FIELD_FP);
    gNumberTextures[0] = LoadTexture(ZERO_FP);
    gNumberTextures[1] = LoadTexture(ONE_FP);
    gNumberTextures[2] = LoadTexture(TWO_FP);
    gNumberTextures[3] = LoadTexture(THREE_FP);
    gNumberTextures[4] = LoadTexture(FOUR_FP);
    gNumberTextures[5] = LoadTexture(FIVE_FP);
    gNumberTextures[6] = LoadTexture(SIX_FP);
    gNumberTextures[7] = LoadTexture(SEVEN_FP);
    gNumberTextures[8] = LoadTexture(EIGHT_FP);
    gNumberTextures[9] = LoadTexture(NINE_FP);
    gWinTextures[0] = LoadTexture(WIN_FP);
    gWinTextures[1] = LoadTexture(WIN2_FP);
    gWinTextures[2] = LoadTexture(WIN3_FP);
    gLoseTextures[0] = LoadTexture(LOSE_FP);
    gLoseTextures[1] = LoadTexture(LOSE2_FP);
    gLoseTextures[2] = LoadTexture(LOSE3_FP);
    gStartScreenTexture = LoadTexture(START_FP);
    gBGM = LoadMusicStream(BGM_FP);
    SetMusicVolume(gBGM, 0.25f);
    PlayMusicStream(gBGM);

    gCheer = LoadSound(CHEER_FP);
    SetSoundVolume(gCheer, 0.7f);

    gGoal = LoadSound(GOAL_FP);
    SetSoundVolume(gGoal, 0.7f);

    for (int i = 0; i < MAX_BALLS; i++) {
        gBallPositions[i] = ORIGIN;
        
        if (i % 4 == 0) {
            gBallMovements[i] = { -1.0f, 1.0f };
        } else if (i % 4 == 1) {
            gBallMovements[i] = { 1.0f, -0.5f };
        } else if (i % 4 == 2) {
            gBallMovements[i] = { -1.0f, -0.75f };
        } else {
            gBallMovements[i] = { 1.0f, 0.5f};
        }

        gBallRotations[i] = 0.0f;
        gBallRotationDirections[i] = 1.0f;
    }

    SetTargetFPS(FPS);
}

void resetGame() {
    gLeftScore = 0;
    gRightScore = 0;
    gActiveBalls = 1;
    gAIDirection = 1.0f;
    gResultAnimationTime = 0.0f;

    gLeftGoaliePosition = { 40.0f, 225.0f };
    gRightGoaliePosition = { 760.0f, 225.0f };

    for (int i = 0; i < MAX_BALLS; i++) {
        gBallPositions[i] = ORIGIN;

        if (i % 4 == 0) {
            gBallMovements[i] = { -1.0f, 1.0f };
        } else if (i % 4 == 1) {
            gBallMovements[i] = { 1.0f, -0.5f };
        } else if (i % 4 == 2) {
            gBallMovements[i] = { -1.0f, -0.75f };
        } else {
            gBallMovements[i] = { 1.0f, 0.5f };
        }

        gBallRotations[i] = 0.0f;
        gBallRotationDirections[i] = 1.0f;
    }
}

void processInput() {
    gLeftGoalieMovement = { 0.0f, 0.0f };
    gRightGoalieMovement = { 0.0f, 0.0f };

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) {
        gAppStatus = TERMINATED;
        return;
    }

    if (!gGameStarted) {
        if (IsKeyPressed(KEY_B)) {
            gGameStarted = true;
        }
        return;
    }

    if (IsKeyPressed(KEY_T)) {
        gAIMode = !gAIMode;
    }

    int oldBallCount = gActiveBalls;

    if (IsKeyPressed(KEY_ONE)) {
        gActiveBalls = 1;
    } else if (IsKeyPressed(KEY_TWO)) {
        gActiveBalls = 2;
    } else if (IsKeyPressed(KEY_THREE)) {
        gActiveBalls = 3;
    } else if (IsKeyPressed(KEY_FOUR)) {
        gActiveBalls = 4;
    } else if (IsKeyPressed(KEY_FIVE)) {
        gActiveBalls = 5;
    } else if (IsKeyPressed(KEY_SIX)) {
        gActiveBalls = 6;
    } else if (IsKeyPressed(KEY_SEVEN)) {
        gActiveBalls = 7;
    } else if (IsKeyPressed(KEY_EIGHT)) {
        gActiveBalls = 8;
    } else if (IsKeyPressed(KEY_NINE)) {
        gActiveBalls = 9;
    }

    if (gActiveBalls > oldBallCount) {
        for (int i = oldBallCount; i < gActiveBalls; i++) {
            gBallPositions[i] = {
                ORIGIN.x + (i % 3 - 1) * 40.0f,
                ORIGIN.y + (i % 3 - 1) * 40.0f
            };

            if (i % 4 == 0) {
                gBallMovements[i] = { -1.0f, 1.0f };
            } else if (i % 4 == 1) {
                gBallMovements[i] = { -1.0f, -0.75f };
            } else {
                gBallMovements[i] = { 1.0f, 0.5f };
            }

            gBallRotations[i] = 0.0f;
            gBallRotationDirections[i] = 1.0f;
        }
    }

    if (IsKeyDown(KEY_W)) {
        gLeftGoalieMovement.y = -1.0f;
    } else if (IsKeyDown(KEY_S)) {
        gLeftGoalieMovement.y = 1.0f;
    }

    if (!gAIMode) {
        if (IsKeyDown(KEY_UP)) {
            gRightGoalieMovement.y = -1.0f;
        } else if (IsKeyDown(KEY_DOWN)) {
            gRightGoalieMovement.y = 1.0f;
        }
    }

    if (IsKeyPressed(KEY_R)) {
        resetGame();
    }

    if (IsKeyPressed(KEY_H)) {
        gGameStarted = false;
    }
}

void update() {
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    if (!gGameStarted) {
        UpdateMusicStream(gBGM);
        return;
    }

    if (gLeftScore >= WINNING_SCORE || gRightScore >= WINNING_SCORE) {
        gResultAnimationTime += deltaTime;
        UpdateMusicStream(gBGM);
        return;
    }

    for (int i = 0; i < gActiveBalls; i++) {
        gBallRotations[i] += 90.0f * gBallRotationDirections[i] * deltaTime;
    }
    
    gLeftGoaliePosition.y += GOALIE_SPEED * gLeftGoalieMovement.y * deltaTime;
    if (gAIMode) {
        gRightGoaliePosition.y += GOALIE_SPEED * gAIDirection * deltaTime;

        if (gRightGoaliePosition.y <= gRightGoalieScale.y / 2.0f) {
            gRightGoaliePosition.y = gRightGoalieScale.y / 2.0f;
            gAIDirection = 1.0f;
        } else if (gRightGoaliePosition.y >= SCREEN_HEIGHT - gRightGoalieScale.y / 2.0f) {
            gRightGoaliePosition.y = SCREEN_HEIGHT - gRightGoalieScale.y / 2.0f;
            gAIDirection = -1.0f;
        }
    } else {
        gRightGoaliePosition.y += GOALIE_SPEED * gRightGoalieMovement.y * deltaTime;
    }

    if (gLeftGoaliePosition.y - gLeftGoalieScale.y / 2.0f < 0.0f) {
        gLeftGoaliePosition.y = gLeftGoalieScale.y / 2.0f;
    }

    if (gLeftGoaliePosition.y + gLeftGoalieScale.y / 2.0f > SCREEN_HEIGHT) {
        gLeftGoaliePosition.y = SCREEN_HEIGHT - gLeftGoalieScale.y / 2.0f;
    }

    if (gRightGoaliePosition.y - gRightGoalieScale.y / 2.0f < 0.0f) {
        gRightGoaliePosition.y = gRightGoalieScale.y / 2.0f;
    }

    if (gRightGoaliePosition.y + gRightGoalieScale.y / 2.0f > SCREEN_HEIGHT) {
        gRightGoaliePosition.y = SCREEN_HEIGHT - gRightGoalieScale.y / 2.0f;
    }

    for (int i = 0; i < gActiveBalls; i++) {
        gBallPositions[i].x += BALL_SPEED * gBallMovements[i].x * deltaTime;
        gBallPositions[i].y += BALL_SPEED * gBallMovements[i].y * deltaTime;
    }

    for (int i = 0; i < gActiveBalls; i++) {
        if (gBallPositions[i].y - gBallScale.y / 2.0f <= 0.0f) {
            gBallMovements[i].y *= -1.0f;
            gBallRotationDirections[i] *= -1.0f;
            gBallPositions[i].y = gBallScale.y / 2.0f;
        }

        if (gBallPositions[i].y + gBallScale.y / 2.0f >= SCREEN_HEIGHT) {
            gBallMovements[i].y *= -1.0f;
            gBallRotationDirections[i] *= -1.0f;
            gBallPositions[i].y = SCREEN_HEIGHT - gBallScale.y / 2.0f;
        }
    }

    for (int i = 0; i < gActiveBalls; i++) {
        if (
            gBallMovements[i].x < 0.0f &&
            gBallPositions[i].x > gLeftGoaliePosition.x &&
            fabs(gBallPositions[i].x - gLeftGoaliePosition.x) < 
                (gBallScale.x + gLeftGoalieScale.x) / 2.0f &&
            fabs(gBallPositions[i].y - gLeftGoaliePosition.y) < 
                (gBallScale.y + gLeftGoalieScale.y) / 2.0f
        ) {
            gBallMovements[i].x = 1.0f;
            gBallRotationDirections[i] *= -1.0f;
            gBallPositions[i].x = gLeftGoaliePosition.x 
                            + gLeftGoalieScale.x / 2.0f
                            + gBallScale.x / 2.0f;

            PlaySound(gCheer);
        }

        if (
            gBallMovements[i].x > 0.0f &&
            gBallPositions[i].x + gBallScale.x / 2.0f >=
                gRightGoaliePosition.x - gRightGoalieScale.x / 2.0f &&
            gBallPositions[i].x <= gRightGoaliePosition.x &&
            fabs(gBallPositions[i].y - gRightGoaliePosition.y) <
                (gBallScale.y + gRightGoalieScale.y) / 2.0f
        ) {
            gBallMovements[i].x = -1.0f;
            gBallRotationDirections[i] *= -1.0f;
            gBallPositions[i].x = gRightGoaliePosition.x
                            - gRightGoalieScale.x / 2.0f
                            - gBallScale.x / 2.0f;

            PlaySound(gCheer);
        }
    }

    for (int i = 0; i < gActiveBalls; i++) {
        if (gBallPositions[i].x + gBallScale.x / 2.0f < 0.0f) {
            if (gRightScore < WINNING_SCORE) {
                gRightScore++;
            }
            gBallPositions[i] = ORIGIN;
            gBallMovements[i] = { 
                1.0f, 
                0.5f + (i % 4) * 0.15f 
            };

            PlaySound(gGoal);
        }

        if (gBallPositions[i].x - gBallScale.x / 2.0f > SCREEN_WIDTH) {
            if (gLeftScore < WINNING_SCORE) {
                gLeftScore++;
            }
            gBallPositions[i] = ORIGIN;
            gBallMovements[i] = { 
                -1.0f, 
                0.5f + (i % 4) * 0.15f
            };

            PlaySound(gGoal);
        }
    }

    if (gLeftScore >= WINNING_SCORE || gRightScore >= WINNING_SCORE) {
        for (int i = 0; i < gActiveBalls; i++) {
            gBallMovements[i] = { 0.0f, 0.0f };
        }
    }

    UpdateMusicStream(gBGM);
}

void render() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (!gGameStarted) {
        Rectangle startTextureArea = {
            0.0f,
            0.0f,
            static_cast<float>(gStartScreenTexture.width),
            static_cast<float>(gStartScreenTexture.height)
        };

        Rectangle startTextureDestinationArea = {
            0.0f,
            0.0f,
            static_cast<float>(SCREEN_WIDTH),
            static_cast<float>(SCREEN_HEIGHT)
        };

        DrawTexturePro(
            gStartScreenTexture,
            startTextureArea,
            startTextureDestinationArea,
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );

        EndDrawing();
        return;
    }

    Rectangle fieldTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(gFieldTexture.width),
        static_cast<float>(gFieldTexture.height)
    };

    Rectangle fieldDestinationArea = {
        0.0f,
        0.0f,
        static_cast<float>(SCREEN_WIDTH),
        static_cast<float>(SCREEN_HEIGHT)
    };

    Vector2 fieldOrigin = {
        0.0f,
        0.0f
    };

    DrawTexturePro(
        gFieldTexture,
        fieldTextureArea,
        fieldDestinationArea,
        fieldOrigin,
        0.0f,
        WHITE
    );

    Texture2D gLeftScoreTexture = gNumberTextures[gLeftScore % 10];

    Rectangle leftScoreTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(gLeftScoreTexture.width),
        static_cast<float>(gLeftScoreTexture.height)
    };

    Rectangle leftScoreDestinationArea = {
        330.0f,
        40.0f,
        45.0f,
        55.0f
    };

    Vector2 leftScoreOrigin = {
        0.0f,
        0.0f
    };

    DrawTexturePro(
        gLeftScoreTexture,
        leftScoreTextureArea,
        leftScoreDestinationArea,
        leftScoreOrigin,
        0.0f,
        WHITE
    );

    if (gLeftScore >= 10) {
        Rectangle leftTensTextureArea = {
            0.0f,
            0.0f,
            static_cast<float>(gNumberTextures[1].width),
            static_cast<float>(gNumberTextures[1].height)
        };

        Rectangle leftTensDestinationArea = {
            285.0f,
            40.0f,
            45.0f,
            55.0f
        };

        DrawTexturePro(
            gNumberTextures[1],
            leftTensTextureArea,
            leftTensDestinationArea,
            leftScoreOrigin,
            0.0f,
            WHITE
        );
    }

    Texture2D gRightScoreTexture = gNumberTextures[gRightScore % 10];

    Rectangle rightScoreTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(gRightScoreTexture.width),
        static_cast<float>(gRightScoreTexture.height)
    };

    Rectangle rightScoreDestinationArea = {
        425.0f,
        40.0f,
        45.0f,
        55.0f
    };

    if (gRightScore >= 10) {
        rightScoreDestinationArea.x = 470.0f;
    }

    Vector2 rightScoreOrigin = {
        0.0f,
        0.0f
    };

    DrawTexturePro(
        gRightScoreTexture,
        rightScoreTextureArea,
        rightScoreDestinationArea,
        rightScoreOrigin,
        0.0f,
        WHITE
    );

    if (gRightScore >= 10) {
        Rectangle rightTensTextureArea = {
            0.0f,
            0.0f,
            static_cast<float>(gNumberTextures[1].width),
            static_cast<float>(gNumberTextures[1].height)
        };

        Rectangle rightTensDestinationArea = {
            425.0f,
            40.0f,
            45.0f,
            55.0f
        };

        DrawTexturePro(
            gNumberTextures[1],
            rightTensTextureArea,
            rightTensDestinationArea,
            rightScoreOrigin,
            0.0f,
            WHITE
        );
    }

    Rectangle ballTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(gBallTexture.width),
        static_cast<float>(gBallTexture.height)
    };

    for (int i = 0; i < gActiveBalls; i++) {
        Rectangle ballDestinationArea = {
            gBallPositions[i].x,
            gBallPositions[i].y,
            gBallScale.x,
            gBallScale.y
        };

    Vector2 ballOrigin = {
        12.5f,
        12.5f
    };

    DrawTexturePro(
        gBallTexture,
        ballTextureArea,
        ballDestinationArea,
        ballOrigin,
        gBallRotations[i],
        WHITE
    );
    }

    Rectangle leftGoalieTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(gLeftGoalieTexture.width),
        static_cast<float>(gLeftGoalieTexture.height)
    };

    Rectangle leftGoalieDestinationArea = {
        gLeftGoaliePosition.x,
        gLeftGoaliePosition.y,
        gLeftGoalieScale.x,
        gLeftGoalieScale.y  
    };

    Vector2 leftGoalieOrigin = {
        gLeftGoalieScale.x / 2.0f,
        gLeftGoalieScale.y / 2.0f
    };

    DrawTexturePro(
        gLeftGoalieTexture,
        leftGoalieTextureArea,
        leftGoalieDestinationArea,
        leftGoalieOrigin,
        0.0f,
        WHITE
    );

    Rectangle rightGoalieTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(gRightGoalieTexture.width),
        static_cast<float>(gRightGoalieTexture.height)
    };

    Rectangle rightGoalieDestinationArea = {
        gRightGoaliePosition.x,
        gRightGoaliePosition.y,
        gRightGoalieScale.x,
        gRightGoalieScale.y
    };

    Vector2 rightGoalieOrigin = {
        gRightGoalieScale.x / 2.0f,
        gRightGoalieScale.y / 2.0f
    };

    DrawTexturePro(
        gRightGoalieTexture,
        rightGoalieTextureArea,
        rightGoalieDestinationArea,
        rightGoalieOrigin,
        0.0f,
        WHITE
    );

    if (gLeftScore >= WINNING_SCORE || gRightScore >= WINNING_SCORE) {
        DrawRectangle(
            0,
            0,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            Fade(BLACK, 0.6f)
        );

        int pictureIndex = static_cast<int>(gResultAnimationTime / 1.0f) % 3;

        Texture2D leftResultTexture;
        Texture2D rightResultTexture;

        if (gLeftScore >= WINNING_SCORE) {
            leftResultTexture = gWinTextures[pictureIndex];
            rightResultTexture = gLoseTextures[pictureIndex];
        } else {
            leftResultTexture = gLoseTextures[pictureIndex];
            rightResultTexture = gWinTextures[pictureIndex];
        }

    Rectangle leftResultTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(leftResultTexture.width),
        static_cast<float>(leftResultTexture.height)
    };

    Rectangle leftResultDestinationArea = {
        100.0f,
        90.0f,
        200.0f,
        200.0f * leftResultTexture.height / leftResultTexture.width
    };

    Vector2 leftResultOrigin = {
        0.0f,
        0.0f
    };

    DrawTexturePro(
        leftResultTexture,
        leftResultTextureArea,
        leftResultDestinationArea,
        leftResultOrigin,
        0.0f,
        WHITE
    );

    Rectangle rightResultTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(rightResultTexture.width),
        static_cast<float>(rightResultTexture.height)
    };

    Rectangle rightResultDestinationArea = {
        500.0f,
        90.0f,
        200.0f,
        200.0f * rightResultTexture.height / rightResultTexture.width
    };

    Vector2 rightResultOrigin = {
        0.0f,
        0.0f
    };

    DrawTexturePro(
        rightResultTexture,
        rightResultTextureArea,
        rightResultDestinationArea,
        rightResultOrigin,
        0.0f,
        WHITE
    );
    }

    EndDrawing();
}

void shutdown() {
    UnloadMusicStream(gBGM);
    UnloadSound(gCheer);
    UnloadSound(gGoal);
    CloseAudioDevice();

    UnloadTexture(gLeftGoalieTexture);
    UnloadTexture(gRightGoalieTexture);
    UnloadTexture(gBallTexture);
    UnloadTexture(gFieldTexture);
    for (int i = 0; i < 10; i++) {
        UnloadTexture(gNumberTextures[i]);
    }
    for (int i = 0; i < 3; i++) { 
        UnloadTexture(gWinTextures[i]);
        UnloadTexture(gLoseTextures[i]);
    }
    UnloadTexture(gStartScreenTexture);

    CloseWindow();
}

int main(void) {
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
