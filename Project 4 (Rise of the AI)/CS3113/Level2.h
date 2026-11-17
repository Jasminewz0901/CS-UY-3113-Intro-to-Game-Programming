#ifndef Level2_H
#define Level2_H

#include "Scene.h"

class Level2 : public Scene
{
private:
    static constexpr int LEVEL_WIDTH = 18;
    static constexpr int LEVEL_HEIGHT = 20;
    
    Texture2D mBackgroundTexture = {};

    static constexpr int TURKEY_COUNT = 5;

    std::vector<Entity *> mTurkeys;

    float mTurkeyStartX[TURKEY_COUNT] = {
        400.0f, 
        250.0f,
        700.0f,
        700.0f, 
        550.0f
    };

    float mTurkeyStartY[TURKEY_COUNT] = {
        550.0f,
        100.0f,
        550.0f,
        0.0f,
        -200.0f
    };

    float mTurkeyLeftBoundaries[TURKEY_COUNT] = {
        350.0f,
        250.0f,
        650.0f,
        600.0f,
        400.0f
    };

    float mTurkeyRightBoundaries[TURKEY_COUNT] = {
        420.0f,
        300.0f,
        770.0f,
        770.0f,
        650.0f
    };

    bool mTurkeyMovingLeft[TURKEY_COUNT] = {
        true,
        true,
        true,
        true,
        true
    };

    bool mTurkeyDefeated[TURKEY_COUNT] = {
        false,
        false,
        false,
        false,
        false
    };

    float mTurkeyDefeatTime[TURKEY_COUNT] = {
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f
    };

    static constexpr int CORNUCOPIA_COUNT = 3;

    std::vector<Entity *> mCornucopias;

    float mCornucopiaStartX[CORNUCOPIA_COUNT] = {
        200.0f,
        100.0f,
        450.0f
    };

    float mCornucopiaStartY[CORNUCOPIA_COUNT] = {
        550.0f,
        400.0f,
        550.0f
    };

    float mCornucopiaLeftBoundaries[CORNUCOPIA_COUNT] = {
        50.0f,
        50.0f,
        450.0f
    };

    float mCornucopiaRightBoundaries[CORNUCOPIA_COUNT] = {
        250.0f,
        180.0f,
        575.0f
    };

    bool mCornucopiaDefeated[CORNUCOPIA_COUNT] = {
        false,
        false,
        false
    };

    float mCornucopiaDefeatTime[CORNUCOPIA_COUNT] = {
        0.0f,
        0.0f,
        0.0f
    };

    static constexpr float CORNUCOPIA_CHASE_RANGE = 250.0f;

    static constexpr int LEAF_COUNT = 4;

    std::vector<Entity *> mLeaves;

    float mLeafStartX[LEAF_COUNT] = {
        50.0f,
        350.0f,
        450.0f,
        750.0f
    };

    float mLeafStartY[LEAF_COUNT] = {
        -700.0f,
        -300.0f,
        100.0f,
        500.0f
    };

    float mLeafSpeed = 40.0f;

    bool mLeafDefeated[LEAF_COUNT] = {
        false, 
        false, 
        false,
        false
    };

    float mLeafDefeatTime[LEAF_COUNT] = {
        0.0f, 
        0.0f, 
        0.0f,
        0.0f
    };

    Texture2D mHeartsTexture = {};

    Entity *mThanksgivingKey = nullptr;
    Sound mItemPickupSound = {};

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
        3, 4, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 3, 
        3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 4, 4, 4, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 3, 
        3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 3,
        3, 4, 0, 0, 4, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 4, 4, 4, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 3, 
        3, 0, 0, 4, 4, 4, 4, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 3, 
        3, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 3, 
        3, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 4, 0, 0, 4, 4, 3,
        3, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
        3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,
        3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
    };

public:
    static constexpr float TILE_DIMENSION          = 50.0f,
                           ACCELERATION_OF_GRAVITY = 981.0f,
                           END_GAME_THRESHOLD      = 800.0f;

    Level2();
    Level2(Vector2 origin, const char *bgHexCode);
    ~Level2();

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
    void setThanksgivingKeyCollected(bool *collected);
};

#endif // Level2_H
