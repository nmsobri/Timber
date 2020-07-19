#include "include/Text.h"

game::Text::Text(const char* filePath, size_t size, sf::Color color) {
    font.loadFromFile(filePath);
    this->setFont(font);
    this->setCharacterSize(size);
    this->setFillColor(color);
}

void game::Text::setPosition(float x, float y, origin o) {
    /* Get bounding box*/
    sf::FloatRect boundingBox = this->getLocalBounds();

    /* Set text point origin to the center of the text */
    if (o == origin::MIDDLE) {
        this->setOrigin(
            (boundingBox.left + boundingBox.width) / 2.0f,
            (boundingBox.top + boundingBox.height) / 2.0f
        );
    } else if (o == origin::LEFT_TOP) {
        this->setOrigin(boundingBox.left, boundingBox.top);
    } else {
        this->setOrigin(boundingBox.left + boundingBox.width, boundingBox.top);
    }

    sf::Text::setPosition(x, y);
}

game::Text::~Text() {
}