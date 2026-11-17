#include "CS3113/Menu.h"
#include "CS3113/Instructions.h"
#include "CS3113/Level1.h"
#include "CS3113/Level2.h"
#include "CS3113/Level3.h"
#include "CS3113/Boss.h"
#include "CS3113/Lose.h"
#include "CS3113/Win.h"

/**
* Author: Jasmine Zhang
* Assignment: Rise of the AI
* Date due: 11/16/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

// Global Constants
constexpr int SCREEN_WIDTH  = 800,
              SCREEN_HEIGHT = 450,
              FPS           = 120;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus       = RUNNING;
float     gPreviousTicks   = 0.0f,
          gTimeAccumulator = 0.0f;

int gHearts = 3;
bool gHalloweenKeyCollected = false;
bool gThanksgivingKeyCollected = false;
bool gWinterKeyCollected = false;
bool gInstructionsPaused = false;
Scene *gPausedScene = nullptr;

Texture2D gKeySilhouette = {};
Texture2D gHalloweenKeyTexture = {};
Texture2D gThanksgivingKeyTexture = {};
Texture2D gWinterKeyTexture = {};

Camera2D gCamera = {};

Music gMenuMusic = {};
Music gLoseMusic = {};

Scene                     *gCurrentScene = nullptr;
std::map<SceneID, Scene *> gLevels       = {};

// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene) {
    bool wasMenu = gCurrentScene == gLevels[MENU] ||
                   gCurrentScene == gLevels[INSTRUCTIONS];

    bool goingToMenu = scene == gLevels[MENU] ||
                       scene == gLevels[INSTRUCTIONS];

    if (gCurrentScene != nullptr) {
        gCurrentScene->shutdown();
    }

    if (wasMenu && !goingToMenu) {
        StopMusicStream(gMenuMusic);
    }

    if (gCurrentScene == gLevels[LOSE]) {
        StopMusicStream(gLoseMusic);
    }

    gCurrentScene = scene;
    gCurrentScene->initialise();

    if (goingToMenu && !wasMenu) {
        PlayMusicStream(gMenuMusic);
    }

    if (scene == gLevels[LOSE]) {
        PlayMusicStream(gLoseMusic);
    }

    if (gCurrentScene == gLevels.at(BOSS)) {
        gCamera.target = ORIGIN;
    } else if (gCurrentScene->getState().player != nullptr) {
        gCamera.target = { 350.0f, ORIGIN.y };
    } else {
        gCamera.target = ORIGIN;
    }
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 4: Operation: Save Christmas");
    InitAudioDevice();

    gKeySilhouette = LoadTexture("assets/key/silhouette.png");
    gHalloweenKeyTexture = LoadTexture("assets/key/halloween_key.png");
    gThanksgivingKeyTexture = LoadTexture("assets/key/thanksgiving_key.png");
    gWinterKeyTexture = LoadTexture("assets/key/winter_key.png");

    gMenuMusic = LoadMusicStream("assets/menu_music.mp3");
    gLoseMusic = LoadMusicStream("assets/lose.mp3");

    gMenuMusic.looping = true;
    gLoseMusic.looping = true;

    PlayMusicStream(gMenuMusic);

    gLevels[MENU] = new Menu(ORIGIN, "#000000");
    gLevels[INSTRUCTIONS] = new Instructions(ORIGIN, "#000000");
    gLevels[LOSE] = new Lose(ORIGIN, "#000000");
    gLevels[WIN] = new Win(ORIGIN, "#000000");
    gLevels[LEVEL1] = new Level1(ORIGIN, "#000000");

    static_cast<Level1*>(gLevels.at(LEVEL1))->setHearts(&gHearts);
    static_cast<Level1*>(gLevels.at(LEVEL1))->setHalloweenKeyCollected(&gHalloweenKeyCollected);

    gLevels[LEVEL2] = new Level2(ORIGIN, "#000000");

    static_cast<Level2*>(gLevels.at(LEVEL2))->setHearts(&gHearts);
    static_cast<Level2*>(gLevels.at(LEVEL2))->setThanksgivingKeyCollected(&gThanksgivingKeyCollected);

    gLevels[LEVEL3] = new Level3(ORIGIN, "#000000");

    static_cast<Level3*>(gLevels.at(LEVEL3))->setHearts(&gHearts);
    static_cast<Level3*>(gLevels.at(LEVEL3))->setWinterKeyCollected(&gWinterKeyCollected);
    static_cast<Level3*>(gLevels.at(LEVEL3))->setHalloweenKeyCollected(&gHalloweenKeyCollected);
    static_cast<Level3*>(gLevels.at(LEVEL3))->setThanksgivingKeyCollected(&gThanksgivingKeyCollected);

    gLevels[BOSS] = new Boss(ORIGIN, "#000000");
    static_cast<Boss*>(gLevels.at(BOSS))->setHearts(&gHearts);

    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    switchToScene(gLevels[MENU]);

    SetTargetFPS(FPS);
}

void processInput()
{
    Entity *player = gCurrentScene->getState().player;

    if (gInstructionsPaused) {
        if (IsKeyPressed(KEY_H)) {
            static_cast<Instructions *>(gLevels.at(INSTRUCTIONS))->setPaused(false);

            gLevels.at(INSTRUCTIONS)->shutdown();
            gInstructionsPaused = false;
            gPausedScene = nullptr;
        }

        if (WindowShouldClose()) {
            gAppStatus = TERMINATED;
        }

        return;
    }

    if (IsKeyPressed(KEY_H) &&
        (gCurrentScene == gLevels.at(LEVEL1) ||
        gCurrentScene == gLevels.at(LEVEL2) ||
        gCurrentScene == gLevels.at(LEVEL3) || 
        gCurrentScene == gLevels.at(BOSS))) {

        gPausedScene = gCurrentScene;
        gLevels.at(INSTRUCTIONS)->initialise();

        static_cast<Instructions *>(gLevels.at(INSTRUCTIONS))->setPaused(true);

        gInstructionsPaused = true;
        return;
    }

    if (gCurrentScene == gLevels.at(LEVEL3)) {
        Level3 *level = static_cast<Level3 *>(gCurrentScene);

        if (level->isDoorMessageShowing()) {
            if (WindowShouldClose()) {
                gAppStatus = TERMINATED;
            }
            return;
        }
    }

    if (gCurrentScene == gLevels.at(LEVEL1) ||
        gCurrentScene == gLevels.at(LEVEL2) ||
        gCurrentScene == gLevels.at(LEVEL3)) {

        if (IsKeyPressed(KEY_ONE) && gCurrentScene != gLevels.at(LEVEL1)) {
            switchToScene(gLevels.at(LEVEL1));
            return;
        }

        if (IsKeyPressed(KEY_TWO) && gCurrentScene != gLevels.at(LEVEL2)) {
            switchToScene(gLevels.at(LEVEL2));
            return;
        }

        if (IsKeyPressed(KEY_THREE) && gCurrentScene != gLevels.at(LEVEL3)) {
            switchToScene(gLevels.at(LEVEL3));
            return;
        }
    }

    if (IsKeyPressed(KEY_R) && gCurrentScene == gLevels.at(LEVEL1)) {
        static_cast<Level1 *>(gCurrentScene)->restart();
        return;
    }

    if (IsKeyPressed(KEY_R) && gCurrentScene == gLevels.at(LEVEL2)) {
        static_cast<Level2 *>(gCurrentScene)->restart();
        return;
    }

    if (IsKeyPressed(KEY_R) && gCurrentScene == gLevels.at(LEVEL3)) {
        static_cast<Level3 *>(gCurrentScene)->restart();
        return;
    }

    if (gCurrentScene == gLevels.at(LEVEL3) &&
        IsKeyPressed(KEY_E)) {

        static_cast<Level3 *>(gCurrentScene)->interactWithDoor();
    }

    if (player != nullptr) {
        player->resetMovement();

        if (IsKeyDown(KEY_A)) {
            player->moveLeft();
        } else if (IsKeyDown(KEY_D)) {
            player->moveRight();
        }

        if (IsKeyPressed(KEY_W) && player->isCollidingBottom()) {
            player->jump();
            PlaySound(gCurrentScene->getState().jumpSound);
        }

    if (IsKeyPressed(KEY_J)) {
        if (gCurrentScene == gLevels.at(LEVEL1)) {
            Level1 *level = static_cast<Level1 *>(gCurrentScene);

            if (level->canThrowSnowball()) {
                player->throwSnowball();
                level->throwSnowball();
            }
        } else if (gCurrentScene == gLevels.at(LEVEL2)) {
            Level2 *level = static_cast<Level2 *>(gCurrentScene);

            if (level->canThrowSnowball()) {
                player->throwSnowball();
                level->throwSnowball();
            }
        } else if (gCurrentScene == gLevels.at(LEVEL3)) {
            Level3 *level = static_cast<Level3 *>(gCurrentScene);

            if (level->canThrowSnowball()) {
                player->throwSnowball();
                level->throwSnowball();
            }
        } else if (gCurrentScene == gLevels.at(BOSS)) {
            Boss *level = static_cast<Boss *>(gCurrentScene);

            if (level->canThrowSnowball()) {
                player->throwSnowball();
                level->throwSnowball();
            }
        }
    }

        if(GetLength(player->getMovement()) > 1.0f) {
            player->normaliseMovement();
        }
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    // Delta time
    float ticks     = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (gInstructionsPaused) {
        UpdateMusicStream(gPausedScene->getState().bgm);

        gPreviousTicks = static_cast<float>(GetTime());
        gTimeAccumulator = 0.0f;
        return;
    }

    if (gCurrentScene == gLevels[MENU] || gCurrentScene == gLevels[INSTRUCTIONS]) {
        UpdateMusicStream(gMenuMusic);
    }

    if (gCurrentScene == gLevels[LOSE]) {
        UpdateMusicStream(gLoseMusic);
    }

    if (deltaTime < FIXED_TIMESTEP) {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;

        if (gCurrentScene->getState().player != nullptr &&
            gCurrentScene != gLevels.at(BOSS)) {

            float playerX = gCurrentScene->getState().player->getPosition().x;
            float playerY = gCurrentScene->getState().player->getPosition().y;

            Vector2 cameraPosition = {
                playerX,
                playerY - 50.0f
            };

            // Horizontal boundaries
            if (cameraPosition.x < 350.0f) {
                cameraPosition.x = 350.0f;
            }

            if (cameraPosition.x > 450.0f) {
                cameraPosition.x = 450.0f;
            }

            // Vertical boundaries
            if (cameraPosition.y < -50.0f) {
                cameraPosition.y = -50.0f;
            }

            if (cameraPosition.y > 675.0f) {
                cameraPosition.y = 675.0f;
            }

            panCamera(&gCamera, &cameraPosition);
        }
    }

    gTimeAccumulator = deltaTime;
}

void renderKeyHUD() {
    Rectangle destination = {
        5.0f, 40.0f, 65.0f, 100.0f
    };

    DrawTexturePro(
        gKeySilhouette,
        { 0, 0, (float)gKeySilhouette.width, (float)gKeySilhouette.height },
        destination,
        { 0, 0 },
        0,
        WHITE
    );

    if (gHalloweenKeyCollected) {
        DrawTexturePro(
            gHalloweenKeyTexture,
            { 0, 0, (float)gHalloweenKeyTexture.width, (float)gHalloweenKeyTexture.height },
            destination,
            { 0, 0 },
            0,
            WHITE
        );
    }

    if (gThanksgivingKeyCollected) {
        DrawTexturePro(
            gThanksgivingKeyTexture,
            { 0, 0, (float)gThanksgivingKeyTexture.width, (float)gThanksgivingKeyTexture.height },
            destination,
            { 0, 0 },
            0,
            WHITE
        );
    }

    if (gWinterKeyCollected) {
        DrawTexturePro(
            gWinterKeyTexture,
            { 0, 0, (float)gWinterKeyTexture.width, (float)gWinterKeyTexture.height },
            destination,
            { 0, 0 },
            0,
            WHITE
        );
    }
}

void render()
{
    BeginDrawing();
    BeginMode2D(gCamera);

    gCurrentScene->render();

    EndMode2D();

    if (gInstructionsPaused) {
        ClearBackground(BLACK);
        gLevels.at(INSTRUCTIONS)->render();
        EndDrawing();
        return;
    }

    if (gCurrentScene == gLevels.at(LEVEL1)) {
        static_cast<Level1 *>(gCurrentScene)->renderHearts();
        renderKeyHUD();
    }

    if (gCurrentScene == gLevels.at(LEVEL2)) {
        static_cast<Level2 *>(gCurrentScene)->renderHearts();
        renderKeyHUD();
    }

    if (gCurrentScene == gLevels.at(LEVEL3)) {
        static_cast<Level3 *>(gCurrentScene)->renderHearts();
        renderKeyHUD();
        static_cast<Level3 *>(gCurrentScene)->renderDoorMessage();
    }

    if (gCurrentScene == gLevels.at(BOSS)) {
        static_cast<Boss *>(gCurrentScene)->renderHearts();
        static_cast<Boss *>(gCurrentScene)->renderBossHealth();
    }

    EndDrawing();
}

void shutdown() {
    for (std::pair<const SceneID, Scene *> &entry : gLevels)
        delete entry.second;

    gLevels.clear();

    UnloadTexture(gKeySilhouette);
    UnloadTexture(gHalloweenKeyTexture);
    UnloadTexture(gThanksgivingKeyTexture);
    UnloadTexture(gWinterKeyTexture);

    StopMusicStream(gMenuMusic);
    StopMusicStream(gLoseMusic);

    UnloadMusicStream(gMenuMusic);
    UnloadMusicStream(gLoseMusic);

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        if (gCurrentScene->getState().nextSceneID != NO_SCENE)
        {
            SceneID id = gCurrentScene->getState().nextSceneID;

            if (id == MENU) {
                gHearts = 3;
                gHalloweenKeyCollected = false;
                gThanksgivingKeyCollected = false;
                gWinterKeyCollected = false;
            }

            switchToScene(gLevels[id]);
        }

        render();
    }

    shutdown();

    return 0;
}
