#pragma once

enum AiAction {
	
	NO_ACTION,      //0
    WANDER,			//STANDBY    1
    THREATEN,		//OFFENSIVE  2
    LUNGE,			//OFFENSIVE  3
    MELEE,			//OFFENSIVE  4
    DISTANCE,		//OFFENSIVE  5
    KEEPDISTANCE,	//OFFENSIVE  6 
    ESCAPE,			//DEFENSIVE  7
    REGROUP			//DEFENSIVE  8
};