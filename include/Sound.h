#include <SFML/Audio.hpp>

namespace game {
class Sound : public sf::Sound {
    sf::SoundBuffer buffer;

   public:
    Sound(const char* filePath);
    virtual ~Sound();
};
}  // namespace game