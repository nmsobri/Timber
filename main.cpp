#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    /* Create video mode */
    sf::VideoMode vm(1920, 1080);

    /* Create window object and open a window */
    sf::RenderWindow window(vm, "Timber", sf::Style::Fullscreen);

    while (window.isOpen()) {
        /*
        *******************************************
        * Handle player input
        * *****************************************
        */
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        /*
        *******************************************
        * Update scene
        * *****************************************
        */

        /*
        *******************************************
        * Draw scene
        * *****************************************
        */

        /* Clear everything from last frame*/
        window.clear();

        /* Draw our scene here*/

        /* Render what we just drew*/
        window.display();
    }
    return 0;
}