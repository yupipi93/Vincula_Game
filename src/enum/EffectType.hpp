#pragma once

enum EffectType {
    //Efectos provisionales 
    NOEFFECT,       //Sin efecto;
    ROTATIVE,       //Rotateeffect;
    POISON,         //Resta vida; 
    PARALYSIS,      //Estunea;
    SLOWDOWN,       //Ralentiza;
    WINHEART,       //~Enamorar: el enemigo con este efecto sera hostil contra el resto de enemigos y no atacara al player
    MAGNIFYBULLET,  //MAGNIFICAR: la bala aumenta de tamanyo con el tiempo 
    SMARTBULLET,     //Bala inteligente: si detecta el sensor de un enemigo va hacia el                                  
    HEALTHSTEAL
};