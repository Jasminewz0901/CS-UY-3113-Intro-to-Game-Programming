#ifndef Level3_H
#define Level3_H

#include "Scene.h"

class Level3 : public Scene
{
private:
    static constexpr int LEVEL_WIDTH = 18;
    static constexpr int LEVEL_HEIGHT = 20;
    
    Texture2D mBackgroundTexture = {};

    static constexpr int GINGERBREAD_COUNT = 4;

    std::vector<Entity *> mGingerbreads;

    float mGingerbreadStartX[GINGERBREAD_COUNT] = {
        250.0f,
        300.0f,
        550.0f,
        750.0f
    };

    float mGingerbreadStartY[GINGERBREAD_COUNT] = {
        550.0f,
        -100.0f,
        50.0f,
        550.0f
    };

    float mGingerbreadLeftBoundaries[GINGERBREAD_COUNT] = {
        50.0f,
        200.0f, 
        500.0f,
        700.0f
    };

    float mGingerbreadRightBoundaries[GINGERBREAD_COUNT] = {
        250.0f,
        400.0f,
        570.0f,
        770.0f
    };

    bool mGingerbreadMovingLeft[GINGERBREAD_COUNT] = {
        true,
        true,
        true,
        true
    };

    bool mGingerbreadDefeated[GINGERBREAD_COUNT] = {
        false,
        false,
        false,
        false
    };

    float mGingerbreadDefeatTime[GINGERBREAD_COUNT] = {
        0.0f,
        0.0f,
        0.0f,
        0.0f
    };

    static constexpr int SNOWMAN_COUNT = 3;

    std::vector<Entity *> mSnowmans;

    float mSnowmanStartX[SNOWMAN_COUNT] = {
        300.0f,
        350.0f,
        450.0f
    };

    float mSnowmanStartY[SNOWMAN_COUNT] = {
        50.0f,
        -100.0f,
        550.0f
    };

    float mSnowmanLeftBoundaries[SNOWMAN_COUNT] = {
        250.0f,
        350.0f,
        300.0f
    };

    float mSnowmanRightBoundaries[SNOWMAN_COUNT] = {
        320.0f,
        600.0f,
        320.0f
    };

    bool mSnowmanDefeated[SNOWMAN_COUNT] = {
        false,
        false,
        false
    };

    float mSnowmanDefeatTime[SNOWMAN_COUNT] = {
        0.0f,
        0.0f,
        0.0f
    };

    static constexpr float SNOWMAN_CHASE_RANGE = 250.0f;
    float mSnowmanAttackCooldown[SNOWMAN_COUNT] = { 0.0f, 0.0f };
    float mSnowmanAttackTime[SNOWMAN_COUNT] = { 0.0f, 0.0f };
    bool mSnowmanAttacking[SNOWMAN_COUNT] = { false, false };

    static constexpr float SNOWMAN_ATTACK_COOLDOWN = 0.7f;
    static constexpr float SNOWMAN_ATTACK_DURATION = 0.8f;

    std::vector<Entity *> mEnemySnowballs;
    std::vector<float> mEnemySnowballDirections;

    float mEnemySnowballSpeed = 250.0f;

    std::vector<bool> mEnemySnowballImpact;
    std::vector<float> mEnemySnowballImpactTime;

    Texture2D mHeartsTexture = {};

    Entity *mWinterKey = nullptr;

    Entity *mFinalDoor = nullptr;
    bool mDoorOpening = false;
    float mDoorOpeningTime = 0.0f;

    Texture2D mDoorMessageTexture;
    bool mShowDoorMessage = false;
    float mDoorMessageTime = 0.0f;

    Sound mItemPickupSound = {};

    Sound mDoorSound;

    bool *mWinterKeyCollected = nullptr;
    bool *mHalloweenKeyCollected = nullptr;
    bool *mThanksgivingKeyCollected = nullptr;

    int *mHearts = nullptr;

    float mInvincibilityTime = 0.0f;
    static constexpr float INVINCIBILITY_DURATION = 1.5f;

    std::vector<Entity *> mSnowballs;
    std::vector<float> mSnowballDirections;
    std::vector<bool> mSnowballImpact;
    std::vector<float> mSnowballImpactTime;

    Sound mSnowballImpactSound = {};

    float mSnowballSpeed = 350.0f;
    float mSnowballCooldown = 0.0f;
    static constexpr float SNOWBALL_COOLDOWN_TIME = 0.5f;

   unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 3,
        3, 0, 0, 4, 4, 4, 4, 4, 0, 0, 4, 4, 4, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 0, 0, 4, 3,
        3, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 4, 0, 4, 4, 0, 4, 4, 0, 4, 0, 0, 4, 3,
        3, 0, 0, 4, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 0, 3,
        3, 4, 0, 4, 4, 0, 4, 4, 0, 4, 4, 0, 4, 4, 4, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,
        3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
   };


public:
    static constexpr float TILE_DIMENSION          = 50.0f,
                           ACCELERATION_OF_GRAVITY = 981.0f,
                           END_GAME_THRESHOLD      = 800.0f;

    Level3();
    Level3(Vector2 origin, const char *bgHexCode);
    ~Level3();

    bool canThrowSnowball() const {
        return mSnowballCooldown <= 0.0f;
    }

    bool isDoorMessageShowing() const {
        return mShowDoorMessage;
    }

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
    void throwSnowball();
    void restart();
    void renderHearts();
    void renderDoorMessage();
    void setHearts(int *hearts);
    void setWinterKeyCollected(bool *collected);
    void setThanksgivingKeyCollected (bool *collected);
    void setHalloweenKeyCollected (bool *collected);
    void throwEnemySnowball(int snowmanIndex);
    void interactWithDoor();
};

#endif // Level3_H
