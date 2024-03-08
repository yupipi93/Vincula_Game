#include "StatsSystem.hpp"

//console.log

#include <tools/Utils.hpp>

#include <eng/GameEngine.hpp>



//#####################################################
//                == STATS SYSTEM ==
//  - Check attacks to substract health to the entities
//      which have been attacked
//
//   Using Componets:
//      - Stats         (Read and Write)
//#####################################################
const std::array<float, 3> normalColor {1.0f, 1.0f, 1.0f};


StatsSystem::StatsSystem() {}

StatsSystem::~StatsSystem() {}

void StatsSystem::update(GameEngine &gameContext) const {
    if(isPlayerRecivedDamage(gameContext)) {
        gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerId, PLAYER_INJURED);
    }

    refreshPlayerHeart(gameContext);
    subtractDamageReceivedToCurrentLife(gameContext);

}

void StatsSystem::refreshPlayerHeart(GameEngine &gameContext) const {
    auto &statsComp = gameContext.entityMan.getComponent<StatsComponent>(gameContext.playerId);
    auto &hud = gameContext.entityMan.getComponent<HUDComponent>(gameContext.HUDId);
    std::string resource;

    if (statsComp.currentHealth * 100 / statsComp.maxHealth < 35) {
        if (hud.heartTimer <= 0) {
            hud.heart = !hud.heart;
            if (hud.heart)
                resource = "./media/hud/healthDetails.png";
            else
                resource = "./media/hud/healthDetails2.png";

            hud.heartTimer = hud.maxHeartTimer;
        } else {
            resource = hud.healthDetails;
            hud.heartTimer -= gameContext.getDeltaTime();
        }
    } else
        resource = "./media/hud/healthDetails.png";

    hud.healthDetails = resource;
}

bool StatsSystem::isPlayerRecivedDamage(GameEngine &gameContext) const {
    auto &statsComp = gameContext.entityMan.getComponent<StatsComponent>(gameContext.playerId);
    return statsComp.damageReceived;
}


void StatsSystem::subtractDamageReceivedToCurrentLife(GameEngine &gameContext) const {
    auto &stats = gameContext.entityMan.getComponents<StatsComponent>();

    for (auto &st : stats) {

        if (st.damaged) {
            if (st.damagedTime >= 0.2) {
                st.damaged = false;
                st.damagedTime = 0;
                gameContext.getWindowFacadeRef().addColorToEntity(st.id, normalColor);

            } else {
                st.damagedTime += gameContext.getDeltaTime();
            }
        }

        if (st.healing) {
            if (st.healingTime >= 0.2) {
                st.healing = false;
                st.healingTime = 0;
                gameContext.getWindowFacadeRef().addColorToEntity(st.id, normalColor);

            } else {
                st.healingTime += gameContext.getDeltaTime();
            }
        }
        int16_t newHealth = st.currentHealth - st.damageReceived;
        newHealth += st.damageHealing;

        if (newHealth <= 0 ) {
            newHealth = 0;
            //Avoid entity death in one touch
            if(st.id == gameContext.playerId) {
                if(st.currentHealth == st.maxHealth) {
                    newHealth = 1;
                }

            }
        } else if (newHealth > st.maxHealth ) {
            newHealth = st.maxHealth;
        }

        st.currentHealth = newHealth;
        if(st.damageReceived != 0) {
            st.lastDamageReceived = st.damageReceived;
        }
        st.damageReceived = 0;

        if(st.damageHealing != 0) {
            st.lastDamageHealing = st.damageHealing;
        }
        st.damageHealing = 0;
    }
}

