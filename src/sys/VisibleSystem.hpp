#pragma once

#include "System.hpp"
#include <unordered_map>
#include <tools/Storage.hpp>

class VisibleSystem : public System {
public:
    VisibleSystem();
    ~VisibleSystem();

    void update(GameEngine &gameContext) const override;

private:
    template <typename T>
    void refreshDrawables(GameEngine &gameContext, Storage<T> &storage, float limitLeft, float limitRigth, float limitTop, float limitBottom) const;
    void refreshDrawable(GameEngine &gameContext, uint32_t entityId, float limitLeft, float limitRigth, float limitTop, float limitBottom) const;
    bool checkSituationIsOutsideLimits(SituationComponent &situation, float limitLeft, float limitRigth, float limitTop, float limitBottom) const;
    
    /*
        void getEntitiesToDeleteDrawable(GameEngine &gameContext, uint32_t terrainId) const;
        template <typename T>
        void calculateEntitiesPositionToDelete(GameEngine &gameContext, Storage<T> &entities, float limitLeft, float limitRight, float limitTop, float limitBottom) const;
        void calculateUniqueEntitiesPositionToDelete(GameEngine &gameContext, uint32_t entityID, float limitLeft, float limitRight, float limitTop, float limitBottom) const;
        void getEntitiesToGiveDrawable(GameEngine &gameContext, uint32_t terrainId) const;
        template <typename T>
        void calculateEntitiesPositionToDraw(GameEngine &gameContext, Storage<T> &entities, float limitLeft, float limitRight, float limitTop, float limitBottom, EntityType type) const;
        void calculateUniqueEntitiesPositionToDraw(GameEngine &gameContext, uint32_t entityID, float limitLeft, float limitRight, float limitTop, float limitBottom, EntityType type) const;

        bool checkIfHasDrawable(uint32_t cubeId, GameEngine &gameContext) const;
        void giveDrawableToEntity(GameEngine &gameContext, uint32_t entityID, EntityType type) const;
        void giveDrawableToMeleeWeapon(GameEngine &gameContext, uint32_t meleeID, DrawableComponent &drawable) const;
        void giveDrawableToDistanceWeapon(GameEngine &gameContext, uint32_t distanceID, DrawableComponent &drawable) const;
    */
};