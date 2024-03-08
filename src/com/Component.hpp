#pragma once

#include <cstdint>

class Component {
public:
    explicit Component(uint32_t _id) : id(_id) {}
    virtual ~Component() = default;
    uint32_t	id 	{};
};
