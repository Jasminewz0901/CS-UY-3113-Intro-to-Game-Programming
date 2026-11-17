#ifndef Level1_H
#define Level1_H

#include "Scene.h"

constexpr int LEVEL_WIDTH  = 18,
              LEVEL_HEIGHT = 20;

class Level1 : public Scene
{
private:
    Texture2D mBackgroundTexture = {};

    static constexpr int PUMPKIN_COUNT = 5;

    std::vector<Entity *> mPumpkins;

    float mPumpkinStartX[PUMPKIN_COUNT] = {
        150.0f,  
        650.0f, 
        450.0f, 
        700.0f, 
        300.0f 
    };

    float mPumpkinStartY[PUMPKIN_COUNT] = {
        -150.0f,
        -150.0f,
        100.0f,
        550.0f,
        350.0f
    };

    float mPumpkinLeftBoundaries[PUMPKIN_COUNT] = {
        150.0f, 
        550.0f, 
        450.0f,  
        650.0f,  
        300.0f   
    };

    float mPumpkinRightBoundaries[PUMPKIN_COUNT] = {
        250.0f,  
        650.0f, 
        500.0f, 
        730.0f,
        400.0f  
    };

    bool mPumpkinMovingLeft[PUMPKIN_COUNT] = {
        true, 
        true, 
        true, 
        true, 
        true
    };

    bool mPumpkinDefeated[PUMPKIN_COUNT] = {
        false, 
        false, 
        false, 
        false, 
        false
    };

    float mPumpkinDefeatTime[PUMPKIN_COUNT] = {
        0.0f, 
        0.0f, 
        0.0f, 
        0.0f, 
        0.0f
    };

    static constexpr int SKELETON_COUNT = 2;

    std::vector<Entity *> mSkeletons;

    float mSkeletonStartX[SKELETON_COUNT] = {
        350.0f,  
        400.0f  
    };

    float mSkeletonStartY[SKELETON_COUNT] = {
        500.0f, 
        -50.0f 
    };

    float mSkeletonLeftBoundaries[SKELETON_COUNT] = {
        250.0f,
        350.0f
    };

    float mSkeletonRightBoundaries[SKELETON_COUNT] = {
        420.0f,
        470.0f
    };

    bool mSkeletonDefeated[SKELETON_COUNT] = {
        false, 
        false
    };

    float mSkeletonDefeatTime[SKELETON_COUNT] = {
        0.0f, 
        0.0f
    };

    static constexpr float SKELETON_CHASE_RANGE = 250.0f;

    Texture2D mHeartsTexture = {};

    Entity *mHalloweenKey = nullptr;
    Sound mItemPickupSound = {};

    bool *mHalloweenKeyCollected = nullptr;

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
        3, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 3,
        3, 4, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 4, 3,
        3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 3, 
        3, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 3,
        3, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 4, 3,
        3, 0, 0, 0, 4, 0, 4, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 3,
        3, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 0, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 4, 3,
        3, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
        3, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 4, 3, 
        3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 
        3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 
    };

public:
    static constexpr float TILE_DIMENSION          = 50.0f,
                           ACCELERATION_OF_GRAVITY = 981.0f,
                           END_GAME_THRESHOLD      = 800.0f;

    Level1();
    Level1(Vector2 origin, const char *bgHexCode);
    ~Level1();

    bool canThrowSnowball() const {
        return mSnowballCooldown <= 0.0f;
    }

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
    void throwSnowball();
    void restart();
    void renderHearts();
    void setHearts(int *hearts);
    void setHalloweenKeyCollected(bool *collected);
};

#endif // Level1_H
