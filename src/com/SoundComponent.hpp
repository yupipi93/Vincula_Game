#pragma once

#include "Component.hpp"
#include <string>

class SoundComponent : public Component {
public:
    explicit SoundComponent(const uint32_t id);
    ~SoundComponent() override;

    void setVolume(float pVolume);
	void setPitch(float pPitch);

    std::string	eventName		{"0"};
    float  	initialVolume		{1};
    float  	initialPitch		{1};

    float  	volume 				{1};
    bool 	needUpdateVolume	{false};
    float  	pitch				{1};
    bool 	needUpdatePitch		{false};
};
