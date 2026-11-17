
#ifndef LOSE_H
#define LOSE_H

#include "Scene.h"

class Lose : public Scene {
private:
    Texture2D mLoseTexture = {};

public:
    Lose(Vector2 origin, const char *bgHexCode);
    ~Lose();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LOSE_H
