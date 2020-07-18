#include "Sprite.h"

game::Sprite::Sprite(const char* filePath) {
    texture.loadFromFile(filePath);
    this->setTexture(texture);
}

game::Sprite::Sprite(const char* filePath, float x, float y) {
    texture.loadFromFile(filePath);
    this->setTexture(texture);
    this->setPosition(x, y);
}

game::Sprite::~Sprite() {
}
