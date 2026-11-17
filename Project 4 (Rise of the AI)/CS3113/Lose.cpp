
#include "Lose.h"

/**
* Author: Jasmine Zhang
* Assignment: Rise of the AI
* Date due: 11/16/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

Lose::Lose(Vector2 origin, const char *bgHexCode)
    : Scene { origin, bgHexCode } {}

Lose::~Lose() {
    shutdown();
}

void Lose::initialise() {
    mGameState.player = nullptr;
    mGameState.map = nullptr;
    mGameState.nextSceneID = NO_SCENE;

    mLoseTexture = LoadTexture("assets/lose.png");
}

void Lose::update(float deltaTime) {
    if (IsKeyPressed(KEY_T)) {
        mGameState.nextSceneID = MENU;
    }
}

void Lose::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    DrawTexturePro(
        mLoseTexture,
        { 0.0f, 0.0f, (float)mLoseTexture.width, (float)mLoseTexture.height },
        { 0.0f, 0.0f, 800.0f, 450.0f },
        { 0.0f, 0.0f },
        0.0f,
        WHITE
    );
}

void Lose::shutdown() {
    if (mLoseTexture.id != 0) {
        UnloadTexture(mLoseTexture);
        mLoseTexture = {};
    }
}
