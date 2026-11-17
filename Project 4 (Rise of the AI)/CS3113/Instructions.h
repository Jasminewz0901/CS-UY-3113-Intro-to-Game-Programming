#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "Scene.h"

class Instructions : public Scene {
private:
    Texture2D mInstructionsTexture = {};
    Texture2D mPausedInstructionsTexture = {};
    bool mIsPaused = false;

public:
    Instructions(Vector2 origin, const char *bgHexCode);
    ~Instructions();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
    void setPaused(bool paused);
};

#endif // INSTRUCTIONS_H