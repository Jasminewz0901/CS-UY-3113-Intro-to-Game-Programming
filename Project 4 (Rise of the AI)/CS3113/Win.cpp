#include "Win.h"

/**
* Author: Jasmine Zhang
* Assignment: Rise of the AI
* Date due: 11/16/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

Win::Win(Vector2 origin, const char *bgHexCode)
    : Scene { origin, bgHexCode } {}

Win::~Win() {
    shutdown();
}

void Win::initialise() {
    mGameState.player = nullptr;
    mGameState.map = nullptr;
    mGameState.nextSceneID = NO_SCENE;

    mWinTexture = LoadTexture("assets/win.png");

    mVictoryMusic = LoadMusicStream("assets/final/sounds/victory.mp3");
    SetMusicVolume(mVictoryMusic, 0.5f);
    PlayMusicStream(mVictoryMusic);
}

void Win::update(float deltaTime) {
    UpdateMusicStream(mVictoryMusic);

    if (IsKeyPressed(KEY_R)) {
        mGameState.nextSceneID = MENU;
    }
}

void Win::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    DrawTexturePro(
        mWinTexture,
        { 0.0f, 0.0f, (float)mWinTexture.width, (float)mWinTexture.height },
        { 0.0f, 0.0f, 800.0f, 450.0f },
        { 0.0f, 0.0f },
        0.0f,
        WHITE
    );
}

void Win::shutdown() {
    if (mWinTexture.id != 0) {
        UnloadTexture(mWinTexture);
        mWinTexture = {};
    }

    StopMusicStream(mVictoryMusic);
    UnloadMusicStream(mVictoryMusic);
}
