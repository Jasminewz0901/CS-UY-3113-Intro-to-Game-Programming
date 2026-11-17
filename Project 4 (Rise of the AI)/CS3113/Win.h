
#ifndef WIN_H
#define WIN_H

#include "Scene.h"

class Win : public Scene {
private:
    Texture2D mWinTexture = {};

    Music mVictoryMusic = {};

public:
    Win(Vector2 origin, const char *bgHexCode);
    ~Win();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // WIN_H
