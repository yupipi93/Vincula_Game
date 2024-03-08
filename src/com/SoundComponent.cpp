#include "SoundComponent.hpp"

SoundComponent::SoundComponent(const uint32_t id)
    : Component(id) {}

SoundComponent::~SoundComponent() {}


void SoundComponent::setVolume(float pVolume) {
    volume = pVolume;
    needUpdateVolume = true;
}


void SoundComponent::setPitch(float pPitch) {
    pitch = pPitch;
    needUpdatePitch = true;
}