#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class SoundSystem : public System {
public:
    SoundSystem();
    ~SoundSystem();

    void update(GameEngine &gameContext) const override;
private:

	void setPause	(GameEngine &gameContext, bool pause) const;
	void updatePlayerSound	(GameEngine &gameContext) const;
	
	void updateMusicLinkToLife	(GameEngine &gameContext) const;
	void updateBiomeAmbient	(GameEngine &gameContext) const;
	
	void attackSound		(GameEngine &gameContext) const; 

	void updateEggsChant	(GameEngine &gameContext) const;
	void updateMenuSounds	(GameEngine &gameContext) const;
	void updateStateSound	(GameEngine &gameContext) const;


	void updateInterestPointsChant(GameEngine &gameContext) const;
	bool isDiscovered(GameEngine &gameContext, CollisionComponent &playerCollidable, IPDataComponent &ipComp) const;
	void stopSound(GameEngine &gameContext, uint32_t id) const;
	

	void update3DSounds			(GameEngine &gameContext) const;
	void updateSoundsParameters	(GameEngine &gameContext) const;
	void changeVolumeSoftly		(GameEngine &gameContext, SoundComponent &soundComp) const;
	void changePitchSoftly		(GameEngine &gameContext, SoundComponent &soundComp) const;

	bool compareFloats(float a, float b) const; 



};