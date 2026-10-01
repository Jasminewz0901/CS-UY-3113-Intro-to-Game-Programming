#include "CS3113/cs3113.h"
#include <math.h>

/** 
* Author: Jasmine Zhang
* Assignment: 2D Simple Scene
* Date Due: 10/05/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on 
* Academic Misconduct
**/

constexpr int SCREEN_WIDTH  = 1600 / 2,
              SCREEN_HEIGHT = 900 / 2,
              FPS           = 60;

constexpr char DAWN_COLOR[] = "#ffb38aff";
constexpr char MORNING_COLOR[] = "#1eddfffe";
constexpr char EVENING_COLOR[] = "#ffcc00ff";

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr char LUFFY_FP[] = "assets/luffy3.png"; 
constexpr char BALL_FP[] = "assets/ball.png"; 
constexpr char SUN_FP[] = "assets/sun.png"; 
constexpr char GRASS_FP[] = "assets/grass.png"; 
constexpr char STAR_FP[] = "assets/stars.png";

AppStatus gAppStatus = RUNNING;

float gLuffyTime     = 0.0f,
      gPreviousTicks = 0.0f,
      gSunOrbit      = 0.0f,
      gBallTime      = 0.0f,
      gLuffyRotation = 0.0f;

Color gBackgroundColor = ColorFromHex(DAWN_COLOR);

Vector2 gLuffyPosition = { 200.0f, 280.0f };
Vector2 gBallPosition = { 350.0f, 280.0f };
Vector2 gSunPosition = { 100.0f, 100.0f };

Vector2 gLuffyScale = { 150.0f, 150.0f };
Vector2 gBallScale = { 50.0f, 50.0f };
Vector2 gSunScale = { 100.0f, 100.0f };

Texture2D gLuffyTexture;
Texture2D gBallTexture;
Texture2D gSunTexture;
Texture2D gGrassTexture;
Texture2D gStarTexture;

void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 1: Simple 2D Scene");

    gLuffyTexture = LoadTexture(LUFFY_FP);
    gBallTexture = LoadTexture(BALL_FP);
    gSunTexture = LoadTexture(SUN_FP);
    gGrassTexture = LoadTexture(GRASS_FP);
    gStarTexture = LoadTexture(STAR_FP);

    SetTargetFPS(FPS);
}

void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    gSunOrbit += 0.5f * deltaTime;
    gSunPosition.x = ORIGIN.x + 300.0f * cosf(gSunOrbit);
    gSunPosition.y = ORIGIN.y + 175.0f * sinf(gSunOrbit);

    gBallTime += 1.0f * deltaTime;
    gBallPosition.x += 100.0f * deltaTime;
    gBallPosition.y = 250.0f + 30.0f * sinf(gBallTime * 4.0f);

    gBallScale.x = 50.0f + 10.0f * sinf(gBallTime * 4.0f);
    gBallScale.y = 50.0f + 10.0f * sinf(gBallTime * 4.0f);

    if (gBallPosition.x > SCREEN_WIDTH) {
        gBallPosition.x = 0.0f;
    }

    if (gLuffyPosition.x < gBallPosition.x) {
        gLuffyPosition.x += 80.0f * deltaTime;
    } else if (gLuffyPosition.x > gBallPosition.x) {
        gLuffyPosition.x -= 80.0f * deltaTime;
    }

    gLuffyTime += 1.0f * deltaTime;
    gLuffyPosition.y = 280.0f + 10.0f * sinf(gLuffyTime * 6.0f);
    gLuffyRotation = 10.0f * sinf(gLuffyTime * 4.0f);

    if (gSunPosition.x < 200.0f) {
        gBackgroundColor = ColorFromHex(DAWN_COLOR);
    } else if (gSunPosition.x < 550.0f) {
        gBackgroundColor = ColorFromHex(MORNING_COLOR);
    } else {
        gBackgroundColor = ColorFromHex(EVENING_COLOR);
    }
}

void render() {
    BeginDrawing();

    ClearBackground(gBackgroundColor);

    if (gSunPosition.y >= 340.0f) {
        Rectangle starTextureArea = {
            0.0f,
            0.0f,
            static_cast<float>(gStarTexture.width),
            static_cast<float>(gStarTexture.height)
        };
        
        Rectangle starDestinationArea = {
            0.0f,
            0.0f,
            static_cast<float>(SCREEN_WIDTH),
            static_cast<float>(SCREEN_HEIGHT)
        };

        Vector2 starOrigin = {
            0.0f,
            0.0f
        };

        DrawTexturePro(
            gStarTexture,
            starTextureArea,
            starDestinationArea,
            starOrigin,
            0.0f,
            WHITE
        );
    }

    Rectangle sunTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(gSunTexture.width),
        static_cast<float>(gSunTexture.height)
    };

    Rectangle sunDestinationArea = {
        gSunPosition.x,
        gSunPosition.y,
        gSunScale.x,
        gSunScale.y
    };

    Vector2 sunOrigin = {
        gSunScale.x / 2.0f,
        gSunScale.y / 2.0f
    };

    DrawTexturePro(
        gSunTexture,
        sunTextureArea,
        sunDestinationArea,
        sunOrigin,
        0.0f,
        WHITE
    );

    Rectangle grassTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(gGrassTexture.width),
        static_cast<float>(gGrassTexture.height)
    };

    Rectangle grassDestinationArea = {
        0.0f,
        300.0f,
        static_cast<float>(SCREEN_WIDTH),
        150.0f
    };

    Vector2 grassOrigin = {
        0.0f,
        0.0f
    };

    DrawTexturePro(
        gGrassTexture,
        grassTextureArea,
        grassDestinationArea,
        grassOrigin,
        0.0f,
        WHITE
    );

    Rectangle ballTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(gBallTexture.width),
        static_cast<float>(gBallTexture.height)
    };

    Rectangle ballDestinationArea = {
        gBallPosition.x,
        gBallPosition.y,
        gBallScale.x,
        gBallScale.y
    };

    Vector2 ballOrigin = {
        gBallScale.x / 2.0f,
        gBallScale.y / 2.0f
    };

    DrawTexturePro(
        gBallTexture,
        ballTextureArea,
        ballDestinationArea,
        ballOrigin,
        0.0f,
        WHITE
    );

    Rectangle luffyTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(gLuffyTexture.width),
        static_cast<float>(gLuffyTexture.height)
    };

    Rectangle luffyDestinationArea = {
        gLuffyPosition.x,
        gLuffyPosition.y,
        gLuffyScale.x,
        gLuffyScale.y
    };

    Vector2 luffyOrigin = {
        gLuffyScale.x / 2.0f,
        gLuffyScale.y / 2.0f
    };

    DrawTexturePro(
        gLuffyTexture,
        luffyTextureArea,
        luffyDestinationArea,
        luffyOrigin,
        gLuffyRotation,
        WHITE
    );

    EndDrawing();
}

void shutdown() {
    CloseWindow();
}

int main(void) {
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }

    shutdown();
    return 0;
}
