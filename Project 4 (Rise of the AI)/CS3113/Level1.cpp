#include "Level1.h"

/**
* Author: Jasmine Zhang
* Assignment: Rise of the AI
* Date due: 11/16/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

Level1::Level1()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

Level1::Level1(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

Level1::~Level1() { shutdown(); }

void Level1::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mBackgroundTexture = LoadTexture("assets/level1/background.png");
    mHeartsTexture = LoadTexture("assets/hearts.png");

    mInvincibilityTime = 0.0f;

    mGameState.bgm = LoadMusicStream("assets/level1/sounds/level1_music.mp3");
    SetMusicVolume(mGameState.bgm, 0.30f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("assets/jump.mp3");
    mSnowballImpactSound = LoadSound("assets/snowball_impact.mp3");

    mGameState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT, // map grid cols & rows
        mLevelData,                // grid data
        "assets/level1/tiles.png", // texture filepath
        TILE_DIMENSION,            // tile size
        4, 1,                      // texture cols & rows
        mOrigin                    // in-game origin
    );

    std::map<Direction, std::vector<int>> playerAnimationAtlas = {
        { DOWN,  {  0,  1,  2,  3,  4,  5,  6,  7 } },
        { LEFT,  {  8,  9, 10, 11, 12, 13, 14, 15 } },
        { UP,    { 16, 17, 18, 19 } },
        { RIGHT, { 8, 9, 10, 11, 12, 13, 14, 15 } },
    };

    mGameState.player = new Entity(
        { mOrigin.x - 300.0f, mOrigin.y + 320.0f }, // position
        { 75.0f, 75.0f },             // scale
        "assets/player.png",                     // texture file address
        ATLAS,                                      // single image or atlas?
        { 8, 5 },                                   // atlas dimensions
        playerAnimationAtlas,                      // actual atlas
        PLAYER                                      // entity type
    );

    mGameState.player->setJumpingPower(550.0f);
    mGameState.player->setColliderDimensions({
        mGameState.player->getScale().x / 2.5f,
        mGameState.player->getScale().y * 0.8f
    });
    mGameState.player->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

    std::map<Direction, std::vector<int>> pumpkinAnimationAtlas = {
        { DOWN, { 0, 1, 2, 3, 4, 5 } },
        { LEFT, { 6, 7, 8, 9, 10, 11 } },
        { RIGHT, { 6, 7, 8, 9, 10, 11 } },
        { UP, { 12, 13, 14, 15, 16, 17 } }
    };

    for (int i = 0; i < PUMPKIN_COUNT; i++) {
        Entity *pumpkin = new Entity(
            { mPumpkinStartX[i], mPumpkinStartY[i] },
            { 60.0f, 60.0f },
            "assets/level1/pumpkin.png",
            ATLAS,
            { 6, 3 },
            pumpkinAnimationAtlas,
            NPC
        );

        pumpkin->setSpeed(100);
        pumpkin->setFrameSpeed(8);
        pumpkin->setColliderDimensions({ 40.0f, 40.0f });
        pumpkin->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
        pumpkin->setAIState(IDLE);
        pumpkin->setAIType(PATROLLER);

        mPumpkins.push_back(pumpkin);

        mPumpkinMovingLeft[i] = true;
        mPumpkinDefeated[i] = false;
        mPumpkinDefeatTime[i] = 0.0f;
    }

    std::map<Direction, std::vector<int>> skeletonAnimationAtlas = {
        { DOWN,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { LEFT,  { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { RIGHT, { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { UP,    { 16, 17, 18, 19, 20, 21, 22, 23 } }
    };

    for (int i = 0; i < SKELETON_COUNT; i++) {
        Entity *skeleton = new Entity(
            { mSkeletonStartX[i], mSkeletonStartY[i] },
            { 60.0f, 60.0f },
            "assets/level1/skeleton.png",
            ATLAS,
            { 8, 3 },
            skeletonAnimationAtlas,
            NPC
        );

        skeleton->setSpeed(120);
        skeleton->setFrameSpeed(8);
        skeleton->setColliderDimensions({ 40.0f, 40.0f });
        skeleton->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

        skeleton->setAIType(PATROLLER);
        skeleton->setAIState(IDLE);
        skeleton->setDirection(DOWN);

        mSkeletons.push_back(skeleton);

        mSkeletonDefeated[i] = false;
        mSkeletonDefeatTime[i] = 0.0f;
    }

    mSnowballCooldown = 0.0f;

    std::map<Direction, std::vector<int>> keyAnimationAtlas = {
        { DOWN,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { UP,    { 0, 1, 2, 3, 4, 5, 6, 7 } }
    };

    mHalloweenKey = new Entity(
        { 525.0f, 100.0f },
        { 50.0f, 100.0f },
        "assets/level1/key.png",
        ATLAS,
        { 8, 1 },
        keyAnimationAtlas,
        NONE
    );

    mHalloweenKey->setDirection(DOWN);
    mHalloweenKey->setFrameSpeed(6);
    mHalloweenKey->setColliderDimensions({ 35.0f, 35.0f });

    if (*mHalloweenKeyCollected) {
        mHalloweenKey->deactivate();
    }

    mItemPickupSound = LoadSound("assets/item_pickup.mp3");
}

void Level1::restart() {
    mGameState.player->setPosition({
        mOrigin.x - 300.0f,
        mOrigin.y + 320.0f
    });

    mGameState.player->resetMovement();
    mGameState.player->setVelocity({ 0.0f, 0.0f });

    for (int i = 0; i < PUMPKIN_COUNT; i++) {
        Entity *pumpkin = mPumpkins[i];

        pumpkin->setPosition({
            mPumpkinStartX[i],
            mPumpkinStartY[i]
        });

        pumpkin->resetMovement();
        pumpkin->setVelocity({ 0.0f, 0.0f });

        pumpkin->activate();
        pumpkin->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
        pumpkin->setDirection(LEFT);

        mPumpkinMovingLeft[i] = true;
        mPumpkinDefeated[i] = false;
        mPumpkinDefeatTime[i] = 0.0f;
    }

    for (int i = 0; i < SKELETON_COUNT; i++) {
        Entity *skeleton = mSkeletons[i];

        skeleton->setPosition({
            mSkeletonStartX[i],
            mSkeletonStartY[i]
        });

        skeleton->resetMovement();
        skeleton->setVelocity({ 0.0f, 0.0f });

        skeleton->activate();
        skeleton->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
        skeleton->setDirection(DOWN);

        mSkeletonDefeated[i] = false;
        mSkeletonDefeatTime[i] = 0.0f;
    }

    for (int i = 0; i < mSnowballs.size(); i++) {
        delete mSnowballs[i];
    }

    mSnowballs.clear();
    mSnowballDirections.clear();
    mSnowballImpact.clear();
    mSnowballImpactTime.clear();

    mSnowballCooldown = 0.0f;

    mInvincibilityTime = 0.0f;
}

void Level1::throwSnowball() {
    if (mSnowballCooldown > 0.0f) return;

    mSnowballCooldown = SNOWBALL_COOLDOWN_TIME;

    Vector2 playerPosition = mGameState.player->getPosition();

    float direction = 1.0f;

    if (mGameState.player->isFacingLeft()) {
        direction = -1.0f;
    }

    std::map<Direction, std::vector<int>> snowballAnimationAtlas = {
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { UP,    { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { DOWN,  { 0, 1, 2, 3, 4, 5, 6, 7 } }
    };

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

    snowball->setFrameSpeed(14);
    snowball->setFacingLeft(direction < 0.0f);

    mSnowballs.push_back(snowball);
    mSnowballDirections.push_back(direction);  
    mSnowballImpact.push_back(false);
    mSnowballImpactTime.push_back(0.0f);
}

void Level1::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    if (mInvincibilityTime > 0.0f) {
        mInvincibilityTime -= deltaTime;
    }

    if (mSnowballCooldown > 0.0f) {
        mSnowballCooldown -= deltaTime;
    }

    mGameState.player->update(
        deltaTime,      // delta time / fixed timestep
        nullptr,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

    if (mGameState.player->getPosition().y > END_GAME_THRESHOLD) {
        if (*mHearts > 0) {
            (*mHearts)--;
        }

        if (*mHearts <= 0) {
            mGameState.nextSceneID = LOSE;
            return;
        }

        mGameState.player->setPosition({
            mOrigin.x - 300.0f,
            mOrigin.y + 320.0f
        });

        mGameState.player->resetMovement();
        mGameState.player->setVelocity({0.0f, 0.0f});

        mGameState.player->hurt();

        mInvincibilityTime = INVINCIBILITY_DURATION;
    }

    if (!*mHalloweenKeyCollected && mHalloweenKey->isActive()) {
        mHalloweenKey->update(
            deltaTime,
            nullptr,
            nullptr,
            nullptr,
            0
        );

        Vector2 playerPosition = mGameState.player->getPosition();
        Vector2 keyPosition = mHalloweenKey->getPosition();

        Vector2 playerSize = mGameState.player->getColliderDimensions();
        Vector2 keySize = mHalloweenKey->getColliderDimensions();

        if (fabsf(playerPosition.x - keyPosition.x) <
                (playerSize.x + keySize.x) / 2.0f &&
            fabsf(playerPosition.y - keyPosition.y) <
                (playerSize.y + keySize.y) / 2.0f) {

            *mHalloweenKeyCollected = true;
            mHalloweenKey->deactivate();

            PlaySound(mItemPickupSound);
        }
    }

    for (int i = 0; i < PUMPKIN_COUNT; i++) {
        Entity *pumpkin = mPumpkins[i];

        if (!mPumpkinDefeated[i]) {
            float pumpkinX = pumpkin->getPosition().x;

            if (pumpkinX <= mPumpkinLeftBoundaries[i]) {
                mPumpkinMovingLeft[i] = false;
            } else if (pumpkinX >= mPumpkinRightBoundaries[i]) {
                mPumpkinMovingLeft[i] = true;
            }

            if (mPumpkinMovingLeft[i]) {
                pumpkin->moveLeft();
            } else {
                pumpkin->moveRight();
            }

            pumpkin->update(
                deltaTime,
                mGameState.player,
                mGameState.map,
                nullptr,
                0
            );
        } else if (pumpkin->isActive()) {
            mPumpkinDefeatTime[i] += deltaTime;

            pumpkin->resetMovement();
            pumpkin->setVelocity({ 0.0f, 0.0f });
            pumpkin->setAcceleration({ 0.0f, 0.0f });

            pumpkin->update(
                deltaTime,
                mGameState.player,
                nullptr,
                nullptr,
                0
            );

            if (mPumpkinDefeatTime[i] >= 6.0f / 8.0f) {
                pumpkin->deactivate();
            }
        }
    }

    for (int i = 0; i < SKELETON_COUNT; i++) {
        Entity *skeleton = mSkeletons[i];

        if (!mSkeletonDefeated[i]) {
            Vector2 skeletonPosition = skeleton->getPosition();
            Vector2 playerPosition = mGameState.player->getPosition();

            float distanceX = fabsf(playerPosition.x - skeletonPosition.x);

            bool chasePlayer = distanceX <= SKELETON_CHASE_RANGE;

            if (chasePlayer && distanceX > 25.0f) {
                if (playerPosition.x < skeletonPosition.x &&
                    skeletonPosition.x > mSkeletonLeftBoundaries[i]) {
                    skeleton->moveLeft();
                } else if (playerPosition.x > skeletonPosition.x &&
                        skeletonPosition.x < mSkeletonRightBoundaries[i]) {
                    skeleton->moveRight();
                } else {
                    skeleton->resetMovement();
                    skeleton->setDirection(DOWN);
                }
            } else {
                skeleton->resetMovement();
                skeleton->setDirection(DOWN);
            }

            skeleton->update(
                deltaTime,
                mGameState.player,
                mGameState.map,
                nullptr,
                0
            );
        } else if (skeleton->isActive()) {
            mSkeletonDefeatTime[i] += deltaTime;

            skeleton->resetMovement();
            skeleton->setVelocity({ 0.0f, 0.0f });
            skeleton->setAcceleration({ 0.0f, 0.0f });

            skeleton->update(
                deltaTime,
                mGameState.player,
                nullptr,
                nullptr,
                0
            );

            if (mSkeletonDefeatTime[i] >= 8.0f / 8.0f) {
                skeleton->deactivate();
            }
        }
    }

    if (mInvincibilityTime <= 0.0f && *mHearts > 0) {
        Vector2 playerPosition = mGameState.player->getPosition();
        Vector2 playerSize = mGameState.player->getColliderDimensions();

        bool hitEnemy = false;

        for (int i = 0; i < PUMPKIN_COUNT; i++) {
            if (mPumpkinDefeated[i] || !mPumpkins[i]->isActive()) {
                continue;
            }

            Vector2 enemyPosition = mPumpkins[i]->getPosition();
            Vector2 enemySize = mPumpkins[i]->getColliderDimensions();

            if (fabsf(playerPosition.x - enemyPosition.x) <
                    (playerSize.x + enemySize.x) / 2.0f &&
                fabsf(playerPosition.y - enemyPosition.y) <
                    (playerSize.y + enemySize.y) / 2.0f) {
                hitEnemy = true;
                break;
            }
        }

        if (!hitEnemy) {
            for (int i = 0; i < SKELETON_COUNT; i++) {
                if (mSkeletonDefeated[i] || !mSkeletons[i]->isActive()) {
                    continue;
                }

                Vector2 enemyPosition = mSkeletons[i]->getPosition();
                Vector2 enemySize = mSkeletons[i]->getColliderDimensions();

                if (fabsf(playerPosition.x - enemyPosition.x) <
                        (playerSize.x + enemySize.x) / 2.0f &&
                    fabsf(playerPosition.y - enemyPosition.y) <
                        (playerSize.y + enemySize.y) / 2.0f) {
                    hitEnemy = true;
                    break;
                }
            }
        }

        if (hitEnemy) {
            (*mHearts)--;
            mInvincibilityTime = INVINCIBILITY_DURATION;
            mGameState.player->hurt();
        }
    }

    if (*mHearts <= 0) {
        mGameState.nextSceneID = LOSE;
        return;
    }

    for (int i = 0; i < mSnowballs.size(); i++) {
        if (!mSnowballImpact[i]) {
            Vector2 position = mSnowballs[i]->getPosition();

            position.x += mSnowballSpeed * mSnowballDirections[i] * deltaTime;

            mSnowballs[i]->setPosition(position);

            float xOverlap = 0.0f;
            float yOverlap = 0.0f;
            
            bool hitWall = mGameState.map->isSolidTileAt(
                position, &xOverlap, &yOverlap
            );

            int hitPumpkinIndex = -1;

            for (int j = 0; j < PUMPKIN_COUNT; j++) {
                if (mPumpkinDefeated[j] || !mPumpkins[j]->isActive()) {
                    continue;
                }

                Vector2 pumpkinPosition = mPumpkins[j]->getPosition();
                Vector2 pumpkinSize = mPumpkins[j]->getColliderDimensions();

                if (fabs(position.x - pumpkinPosition.x) < (pumpkinSize.x + 20.0f) / 2.0f &&
                    fabs(position.y - pumpkinPosition.y) < (pumpkinSize.y + 20.0f) / 2.0f) {
                    hitPumpkinIndex = j;
                    break;
                }
            }

            int hitSkeletonIndex = -1;

            for (int j = 0; j < SKELETON_COUNT; j++) {
                if (mSkeletonDefeated[j] || !mSkeletons[j]->isActive()) {
                    continue;
                }

                Vector2 skeletonPosition = mSkeletons[j]->getPosition();
                Vector2 skeletonSize = mSkeletons[j]->getColliderDimensions();

                if (fabsf(position.x - skeletonPosition.x) < (skeletonSize.x + 20.0f) / 2.0f &&
                    fabsf(position.y - skeletonPosition.y) < (skeletonSize.y + 20.0f) / 2.0f) {
                    hitSkeletonIndex = j;
                    break;
                }
            }

            if (hitWall || hitPumpkinIndex != -1 || hitSkeletonIndex != -1) {
                mSnowballImpact[i] = true;
                mSnowballImpactTime[i] = 0.0f;

                mSnowballs[i]->setDirection(UP);
                PlaySound(mSnowballImpactSound);

                if (hitPumpkinIndex != -1) {
                    Entity *pumpkin = mPumpkins[hitPumpkinIndex];

                    mPumpkinDefeated[hitPumpkinIndex] = true;
                    mPumpkinDefeatTime[hitPumpkinIndex] = 0.0f;

                    pumpkin->resetMovement();
                    pumpkin->setVelocity({ 0.0f, 0.0f });
                    pumpkin->setDirection(UP);
                }

                if (hitSkeletonIndex != -1) {
                    Entity *skeleton = mSkeletons[hitSkeletonIndex];

                    mSkeletonDefeated[hitSkeletonIndex] = true;
                    mSkeletonDefeatTime[hitSkeletonIndex] = 0.0f;

                    skeleton->resetMovement();
                    skeleton->setVelocity({ 0.0f, 0.0f });
                    skeleton->setDirection(UP);
                }
            }
        } else {
            mSnowballImpactTime[i] += deltaTime;

            if (mSnowballImpactTime[i] >= 8.0f / 14.0f) {
                delete mSnowballs[i];

                mSnowballs.erase(mSnowballs.begin() + i);
                mSnowballDirections.erase(mSnowballDirections.begin() + i);
                mSnowballImpact.erase(mSnowballImpact.begin() + i);
                mSnowballImpactTime.erase(mSnowballImpactTime.begin() + i);

                i--;
                continue;
            }
        }

        mSnowballs[i]->update(
            deltaTime,
            nullptr,
            nullptr,
            nullptr,
            0
        );
    }
}

void Level1::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    for (int i = -1; i < 3; i++) {
        DrawTexturePro(
            mBackgroundTexture,
            {
                0.0f,
                0.0f,
                (float)mBackgroundTexture.width,
                (float)mBackgroundTexture.height
            },
            {
                mOrigin.x - 450.0f,
                mOrigin.y - 500.0f,
                900.0f,
                1000.0f
            },
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );      
    }

    mGameState.map->render();

    if (mHalloweenKey->isActive()) {
        mHalloweenKey->render();
    }
    
    mGameState.player->render();

    for (int i = 0; i < PUMPKIN_COUNT; i++) {
        if (mPumpkins[i]->isActive()) {
            mPumpkins[i]->render();
        }
    }

    for (int i = 0; i < SKELETON_COUNT; i++) {
        if (mSkeletons[i]->isActive()) {
            mSkeletons[i]->render();
        }
    }

    for (int i = 0; i < mSnowballs.size(); i++) {
        mSnowballs[i]->render();
    }
}

void Level1::renderHearts() {
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

void Level1::setHearts(int *hearts) {
    mHearts = hearts;
}

void Level1::shutdown()
{
    delete mGameState.player;
    mGameState.player = nullptr;
    delete mGameState.map;
    mGameState.map = nullptr;

    for (int i = 0; i < mSnowballs.size(); i++) {
        delete mSnowballs[i];
    }

    mSnowballs.clear();
    mSnowballDirections.clear();

    mSnowballImpact.clear();
    mSnowballImpactTime.clear();

    for (int i = 0; i < mPumpkins.size(); i++) {
        delete mPumpkins[i];
    }

    mPumpkins.clear();

    for (int i = 0; i < mSkeletons.size(); i++) {
        delete mSkeletons[i];
    }

    mSkeletons.clear();

    delete mHalloweenKey;
    mHalloweenKey = nullptr;

    UnloadSound(mItemPickupSound);
    mItemPickupSound = {};

    UnloadSound(mSnowballImpactSound);
    mSnowballImpactSound = {};

    UnloadTexture(mBackgroundTexture);
    mBackgroundTexture = {};

    UnloadMusicStream(mGameState.bgm);
    mGameState.bgm = {};

    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};

    UnloadTexture(mHeartsTexture);
    mHeartsTexture = {};
}

void Level1::setHalloweenKeyCollected(bool *collected) {
    mHalloweenKeyCollected = collected;
}
