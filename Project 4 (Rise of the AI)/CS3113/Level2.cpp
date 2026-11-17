#include "Level2.h"

/**
* Author: Jasmine Zhang
* Assignment: Rise of the AI
* Date due: 11/16/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

Level2::Level2()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

Level2::Level2(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

Level2::~Level2() { shutdown(); }

void Level2::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mBackgroundTexture = LoadTexture("assets/level2/background.png");
    mHeartsTexture = LoadTexture("assets/hearts.png");

    mInvincibilityTime = 0.0f;

    /*
    "Canon in D for Two Harps" Kevin MacLeod (incompetech.com)
    Licensed under Creative Commons: By Attribution 4.0 License
    http://creativecommons.org/licenses/by/4.0/
    */
    mGameState.bgm = LoadMusicStream("assets/level2/sounds/level2_music.mp3");
    SetMusicVolume(mGameState.bgm, 1.0f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("assets/jump.mp3");
    mSnowballImpactSound = LoadSound("assets/snowball_impact.mp3");

    mGameState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT, 
        mLevelData,             
        "assets/level2/tiles.png",
        TILE_DIMENSION,            
        4, 1,                      
        mOrigin                  
    );
    
    std::map<Direction, std::vector<int>> playerAnimationAtlas = {
        { DOWN,  {  0,  1,  2,  3,  4,  5,  6,  7 } },
        { LEFT,  {  8,  9, 10, 11, 12, 13, 14, 15 } },
        { UP,    { 16, 17, 18, 19 } },
        { RIGHT, { 8, 9, 10, 11, 12, 13, 14, 15 } },
    };

    mGameState.player = new Entity(
        { mOrigin.x - 300.0f, mOrigin.y + 320.0f }, 
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
    mGameState.player->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

    std::map<Direction, std::vector<int>> turkeyAnimationAtlas = {
        { DOWN, { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { LEFT, { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { RIGHT, { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { UP, { 16, 17, 18, 19, 20, 21, 22, 23 } }
    };

    for (int i = 0; i < TURKEY_COUNT; i++) {
        Entity *turkey = new Entity(
            { mTurkeyStartX[i], mTurkeyStartY[i] },
            { 60.0f, 60.0f },
            "assets/level2/turkey.png",
            ATLAS,
            { 8, 3 },
            turkeyAnimationAtlas,
            NPC
        );

        turkey->setSpeed(100);
        turkey->setFrameSpeed(8);
        turkey->setColliderDimensions({ 40.0f, 40.0f });
        turkey->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
        turkey->setAIState(IDLE);
        turkey->setAIType(PATROLLER);

        mTurkeys.push_back(turkey);

        mTurkeyMovingLeft[i] = true;
        mTurkeyDefeated[i] = false;
        mTurkeyDefeatTime[i] = 0.0f;
    }

    std::map<Direction, std::vector<int>> cornucopiaAnimationAtlas = {
        { DOWN,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { LEFT,  { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { RIGHT, { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { UP,    { 16, 17, 18, 19, 20, 21, 22, 23 } }
    };

    for (int i = 0; i < CORNUCOPIA_COUNT; i++) {
        Entity *cornucopia = new Entity(
            { mCornucopiaStartX[i], mCornucopiaStartY[i] },
            { 60.0f, 60.0f },
            "assets/level2/cornucopia.png",
            ATLAS,
            { 8, 3 },
            cornucopiaAnimationAtlas,
            NPC
        );

        cornucopia->setSpeed(120);
        cornucopia->setFrameSpeed(8);
        cornucopia->setColliderDimensions({ 40.0f, 40.0f });
        cornucopia->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

        cornucopia->setAIType(PATROLLER);
        cornucopia->setAIState(IDLE);
        cornucopia->setDirection(DOWN);

        mCornucopias.push_back(cornucopia);

        mCornucopiaDefeated[i] = false;
        mCornucopiaDefeatTime[i] = 0.0f;
    }

    std::map<Direction, std::vector<int>> leafAnimationAtlas = {
        { DOWN,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { UP,    { 8, 9, 10, 11, 12, 13, 14, 15 } }
    };

    for (int i = 0; i < LEAF_COUNT; i++) {
        Entity *leaf = new Entity(
            { mLeafStartX[i], mLeafStartY[i] },
            { 50.0f, 50.0f },
            "assets/level2/leaf.png",
            ATLAS,
            { 8, 2 },
            leafAnimationAtlas,
            NONE
        );

        leaf->setDirection(DOWN);
        leaf->setFrameSpeed(8);
        leaf->setColliderDimensions({ 35.0f, 35.0f });

        mLeaves.push_back(leaf);

        mLeafDefeated[i] = false;
        mLeafDefeatTime[i] = 0.0f;
    }

    mSnowballCooldown = 0.0f;

    std::map<Direction, std::vector<int>> keyAnimationAtlas = {
        { DOWN,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { UP,    { 0, 1, 2, 3, 4, 5, 6, 7 } }
    };

    mThanksgivingKey = new Entity(
        { 325.0f, 550.0f },
        { 50.0f, 100.0f },
        "assets/level2/key.png",
        ATLAS,
        { 8, 1 },
        keyAnimationAtlas,
        NONE
    );

    mThanksgivingKey->setDirection(DOWN);
    mThanksgivingKey->setFrameSpeed(6);
    mThanksgivingKey->setColliderDimensions({ 35.0f, 35.0f });

    if (*mThanksgivingKeyCollected) {
        mThanksgivingKey->deactivate();
    }

    mItemPickupSound = LoadSound("assets/item_pickup.mp3");
}

void Level2::restart() {
    mGameState.player->setPosition({
        mOrigin.x - 300.0f,
        mOrigin.y + 320.0f
    });

    mGameState.player->resetMovement();
    mGameState.player->setVelocity({ 0.0f, 0.0f });

    for (int i = 0; i < TURKEY_COUNT; i++) {
        Entity *turkey = mTurkeys[i];

        turkey->setPosition({
            mTurkeyStartX[i],
            mTurkeyStartY[i]
        });

        turkey->resetMovement();
        turkey->setVelocity({ 0.0f, 0.0f });

        turkey->activate();
        turkey->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
        turkey->setDirection(LEFT);

        mTurkeyMovingLeft[i] = true;
        mTurkeyDefeated[i] = false;
        mTurkeyDefeatTime[i] = 0.0f;
    }

    for (int i = 0; i < CORNUCOPIA_COUNT; i++) {
        Entity *cornucopia = mCornucopias[i];

        cornucopia->setPosition({
            mCornucopiaStartX[i],
            mCornucopiaStartY[i]
        });

        cornucopia->resetMovement();
        cornucopia->setVelocity({ 0.0f, 0.0f });

        cornucopia->activate();
        cornucopia->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
        cornucopia->setDirection(DOWN);

        mCornucopiaDefeated[i] = false;
        mCornucopiaDefeatTime[i] = 0.0f;
    }

    for (int i = 0; i < LEAF_COUNT; i++) {
        Entity *leaf = mLeaves[i];

        leaf->setPosition({
            mLeafStartX[i],
            mLeafStartY[i]
        });

        leaf->resetMovement();
        leaf->setVelocity({ 0.0f, 0.0f });
        leaf->setDirection(DOWN);
        leaf->activate();

        mLeafDefeated[i] = false;
        mLeafDefeatTime[i] = 0.0f;
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

void Level2::throwSnowball() {
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

void Level2::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

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

    if (mGameState.player->getPosition().y > END_GAME_THRESHOLD) {
        if (*mHearts > 0) {
            (*mHearts)--;
        }

        if (*mHearts <= 0) {
            mGameState.nextSceneID = LOSE;
            return;
        }

        mGameState.player->setPosition({
            mOrigin.x - 300,
            mOrigin.y + 320
        });

        mGameState.player->resetMovement();
        mGameState.player->setVelocity({0.0f, 0.0f});

        mGameState.player->hurt();

        mInvincibilityTime = INVINCIBILITY_DURATION;
    }

    if (!*mThanksgivingKeyCollected && mThanksgivingKey->isActive()) {
        mThanksgivingKey->update(
            deltaTime,
            nullptr,
            nullptr,
            nullptr,
            0
        );

        Vector2 playerPosition = mGameState.player->getPosition();
        Vector2 keyPosition = mThanksgivingKey->getPosition();

        Vector2 playerSize = mGameState.player->getColliderDimensions();
        Vector2 keySize = mThanksgivingKey->getColliderDimensions();

        if (fabsf(playerPosition.x - keyPosition.x) <
                (playerSize.x + keySize.x) / 2.0f &&
            fabsf(playerPosition.y - keyPosition.y) <
                (playerSize.y + keySize.y) / 2.0f) {

            *mThanksgivingKeyCollected = true;
            mThanksgivingKey->deactivate();

            PlaySound(mItemPickupSound);
        }
    }

    for (int i = 0; i < TURKEY_COUNT; i++) {
        Entity *turkey = mTurkeys[i];

        if (!mTurkeyDefeated[i]) {
            float turkeyX = turkey->getPosition().x;

            if (turkeyX <= mTurkeyLeftBoundaries[i]) {
                mTurkeyMovingLeft[i] = false;
            } else if (turkeyX >= mTurkeyRightBoundaries[i]) {
                mTurkeyMovingLeft[i] = true;
            }

            if (mTurkeyMovingLeft[i]) {
                turkey->moveLeft();
            } else {
                turkey->moveRight();
            }

            turkey->update(
                deltaTime,
                mGameState.player,
                mGameState.map,
                nullptr,
                0
            );
        } else if (turkey->isActive()) {
            mTurkeyDefeatTime[i] += deltaTime;

            turkey->resetMovement();
            turkey->setVelocity({ 0.0f, 0.0f });
            turkey->setAcceleration({ 0.0f, 0.0f });

            turkey->update(
                deltaTime,
                mGameState.player,
                nullptr,
                nullptr,
                0
            );

            if (mTurkeyDefeatTime[i] >= 6.0f / 8.0f) {
                turkey->deactivate();
            }
        }
    }

    for (int i = 0; i < CORNUCOPIA_COUNT; i++) {
        Entity *cornucopia = mCornucopias[i];

        if (!mCornucopiaDefeated[i]) {
            Vector2 cornucopiaPosition = cornucopia->getPosition();
            Vector2 playerPosition = mGameState.player->getPosition();

            float distanceX = fabsf(playerPosition.x - cornucopiaPosition.x);

            bool chasePlayer = distanceX <= CORNUCOPIA_CHASE_RANGE;

            if (chasePlayer && distanceX > 25.0f) {
                if (playerPosition.x < cornucopiaPosition.x &&
                    cornucopiaPosition.x > mCornucopiaLeftBoundaries[i]) {
                    cornucopia->moveLeft();
                } else if (playerPosition.x > cornucopiaPosition.x &&
                        cornucopiaPosition.x < mCornucopiaRightBoundaries[i]) {
                    cornucopia->moveRight();
                } else {
                    cornucopia->resetMovement();
                    cornucopia->setDirection(DOWN);
                }
            } else {
                cornucopia->resetMovement();
                cornucopia->setDirection(DOWN);
            }

            cornucopia->update(
                deltaTime,
                mGameState.player,
                mGameState.map,
                nullptr,
                0
            );
        } else if (cornucopia->isActive()) {
            mCornucopiaDefeatTime[i] += deltaTime;

            cornucopia->resetMovement();
            cornucopia->setVelocity({ 0.0f, 0.0f });
            cornucopia->setAcceleration({ 0.0f, 0.0f });

            cornucopia->update(
                deltaTime,
                mGameState.player,
                nullptr,
                nullptr,
                0
            );

            if (mCornucopiaDefeatTime[i] >= 8.0f / 8.0f) {
                cornucopia->deactivate();
            }
        }
    }

    for (int i = 0; i < LEAF_COUNT; i++) {
        Entity *leaf = mLeaves[i];

        if (!mLeafDefeated[i] && leaf->isActive()) {
            Vector2 leafPosition = leaf->getPosition();
            Vector2 playerPosition = mGameState.player->getPosition();

            float movement = mLeafSpeed * deltaTime;

            if (leafPosition.x < playerPosition.x) {
                leafPosition.x += movement;

                if (leafPosition.x > playerPosition.x) {
                    leafPosition.x = playerPosition.x;
                }
            }
            else if (leafPosition.x > playerPosition.x) {
                leafPosition.x -= movement;

                if (leafPosition.x < playerPosition.x) {
                    leafPosition.x = playerPosition.x;
                }
            }

            if (leafPosition.y < playerPosition.y) {
                leafPosition.y += movement;

                if (leafPosition.y > playerPosition.y) {
                    leafPosition.y = playerPosition.y;
                }
            }
            else if (leafPosition.y > playerPosition.y) {
                leafPosition.y -= movement;

                if (leafPosition.y < playerPosition.y) {
                    leafPosition.y = playerPosition.y;
                }
            }

            leaf->setPosition(leafPosition);

            leaf->update(
                deltaTime,
                nullptr,
                nullptr,
                nullptr,
                0
            );
        }
        else if (mLeafDefeated[i] && leaf->isActive()) {
            mLeafDefeatTime[i] += deltaTime;

            leaf->update(
                deltaTime,
                nullptr,
                nullptr,
                nullptr,
                0
            );

            if (mLeafDefeatTime[i] >= 8.0f / 8.0f) {
                leaf->deactivate();
            }
        }
    }

    if (mInvincibilityTime <= 0.0f && *mHearts > 0) {
        Vector2 playerPosition = mGameState.player->getPosition();
        Vector2 playerSize = mGameState.player->getColliderDimensions();

        bool hitEnemy = false;

        for (int i = 0; i < TURKEY_COUNT; i++) {
            if (mTurkeyDefeated[i] || !mTurkeys[i]->isActive()) {
                continue;
            }

            Vector2 enemyPosition = mTurkeys[i]->getPosition();
            Vector2 enemySize = mTurkeys[i]->getColliderDimensions();

            if (fabsf(playerPosition.x - enemyPosition.x) <
                    (playerSize.x + enemySize.x) / 2.0f &&
                fabsf(playerPosition.y - enemyPosition.y) <
                    (playerSize.y + enemySize.y) / 2.0f) {
                hitEnemy = true;
                break;
            }
        }

        if (!hitEnemy) {
            for (int i = 0; i < CORNUCOPIA_COUNT; i++) {
                if (mCornucopiaDefeated[i] || !mCornucopias[i]->isActive()) {
                    continue;
                }

                Vector2 enemyPosition = mCornucopias[i]->getPosition();
                Vector2 enemySize = mCornucopias[i]->getColliderDimensions();

                if (fabsf(playerPosition.x - enemyPosition.x) <
                        (playerSize.x + enemySize.x) / 2.0f &&
                    fabsf(playerPosition.y - enemyPosition.y) <
                        (playerSize.y + enemySize.y) / 2.0f) {
                    hitEnemy = true;
                    break;
                }
            }
        }

        if (!hitEnemy) {
            for (int i = 0; i < LEAF_COUNT; i++) {
                if (mLeafDefeated[i] || !mLeaves[i]->isActive()) {
                    continue;
                }

                Vector2 leafPosition = mLeaves[i]->getPosition();
                Vector2 leafSize = mLeaves[i]->getColliderDimensions();

                if (fabsf(playerPosition.x - leafPosition.x) <
                        (playerSize.x + leafSize.x) / 2.0f &&
                    fabsf(playerPosition.y - leafPosition.y) <
                        (playerSize.y + leafSize.y) / 2.0f) {

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

            int hitTurkeyIndex = -1;

            for (int j = 0; j < TURKEY_COUNT; j++) {
                if (mTurkeyDefeated[j] || !mTurkeys[j]->isActive()) {
                    continue;
                }

                Vector2 turkeyPosition = mTurkeys[j]->getPosition();
                Vector2 turkeySize = mTurkeys[j]->getColliderDimensions();

                if (fabs(position.x - turkeyPosition.x) < (turkeySize.x + 20.0f) / 2.0f &&
                    fabs(position.y - turkeyPosition.y) < (turkeySize.y + 20.0f) / 2.0f) {
                    hitTurkeyIndex = j;
                    break;
                }
            }

            int hitCornucopiaIndex = -1;

            for (int j = 0; j < CORNUCOPIA_COUNT; j++) {
                if (mCornucopiaDefeated[j] || !mCornucopias[j]->isActive()) {
                    continue;
                }

                Vector2 cornucopiaPosition = mCornucopias[j]->getPosition();
                Vector2 cornucopiaSize = mCornucopias[j]->getColliderDimensions();

                if (fabsf(position.x - cornucopiaPosition.x) < (cornucopiaSize.x + 20.0f) / 2.0f &&
                    fabsf(position.y - cornucopiaPosition.y) < (cornucopiaSize.y + 20.0f) / 2.0f) {
                    hitCornucopiaIndex = j;
                    break;
                }
            }

    int hitLeafIndex = -1;

    for (int j = 0; j < LEAF_COUNT; j++) {
        if (mLeafDefeated[j] || !mLeaves[j]->isActive()) {
            continue;
        }

        Vector2 leafPosition = mLeaves[j]->getPosition();
        Vector2 leafSize = mLeaves[j]->getColliderDimensions();

        if (fabsf(position.x - leafPosition.x) <
                (leafSize.x + 20.0f) / 2.0f &&
            fabsf(position.y - leafPosition.y) <
                (leafSize.y + 20.0f) / 2.0f) {

            hitLeafIndex = j;
            break;
        }
    }

            if (hitWall || hitTurkeyIndex != -1 || hitCornucopiaIndex != -1 || hitLeafIndex != -1) {
                mSnowballImpact[i] = true;
                mSnowballImpactTime[i] = 0.0f;

                mSnowballs[i]->setDirection(UP);
                PlaySound(mSnowballImpactSound);

                if (hitTurkeyIndex != -1) {
                    Entity *turkey = mTurkeys[hitTurkeyIndex];

                    mTurkeyDefeated[hitTurkeyIndex] = true;
                    mTurkeyDefeatTime[hitTurkeyIndex] = 0.0f;

                    turkey->resetMovement();
                    turkey->setVelocity({ 0.0f, 0.0f });
                    turkey->setDirection(UP);
                }

                if (hitCornucopiaIndex != -1) {
                    Entity *cornucopia = mCornucopias[hitCornucopiaIndex];

                    mCornucopiaDefeated[hitCornucopiaIndex] = true;
                    mCornucopiaDefeatTime[hitCornucopiaIndex] = 0.0f;

                    cornucopia->resetMovement();
                    cornucopia->setVelocity({ 0.0f, 0.0f });
                    cornucopia->setDirection(UP);
                }

                if (hitLeafIndex != -1) {
                    Entity *leaf = mLeaves[hitLeafIndex];

                    mLeafDefeated[hitLeafIndex] = true;
                    mLeafDefeatTime[hitLeafIndex] = 0.0f;

                    leaf->resetMovement();
                    leaf->setVelocity({ 0.0f, 0.0f });
                    leaf->setDirection(UP);
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

void Level2::render() {
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

    if (mThanksgivingKey->isActive()) {
        mThanksgivingKey->render();
    }

    mGameState.player->render();

    for (int i = 0; i < TURKEY_COUNT; i++) {
        if (mTurkeys[i]->isActive()) {
            mTurkeys[i]->render();
        }
    }

    for (int i = 0; i < CORNUCOPIA_COUNT; i++) {
        if (mCornucopias[i]->isActive()) {
            mCornucopias[i]->render();
        }
    }

    for (int i = 0; i < LEAF_COUNT; i++) {
        if (mLeaves[i]->isActive()) {
            mLeaves[i]->render();
        }
    }

    for (int i = 0; i < mSnowballs.size(); i++) {
        mSnowballs[i]->render();
    }
}

void Level2::renderHearts() {
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

void Level2::setHearts(int *hearts) {
    mHearts = hearts;
}

void Level2::shutdown() {
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

    for (int i = 0; i < mTurkeys.size(); i++) {
        delete mTurkeys[i];
    }

    mTurkeys.clear();

    for (int i = 0; i < mCornucopias.size(); i++) {
        delete mCornucopias[i];
    }

    mCornucopias.clear();

    for (int i = 0; i < mLeaves.size(); i++) {
        delete mLeaves[i];
    }

    mLeaves.clear();

    delete mThanksgivingKey;
    mThanksgivingKey = nullptr;

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

void Level2::setThanksgivingKeyCollected(bool *collected) {
    mThanksgivingKeyCollected = collected;
}
