#include "VibrateEngine.hpp"
#include <string>

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>

//#####################################################
//                == FMOD ENGINE ==
//  - Load Banks
//  - Load Event Descriptions
//  - Create Instances
//  - Pocess sound in game
//
//   Using Componets:
//      - Sound         (Read)
//      - Soituation    (Read)
//#####################################################



const bool  SHOW_PERFORMANCE    = false;
const bool  SHOW_EVENTS         = false;
const float MFACTOR             = 0.1;

const std::string BANK_PATH     = "./FmodStudioProyect/Build/Desktop/";
const std::string EVENT_PATH    = "event:/";
const std::string BUS_PATH      = "bus:/";
const bool LOAD_IN_RAM          = true;
const uint32_t MAX_PATH_LENGTH  = 2048;



VibrateEngine::VibrateEngine() {
    int maxchannels         = 2048;
    float dopplerscale      = 1.f;
    float distancefactor    = 1.f;
    float rolloffscale      = 1.f;
    ErrorCheck(FMOD_Studio_System_Create(&mSystem, FMOD_VERSION));
    if (mSystem) {
        ErrorCheck(FMOD_Studio_System_Initialize(mSystem, maxchannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));
        //ErrorCheck(FMOD_Studio_System_Update(mSystem));
    }

    //ErrorCheck( FMOD_Studio_System_GetCoreSystem(mSystem, &coreSystem) );


    // Load the master banks (strings first)
    loadBank("Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
    loadBank("Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
}

VibrateEngine::~VibrateEngine() {
    stopAllInstances();
    unloadAllBanks();

    if (mSystem) {
        ErrorCheck( FMOD_Studio_System_UnloadAll(mSystem));
        ErrorCheck( FMOD_Studio_System_Release(mSystem));
    }


    mBanks.clear();
    mEvents.clear();
    mEventInstances.clear();
    mInstancesToUpdate.clear();
}




void VibrateEngine::update(GameEngine &gameContext) {

    updateListener(gameContext);
    deleteStopedInstances();
    ErrorCheck( FMOD_Studio_System_Update(mSystem));

    if(SHOW_PERFORMANCE) {
        showPerformanceData();
    }

    if(SHOW_EVENTS) {
        showEventNames();
    }

}


void VibrateEngine::update3DSounds(GameEngine &gameContext) {
    for(uint32_t entityId : mInstancesToUpdate) {
        set3DAttributes(gameContext, entityId);
    }
    mInstancesToUpdate.clear();
}





void VibrateEngine::setVolumeEventByID(uint32_t id, float volume) const {
    if(volume < 0.f) {
        volume = 0.f;
    } else if(volume > 1.f) {
        volume = 1.f;
    }

    FMOD_STUDIO_EVENTINSTANCE *instance = nullptr;
    instance = getEventInstanceByID(id);
    if(instance) {
        ErrorCheck(FMOD_Studio_EventInstance_SetVolume(instance, volume));
    }
}

float VibrateEngine::getVolumeEventByID(uint32_t id) const {
    float volume = 0.f;
    float finalVolume = 0.f;

    FMOD_STUDIO_EVENTINSTANCE *instance = nullptr;
    instance = getEventInstanceByID(id);
    if(instance) {
        ErrorCheck(FMOD_Studio_EventInstance_GetVolume(instance, &volume, &finalVolume));
    }
    return volume;
}

void VibrateEngine::setPitchEventByID(uint32_t id, float pitch) const {
    if(pitch < 0.f) {
        pitch = 0.f;
    }
    FMOD_STUDIO_EVENTINSTANCE *instance = nullptr;
    instance = getEventInstanceByID(id);
    if(instance) {
        ErrorCheck(FMOD_Studio_EventInstance_SetPitch(instance, pitch));
    }
}

float VibrateEngine::getPitchEventByID(uint32_t id) const {
    float pitch = 0.f;
    float finalpitch = 0.f;

    FMOD_STUDIO_EVENTINSTANCE *instance = nullptr;
    instance = getEventInstanceByID(id);
    if(instance) {
        ErrorCheck(FMOD_Studio_EventInstance_GetPitch(instance, &pitch, &finalpitch));
    }
    return pitch;
}







//BUS

void VibrateEngine::setVolumeBusByName(std::string name, float volume) const {
    if(volume < 0.f) {
        volume = 0.f;
    } else if(volume > 1.f) {
        volume = 1.f;
    }
    FMOD_STUDIO_BUS *bus = getBusByName(name);
    if(bus) {
        ErrorCheck(FMOD_Studio_Bus_SetVolume(bus, volume));

    }

}

float VibrateEngine::getVolumeBusByName(std::string name) const {
    float volume = 0.f;
    float finalVolume = 0.f;

    FMOD_STUDIO_BUS *bus = getBusByName(name);
    if(bus) {
        ErrorCheck(FMOD_Studio_Bus_GetVolume(bus, &volume, &finalVolume));

    }


    return volume;
}





void VibrateEngine::setPauseBusByName(std::string name, bool pause) const {
    FMOD_STUDIO_BUS *bus = nullptr;
    bus = getBusByName(name);
    if(bus) {
        ErrorCheck(FMOD_Studio_Bus_SetPaused(bus, pause));

    }







}


FMOD_STUDIO_BUS *VibrateEngine::getBusByName(std::string busName) const {
    FMOD_STUDIO_BUS *bus = nullptr;
    std::string busFullPath = BUS_PATH + busName;
    ErrorCheck(FMOD_Studio_System_GetBus(mSystem, busFullPath.data(), &bus));
    if(!FMOD_Studio_Bus_IsValid(bus)) {
        return nullptr;
    }
    return bus;
}


void VibrateEngine::setPausedEventByID(uint32_t id, bool pause) const {
    auto instance = getEventInstanceByID(id);
    if (instance) {
        ErrorCheck(FMOD_Studio_EventInstance_SetPaused(instance, pause));
    }
}







bool VibrateEngine::isStopedInstance(FMOD_STUDIO_EVENTINSTANCE *instance)const {
    FMOD_STUDIO_PLAYBACK_STATE state;
    ErrorCheck(FMOD_Studio_EventInstance_GetPlaybackState(instance, &state));
    if (state == FMOD_STUDIO_PLAYBACK_STOPPED) {
        return true;
    }
    return false;
}



void VibrateEngine::set3DAttributes(GameEngine &gameContext, uint32_t instanceId) const {
    FMOD_STUDIO_EVENTINSTANCE *instance = nullptr;
    instance = getEventInstanceByID(instanceId);
    if(instance) {
        if(is3DInstance(instance)) {
            auto &situationComp = gameContext.entityMan.getComponent<SituationComponent>(instanceId);
            FMOD_3D_ATTRIBUTES attr3D = create3DAttributes(gameContext, situationComp.x, situationComp.y, situationComp.z);
            ErrorCheck(FMOD_Studio_EventInstance_Set3DAttributes(instance, &attr3D));
        }
    }

}


void VibrateEngine::add3DSoundToUpdate(std::vector<uint32_t> ids) {
    for(uint32_t id : ids) {
        mInstancesToUpdate.emplace_back(id);
    }

}



void VibrateEngine::playEvent(GameEngine &gameContext, uint32_t id) {
    if(mEventInstances.find(id) != mEventInstances.end()) return;//avoid duplicates

    if(gameContext.entityMan.existsComponent<SoundComponent>(id) ) {


        auto &soundComp    {gameContext.entityMan.getComponent<SoundComponent>(id)};
        std::string eventName   {soundComp.eventName};

        FMOD_STUDIO_EVENTDESCRIPTION *eventDescription = nullptr;
        eventDescription = getEventByName(eventName);
        if(eventDescription) {
            FMOD_STUDIO_EVENTINSTANCE *instance = nullptr;
            instance = createEventInstance(id, eventDescription);
            if(instance) {
                if(soundComp.initialVolume != 1.f) {
                    ErrorCheck(FMOD_Studio_EventInstance_SetVolume(instance, soundComp.initialVolume));
                }
                if(soundComp.initialPitch != 1.f) {
                    ErrorCheck(FMOD_Studio_EventInstance_SetPitch(instance, soundComp.initialPitch));
                }


                ErrorCheck(FMOD_Studio_EventInstance_Start(instance));
                set3DAttributes(gameContext, id);
            }
        }
    }
}

void VibrateEngine::playShootSound(std::string eventName) {
    FMOD_STUDIO_EVENTDESCRIPTION *eventDescription = nullptr;
    eventDescription = getEventByName(eventName);
    if(eventDescription) {
        FMOD_STUDIO_EVENTINSTANCE *instance = nullptr;
        ErrorCheck(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &instance));
        if(instance) {
            //mEventInstances.emplace(id, instance);
            ErrorCheck(FMOD_Studio_EventInstance_Start(instance));
        }
    }

}

FMOD_STUDIO_EVENTINSTANCE *VibrateEngine::createEventInstance(uint32_t id, FMOD_STUDIO_EVENTDESCRIPTION *eventDescription) {
    FMOD_STUDIO_EVENTINSTANCE *instance = nullptr;
    ErrorCheck(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &instance));
    if(instance) {
        mEventInstances.emplace(id, instance);
    }
    return instance;


}


void VibrateEngine::stopEventByID(uint32_t id, bool allowFadeOut) const {
    FMOD_STUDIO_EVENTINSTANCE *instance = nullptr;
    instance = getEventInstanceByID(id);
    if (instance) {
        FMOD_STUDIO_STOP_MODE mode = allowFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;
        ErrorCheck(FMOD_Studio_EventInstance_Stop(instance, mode));
    }
}

void VibrateEngine::setParameterEventByID(uint32_t id, float value, uint32_t index) const {
    bool ignoreSeekSpeed = false;
    FMOD_STUDIO_PARAMETER_ID parameterId;

    FMOD_STUDIO_EVENTINSTANCE *instance = nullptr;
    instance = getEventInstanceByID(id);
    if (instance) {
        parameterId = getParameterIdByIndex(instance, index);
        FMOD_Studio_EventInstance_SetParameterByID(instance, parameterId, value, ignoreSeekSpeed);
    }


}



FMOD_STUDIO_PARAMETER_ID VibrateEngine::getParameterIdByIndex(FMOD_STUDIO_EVENTINSTANCE *instance, uint32_t index) const {
    FMOD_STUDIO_PARAMETER_DESCRIPTION parameterDescription;
    FMOD_STUDIO_PARAMETER_ID parameterId;
    FMOD_STUDIO_EVENTDESCRIPTION *eventDescription = nullptr;

    if(instance)
        ErrorCheck(FMOD_Studio_EventInstance_GetDescription(instance, &eventDescription));

    if(eventDescription)
        ErrorCheck(FMOD_Studio_EventDescription_GetParameterDescriptionByIndex(eventDescription, index, &parameterDescription));

    parameterId = parameterDescription.id;
    return parameterId;
}





std::vector<std::string> VibrateEngine::getEventList() const {
    char eventName[MAX_PATH_LENGTH];
    std::vector<std::string> names;

    //get all names
    for(auto &event : mEvents) {
        ErrorCheck(FMOD_Studio_EventDescription_GetPath(event.second, eventName, MAX_PATH_LENGTH, nullptr));
        names.emplace_back(eventName);
    }

    //split path
    for(auto &name :  names) {
        size_t pos = name.find("/");
        name = name.substr(++pos);
    }

    return names;
}






void VibrateEngine::stopAllInstances() {
    for(auto &instance : mEventInstances) {
        ErrorCheck(FMOD_Studio_EventInstance_Stop(instance.second, FMOD_STUDIO_STOP_IMMEDIATE));
    }
}


void VibrateEngine::loadBank(const std::string &bankName, uint32_t flags) {
    if (mBanks.find(bankName) != mBanks.end()) return;//avoid duplicates

    FMOD_STUDIO_BANK *bank = nullptr;
    ErrorCheck(FMOD_Studio_System_LoadBankFile(mSystem, (BANK_PATH + bankName).data(), flags, &bank));
    if (bank) {
        mBanks.emplace(bankName, bank);

        if(LOAD_IN_RAM) {
            FMOD_Studio_Bank_LoadSampleData(bank);// Load all non-streaming sample data
        }

        loadAllEventDescriptions(bank);

    }
}



void VibrateEngine::loadAllEventDescriptions(FMOD_STUDIO_BANK *bank) {
    int numEvents = 0;

    ErrorCheck(FMOD_Studio_Bank_GetEventCount(bank, &numEvents));

    if (numEvents > 0) {
        std::vector<FMOD_STUDIO_EVENTDESCRIPTION *> eventsList(numEvents);
        ErrorCheck(FMOD_Studio_Bank_GetEventList(bank, eventsList.data(), numEvents, &numEvents));

        for (auto &event : eventsList) {
            createEventDescriptions(event);
        }

    }
}

void VibrateEngine::createEventDescriptions(FMOD_STUDIO_EVENTDESCRIPTION *eventDescription) {
    char eventName[MAX_PATH_LENGTH];

    ErrorCheck(FMOD_Studio_EventDescription_GetPath(eventDescription, eventName, MAX_PATH_LENGTH, nullptr));
    mEvents.emplace(eventName, eventDescription);
}







FMOD_STUDIO_EVENTDESCRIPTION *VibrateEngine::getEventByName(std::string name) const {
    FMOD_STUDIO_EVENTDESCRIPTION *eventDescription = nullptr;
    auto iter = mEvents.find((EVENT_PATH + name));
    if (iter != mEvents.end()) {
        eventDescription = iter->second;
        if(!FMOD_Studio_EventDescription_IsValid(eventDescription)) {
            return nullptr;
        }
    }
    return eventDescription;
}


FMOD_STUDIO_EVENTINSTANCE *VibrateEngine::getEventInstanceByID(uint32_t id) const {
    FMOD_STUDIO_EVENTINSTANCE *instance = nullptr;
    auto iter = mEventInstances.find(id);
    if (iter != mEventInstances.end()) {
        instance = iter->second;
        if(!FMOD_Studio_EventInstance_IsValid(instance)) {
            return nullptr;
        }

    }
    return instance;
}







void VibrateEngine::setListenerId(uint32_t id) {
    listenerId = id;
}

void VibrateEngine::updateListener(GameEngine &gameContext) const {
    auto &situationComponent {gameContext.entityMan.getComponent<SituationComponent>(listenerId)};
    setListener3DAttributes(gameContext, situationComponent.x, situationComponent.y, situationComponent.z);

}


void VibrateEngine::setListener3DAttributes(GameEngine &gameContext, float x, float y, float z) const {
    FMOD_3D_ATTRIBUTES attr3D = create3DAttributes(gameContext, x, y, z);
    // Here we have to manage if it is Manajaro or Windows
    ErrorCheck(FMOD_Studio_System_SetListenerAttributes(mSystem, 0, &attr3D, nullptr));
}



FMOD_3D_ATTRIBUTES VibrateEngine::create3DAttributes(GameEngine &gameContext, float x, float y, float z) const {
    float pAngle = getAngle(gameContext);

    FMOD_3D_ATTRIBUTES attr3D;
    attr3D.position     = getFmodVector(x * MFACTOR, y * MFACTOR, z * MFACTOR);
    attr3D.up           = {0.0f, 1.f, 0.0f};
    attr3D.forward      = {sin(pAngle), 0.0f, cos(pAngle)};
    attr3D.velocity     = {0.0f, 0.0f, 0.0f}; //Only using in Doppler effect
    return attr3D;

}

float VibrateEngine::getAngle(GameEngine &gameContext) const {
    float angle = 0.f;
    if(gameContext.playerId != 0) {
        InputComponent &playerInput = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);
        if(Utils::isSmartCamera(playerInput)) {
            SituationComponent &playerSituation = gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId);
            angle = atan2(playerSituation.x, playerSituation.z);
        }
    }
    return angle;
}

FMOD_VECTOR VibrateEngine::getFmodVector (float x, float y, float z) const {
    /*
    //TO OPENGL
    // Convert from our coordinates (+x forward, +y right, +z up)
    // to FMOD (+z forward, +x right, +y up)
    FMOD_VECTOR fmodVector;
    fmodVector.x = y;
    fmodVector.y = z;
    fmodVector.z = x;
    */

    //TO IRRLICH
    FMOD_VECTOR fmodVector;
    fmodVector.x = x;
    fmodVector.y = y;
    fmodVector.z = z;
    return fmodVector;
}




bool VibrateEngine::is3DInstance(FMOD_STUDIO_EVENTINSTANCE *instance) const {
    FMOD_BOOL ret = false;
    if (instance) {
        FMOD_STUDIO_EVENTDESCRIPTION *eventDescription = nullptr;
        ErrorCheck(FMOD_Studio_EventInstance_GetDescription(instance, &eventDescription));
        if (eventDescription) {
            FMOD_Studio_EventDescription_Is3D(eventDescription, &ret);//?????????
        }
    }
    return ret;
}





void VibrateEngine::deleteStopedInstances() {
    std::vector<uint32_t> toErase;
    for (auto &iter : mEventInstances) {

        FMOD_STUDIO_EVENTINSTANCE *instance = iter.second;

        if(instance) {
            if(!FMOD_Studio_EventInstance_IsValid(instance)) {
                ErrorCheck(FMOD_Studio_EventInstance_Release(instance));
                toErase.emplace_back(iter.first);
            } else if (isStopedInstance(instance)) {
                ErrorCheck(FMOD_Studio_EventInstance_Release(instance));
                toErase.emplace_back(iter.first);
            }
        }
    }

    for (auto id : toErase) {
        mEventInstances.erase(id);
    }
}

void VibrateEngine::unloadAllBanks() {
    for (auto &bank : mBanks) {
        // Unload the sample data, then the bank itself
        ErrorCheck(FMOD_Studio_Bank_UnloadSampleData(bank.second));
        ErrorCheck(FMOD_Studio_Bank_Unload(bank.second));
    }

}


void VibrateEngine::showPerformanceData() const {
    FMOD_STUDIO_CPU_USAGE cpuUsage;
    int currentAlloc = 0;
    int maxAlloc = 0;
    FMOD_BOOL blocking = false;


    ErrorCheck(FMOD_Studio_System_GetCPUUsage(mSystem, &cpuUsage));


    ErrorCheck(FMOD_Memory_GetStats(&currentAlloc, &maxAlloc, blocking));

}

void VibrateEngine::showEventNames() const {

}



bool VibrateEngine::ErrorCheck(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        return true;
    }
    return false;
}