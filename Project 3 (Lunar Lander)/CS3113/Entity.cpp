#include "Entity.h"

Entity::Entity() :
    mPosition{ 0.0f, 0.0f }, mMovement{ 0.0f, 0.0f },
    mVelocity{ 0.0f, 0.0f }, mAcceleration{ 0.0f, 0.0f },
    mScale{ DEFAULT_SIZE, DEFAULT_SIZE },
    mColliderDimensions{ DEFAULT_SIZE, DEFAULT_SIZE },
    mTexture{}, mTextureType{ SINGLE }, mSpriteSheetDimensions{},
    mAnimationAtlas{}, mAnimationIndices{}, mDirection{ RIGHT },
    mFrameSpeed{ 0 }, mSpeed{ DEFAULT_SPEED }, mAngle{ 0.0f },
    mEntityType{ NONE } {}

Entity::Entity(
    Vector2     position,
    Vector2     scale,
    const char *textureFilepath,
    EntityType  entityType
) :
    mPosition{ position }, mMovement{ 0.0f, 0.0f },
    mVelocity{ 0.0f, 0.0f }, mAcceleration{ 0.0f, 0.0f },
    mScale{ scale }, mColliderDimensions{ scale },
    mTexture{ LoadTexture(textureFilepath) }, mTextureType{ SINGLE },
    mSpriteSheetDimensions{}, mAnimationAtlas{}, mAnimationIndices{},
    mDirection{ RIGHT }, mFrameSpeed{ 0 }, mSpeed{ DEFAULT_SPEED },
    mAngle{ 0.0f }, mEntityType{ entityType } {}

Entity::Entity(
    Vector2                               position,
    Vector2                               scale,
    const char                           *textureFilepath,
    TextureType                           textureType,
    Vector2                               spriteSheetDimensions,
    std::map<Direction, std::vector<int>> animationAtlas,
    EntityType                            entityType
) :
    mPosition{ position }, mMovement{ 0.0f, 0.0f },
    mVelocity{ 0.0f, 0.0f }, mAcceleration{ 0.0f, 0.0f },
    mScale{ scale }, mColliderDimensions{ scale },
    mTexture{ LoadTexture(textureFilepath) }, mTextureType{ textureType },
    mSpriteSheetDimensions{ spriteSheetDimensions },
    mAnimationAtlas{ animationAtlas },
    mAnimationIndices{ animationAtlas.at(DOWN) }, mDirection{ DOWN },
    mFrameSpeed{ DEFAULT_FRAME_SPEED }, mSpeed{ DEFAULT_SPEED },
    mAngle{ 0.0f }, mEntityType{ entityType } {}

Entity::~Entity() { UnloadTexture(mTexture); }

/**
 * Iterates through a list of collidable entities, checks for collisions with
 * the player entity, and resolves any vertical overlap by adjusting the
 * player's position and velocity accordingly.
 *
 * @param collidableEntities An array of pointers to `Entity` objects that
 * represent the entities that the current `Entity` instance can potentially
 * collide with. The `collisionCheckCount` parameter specifies the number of
 * entities in the `collidableEntities` array that need to be checked for
 * collision.
 * @param collisionCheckCount The number of entities that the current entity
 * (`Entity`) should check for collisions with. This parameter specifies how
 * many entities are in the `collidableEntities` array that need to be checked
 * for collisions with the current entity.
 */
void Entity::checkCollisionY(
    Entity *collidableEntities, int collisionCheckCount
)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity))
        {
            float yDistance = fabs(getColliderPosition().y - collidableEntity->getColliderPosition().y);
            float yOverlap  = fabs(
                yDistance - (mColliderDimensions.y / 2.0f) -
                (collidableEntity->mColliderDimensions.y / 2.0f)
            );

            if (mVelocity.y > 0)
            {
                mPosition.y        -= yOverlap;
                mVelocity.y         = 0;
                mIsCollidingBottom  = true;
            }
            else if (mVelocity.y < 0)
            {
                mPosition.y     += yOverlap;
                mVelocity.y      = 0;
                mIsCollidingTop  = true;

                if (collidableEntity->mEntityType == BLOCK)
                    collidableEntity->deactivate();
            }
        }
    }
}

void Entity::checkCollisionX(
    Entity *collidableEntities, int collisionCheckCount
)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity))
        {
            float yDistance = fabs(getColliderPosition().y - collidableEntity->getColliderPosition().y);
            float yOverlap  = fabs(
                yDistance - (mColliderDimensions.y / 2.0f) -
                (collidableEntity->mColliderDimensions.y / 2.0f)
            );

            if (yOverlap < Y_COLLISION_THRESHOLD) continue;

            float xDistance = fabs(getColliderPosition().x - collidableEntity->getColliderPosition().x);
            float xOverlap  = fabs(
                xDistance - (mColliderDimensions.x / 2.0f) -
                (collidableEntity->mColliderDimensions.x / 2.0f)
            );

            if (mVelocity.x > 0)
            {
                mPosition.x -= xOverlap;
                mVelocity.x  = 0;

                mIsCollidingRight = true;
            }
            else if (mVelocity.x < 0)
            {
                mPosition.x += xOverlap;
                mVelocity.x  = 0;

                mIsCollidingLeft = true;
            }
        }
    }
}

/**
 * Checks if two entities are colliding based on their positions and collider
 * dimensions.
 *
 * @param other represents another Entity with which you want to check for
 * collision. It is a pointer to the Entity class.
 *
 * @return returns `true` if the two entities are colliding based on their
 * positions and collider dimensions, and `false` otherwise.
 */
bool Entity::isColliding(Entity *other) const {
    if (!other->isActive()) return false;

    Vector2 thisPosition = getColliderPosition();
    Vector2 otherPosition = other->getColliderPosition();

    float xDistance = fabs(thisPosition.x - otherPosition.x) -
        ((mColliderDimensions.x + other->getColliderDimensions().x) / 2.0f);

    float yDistance = fabs(thisPosition.y - otherPosition.y) -
        ((mColliderDimensions.y + other->getColliderDimensions().y) / 2.0f);

    return xDistance < 0.0f && yDistance < 0.0f;
}

/**
 * Updates the current frame index of an entity's animation based on the
 * elapsed time and frame speed.
 *
 * @param deltaTime represents the time elapsed since the last frame update.
 */
void Entity::animate(float deltaTime)
{
    mAnimationIndices = mAnimationAtlas.at(mDirection);

    mAnimationTime += deltaTime;
    float secondsPerFrame = 1.0f / mFrameSpeed;

    if (mAnimationTime >= secondsPerFrame)
    {
        mAnimationTime = 0.0f;

        if (mDirection == UP || (mEntityType == BLOCK && mDirection == DOWN) ||
            (mEntityType == PLATFORM && mDirection == DOWN)) {
                if (mCurrentFrameIndex < static_cast<int>(mAnimationIndices.size()) - 1) {
                    mCurrentFrameIndex++;
                }
        } else {
            mCurrentFrameIndex++;
            mCurrentFrameIndex %= mAnimationIndices.size();
        }
    }
}

void Entity::displayCollider()
{
    Vector2 colliderPosition = getColliderPosition();

    Rectangle colliderBox = {
        colliderPosition.x - mColliderDimensions.x / 2.0f,
        colliderPosition.y - mColliderDimensions.y / 2.0f,
        mColliderDimensions.x,
        mColliderDimensions.y
    };

    DrawRectangleLines(
        colliderBox.x,    
        colliderBox.y,    
        colliderBox.width,  
        colliderBox.height, 
        GREEN              
    );
}

void Entity::update(
    float deltaTime,
    Entity *collidableEntities,
    int collisionCheckCount,

    Entity *additionalCollidableEntities,
    int additionalCollisionCheckCount,
    float fixedTimestep
)
{
    if (mEntityStatus == INACTIVE) return;

    gTimeAccumulator += deltaTime;

    while (gTimeAccumulator >= fixedTimestep)
    {
        resetColliderFlags();

        mVelocity.x += mAcceleration.x * fixedTimestep;
        mVelocity.y += mAcceleration.y * fixedTimestep;

        if (mAcceleration.x == 0.0f) {
            mVelocity.x *= 0.95f;

            if (fabs(mVelocity.x) < 1.0f) {
                mVelocity.x = 0.0f;
            }
        }

        if (mIsJumping)
        {
            mIsJumping = false;
            mVelocity.y -= mJumpingPower;
        }

        mPosition.y += mVelocity.y * fixedTimestep;
        checkCollisionY(collidableEntities, collisionCheckCount);

        checkCollisionY(
            additionalCollidableEntities, additionalCollisionCheckCount
        );

        mPosition.x += mVelocity.x * fixedTimestep;

        checkCollisionX(collidableEntities, collisionCheckCount);

        if (mTextureType == ATLAS) {
            animate(fixedTimestep);
        }

        gTimeAccumulator -= fixedTimestep;
    }
}

void Entity::render()
{
    if (mEntityStatus == INACTIVE) return;

    Rectangle textureArea;

    switch (mTextureType)
    {
    case SINGLE:
        textureArea = {
            0.0f, 0.0f,

            static_cast<float>(mTexture.width),
            static_cast<float>(mTexture.height)
        };
        break;

    case ATLAS:
    {
        float columns = mSpriteSheetDimensions.x;
        float rows = mSpriteSheetDimensions.y;

        float frameWidth = static_cast<float>(mTexture.width) / columns;
        float frameHeight = static_cast<float>(mTexture.height) / rows;

        int frame = mAnimationIndices[mCurrentFrameIndex];

        textureArea = {
            (frame % static_cast<int>(columns)) * frameWidth,
            (frame / static_cast<int>(columns)) * frameHeight,
            frameWidth,
            frameHeight
        };
        break;
    }
    default:
        break;
    }

    Rectangle destinationArea = {
        mPosition.x,
        mPosition.y,
        static_cast<float>(mScale.x),
        static_cast<float>(mScale.y)
    };

    Vector2 originOffset = {
        static_cast<float>(mScale.x) / 2.0f,
        static_cast<float>(mScale.y) / 2.0f
    };

    DrawTexturePro(
        mTexture,
        textureArea, destinationArea, originOffset,
        mAngle, WHITE
    );

    displayCollider();
}
