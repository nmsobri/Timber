#include <SFML/Graphics.hpp>

namespace game {

class Sprite : public sf::Sprite {
    sf::Texture texture;

   public:
    Sprite() = delete;
    Sprite(const char* filePath);
    Sprite(const char* filePath, float x, float y);
    virtual ~Sprite();
};

}  // namespace game
