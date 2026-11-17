#ifndef BOSS_H
#define BOSS_H

#include "Scene.h"

class Boss : public Scene
{
private:
    static constexpr int LEVEL_WIDTH = 16;
    static constexpr int LEVEL_HEIGHT = 9;

    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
    };

    Texture2D mBackgroundTexture = {};
    Texture2D mHeartsTexture = {};

    Entity *mGrinch = nullptr;
    Entity *mMariah = nullptr;

    float mGrinchSpeed = 80.0f;

    float mGrinchLeftBoundary = 450.0f;
    float mGrinchRightBoundary = 700.0f;

    bool mGrinchMovingLeft = true;

    bool mGrinchAttacking = false;

    float mGrinchAttackCooldown = 0.0f;
    float mGrinchAttackDuration = 0.0f;

    static constexpr float GRINCH_ATTACK_INTERVAL = 3.0f;
    static constexpr float GRINCH_ATTACK_DURATION = 1.0f;

    int mBossHearts = 25;
    bool mBossDefeated = false;

    float mGrinchDefeatTime = 0.0f;
    static constexpr float GRINCH_DEFEAT_DURATION = 1.0f;

    bool mMariahRevealing = false;
    float mMariahRevealTime = 0.0f;
    Music mItsTimeMusic = {};

    Sound mSnowballImpactSound = {};

    static constexpr float MARIAH_REVEAL_DURATION = 16.0f;

    float mBossAttackTime = 0.0f;

    std::vector<Entity *> mPresents;

    std::vector<bool> mPresentImpact;
    std::vector<float> mPresentImpactTime;

    float mPresentSpeed = 200.0f;
    int mPresentsThrown = 0;
    float mPresentSpawnTimer = 0.0f;

    std::vector<Entity *> mSnowballs;
    std::vector<float> mSnowballDirections;
    std::vector<bool> mSnowballImpact;
    std::vector<float> mSnowballImpactTime;

    float mSnowballSpeed = 350.0f;
    float mSnowballCooldown = 0.0f;
    static constexpr float SNOWBALL_COOLDOWN_TIME = 0.5f;

    int *mHearts = nullptr;
    float mInvincibilityTime = 0.0f;
    static constexpr float INVINCIBILITY_DURATION = 1.5f;

public:
    static constexpr float TILE_DIMENSION = 50.0f;
    static constexpr float ACCELERATION_OF_GRAVITY = 981.0f;

    Boss();
    Boss(Vector2 origin, const char *bgHexCode);
    ~Boss();

    bool canThrowSnowball() const {
        return mSnowballCooldown <= 0.0f;
    }

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;

    void throwSnowball();
    void throwPresent();
    void renderHearts();
    void renderBossHealth();
    void setHearts(int *hearts);
};

#endif // BOSS_H
