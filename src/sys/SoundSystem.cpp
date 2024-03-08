#include "SoundSystem.hpp"

#include <eng/GameEngine.hpp>

//console.log


//#####################################################
//                == SOUND SYSTEM ==
//  - Update 3D Parameters in VibrateEngine
//  - Update Eggs an Interest Poinst Sounds
//
//   Using Componets:
//    - soundComponet
//#####################################################


constexpr float FACTOR = 0.01;
constexpr float MAX_PITCH = 1.f;


SoundSystem::SoundSystem() {}

SoundSystem::~SoundSystem() {}

void SoundSystem::update(GameEngine &gameContext) const {


    if (gameContext.getGameState() != PLAYING) {
        setPause(gameContext, true);
        if(gameContext.getGameState() == GAMEOVER || gameContext.getGameState() == WIN) {
            updateStateSound(gameContext);
        } else {
            updateMenuSounds(gameContext);

        }


    } else {


        setPause(gameContext, false);

        updatePlayerSound(gameContext);

        updateBiomeAmbient(gameContext);

        updateMusicLinkToLife(gameContext);

        updateInterestPointsChant(gameContext);

        updateEggsChant(gameContext);

        updateSoundsParameters(gameContext);

        update3DSounds(gameContext); //LAST



    }




}




void SoundSystem::updateStateSound(GameEngine &gameContext) const {
    if (gameContext.getGameState() == WIN) {
        gameContext.getSoundFacadeRef().playEvent(gameContext, gameContext.winEventId);
    } else if(gameContext.getGameState() == GAMEOVER) {
        gameContext.getSoundFacadeRef().playEvent(gameContext, gameContext.gameOverEventId);

    }

}


void SoundSystem::updatePlayerSound(GameEngine &gameContext) const {
    auto &imputComp     = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);
    auto &dashComp      = gameContext.entityMan.getComponent<DashComponent>(gameContext.playerId);
    auto &playerLevel   = gameContext.entityMan.getComponent<LevelComponent>(gameContext.playerId);
    auto &map           = gameContext.entityMan.getComponent<MapComponent>(gameContext.mapId);


    //SAMERSOULT
    if(dashComp.dashing) {
        gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerId, PLAYER_SOMERSOULT);

        //ATACK
    } else if(imputComp.controlClickLeft) {
        attackSound(gameContext);

        //STEPS
    } else if(imputComp.pressingW || imputComp.pressingA || imputComp.pressingS || imputComp.pressingD) {
        gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerId, gameContext.playerBiome + 1);

        //STANBY SOUND
    } else {
        gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerId, STANDBY_SOUND);
    }




    //CHANGE WEAPON
    if(imputComp.changingWeapon) {
        gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerActionsEventId, CHANGE_WEAPON_SOUND);
    } else

        //PRESS F
        if(imputComp.interacting) {
            //gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerActionsEventId, PRESS_F_BAD_SOUND);
        } else

            //OPEN MAP
            if (imputComp.lookingMap) {
                gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerActionsEventId, OPEN_MAP_SOUND);

            } else {
                gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerActionsEventId, STANDBY_SOUND);
            }




}

void SoundSystem::updateMusicLinkToLife(GameEngine &gameContext) const {
    //MUSIC LINK TO LIFE
    auto &statsComp = gameContext.entityMan.getComponent<StatsComponent>(gameContext.playerId);


    if(statsComp.currentHealth != statsComp.lastHealth) {
        //percent = currentHealth/(maxHealth/100.f)
        //newValue = percent * max_Value / 100


        float maxHealth = statsComp.maxHealth;
        float actueWhenLifeLessTo = 50;
        float percent = statsComp.currentHealth / (statsComp.maxHealth / 100.f);

        percent += actueWhenLifeLessTo;
        if(percent > 100) {
            percent = 100;
        }
        float newValue = percent * (MAX_PITCH / 100);

        //MUSIC
        gameContext.entityMan.getComponent<SoundComponent>(gameContext.ambietMusicId).setPitch(newValue);
        gameContext.entityMan.getComponent<SoundComponent>(gameContext.ambietMusicId).setVolume(newValue);


        //HEART SOUND
        if(newValue >= 1) {
            newValue = 0;
        } else {
            newValue = 2 - newValue;
        }
        gameContext.entityMan.getComponent<SoundComponent>(gameContext.heartSoundId).setPitch(newValue);
        gameContext.entityMan.getComponent<SoundComponent>(gameContext.heartSoundId).setVolume(newValue);

        statsComp.lastHealth = statsComp.currentHealth;
    }
}
void SoundSystem::updateBiomeAmbient(GameEngine &gameContext) const {


    //UPDATE MUSIC
    gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.ambietMusicId, gameContext.distanceToCenter);
    //UPDATE
    if(gameContext.lastPlayerBiome != gameContext.playerBiome && gameContext.playerBiome < 6 && gameContext.lastPlayerBiome < 6) {
        gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.ambientSFXId, gameContext.playerBiome, BIOME_PARAMETER);
    }

    //UPDATE SFX AMBIENT TIMER
    gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.ambientSFXId, gameContext.currentHour, TIME_PARAMETER);



}






void SoundSystem::attackSound(GameEngine &gameContext) const {
    auto &inventoryComp = gameContext.entityMan.getComponent<InventoryComponent>(gameContext.playerId);
    if(inventoryComp.primaryWeaponId) {
        auto typeWeapon = gameContext.getEntity(inventoryComp.primaryWeaponId).getType();
        if(typeWeapon == MELEEWEAPON) {
            gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerId, ATTACKING_MELEE);

        } else if(typeWeapon == DISTANCEWEAPON) {
            //gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerId, ATTACKING_DISTANCE);
        }

    }
}

void SoundSystem::updateMenuSounds(GameEngine &gameContext) const {
    if (gameContext.getGameState() != PLAYING) {

        auto &imputComp = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);
        if(imputComp.controlUpKeyPress) {
            gameContext.getSoundFacadeRef().playShootSound("Menu/MenuDown");
            //gameContext.getSoundFacadeRef().playShootSound("Menu/MenuUp");
        }
        if(imputComp.controlDownKeyPress) {
            gameContext.getSoundFacadeRef().playShootSound("Menu/MenuDown");
        }
        if(imputComp.controlReturnKeyPress || imputComp.controlEscKeyPress) {
            gameContext.getSoundFacadeRef().playShootSound("Menu/MenuConfirm");
        }
    }
}


void SoundSystem::setPause(GameEngine &gameContext, bool pause) const {

    gameContext.getSoundFacadeRef().setPauseBusByName("Ambient", pause);
    gameContext.getSoundFacadeRef().setPauseBusByName("Effects", pause);

}



void SoundSystem::updateInterestPointsChant(GameEngine &gameContext) const {
    //Get all interest points
    auto &allIPComp = gameContext.entityMan.getComponents<IPDataComponent>();
    auto &playerCollidable = gameContext.entityMan.getComponent<CollisionComponent>(gameContext.playerId);

    for(auto &ipComp : allIPComp) {
        if(isDiscovered(gameContext, playerCollidable, ipComp)) {
            if(ipComp.isChanting) {
                stopSound(gameContext, ipComp.id);
                ipComp.isChanting = false;
            }
        }
    }


}



bool SoundSystem::isDiscovered(GameEngine &gameContext, CollisionComponent &playerCollidable, IPDataComponent &ipComp) const {
    if(!ipComp.isDiscoveredForPlayer) {
        //check it then
        for(uint32_t entityCollidinId : playerCollidable.entitiesColliding) {
            if(entityCollidinId == ipComp.secureZoneId) {
                ipComp.isDiscoveredForPlayer = true;
                return true;
            }

        }

    }
    return false;


}


void SoundSystem::updateEggsChant(GameEngine &gameContext) const {
    auto &allEggsComp {gameContext.entityMan.getComponents<EggDataComponent>()};
    for(auto &eggs : allEggsComp) {
        if(eggs.broken) {
            if(eggs.isChanting) {
                stopSound(gameContext, eggs.id);
                eggs.isChanting = false;
            }
        }
    }

}


void SoundSystem::stopSound(GameEngine &gameContext, uint32_t id) const {
    gameContext.getSoundFacadeRef().setParameterEventByID(id, DEAD_SOUND);

}



void SoundSystem::update3DSounds(GameEngine &gameContext) const {
    gameContext.getSoundFacadeRef().update3DSounds(gameContext);
}

void SoundSystem::updateSoundsParameters(GameEngine &gameContext) const {
    auto &allSoundComp = gameContext.entityMan.getComponents<SoundComponent>();
    for(auto &soundComp : allSoundComp) {
        if(soundComp.needUpdateVolume) {
            changeVolumeSoftly(gameContext, soundComp);
        }
        if(soundComp.needUpdatePitch) {
            changePitchSoftly(gameContext, soundComp);
        }
    }
}

void SoundSystem::changeVolumeSoftly(GameEngine &gameContext, SoundComponent &soundComp) const {

    soundComp.needUpdateVolume = true;
    float currentVolume = gameContext.getSoundFacadeRef().getVolumeEventByID(soundComp.id);

    if  (currentVolume > soundComp.volume) {//DECREASE
        gameContext.getSoundFacadeRef().setVolumeEventByID(soundComp.id, currentVolume - FACTOR);
        currentVolume -= FACTOR;

    } else if(currentVolume < soundComp.volume) {//INCREASE
        gameContext.getSoundFacadeRef().setVolumeEventByID(soundComp.id, currentVolume + FACTOR);
        currentVolume = gameContext.getSoundFacadeRef().getVolumeEventByID(soundComp.id);
        currentVolume += FACTOR;
    }


    if(compareFloats(currentVolume, soundComp.volume)) {
        soundComp.needUpdateVolume = false;
    }

}

void SoundSystem::changePitchSoftly(GameEngine &gameContext, SoundComponent &soundComp) const {
    soundComp.needUpdatePitch = true;
    float currentPitch = gameContext.getSoundFacadeRef().getPitchEventByID(soundComp.id);

    if  (currentPitch > soundComp.pitch) {//DECREASE
        gameContext.getSoundFacadeRef().setPitchEventByID(soundComp.id, currentPitch - FACTOR);
        currentPitch -= FACTOR;

    } else if(currentPitch < soundComp.pitch) {//INCREASE
        gameContext.getSoundFacadeRef().setPitchEventByID(soundComp.id, currentPitch + FACTOR);
        currentPitch = gameContext.getSoundFacadeRef().getPitchEventByID(soundComp.id);
        currentPitch += FACTOR;
    }


    if(compareFloats(currentPitch, soundComp.pitch)) {
        soundComp.needUpdatePitch = false;
    }

}


bool SoundSystem::compareFloats(float a, float b) const {
    return fabs(a - b) < FACTOR;
}