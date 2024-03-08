#include "KeyboardReceiver.hpp"

KeyboardReceiver::OptionalIterator
KeyboardReceiver::getIterator(int key) noexcept {

    auto it = pressedKeys.find(key);
    if(it != pressedKeys.end())
        return it;

    return {};
}

bool KeyboardReceiver::isKeyPressed (int key) noexcept {

    if (auto it = getIterator(key))
        return (*it)->second;

    return false;
}

void KeyboardReceiver::setKeyValue(int key, bool value) noexcept {

	;

	if(auto it = getIterator(key))
		(*it)->second = value;

}