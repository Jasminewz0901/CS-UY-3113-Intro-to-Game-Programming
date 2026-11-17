#include "Menu.h"

/**
* Author: Jasmine Zhang
* Assignment: Rise of the AI
* Date due: 11/16/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

Menu::Menu(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

Menu::~Menu() { shutdown(); }

void Menu::initialise() {
    mGameState.nextSceneID = NO_SCENE;
    mGameState.player = nullptr;
    mGameState.map = nullptr;

    mMenuTexture = LoadTexture("assets/menu.png");
}

void Menu::update(float deltaTime) {
    if (IsKeyPressed(KEY_ENTER)) {
        mGameState.nextSceneID = INSTRUCTIONS;
    }
}

void Menu::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    DrawTexturePro(
        mMenuTexture,
        { 0.0f, 0.0f, (float)mMenuTexture.width, (float)mMenuTexture.height },
        { 0.0f, 0.0f, 800.0f, 450.0f },
        { 0.0f, 0.0f },
        0.0f,
        WHITE
    );
}

void Menu::shutdown() {
    if (mMenuTexture.id != 0) {
        UnloadTexture(mMenuTexture);
        mMenuTexture = {};
    }
}