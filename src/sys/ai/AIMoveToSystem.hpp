#pragma once

#include <sys/System.hpp>

class AIMoveToSystem : public System {
public:
    AIMoveToSystem();
    ~AIMoveToSystem();

    void 	update(GameEngine &gameContext) const override;

private:
	void 	processEntities2D			(GameEngine 	&gameContext) const;	
	void 	processEntities3D			(GameEngine 	&gameContext) const;	
	float 	getAngleToTargetInY			(SituationComponent &entitySituation, SituationComponent &targetSituation, float targetSizeY) const; 
	void 	setSpeedDirectionToEntity2D	(VelocityComponent 	&entityVeloc, float angleXZ) const;
	void 	setSpeedDirectionToEntity3D	(VelocityComponent 	&entityVeloc, float angleXZ, float angleY) const;

};