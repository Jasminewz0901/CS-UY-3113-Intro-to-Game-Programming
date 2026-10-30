#include "CS3113/Entity.h"

/**
* Author: Jasmine Zhang
* Assignment: Lunar Lander!
* Date due: 10/30/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

constexpr int SCREEN_WIDTH  = 1600 / 2,
              SCREEN_HEIGHT = 900 / 2,
              FPS           = 60;

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
constexpr float ACCELERATION_OF_GRAVITY = 5.00f;

// Includes a lot of AI-Generated Sprite-Sheet Assets
constexpr char BACKGROUND_FP[] = "assets/Background 2.png";
constexpr char HOMEPAGE_FP[] = "assets/Homepage.png";
constexpr char PUMPKIN_FP[] = "assets/MovingPumpkin.png";
constexpr char HOUSE_FP[] = "assets/Haunted House.png";
constexpr char MACCOMPLISHED_FP[] = "assets/Mission Accomplished.png";
constexpr char MFAILED_FP[] = "assets/Mission Failed.png";
constexpr char WITCH_FP[] = "assets/Witch Obstacle.png";
constexpr char SACK_FP[] = "assets/Sack.png";

constexpr char BGM_FP[] = "assets/sounds/Graveyard Shift.mp3";
/*
"Graveyard Shift" Kevin MacLeod (incompetech.com)
Licensed under Creative Commons: By Attribution 4.0 License
http://creativecommons.org/licenses/by/4.0/
*/

constexpr char PUMP_FP[] = "assets/sounds/pumpkin.mp3";

AppStatus gAppStatus = RUNNING;

enum GameState { HOMEPAGE, PLAYING, WON, LOST };

GameState gGameState = HOMEPAGE;

float gEndTime = 0.0f;
float gPreviousTicks = 0.0f;
float gSackAnimationTime = 0.0f;
bool gShowEndScreen = false;
bool gWitchDisappearing = false;
bool gSackClosing = false;
bool gPumpkinEnteringSack = false;
bool gSackMovingRight = true;

Entity *gPumpkin = nullptr;
Entity *gHouse = nullptr;
Entity *gPorch = nullptr;
Entity *gWitch = nullptr;
Entity *gSack = nullptr;

Texture2D gBackgroundTexture;
Texture2D gHomepageTexture;
Texture2D gWinTexture;
Texture2D gLoseTexture;

Music gBGM;
Sound gPump;

void initialise();
void processInput();
void update();
void render();
void shutdown();
void resetGame();

void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 3: Lunar Lander");
    InitAudioDevice();

    std::map<Direction, std::vector<int>> gPumpkinAtlas = {
        {DOWN, {0, 1, 2, 3}},
        {UP, {4, 5, 6, 7, 8, 9, 10, 11}},
        {RIGHT, {12, 13, 14, 15, 16, 17}}
    };

    gPumpkin = new Entity(
        {400.0f, 80.0f},
        {65.0f, 65.0f},
        PUMPKIN_FP,
        ATLAS,
        {6.0f, 3.0f},
        gPumpkinAtlas,
        PLAYER
    );

    gPumpkin->setDirection(DOWN);
    gPumpkin->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    gPumpkin->setColliderDimensions({45.0f, 45.0f});
    gPumpkin->setColliderOffset({0.0f, 5.0f});

    gHouse = new Entity(
        {680.0f, 320.0f},
        {240.0f, 260.0f},
        HOUSE_FP,
        BLOCK
    );

    gPorch = new Entity(
        {635.0f, 405.0f},
        {120.0f, 15.0f},
        HOUSE_FP,
        PLATFORM
    );

    std::map<Direction, std::vector<int>> gSackAtlas = {
        {RIGHT, {0, 1, 2, 3, 4, 5, 6, 7}},
        {DOWN, {8, 9, 10, 11, 12, 13, 14, 15}}
    };

    gSack = new Entity(
        {300.0f, 405.0f},
        {110.0f, 90.0f},
        SACK_FP,
        ATLAS,
        {8.0f, 2.0f},
        gSackAtlas,
        PLATFORM
    );

    gSack->setDirection(RIGHT);
    gSack->setVelocity({115.0f, 0.0f});
    gSack->setFrameSpeed(10);
    gSack->setColliderDimensions({70.0f, 30.0f});
    gSack->setColliderOffset({0.0f, -12.0f});

    std::map<Direction, std::vector<int>> gWitchAtlas = {
        {RIGHT, {0, 1, 2, 3}},
        {LEFT, {4, 5, 6, 7}},
        {DOWN, {8, 9, 10, 11}}
    };

    gWitch = new Entity(
        {200.0f, 220.0f},
        {110.0f, 75.0f},
        WITCH_FP,
        ATLAS,
        {4.0f, 3.0f},
        gWitchAtlas,
        BLOCK
    );

    gWitch->setDirection(RIGHT);
    gWitch->setFrameSpeed(8);
    gWitch->setVelocity({150.0f, 0.0f});
    gWitch->setColliderDimensions({105.0f, 60.0f});
    gWitch->setColliderOffset({0.0f, 5.0f});

    gBackgroundTexture = LoadTexture(BACKGROUND_FP);
    gHomepageTexture = LoadTexture(HOMEPAGE_FP);
    gWinTexture = LoadTexture(MACCOMPLISHED_FP);
    gLoseTexture = LoadTexture(MFAILED_FP);

    gBGM = LoadMusicStream(BGM_FP);
    SetMusicVolume(gBGM, 0.25f);
    PlayMusicStream(gBGM);

    gPump = LoadSound(PUMP_FP);
    SetSoundVolume(gPump, 0.7f);

    SetTargetFPS(FPS);
}

void resetGame() {
    gGameState = PLAYING;
    gEndTime = 0.0f;
    gShowEndScreen = false;

    gPumpkin->setDirection(DOWN);
    gPumpkin->resetAnimation();
    gPumpkin->setFrameSpeed(14);
    gPumpkin->setPosition({400.0f, 80.0f});
    gPumpkin->setMovement({0.0f, 0.0f});
    gPumpkin->setVelocity({0.0f, 0.0f});
    gPumpkin->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    gPumpkin->setFuel(100.0f);
    gPumpkin->resetTimeAccumulator();
    gPumpkin->resetCollision();
    gPumpkin->setScale({65.0f, 65.0f});

    gWitch->setPosition({200.0f, 220.0f});
    gWitch->setMovement({1.0f, 0.0f});
    gWitch->setVelocity({150.0f, 0.0f});
    gWitch->setAcceleration({0.0f, 0.0f});
    gWitch->setDirection(RIGHT);
    gWitch->resetTimeAccumulator();
    gWitchDisappearing = false;
    gWitch->setFrameSpeed(8);
    gWitch->resetAnimation();

    gSackClosing = false;
    gSack->setPosition({300.0f, 405.0f});
    gSack->setVelocity({115.0f, 0.0f});
    gSack->setDirection(RIGHT);
    gSack->resetAnimation();
    gSack->resetTimeAccumulator();

    gPumpkinEnteringSack = false;
    gSackClosing = false;
    gSackAnimationTime = 0.0f;
    gSackMovingRight = true;

    gPreviousTicks = static_cast<float>(GetTime());
}

void processInput() {
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) {
        gAppStatus = TERMINATED;
        return;
    }

    if (IsKeyPressed(KEY_H)) {
        gGameState = HOMEPAGE;
        return;
    }

    if (gGameState == HOMEPAGE) {
        if (IsKeyPressed(KEY_B)) {
            gGameState = PLAYING;
            gPreviousTicks = static_cast<float>(GetTime());
        }
        return;
    }

    if (IsKeyPressed(KEY_R)) {
        resetGame();
        return;
    }

    if (gGameState != PLAYING) {
        return;
    }

    if (gPumpkin->getFuel() > 0.0f) {
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_D)) {
            PlaySound(gPump);
        }
    }

    Vector2 acceleration = {0.0f, ACCELERATION_OF_GRAVITY};

    if (gPumpkin->getFuel() > 0.0f) {
        if (IsKeyDown(KEY_A)) {
            acceleration.x = -200.0f;
            gPumpkin->subFuel(0.2f);
        } else if (IsKeyDown(KEY_D)) {
            acceleration.x = 200.0f;
            gPumpkin->subFuel(0.2f);
        }
    }

    gPumpkin->setAcceleration(acceleration);
}

void update() {
    float ticks = static_cast<float> (GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    if (gGameState == WON && gPumpkinEnteringSack && !gShowEndScreen) {
        gSackAnimationTime += deltaTime;

        Vector2 position = gPumpkin->getPosition();
        Vector2 sackPosition = gSack->getPosition();

        position.x += (sackPosition.x - position.x) * 3.0f * deltaTime;
        position.y += 60.0f * deltaTime;
        gPumpkin->setPosition(position);

        if (gSackAnimationTime >= 0.4f) {
            Vector2 scale = gPumpkin->getScale();

            scale.x -= 80.0f * deltaTime;
            scale.y -= 80.0f * deltaTime;

            if (scale.x < 0.0f) scale.x = 0.0f;
            if (scale.y < 0.0f) scale.y = 0.0f;

            gPumpkin->setScale(scale);
        }

        if (gSackAnimationTime >= 0.7f && !gSackClosing) {
            gSackClosing = true;
            gSack->setDirection(DOWN);
            gSack->resetAnimation();
            gSack->setFrameSpeed(10);
        }

        if (gSackClosing) {
            gSack->update(
                deltaTime,
                nullptr,
                0,
                nullptr,
                0,
                FIXED_TIMESTEP
            );
        }
    }

    if (gGameState == PLAYING) {
        if (gSackMovingRight) {
            gSack->setVelocity({115.0f, 0.0f});
        } else {
            gSack->setVelocity({-115.0f, 0.0f});
        }

        gSack->update(
            deltaTime,
            nullptr, 0,
            nullptr, 0,
            FIXED_TIMESTEP
        );

        if (gSack->getPosition().x >= 500.0f) {
            gSackMovingRight = false;
        } else if (gSack->getPosition().x <= 200.0f) {
            gSackMovingRight = true;
        }

        gPumpkin->update(
            deltaTime,
            gPorch,
            1, 
            gSack,
            1,
            FIXED_TIMESTEP
        );

        Vector2 position = gPumpkin->getPosition();
        Vector2 velocity = gPumpkin->getVelocity();

        float halfWidth = gPumpkin->getColliderDimensions().x / 2.0f;

        if (position.x < halfWidth) {
            position.x = halfWidth;
            velocity.x = 0.0f;
        } else if (position.x > SCREEN_WIDTH - halfWidth) {
            position.x = SCREEN_WIDTH - halfWidth;
            velocity.x = 0.0f;
        }

        gPumpkin->setPosition(position);
        gPumpkin->setVelocity(velocity);
        
        if (!gWitchDisappearing) {
            if (gWitch->getDirection() == RIGHT) {
                gWitch->setVelocity({150.0f, 0.0f});
            } else if (gWitch->getDirection() == LEFT) {
                gWitch->setVelocity({-150.0f, 0.0f});
            }
        }

        gWitch->update(
            deltaTime,
            nullptr,
            0,
            nullptr,
            0,
            FIXED_TIMESTEP
        );

        if (!gWitchDisappearing) {
            if (gWitch->getPosition().x >= 750.0f &&
                gWitch->getDirection() != LEFT) {
                    gWitch->setVelocity({-150.0f, 0.0f});
                    gWitch->setDirection(LEFT);
            } else if (gWitch->getPosition().x <= 50.0f &&
                    gWitch->getDirection() != RIGHT) {
                gWitch->setVelocity({150.0f, 0.0f});
                gWitch->setDirection(RIGHT);
            }
        }

        if (gPumpkin->getPosition().x + gPumpkin->getColliderDimensions().x / 2.0f >
            gWitch->getPosition().x - gWitch->getColliderDimensions().x / 2.0f &&
            
            gPumpkin->getPosition().x - gPumpkin->getColliderDimensions().x / 2.0f <
            gWitch->getPosition().x + gWitch->getColliderDimensions().x / 2.0f &&
        
            gPumpkin->getPosition().y + gPumpkin->getColliderDimensions().y / 2.0f >
            gWitch->getPosition().y - gWitch->getColliderDimensions().y / 2.0f &&
        
            gPumpkin->getPosition().y - gPumpkin->getColliderDimensions().y / 2.0f <
            gWitch->getPosition().y + gWitch->getColliderDimensions().y / 2.0f ) {
                gGameState = LOST;

                gWitchDisappearing = true;
                gWitch->setVelocity({0.0f, 0.0f});
                gWitch->setAcceleration({0.0f, 0.0f});
                gWitch->setDirection(DOWN);
                gWitch->setFrameSpeed(8);
            } else if (gPumpkin->isCollidingBottom()) {

                float pumpkinBottom = gPumpkin->getColliderPosition().y +
                                      gPumpkin->getColliderDimensions().y / 2.0f;

                float sackTop = gSack->getColliderPosition().y -
                                gSack->getColliderDimensions().y / 2.0f;

                bool landedOnSack =
                    fabs(gPumpkin->getColliderPosition().x -
                        gSack->getColliderPosition().x) <
                    (gPumpkin->getColliderDimensions().x +
                    gSack->getColliderDimensions().x) / 2.0f &&
                    fabs(pumpkinBottom - sackTop) < 15.0f;

                gGameState = WON;

                if (landedOnSack) {
                    gPumpkinEnteringSack = true;
                    gSackAnimationTime = 0.0f;

                    gSack->setVelocity({0.0f, 0.0f});
                    gSack->setAcceleration({0.0f, 0.0f});
                }
            } else if (gPumpkin->getColliderPosition().y + gPumpkin->getColliderDimensions().y / 2.0f >= SCREEN_HEIGHT) {
                gGameState = LOST;

                Vector2 position = gPumpkin->getPosition();
                position.y = SCREEN_HEIGHT - gPumpkin->getColliderDimensions().y / 2.0f - 5.0f;
                gPumpkin->setPosition(position);
            }
    }

    if ((gGameState == WON || gGameState == LOST) && !gShowEndScreen && gEndTime != 0.0f) {
        if (gGameState == WON || (gGameState == LOST && !gWitchDisappearing)) {
            if (gWitch->getDirection() == RIGHT) {
                gWitch->setVelocity({150.0f, 0.0f});
            } else if (gWitch->getDirection() == LEFT) {
                gWitch->setVelocity({-150.0f, 0.0f});
            }
        }

        gWitch->update(
            deltaTime,
            nullptr,
            0,
            nullptr,
            0,
            FIXED_TIMESTEP
        );

        if (gGameState == WON || (gGameState == LOST && !gWitchDisappearing)) {
            if (gWitch->getPosition().x >= 750.0f && gWitch->getDirection() != LEFT) {
                gWitch->setVelocity({-150.0f, 0.0f});
                gWitch->setDirection(LEFT);
            } else if (gWitch->getPosition().x <= 50.0f && gWitch->getDirection() != RIGHT) {
                gWitch->setVelocity({150.0f, 0.0f});
                gWitch->setDirection(RIGHT);
            }
        }
    }

    if ((gGameState == WON || gGameState == LOST) && !gShowEndScreen) {
        if (gEndTime == 0.0f) {
            gEndTime = static_cast<float>(GetTime());

            gPumpkin->setVelocity({0.0f, 0.0f});
            gPumpkin->setAcceleration({0.0f, 0.0f});

            if (gGameState == LOST) {
                gPumpkin->setDirection(UP);
                gPumpkin->setFrameSpeed(8);
            } else if (gGameState == WON) {
                gPumpkin->setDirection(RIGHT);
                gPumpkin->setFrameSpeed(8);
            }
        }

        gPumpkin->update(
            deltaTime,
            nullptr,
            0,
            nullptr,
            0,
            FIXED_TIMESTEP
        );

        if (GetTime() - gEndTime >= 1.7f) {
            gShowEndScreen = true;
        }
    }

    UpdateMusicStream(gBGM);
}

void render() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    Rectangle backgroundTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(gBackgroundTexture.width),
        static_cast<float>(gBackgroundTexture.height)
    };

    Rectangle backgroundDestinationArea = {
        0.0f,
        0.0f,
        static_cast<float>(SCREEN_WIDTH),
        static_cast<float>(SCREEN_HEIGHT)
    };

    Vector2 backgroundOrigin = {
        0.0f,
        0.0f
    };

    DrawTexturePro(
        gBackgroundTexture,
        backgroundTextureArea,
        backgroundDestinationArea,
        backgroundOrigin,
        0.0f,
        WHITE
    );

    gHouse->render();

    if (gGameState == PLAYING || ((gGameState == WON || gGameState == LOST) && !gShowEndScreen)) {
        gPumpkin->render();
    }

    gSack->render();

    if (gGameState != LOST || !gShowEndScreen) {
        gWitch->render();
    }

    DrawText(
        TextFormat("Pumpkin Power: %.0f", gPumpkin->getFuel()),
        20,
        20,
        20,
        WHITE
    );

    if (gGameState == WON && gShowEndScreen) {
        Rectangle winTextureArea = {
            0.0f,
            0.0f,
            static_cast<float>(gWinTexture.width),
            static_cast<float>(gWinTexture.height)
        };

        Rectangle winDestinationArea = {
            0.0f,
            0.0f,
            static_cast<float>(SCREEN_WIDTH),
            static_cast<float>(SCREEN_HEIGHT)
        };

        DrawTexturePro(
            gWinTexture,
            winTextureArea,
            winDestinationArea,
            {0.0f, 0.0f},
            0.0f,
            WHITE
        );
    }

    if (gGameState == LOST && gShowEndScreen) {
        Rectangle loseTextureArea = {
            0.0f,
            0.0f,
            static_cast<float>(gLoseTexture.width),
            static_cast<float>(gLoseTexture.height)
        };

        Rectangle loseDestinationArea = {
            0.0f,
            0.0f,
            static_cast<float>(SCREEN_WIDTH),
            static_cast<float>(SCREEN_HEIGHT)
        };

        DrawTexturePro(
            gLoseTexture,
            loseTextureArea,
            loseDestinationArea,
            {0.0f, 0.0f},
            0.0f,
            WHITE
        );
    }

    if (gGameState == HOMEPAGE) {
        Rectangle homepageTextureArea = {
            0.0f,
            0.0f,
            static_cast<float>(gHomepageTexture.width),
            static_cast<float>(gHomepageTexture.height)
        };

        Rectangle homepageDestinationArea = {
            0.0f,
            0.0f,
            static_cast<float>(SCREEN_WIDTH),
            static_cast<float>(SCREEN_HEIGHT)
        };

        DrawTexturePro(
            gHomepageTexture,
            homepageTextureArea,
            homepageDestinationArea,
            {0.0f, 0.0f},
            0.0f,
            WHITE
        );
    }

    EndDrawing();
}

void shutdown() {
    UnloadMusicStream(gBGM);
    UnloadSound(gPump);
    UnloadTexture(gBackgroundTexture);
    UnloadTexture(gHomepageTexture);
    UnloadTexture(gWinTexture);
    UnloadTexture(gLoseTexture);

    delete gPumpkin;
    delete gHouse;
    delete gPorch;
    delete gWitch;
    delete gSack;

    CloseAudioDevice();
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
