#pragma once
#include "fac/SoundFacade.hpp"

#include "fmod_studio.h"
#include "fmod.h"
#include "fmod_errors.h"

class VibrateEngine : public SoundFacade {
public:

    VibrateEngine();
    ~VibrateEngine();

    void update(GameEngine &gameContext) override;
    void playEvent(GameEngine &gameContext, uint32_t id) override;
    void playShootSound(std::string eventName) override;
    void stopEventByID(uint32_t id, bool allowFadeOut) const override;
    void setListenerId(uint32_t id) override;
    void setParameterEventByID(uint32_t id, float value, uint32_t index) const override;
    void setPauseBusByName(std::string name, bool pause) const override;
    void setPausedEventByID(uint32_t id, bool pause) const override;

    void setVolumeEventByID(uint32_t id, float volume) const override;
    float getVolumeEventByID(uint32_t id) const override;
    void setPitchEventByID(uint32_t id, float pitch) const override;
    float getPitchEventByID(uint32_t id) const override;

    void setVolumeBusByName(std::string name, float volume) const override;
    float getVolumeBusByName(std::string name) const override;

    void add3DSoundToUpdate(std::vector<uint32_t> ids) override;
    void update3DSounds(GameEngine &gameContext) override;
    std::vector<std::string> getEventList() const override;


private:
    void loadBank(const std::string &bankName, uint32_t flags);
    void loadAllEventDescriptions(FMOD_STUDIO_BANK *bank);
    void createEventDescriptions(FMOD_STUDIO_EVENTDESCRIPTION *event);
    FMOD_STUDIO_EVENTINSTANCE *createEventInstance(uint32_t id, FMOD_STUDIO_EVENTDESCRIPTION *event);

    FMOD_STUDIO_EVENTDESCRIPTION       *getEventByName(std::string name) const;
    FMOD_STUDIO_EVENTINSTANCE          *getEventInstanceByID(uint32_t id) const;
    FMOD_STUDIO_BUS                    *getBusByName(std::string busName) const;

    void stopAllInstances();
    bool isStopedInstance(FMOD_STUDIO_EVENTINSTANCE *instance) const;
    FMOD_STUDIO_PARAMETER_ID getParameterIdByIndex(FMOD_STUDIO_EVENTINSTANCE *instance, uint32_t index) const;
    void setListener3DAttributes(GameEngine &gameContext, float x, float y, float z) const;
    void set3DAttributes(GameEngine &gameContext, uint32_t instanceId) const;
    FMOD_3D_ATTRIBUTES create3DAttributes(GameEngine &gameContext, float x, float y, float z) const;
    float getAngle(GameEngine &gameContext) const;
    FMOD_VECTOR getFmodVector(float x, float y, float z) const;
    bool is3DInstance(FMOD_STUDIO_EVENTINSTANCE *instance) const;

    void updateListener(GameEngine &gameContext) const;
    void deleteStopedInstances();
    void unloadAllBanks();
    void showPerformanceData() const;
    void showEventNames() const;
    static bool ErrorCheck(FMOD_RESULT result);



    FMOD_STUDIO_SYSTEM *mSystem = nullptr;
    FMOD_SYSTEM *coreSystem = nullptr;
    std::unordered_map<std::string, FMOD_STUDIO_BANK *> mBanks;
    std::unordered_map<std::string, FMOD_STUDIO_EVENTDESCRIPTION *> mEvents;
    std::unordered_map<uint32_t, FMOD_STUDIO_EVENTINSTANCE *> mEventInstances;
    //linearMap<uint32_t, FMOD_STUDIO_EVENTINSTANCE*> mEventInstances;


    std::vector<uint32_t> mInstancesToUpdate;
    uint32_t listenerId = 0;



};