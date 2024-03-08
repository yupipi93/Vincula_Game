#pragma once



#include <string>
#include <unordered_map>
#include <vector>


class GameEngine;

class SoundFacade {
public:

    virtual ~SoundFacade() = default;

    virtual void update(GameEngine &gameContext) = 0;

    virtual void playEvent(GameEngine &gameContext, uint32_t id) = 0;
    virtual void playShootSound(std::string eventName) = 0;
    virtual void setParameterEventByID(uint32_t id, float value, uint32_t index = 0) const = 0;
    virtual void setPauseBusByName(std::string name, bool pause=true) const = 0;
    virtual void setPausedEventByID(uint32_t id, bool pause=true) const = 0;
    
    virtual void setVolumeEventByID(uint32_t id, float volume) const = 0; 
    virtual float getVolumeEventByID(uint32_t id) const = 0; 
    virtual void setPitchEventByID(uint32_t id, float pitch) const = 0; 
    virtual float getPitchEventByID(uint32_t id) const = 0;

    virtual void setVolumeBusByName(std::string name, float volume) const = 0; 
    virtual float getVolumeBusByName(std::string name) const = 0; 


    virtual void stopEventByID(uint32_t id, bool allowFadeOut=true) const = 0;
    virtual void setListenerId(uint32_t id) = 0;
    virtual void add3DSoundToUpdate(std::vector<uint32_t> ids) = 0;
    virtual void update3DSounds(GameEngine &gameContext) = 0;
    virtual std::vector<std::string> getEventList() const = 0;

};