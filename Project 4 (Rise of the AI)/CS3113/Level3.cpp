#include "Level3.h"

/**
* Author: Jasmine Zhang
* Assignment: Rise of the AI
* Date due: 11/16/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

Level3::Level3()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

Level3::Level3(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

Level3::~Level3() { shutdown(); }

void Level3::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mBackgroundTexture = LoadTexture("assets/level3/background.png");
    mHeartsTexture = LoadTexture("assets/hearts.png");

    mInvincibilityTime = 0.0f;
    
    /*
    "Deck the Halls B" Kevin MacLeod (incompetech.com)
    Licensed under Creative Commons: By Attribution 4.0 License
    http://creativecommons.org/licenses/by/4.0/
    */
    mGameState.bgm = LoadMusicStream("assets/level3/sounds/level3_music.mp3");
    SetMusicVolume(mGameState.bgm, 0.30f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("assets/jump.mp3");
    mSnowballImpactSound = LoadSound("assets/snowball_impact.mp3");

    mDoorSound = LoadSound("assets/level3/sounds/door.wav");

    mGameState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT, 
        mLevelData,              
        "assets/level3/tiles.png", 
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

    std::map<Direction, std::vector<int>> gingerbreadAnimationAtlas = {
        { DOWN, { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { LEFT, { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { RIGHT, {  8, 9, 10, 11, 12, 13, 14, 15 } },
        { UP, { 16, 17, 18, 19, 20, 21, 22, 23 } }
    };

    for (int i = 0; i < GINGERBREAD_COUNT; i++) {
        Entity *gingerbread = new Entity(
            { mGingerbreadStartX[i], mGingerbreadStartY[i] },
            { 60.0f, 60.0f },
            "assets/level3/gingerbread.png",
            ATLAS,
            { 8, 3 },
            gingerbreadAnimationAtlas,
            NPC
        );

        gingerbread->setSpeed(100);
        gingerbread->setFrameSpeed(8);
        gingerbread->setColliderDimensions({ 40.0f, 40.0f });
        gingerbread->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
        gingerbread->setAIState(IDLE);
        gingerbread->setAIType(PATROLLER);

        mGingerbreads.push_back(gingerbread);

        mGingerbreadMovingLeft[i] = true;
        mGingerbreadDefeated[i] = false;
        mGingerbreadDefeatTime[i] = 0.0f;
    }

    std::map<Direction, std::vector<int>> snowmanAnimationAtlas = {
        { DOWN,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { LEFT,  { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { RIGHT, { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { UP,    { 16, 17, 18, 19, 20, 21, 22, 23 } }
    };

    for (int i = 0; i < SNOWMAN_COUNT; i++) {
        Entity *snowman = new Entity(
            { mSnowmanStartX[i], mSnowmanStartY[i] },
            { 60.0f, 60.0f },
            "assets/level3/snowman.png",
            ATLAS,
            { 8, 3 },
            snowmanAnimationAtlas,
            NPC
        );

        snowman->setSpeed(120);
        snowman->setFrameSpeed(8);
        snowman->setColliderDimensions({ 40.0f, 40.0f });
        snowman->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

        snowman->setAIType(PATROLLER);
        snowman->setAIState(IDLE);
        snowman->setDirection(DOWN);

        mSnowmans.push_back(snowman);

        mSnowmanDefeated[i] = false;
        mSnowmanDefeatTime[i] = 0.0f;
        mSnowmanAttackCooldown[i] = 0.0f;
        mSnowmanAttackTime[i] = 0.0f;
        mSnowmanAttacking[i] = false;
    }

    mSnowballCooldown = 0.0f;

    std::map<Direction, std::vector<int>> keyAnimationAtlas = {
        { DOWN,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { UP,    { 0, 1, 2, 3, 4, 5, 6, 7 } }
    };

    mWinterKey = new Entity(
        { 675.0f , 400.0f },
        { 50.0f, 100.0f },
        "assets/level3/key.png",
        ATLAS,
        { 8, 1 },
        keyAnimationAtlas,
        NONE
    );

    mWinterKey->setDirection(DOWN);
    mWinterKey->setFrameSpeed(6);
    mWinterKey->setColliderDimensions({ 35.0f, 35.0f });

    if (*mWinterKeyCollected) {
        mWinterKey->deactivate();
    }

    std::map<Direction, std::vector<int>> doorAnimationAtlas = {
        { DOWN,  { 0 } },
        { RIGHT, { 8, 9, 10, 11, 12, 13, 14, 15,
                16, 17, 18, 19, 20, 21, 22, 23 } }
    };

    mFinalDoor = new Entity(
        { 525.0f, -25.0f },
        { 150.0f, 150.0f },
        "assets/level3/final_level_door.png",
        ATLAS,
        { 8, 3 },
        doorAnimationAtlas,
        NONE
    );

    mFinalDoor->setDirection(DOWN);
    mFinalDoor->setFrameSpeed(8);
    mFinalDoor->setColliderDimensions({ 100.0f, 120.0f });

    mDoorOpening = false;
    mDoorOpeningTime = 0.0f;

    mDoorMessageTexture = LoadTexture("assets/level3/door_message.png");
    mShowDoorMessage = false;
    mDoorMessageTime = 0.0f;

    mItemPickupSound = LoadSound("assets/item_pickup.mp3");
}

void Level3::restart() {
    mGameState.player->setPosition({
        mOrigin.x - 300.0f,
        mOrigin.y + 320.0f
    });

    mGameState.player->resetMovement();
    mGameState.player->setVelocity({ 0.0f, 0.0f });

    for (int i = 0; i < GINGERBREAD_COUNT; i++) {
        Entity *gingerbread = mGingerbreads[i];

        gingerbread->setPosition({
            mGingerbreadStartX[i],
            mGingerbreadStartY[i]
        });

        gingerbread->resetMovement();
        gingerbread->setVelocity({ 0.0f, 0.0f });

        gingerbread->activate();
        gingerbread->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
        gingerbread->setDirection(LEFT);

        mGingerbreadMovingLeft[i] = true;
        mGingerbreadDefeated[i] = false;
        mGingerbreadDefeatTime[i] = 0.0f;
    }

    for (int i = 0; i < SNOWMAN_COUNT; i++) {
        Entity *snowman = mSnowmans[i];

        snowman->setPosition({
            mSnowmanStartX[i],
            mSnowmanStartY[i]
        });

        snowman->resetMovement();
        snowman->setVelocity({ 0.0f, 0.0f });

        snowman->activate();
        snowman->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
        snowman->setDirection(DOWN);

        mSnowmanDefeated[i] = false;
        mSnowmanDefeatTime[i] = 0.0f;
        mSnowmanAttackCooldown[i] = 0.0f;
        mSnowmanAttackTime[i] = 0.0f;
        mSnowmanAttacking[i] = false;
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

    for (int i = 0; i < mEnemySnowballs.size(); i++) {
        delete mEnemySnowballs[i];
    }

    mEnemySnowballs.clear();
    mEnemySnowballDirections.clear();
    mEnemySnowballImpact.clear();
    mEnemySnowballImpactTime.clear();

    mShowDoorMessage = false;
    mDoorMessageTime = 0.0f;
    mDoorOpening = false;
    mDoorOpeningTime = 0.0f;
    mFinalDoor->setDirection(DOWN);
}

void Level3::throwSnowball() {
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

void Level3::throwEnemySnowball(int snowmanIndex) {
    Entity *snowman = mSnowmans[snowmanIndex];

    Vector2 snowmanPosition = snowman->getPosition();

    float direction = 1.0f;

    if (snowman->isFacingLeft()) {
        direction = -1.0f;
    }

    std::map<Direction, std::vector<int>> snowballAnimationAtlas = {
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { UP,    { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { DOWN,  { 0, 1, 2, 3, 4, 5, 6, 7 } }
    };

    Entity *snowball = new Entity(
        {
            snowmanPosition.x + 45.0f * direction,
            snowmanPosition.y
        },
        { 30.0f, 30.0f },
        "assets/snowball.png",
        ATLAS,
        { 8, 2 },
        snowballAnimationAtlas,
        NONE
    );

    snowball->setFrameSpeed(14);
    snowball->setFacingLeft(direction < 0.0f);

    mEnemySnowballs.push_back(snowball);
    mEnemySnowballDirections.push_back(direction);
    mEnemySnowballImpact.push_back(false);
    mEnemySnowballImpactTime.push_back(0.0f);
}

void Level3::interactWithDoor()
{
    if (mShowDoorMessage || mDoorOpening) return;

    Vector2 playerPosition = mGameState.player->getPosition();
    Vector2 doorPosition = mFinalDoor->getPosition();

    if (fabsf(playerPosition.x - doorPosition.x) > 100.0f ||
        fabsf(playerPosition.y - doorPosition.y) > 120.0f) {
        return;
    }

    bool hasAllKeys =
        *mHalloweenKeyCollected &&
        *mThanksgivingKeyCollected &&
        *mWinterKeyCollected;

    if (!hasAllKeys) {
        mShowDoorMessage = true;
        mDoorMessageTime = 0.0f;
    } else {
        mDoorOpening = true;
        mDoorOpeningTime = 0.0f;

        mFinalDoor->setDirection(RIGHT);
        PlaySound(mDoorSound);
    }
}

void Level3::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    if (mShowDoorMessage) {
        mDoorMessageTime += deltaTime;

        if (mDoorMessageTime >= 2.0f) {
            mShowDoorMessage = false;
            mDoorMessageTime = 0.0f;
        }

        return;
    }

    if (mDoorOpening) {
        mDoorOpeningTime += deltaTime;

        mFinalDoor->update(
            deltaTime,
            nullptr,
            nullptr,
            nullptr,
            0
        );

        if (mDoorOpeningTime >= 2.0f) {
            mGameState.nextSceneID = BOSS;
        }

        return;
    }

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
            mOrigin.x - 300.0f,
            mOrigin.y + 320.0f
        });

        mGameState.player->resetMovement();
        mGameState.player->setVelocity({0.0f, 0.0f});

        mGameState.player->hurt();

        mInvincibilityTime = INVINCIBILITY_DURATION;
    }

    if (!*mWinterKeyCollected && mWinterKey->isActive()) {
        mWinterKey->update(
            deltaTime,
            nullptr,
            nullptr,
            nullptr,
            0
        );

        Vector2 playerPosition = mGameState.player->getPosition();
        Vector2 keyPosition = mWinterKey->getPosition();

        Vector2 playerSize = mGameState.player->getColliderDimensions();
        Vector2 keySize = mWinterKey->getColliderDimensions();

        if (fabsf(playerPosition.x - keyPosition.x) <
                (playerSize.x + keySize.x) / 2.0f &&
            fabsf(playerPosition.y - keyPosition.y) <
                (playerSize.y + keySize.y) / 2.0f) {

            *mWinterKeyCollected = true;
            mWinterKey->deactivate();

            PlaySound(mItemPickupSound);
        }
    }

    for (int i = 0; i < GINGERBREAD_COUNT; i++) {
        Entity *gingerbread = mGingerbreads[i];

        if (!mGingerbreadDefeated[i]) {
            float gingerbreadX = gingerbread->getPosition().x;

            if (gingerbreadX <= mGingerbreadLeftBoundaries[i]) {
                mGingerbreadMovingLeft[i] = false;
            } else if (gingerbreadX >= mGingerbreadRightBoundaries[i]) {
                mGingerbreadMovingLeft[i] = true;
            }

            if (mGingerbreadMovingLeft[i]) {
                gingerbread->moveLeft();
            } else {
                gingerbread->moveRight();
            }

            gingerbread->update(
                deltaTime,
                mGameState.player,
                mGameState.map,
                nullptr,
                0
            );
        } else if (gingerbread->isActive()) {
            mGingerbreadDefeatTime[i] += deltaTime;

            gingerbread->resetMovement();
            gingerbread->setVelocity({ 0.0f, 0.0f });
            gingerbread->setAcceleration({ 0.0f, 0.0f });

            gingerbread->update(
                deltaTime,
                mGameState.player,
                nullptr,
                nullptr,
                0
            );

            if (mGingerbreadDefeatTime[i] >= 8.0f / 8.0f) {
                gingerbread->deactivate();
            }
        }
    }

    for (int i = 0; i < SNOWMAN_COUNT; i++) {
        Entity *snowman = mSnowmans[i];

        if (!mSnowmanDefeated[i]) {
            Vector2 snowmanPosition = snowman->getPosition();
            Vector2 playerPosition = mGameState.player->getPosition();

            float distanceX = fabsf(playerPosition.x - snowmanPosition.x);
            bool chasePlayer = distanceX <= SNOWMAN_CHASE_RANGE;

            if (mSnowmanAttackCooldown[i] > 0.0f) {
                mSnowmanAttackCooldown[i] -= deltaTime;
            }

            if (mSnowmanAttacking[i]) {
                mSnowmanAttackTime[i] += deltaTime;

                snowman->resetMovement();
                snowman->setVelocity({ 0.0f, 0.0f });

                if (mSnowmanAttackTime[i] >= 0.4f &&
                    mSnowmanAttackTime[i] - deltaTime < 0.4f) {
                    throwEnemySnowball(i);
                }

                if (mSnowmanAttackTime[i] >= SNOWMAN_ATTACK_DURATION) {
                    mSnowmanAttacking[i] = false;
                    mSnowmanAttackTime[i] = 0.0f;
                    mSnowmanAttackCooldown[i] = SNOWMAN_ATTACK_COOLDOWN;
                    snowman->setDirection(DOWN);
                }
            } else if (chasePlayer && mSnowmanAttackCooldown[i] <= 0.0f) {
                mSnowmanAttacking[i] = true;
                mSnowmanAttackTime[i] = 0.0f;

                snowman->resetMovement();
                snowman->setVelocity({ 0.0f, 0.0f });

                if (playerPosition.x < snowmanPosition.x) {
                    snowman->setFacingLeft(true);
                    snowman->setDirection(LEFT);
                } else {
                    snowman->setFacingLeft(false);
                    snowman->setDirection(RIGHT);
                }
            } else if (chasePlayer && distanceX > 25.0f) {
                if (playerPosition.x < snowmanPosition.x &&
                    snowmanPosition.x > mSnowmanLeftBoundaries[i]) {
                    snowman->moveLeft();
                } else if (playerPosition.x > snowmanPosition.x &&
                        snowmanPosition.x < mSnowmanRightBoundaries[i]) {
                    snowman->moveRight();
                } else {
                    snowman->resetMovement();
                    snowman->setDirection(DOWN);
                }
            } else {
                snowman->resetMovement();
                snowman->setDirection(DOWN);
            }

            snowman->update(
                deltaTime,
                mGameState.player,
                mGameState.map,
                nullptr,
                0
            );

        } else if (snowman->isActive()) {
            mSnowmanDefeatTime[i] += deltaTime;

            snowman->resetMovement();
            snowman->setVelocity({ 0.0f, 0.0f });
            snowman->setAcceleration({ 0.0f, 0.0f });

            snowman->update(
                deltaTime,
                mGameState.player,
                nullptr,
                nullptr,
                0
            );

            if (mSnowmanDefeatTime[i] >= 8.0f / 8.0f) {
                snowman->deactivate();
            }
        }
    }

    if (mInvincibilityTime <= 0.0f && *mHearts > 0) {
        Vector2 playerPosition = mGameState.player->getPosition();
        Vector2 playerSize = mGameState.player->getColliderDimensions();

        bool hitEnemy = false;

        for (int i = 0; i < GINGERBREAD_COUNT; i++) {
            if (mGingerbreadDefeated[i] || !mGingerbreads[i]->isActive()) {
                continue;
            }

            Vector2 enemyPosition = mGingerbreads[i]->getPosition();
            Vector2 enemySize = mGingerbreads[i]->getColliderDimensions();

            if (fabsf(playerPosition.x - enemyPosition.x) <
                    (playerSize.x + enemySize.x) / 2.0f &&
                fabsf(playerPosition.y - enemyPosition.y) <
                    (playerSize.y + enemySize.y) / 2.0f) {
                hitEnemy = true;
                break;
            }
        }

        if (!hitEnemy) {
            for (int i = 0; i < SNOWMAN_COUNT; i++) {
                if (mSnowmanDefeated[i] || !mSnowmans[i]->isActive()) {
                    continue;
                }

                Vector2 enemyPosition = mSnowmans[i]->getPosition();
                Vector2 enemySize = mSnowmans[i]->getColliderDimensions();

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

            int hitGingerbreadIndex = -1;

            for (int j = 0; j < GINGERBREAD_COUNT; j++) {
                if (mGingerbreadDefeated[j] || !mGingerbreads[j]->isActive()) {
                    continue;
                }

                Vector2 gingerbreadPosition = mGingerbreads[j]->getPosition();
                Vector2 gingerbreadSize = mGingerbreads[j]->getColliderDimensions();

                if (fabs(position.x - gingerbreadPosition.x) < (gingerbreadSize.x + 20.0f) / 2.0f &&
                    fabs(position.y - gingerbreadPosition.y) < (gingerbreadSize.y + 20.0f) / 2.0f) {
                    hitGingerbreadIndex = j;
                    break;
                }
            }

            int hitSnowmanIndex = -1;

            for (int j = 0; j < SNOWMAN_COUNT; j++) {
                if (mSnowmanDefeated[j] || !mSnowmans[j]->isActive()) {
                    continue;
                }

                Vector2 snowmanPosition = mSnowmans[j]->getPosition();
                Vector2 snowmanSize = mSnowmans[j]->getColliderDimensions();

                if (fabsf(position.x - snowmanPosition.x) < (snowmanSize.x + 20.0f) / 2.0f &&
                    fabsf(position.y - snowmanPosition.y) < (snowmanSize.y + 20.0f) / 2.0f) {
                    hitSnowmanIndex = j;
                    break;
                }
            }

            if (hitWall || hitGingerbreadIndex != -1 || hitSnowmanIndex != -1) {
                mSnowballImpact[i] = true;
                mSnowballImpactTime[i] = 0.0f;

                mSnowballs[i]->setDirection(UP);
                PlaySound(mSnowballImpactSound);

                if (hitGingerbreadIndex != -1) {
                    Entity *gingerbread = mGingerbreads[hitGingerbreadIndex];

                    mGingerbreadDefeated[hitGingerbreadIndex] = true;
                    mGingerbreadDefeatTime[hitGingerbreadIndex] = 0.0f;

                    gingerbread->resetMovement();
                    gingerbread->setVelocity({ 0.0f, 0.0f });
                    gingerbread->setDirection(UP);
                }

                if (hitSnowmanIndex != -1) {
                    Entity *snowman = mSnowmans[hitSnowmanIndex];

                    mSnowmanDefeated[hitSnowmanIndex] = true;
                    mSnowmanDefeatTime[hitSnowmanIndex] = 0.0f;

                    mSnowmanAttacking[hitSnowmanIndex] = false;
                    mSnowmanAttackTime[hitSnowmanIndex] = 0.0f;

                    snowman->resetMovement();
                    snowman->setVelocity({ 0.0f, 0.0f });
                    snowman->setDirection(UP);
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

    for (int i = 0; i < mEnemySnowballs.size(); i++) {
        Entity *snowball = mEnemySnowballs[i];

        if (!mEnemySnowballImpact[i]) {
            Vector2 position = snowball->getPosition();

            position.x += mEnemySnowballSpeed *
                        mEnemySnowballDirections[i] * deltaTime;

            snowball->setPosition(position);

            Vector2 playerPosition = mGameState.player->getPosition();
            Vector2 playerSize = mGameState.player->getColliderDimensions();

            bool hitPlayer =
                fabsf(position.x - playerPosition.x) <
                    (playerSize.x + 20.0f) / 2.0f &&
                fabsf(position.y - playerPosition.y) <
                    (playerSize.y + 20.0f) / 2.0f;

            float xOverlap = 0.0f;
            float yOverlap = 0.0f;

            bool hitWall = mGameState.map->isSolidTileAt(
                position, &xOverlap, &yOverlap
            );

            if (hitPlayer || hitWall) {
                mEnemySnowballImpact[i] = true;
                mEnemySnowballImpactTime[i] = 0.0f;

                snowball->setDirection(UP);
                PlaySound(mSnowballImpactSound);

                if (hitPlayer && mInvincibilityTime <= 0.0f && *mHearts > 0) {
                    (*mHearts)--;
                    mInvincibilityTime = INVINCIBILITY_DURATION;
                    mGameState.player->hurt();

                    if (*mHearts <= 0) {
                        mGameState.nextSceneID = LOSE;
                    }
                }
            }

            if (position.x < -100.0f ||
                position.x > LEVEL_WIDTH * TILE_DIMENSION + 100.0f) {

                delete mEnemySnowballs[i];
                mEnemySnowballs.erase(mEnemySnowballs.begin() + i);
                mEnemySnowballDirections.erase(mEnemySnowballDirections.begin() + i);
                mEnemySnowballImpact.erase(mEnemySnowballImpact.begin() + i);
                mEnemySnowballImpactTime.erase(mEnemySnowballImpactTime.begin() + i);

                i--;
                continue;
            }

        } else {
            mEnemySnowballImpactTime[i] += deltaTime;

            if (mEnemySnowballImpactTime[i] >= 8.0f / 14.0f) {
                delete mEnemySnowballs[i];

                mEnemySnowballs.erase(mEnemySnowballs.begin() + i);
                mEnemySnowballDirections.erase(mEnemySnowballDirections.begin() + i);
                mEnemySnowballImpact.erase(mEnemySnowballImpact.begin() + i);
                mEnemySnowballImpactTime.erase(mEnemySnowballImpactTime.begin() + i);

                i--;
                continue;
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
}

void Level3::render() {
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

    if (mWinterKey->isActive()) {
        mWinterKey->render();
    }

    if (mFinalDoor->isActive()) {
        mFinalDoor->render();
    }

    mGameState.map->render();
    mGameState.player->render();

    if (mWinterKey->isActive()) {
        mWinterKey->render();
    }

    for (int i = 0; i < GINGERBREAD_COUNT; i++) {
        if (mGingerbreads[i]->isActive()) {
            mGingerbreads[i]->render();
        }
    }

    for (int i = 0; i < SNOWMAN_COUNT; i++) {
        if (mSnowmans[i]->isActive()) {
            mSnowmans[i]->render();
        }
    }

    for (int i = 0; i < mSnowballs.size(); i++) {
        mSnowballs[i]->render();
    }

    for (int i = 0; i < mEnemySnowballs.size(); i++) {
        mEnemySnowballs[i]->render();
    }
}

void Level3::renderHearts() {
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

void Level3::setHearts(int *hearts) {
    mHearts = hearts;
}

void Level3::renderDoorMessage() {
    if (!mShowDoorMessage) return;

    DrawRectangle(0, 0, 800, 450, Fade(BLACK, 0.65f));

    float width = 500.0f;
    float height = 200.0f;

    Rectangle source = {
        0, 0,
        (float)mDoorMessageTexture.width,
        (float)mDoorMessageTexture.height
    };

    Rectangle destination = {
        (800.0f - width) / 2.0f,
        (450.0f - height) / 2.0f,
        width,
        height
    };

    DrawTexturePro(
        mDoorMessageTexture,
        source,
        destination,
        { 0, 0 },
        0.0f,
        WHITE
    );
}

void Level3::shutdown() {
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

    for (int i = 0; i < mGingerbreads.size(); i++) {
        delete mGingerbreads[i];
    }

    mGingerbreads.clear();

    for (int i = 0; i < mSnowmans.size(); i++) {
        delete mSnowmans[i];
    }

    mSnowmans.clear();

    for (int i = 0; i < mEnemySnowballs.size(); i++) {
        delete mEnemySnowballs[i];
    }

    mEnemySnowballs.clear();
    mEnemySnowballDirections.clear();
    mEnemySnowballImpact.clear();
    mEnemySnowballImpactTime.clear();

    delete mWinterKey;
    mWinterKey = nullptr;

    delete mFinalDoor;
    mFinalDoor = nullptr;

    UnloadTexture(mDoorMessageTexture);
    UnloadSound(mDoorSound);

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

void Level3::setWinterKeyCollected(bool *collected) {
    mWinterKeyCollected = collected;
}

void Level3::setHalloweenKeyCollected(bool *collected) {
    mHalloweenKeyCollected = collected;
}

void Level3::setThanksgivingKeyCollected(bool *collected) {
    mThanksgivingKeyCollected = collected;
}
