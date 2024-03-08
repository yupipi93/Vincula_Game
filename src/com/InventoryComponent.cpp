#include "InventoryComponent.hpp"

InventoryComponent:: InventoryComponent(const uint32_t id)
    : Component(id) {}

InventoryComponent::~InventoryComponent() {}

void InventoryComponent::setMaxKeys(const uint8_t keys) {
    MAX_KEYS = keys;
    keysCollected.reserve(MAX_KEYS);
}
