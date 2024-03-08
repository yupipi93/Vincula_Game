#pragma once

enum EntityType {
    PLAYER,         //00        
    ENEMY,          //01   
    CAMERA,         //02
    OBSTACLE,       //03 
    COIN,           //04 
    SOURCE,         //05 
    ATTACK,         //06 
    DOOR,           //07 
    WALL,           //08 
    WALLCOLLISION,  //09 
    AIDIRECTOR,     //10 
    TERRAINCUBE,    //11 
    HUDS,           //12 
    TERRAIN,        //13 
    INTERESTPOINT,  //14 
    MELEEWEAPON,    //15 
    DISTANCEWEAPON, //16
    KEY,            //17
    WAYPOINT,       //18
    KEYEGG,         //19
    CHEST,          //20
    OBSTACLEDATA,   //21
    INFOBOX,        //22
    MESSAGEBOX,     //23
    CONTROLSWALL,   //24
    NOTIFICATION,   //25
    BANK,           //26
    BANK_DEPOSITOR, //27
    BANK_EXTRACTOR, //28
    COMPASS,        //29
    SECUREZONE,     //30
    NPC,            //31
    MAP,            //32
    VILLAGE_OBSTACLE, //33
    INTERESTPOINTPART,//34 
    DETECTIONSIGN,  //35
    SOUNDEVENT,     //36
    LIGHT,          //37
    SHAPE_MOUSE,    //38
    PALANTIR,       //39
};