#include "Boss.h"

/**
* Author: Jasmine Zhang
* Assignment: Rise of the AI
* Date due: 11/16/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

Boss::Boss() : Scene() {}

Boss::Boss(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode } {}

Boss::~Boss() {
    shutdown();
}

void Boss::initialise() {
    mGameState.nextSceneID = NO_SCENE;

    mInvincibilityTime = 0.0f;

    mBossHearts = 25;
    mBossDefeated = false;
    mGrinchDefeatTime = 0.0f;

    mMariahRevealing = false;
    mMariahRevealTime = 0.0f;

    mBossAttackTime = 0.0f;
    mSnowballCooldown = 0.0f;

    mGrinchAttacking = false;
    mGrinchAttackCooldown = 0.0f;
    mGrinchAttackDuration = 0.0f;
    mGrinchMovingLeft = true;

    mPresentsThrown = 0;
    mPresentSpawnTimer = 0.0f;

    mBackgroundTexture = LoadTexture("assets/final/background.png");
    mHeartsTexture = LoadTexture("assets/hearts.png");

    mGameState.map = new Map(
        LEVEL_WIDTH,
        LEVEL_HEIGHT,
        mLevelData,
        "assets/final/tiles.png",
        TILE_DIMENSION,
        4,
        1,
        mOrigin
    );

    // ----------- PLAYER -----------
    std::map<Direction, std::vector<int>> playerAnimationAtlas = {
        { DOWN,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { LEFT,  { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { UP,    { 16, 17, 18, 19 } },
        { RIGHT, { 8, 9, 10, 11, 12, 13, 14, 15 } }
    };

    mGameState.player = new Entity(
        { mOrigin.x - 250.0f, mOrigin.y + 50.0f },
        { 75.0f, 75.0f },
        "assets/player.png",
        ATLAS,
        { 8, 5 },
        playerAnimationAtlas,
        PLAYER
    );

    mGameState.player->setJumpingPower(550.0f);

    mGameState.player->setColliderDimensions({
        mGameState.player->getScale().x / 2.5f,
        mGameState.player->getScale().y * 0.8f
    });

    mGameState.player->setAcceleration({
        0.0f,
        ACCELERATION_OF_GRAVITY
    });

    // ----------- GRINCH -----------
    std::map<Direction, std::vector<int>> grinchAnimationAtlas = {
        { DOWN,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { RIGHT, { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { UP,    { 16, 17, 18, 19, 20, 21, 22, 23 } }
    };

    mGrinch = new Entity(
        { mOrigin.x + 250.0f, mOrigin.y + 45.0f },
        { 95.0f, 95.0f },
        "assets/final/grinch.png",
        ATLAS,
        { 8, 3 },
        grinchAnimationAtlas,
        NONE
    );

    mGrinch->setDirection(DOWN);
    mGrinch->setFrameSpeed(8);
    mGrinch->setColliderDimensions({ 60.0f, 80.0f });

    // ----------- MARIAH CAREY -----------
    std::map<Direction, std::vector<int>> mariahAnimationAtlas = {
        { DOWN,  { 0 } },
        { UP,    { 0 } },
        { LEFT,  { 0 } },
        { RIGHT, { 0, 0, 0, 0, 1, 2, 3, 4,
                   5, 6, 7, 8, 9, 10, 11, 12,
                   13, 14, 15, 16, 17, 18, 19, 20,
                   21, 22, 23, 23, 23, 23, 23, 23 } }
    };

    mMariah = new Entity(
        { mOrigin.x, mOrigin.y - 25.0f },
        { 130.0f, 130.0f },
        "assets/final/mariah.png",
        ATLAS,
        { 8, 3 },
        mariahAnimationAtlas,
        NONE
    );

    mMariah->setDirection(DOWN);

    mGameState.jumpSound = LoadSound("assets/jump.mp3");

    mSnowballImpactSound = LoadSound("assets/snowball_impact.mp3");
    SetSoundVolume(mSnowballImpactSound, 1.0f);

    mGameState.bgm = LoadMusicStream("assets/final/sounds/boss_music.mp3");
    SetMusicVolume(mGameState.bgm, 1.0f);
    PlayMusicStream(mGameState.bgm);

    mItsTimeMusic = LoadMusicStream("assets/final/sounds/its_time.mp3");
    SetMusicVolume(mItsTimeMusic, 1.0f);
}

void Boss::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);

    if (mBossDefeated && !mMariahRevealing) {
        mGrinchDefeatTime += deltaTime;

        if (mGrinchDefeatTime >= GRINCH_DEFEAT_DURATION) {
            mMariahRevealing = true;
            mMariahRevealTime = 0.0f;

            mMariah->setDirection(RIGHT);
            mMariah->setFrameSpeed(2);

            StopMusicStream(mGameState.bgm);
            PlayMusicStream(mItsTimeMusic);
        }
    }

    if (mMariahRevealing) {
        UpdateMusicStream(mItsTimeMusic);
        mMariahRevealTime += deltaTime;

        if (mMariahRevealTime >= MARIAH_REVEAL_DURATION) {
            mGameState.nextSceneID = WIN;
            return;
        }
    }

    mMariah->update(
        deltaTime,
        nullptr,
        nullptr,
        nullptr,
        0
    );

    if (mInvincibilityTime > 0.0f) {
        mInvincibilityTime -= deltaTime;
    }

    if (mSnowballCooldown > 0.0f) {
        mSnowballCooldown -= deltaTime;
    }

    mGameState.player->update(
        deltaTime,
        nullptr,
        mGameState.map,
        nullptr,
        0
    );

    if (!mBossDefeated) {
        mGrinchAttackCooldown += deltaTime;

    if (!mGrinchAttacking &&
        mGrinchAttackCooldown >= GRINCH_ATTACK_INTERVAL) {

        mGrinchAttacking = true;
        mGrinchAttackDuration = 0.0f;
        mGrinchAttackCooldown = 0.0f;

        mPresentsThrown = 0;
        mPresentSpawnTimer = 0.0f;

        if (mGameState.player->getPosition().x < mGrinch->getPosition().x) {
            mGrinch->setFacingLeft(true);
        }
        else {
            mGrinch->setFacingLeft(false);
        }

        mGrinch->setDirection(RIGHT);
    }

    // Grinch throwing animation
    if (mGrinchAttacking) {
        mGrinchAttackDuration += deltaTime;
        mPresentSpawnTimer += deltaTime;

        if (mPresentsThrown < 3 &&
            mPresentSpawnTimer >= 0.3f) {

            throwPresent();

            mPresentsThrown++;
            mPresentSpawnTimer = 0.0f;
        }

        if (mGrinchAttackDuration >= GRINCH_ATTACK_DURATION) {
            mGrinchAttacking = false;

            // Return to walking animation
            mGrinch->setDirection(DOWN);
        }
    }
    else {
        // Grinch patrol AI
        Vector2 grinchPosition = mGrinch->getPosition();

        if (mGrinchMovingLeft) {
            grinchPosition.x -= mGrinchSpeed * deltaTime;

            if (grinchPosition.x <= mGrinchLeftBoundary) {
                grinchPosition.x = mGrinchLeftBoundary;
                mGrinchMovingLeft = false;
            }
        }
        else {
            grinchPosition.x += mGrinchSpeed * deltaTime;

            if (grinchPosition.x >= mGrinchRightBoundary) {
                grinchPosition.x = mGrinchRightBoundary;
                mGrinchMovingLeft = true;
            }
        }

        mGrinch->setPosition(grinchPosition);
    }
    }

    mGrinch->update(
        deltaTime,
        nullptr,
        nullptr,
        nullptr,
        0
    );
    

    for (int i = 0; i < mPresents.size(); i++) {
        Entity *present = mPresents[i];

        if (!mPresentImpact[i]) {
            Vector2 position = present->getPosition();

            position.y += mPresentSpeed * deltaTime;

            // Check collision with player
            Vector2 playerPosition = mGameState.player->getPosition();

            float distanceX = position.x - playerPosition.x;
            float distanceY = position.y - playerPosition.y;

            if (distanceX < 0) distanceX = -distanceX;
            if (distanceY < 0) distanceY = -distanceY;

            if (distanceX < 45.0f && distanceY < 45.0f &&
                mInvincibilityTime <= 0.0f) {

                (*mHearts)--;
                mInvincibilityTime = 1.5f;

                mGameState.player->hurt();
            }

            if (position.y >= mOrigin.y + 50.0f) {
                position.y = mOrigin.y + 50.0f;

                mPresentImpact[i] = true;
                mPresentImpactTime[i] = 0.0f;

                PlaySound(mSnowballImpactSound);

                present->setDirection(UP);
            }

            present->setPosition(position);
        }
        else {
            mPresentImpactTime[i] += deltaTime;

            if (mPresentImpactTime[i] >= 0.75f) {
                present->deactivate();
            }
        }

        if (present->isActive()) {
            present->update(
                deltaTime,
                nullptr,
                nullptr,
                nullptr,
                0
            );
        }
    }

    for (int i = 0; i < mSnowballs.size(); i++) {
        Entity *snowball = mSnowballs[i];

        if (!snowball->isActive()) {
            continue;
        }

        Vector2 position = snowball->getPosition();

        position.x += mSnowballDirections[i] * mSnowballSpeed * deltaTime;

        snowball->setPosition(position);

        Vector2 grinchPosition = mGrinch->getPosition();

        float distanceX = position.x - grinchPosition.x;
        float distanceY = position.y - grinchPosition.y;

        if (distanceX < 0) distanceX = -distanceX;
        if (distanceY < 0) distanceY = -distanceY;

        if (distanceX < 45.0f && distanceY < 45.0f &&
            !mBossDefeated) {

            mBossHearts--;

            PlaySound(mSnowballImpactSound);

            snowball->deactivate();

            if (mBossHearts <= 0) {
                mBossHearts = 0;
                mBossDefeated = true;
                mGrinchDefeatTime = 0.0f;

                mGrinch->setDirection(UP);
            }
        }

        snowball->update(
            deltaTime,
            nullptr,
            nullptr,
            nullptr,
            0
        );
    }

    if (*mHearts <= 0) {
        mGameState.nextSceneID = LOSE;
        return;
    }
}

void Boss::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    DrawTexturePro(
        mBackgroundTexture,
        {
            0.0f, 0.0f,
            (float)mBackgroundTexture.width,
            (float)mBackgroundTexture.height
        },
        {
            0.0f, 0.0f,
            800.0f, 450.0f
        },
        { 0.0f, 0.0f },
        0.0f,
        WHITE
    );

    mMariah->render();

    mGameState.map->render();
    mGameState.player->render();

    if (!mMariahRevealing) {
        mGrinch->render();
    }

    for (int i = 0; i < mPresents.size(); i++) {
        if (mPresents[i]->isActive()) {
            mPresents[i]->render();
        }
    }

    for (int i = 0; i < mSnowballs.size(); i++) {
        if (mSnowballs[i]->isActive()) {
            mSnowballs[i]->render();
        }
    }
}

void Boss::shutdown() {
    if (mBackgroundTexture.id != 0) {
        UnloadTexture(mBackgroundTexture);
        mBackgroundTexture = {};
    }

    if (mGameState.map != nullptr) {
        delete mGameState.map;
        mGameState.map = nullptr;
    }

    if (mGameState.player != nullptr) {
        delete mGameState.player;
        mGameState.player = nullptr;
    }

    if (mGrinch != nullptr) {
        delete mGrinch;
        mGrinch = nullptr;
    }

    if (mMariah != nullptr) {
        delete mMariah;
        mMariah = nullptr;
    }

    for (int i = 0; i < mPresents.size(); i++) {
        delete mPresents[i];
    }

    mPresents.clear();
    mPresentImpact.clear();
    mPresentImpactTime.clear();

    for (int i = 0; i < mSnowballs.size(); i++) {
        delete mSnowballs[i];
    }

    mSnowballs.clear();
    mSnowballDirections.clear();
    mSnowballImpact.clear();
    mSnowballImpactTime.clear();

    if (mHeartsTexture.id != 0) {
        UnloadTexture(mHeartsTexture);
        mHeartsTexture = {};
    }

    StopMusicStream(mGameState.bgm);
    UnloadMusicStream(mGameState.bgm);

    UnloadSound(mSnowballImpactSound);

    StopMusicStream(mItsTimeMusic);
    UnloadMusicStream(mItsTimeMusic);

    if (mGameState.jumpSound.frameCount > 0) {
        UnloadSound(mGameState.jumpSound);
        mGameState.jumpSound = {};
    }
}

void Boss::restart() {
    // Reset the boss fight here
}

void Boss::throwSnowball() {
    if (mSnowballCooldown > 0.0f) {
        return;
    }

    std::map<Direction, std::vector<int>> snowballAnimationAtlas = {
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { UP,    { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { DOWN,  { 0, 1, 2, 3, 4, 5, 6, 7 } }
    };

    Vector2 playerPosition = mGameState.player->getPosition();

    float direction = 1.0f;

    if (mGameState.player->isFacingLeft()) {
        direction = -1.0f;
    }

    Entity *snowball = new Entity(
        { playerPosition.x + 35.0f * direction,
          playerPosition.y },
        { 35.0f, 35.0f },
        "assets/snowball.png",
        ATLAS,
        { 8, 2 },
        snowballAnimationAtlas,
        NONE
    );

    snowball->setDirection(DOWN);
    snowball->setFrameSpeed(10);
    snowball->setColliderDimensions({ 25.0f, 25.0f });

    mSnowballs.push_back(snowball);
    mSnowballDirections.push_back(direction);
    mSnowballImpact.push_back(false);
    mSnowballImpactTime.push_back(0.0f);

    mSnowballCooldown = SNOWBALL_COOLDOWN_TIME;
}

void Boss::throwPresent() {
    std::map<Direction, std::vector<int>> presentAnimationAtlas = {
        { DOWN,  { 0, 1, 2, 3, 4, 5 } },
        { UP,    { 6, 7, 8, 9, 10, 11 } },
        { LEFT,  { 0, 1, 2, 3, 4, 5 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5 } }
    };

    float playerX = mGameState.player->getPosition().x;

    float presentX = playerX;

    if (mPresentsThrown == 0) {
        presentX = playerX - 100.0f;
    }
    else if (mPresentsThrown == 1) {
        presentX = playerX;
    }
    else {
        presentX = playerX + 100.0f;
    }

    Entity *present = new Entity(
        { presentX, mOrigin.y - 250.0f },
        { 75.0f, 75.0f },
        "assets/final/gift.png",
        ATLAS,
        { 6, 2 },
        presentAnimationAtlas,
        NONE
    );

    present->setDirection(DOWN);
    present->setFrameSpeed(8);
    present->setColliderDimensions({ 50.0f, 50.0f });

    mPresents.push_back(present);
    mPresentImpact.push_back(false);
    mPresentImpactTime.push_back(0.0f);
}

void Boss::renderBossHealth() {
    DrawText(
        TextFormat("Grinch HP: %d / 25", mBossHearts),
        590,
        20,
        20,
        WHITE
    );
}

void Boss::renderHearts() {
    float frameWidth = mHeartsTexture.width / 4.0f;
    float frameHeight = static_cast<float>(mHeartsTexture.height);

    DrawTexturePro(
        mHeartsTexture,
        {
            (3 - *mHearts) * frameWidth,
            0.0f,
            frameWidth,
            frameHeight
        },
        {
            0.0f,
            -50.0f,
            115.0f,
            115.0f * frameHeight / frameWidth
        },
        { 0.0f, 0.0f },
        0.0f,
        WHITE
    );
}

void Boss::setHearts(int *hearts) {
    mHearts = hearts;
}