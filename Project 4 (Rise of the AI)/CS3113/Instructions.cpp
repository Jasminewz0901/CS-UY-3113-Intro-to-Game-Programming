#include "Instructions.h"

/**
* Author: Jasmine Zhang
* Assignment: Rise of the AI
* Date due: 11/16/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

Instructions::Instructions(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode} {}

Instructions::~Instructions() { shutdown(); }

void Instructions::initialise() {
    mGameState.nextSceneID = NO_SCENE;

    mInstructionsTexture = LoadTexture("assets/instructions.png");
    mPausedInstructionsTexture = LoadTexture("assets/instructions_paused.png");
}

void Instructions::setPaused(bool paused) {
    mIsPaused = paused;
}

void Instructions::update(float deltaTime) {
    if (IsKeyPressed(KEY_ONE)) {
        mGameState.nextSceneID = LEVEL1;
    } else if (IsKeyPressed(KEY_TWO)) {
        mGameState.nextSceneID = LEVEL2;
    } else if (IsKeyPressed(KEY_THREE)) {
        mGameState.nextSceneID = LEVEL3;
    }
}

void Instructions::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    Texture2D texture = mIsPaused
        ? mPausedInstructionsTexture
        : mInstructionsTexture;

    DrawTexturePro(
        texture,
        { 0.0f, 0.0f, (float)texture.width, (float)texture.height },
        { 0.0f, 0.0f, 800.0f, 450.0f },
        { 0.0f, 0.0f },
        0.0f,
        WHITE
    );
}

void Instructions::shutdown() {
    if (mInstructionsTexture.id != 0) {
        UnloadTexture(mInstructionsTexture);
        mInstructionsTexture = {};
    }

    if (mPausedInstructionsTexture.id != 0) {
        UnloadTexture(mPausedInstructionsTexture);
        mPausedInstructionsTexture = {};
    }

    mIsPaused = false;
}