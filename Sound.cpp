#include "include/Sound.h"

game::Sound::Sound(const char* filePath) {
    buffer.loadFromFile(filePath);
    this->setBuffer(buffer);
}

game::Sound::~Sound() {
}