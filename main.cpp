#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <string>
#include "include/Game.h"
#include "include/Sound.h"
#include "include/Sprite.h"
#include "include/Text.h"

int main() {
    /* Create video mode */
    sf::VideoMode vm(SCREEN_WIDTH, SCREEN_HEIGHT);

    /* Create window object and open a window */
    sf::RenderWindow window(vm, "Timber", sf::Style::Fullscreen);

    game::Sprite spriteBackground("asset/graphic/background.png", 0, 0);
    game::Sprite spriteTree("asset/graphic/tree.png", 810, 0);
    game::Sprite spriteBee("asset/graphic/bee.png", -500, 0);
    game::Sprite spriteCloud1("asset/graphic/cloud.png", -500, 0);
    game::Sprite spriteCloud2("asset/graphic/cloud.png", -500, 0);
    game::Sprite spriteCloud3("asset/graphic/cloud.png", -500, 0);

    std::array<game::Sprite, NUM_BRANCHES> spriteBranches = {
        game::Sprite("asset/graphic/branch.png", -500, 0),
        game::Sprite("asset/graphic/branch.png", -500, 0),
        game::Sprite("asset/graphic/branch.png", -500, 0),
        game::Sprite("asset/graphic/branch.png", -500, 0),
        game::Sprite("asset/graphic/branch.png", -500, 0),
        game::Sprite("asset/graphic/branch.png", -500, 0)};

    for (int i = 0; i < NUM_BRANCHES; i++) {
        /* Set the sprite's origin to dead centre, we can then spin it round without changing its position */
        spriteBranches[i].setOrigin(220, 20);
    }

    game::Sprite spritePlayer("asset/graphic/player.png", -500, 0);
    game::Sprite spriteAxe("asset/graphic/axe.png", -500, 0);
    game::Sprite spriteLog("asset/graphic/log.png", -500, 0);
    game::Sprite spriteRip("asset/graphic/rip.png", -500, 0);

    game::Sound soundChop("asset/sound/chop.wav");
    game::Sound soundDeath("asset/sound/death.wav");
    game::Sound soundOutOfTime("asset/sound/out_of_time.wav");

    game::Text textMessage("asset/font/komikap.ttf", 100, sf::Color::White);
    game::Text textScore("asset/font/komikap.ttf", 75, sf::Color::White);

    int score = 0;
    std::string scoreString = std::string("Score = ") + std::to_string(score);

    textScore.setString(scoreString);
    textMessage.setString("Press enter to start");

    textScore.setPosition(20, 20, game::origin::LEFT_TOP);
    textMessage.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, game::origin::MIDDLE);

    std::array<Side, NUM_BRANCHES> branchPositions;
    Side playerSide = Side::LEFT;
    Side prevPlayerSide = Side::LEFT;

    std::array<bool, sf::Keyboard::KeyCount> previousKeyState;
    previousKeyState.fill(false);

    sf::RectangleShape rectTimeBar;
    rectTimeBar.setSize(sf::Vector2f(TIMEBAR_WIDTH, TIMEBAR_HEIGHT));
    rectTimeBar.setFillColor(sf::Color::Red);
    rectTimeBar.setPosition(SCREEN_WIDTH - 30, 100);
    rectTimeBar.setRotation(180);

    sf::Clock clock;
    bool isPaused = true;
    bool isGameOver = false;

    float remainingTime = GAME_TIME;
    float timeBarWidthPerSecond = TIMEBAR_WIDTH / remainingTime;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        /*
        *******************************************
        * Handle player input
        * *****************************************
        */

        /* Check all the window's events that were triggered since the last iteration of the loop */
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }

                    if (event.key.code == sf::Keyboard::Left && !previousKeyState[sf::Keyboard::Left]) {
                        score++;
                        spriteLog.active(true);
                        playerSide = Side::LEFT;

                        if (!isPaused && !isGameOver) {
                            soundChop.play();
                        }

                        updateBranches(score, branchPositions);
                        previousKeyState[sf::Keyboard::Left] = true;
                    }

                    if (event.key.code == sf::Keyboard::Right && !previousKeyState[sf::Keyboard::Right]) {
                        score++;
                        spriteLog.active(true);
                        playerSide = Side::RIGHT;

                        if (!isPaused && !isGameOver) {
                            soundChop.play();
                        }

                        updateBranches(score, branchPositions);
                        previousKeyState[sf::Keyboard::Right] = true;
                    }

                    if (event.key.code == sf::Keyboard::Enter && !previousKeyState[sf::Keyboard::Enter]) {
                        isPaused = !isPaused;

                        if (isGameOver) {
                            score = 0;
                            isGameOver = false;
                            remainingTime = GAME_TIME;

                            textMessage.setString("Press enter to start");
                            textMessage.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, game::origin::MIDDLE);
                        }

                        spriteRip.setPosition(-500, 0);
                        branchPositions.fill(Side::NONE);

                        previousKeyState[sf::Keyboard::Enter] = true;
                    }
                    break;

                case sf::Event::KeyReleased:
                    if (event.key.code == sf::Keyboard::Left && previousKeyState[sf::Keyboard::Left]) {
                        previousKeyState[sf::Keyboard::Left] = false;
                    }

                    if (event.key.code == sf::Keyboard::Right && previousKeyState[sf::Keyboard::Right]) {
                        previousKeyState[sf::Keyboard::Right] = false;
                    }

                    if (event.key.code == sf::Keyboard::Enter && previousKeyState[sf::Keyboard::Enter]) {
                        previousKeyState[sf::Keyboard::Enter] = false;
                    }
                    break;

                default:
                    break;
            }
        }

        /*
        *******************************************
        * Update scene
        * *****************************************
        */
        if (!isPaused && !isGameOver) {
            scoreString = std::string("Score = ") + std::to_string(score);
            textScore.setString(scoreString);

            remainingTime -= deltaTime.asSeconds();
            rectTimeBar.setSize(sf::Vector2f(remainingTime * timeBarWidthPerSecond, TIMEBAR_HEIGHT));

            if (remainingTime <= 0.0f) {
                textMessage.setString("Game Over");
                textMessage.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, game::origin::MIDDLE);

                isGameOver = true;
                soundOutOfTime.play();
            }

            if (!spriteBee.active()) {
                srand((int)time(0) * 10);
                spriteBee.speed((rand() % 200) + 200);

                srand((int)time(0) * 100);
                float beeHigh = (rand() % 500) + 500;

                spriteBee.setPosition(-500, beeHigh);
                spriteBee.active(true);
            } else {
                auto beeXPos = deltaTime.asSeconds() * spriteBee.speed();
                spriteBee.setPosition(spriteBee.getPosition().x + beeXPos, spriteBee.getPosition().y);

                if (spriteBee.getPosition().x > SCREEN_WIDTH) {
                    spriteBee.active(false);
                }
            }

            if (!spriteCloud1.active()) {
                srand(time(0) * 10);
                float cloudHeight = (rand() % 200);
                spriteCloud1.setPosition(-500, cloudHeight);
                spriteCloud1.active(true);
                spriteCloud1.speed((rand() % 100) * 2);
            } else {
                auto cloud1XPos = deltaTime.asSeconds() * spriteCloud1.speed();
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + cloud1XPos, spriteCloud1.getPosition().y);

                if (spriteCloud1.getPosition().x > SCREEN_WIDTH) {
                    spriteCloud1.active(false);
                }
            }

            if (!spriteCloud2.active()) {
                srand(time(0) * 20);
                float cloudHeight = (rand() % 200);
                spriteCloud2.setPosition(-500, cloudHeight);
                spriteCloud2.active(true);
                spriteCloud2.speed((rand() % 100) * 2);
            } else {
                auto cloud2XPos = deltaTime.asSeconds() * spriteCloud2.speed();
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + cloud2XPos, spriteCloud2.getPosition().y);

                if (spriteCloud2.getPosition().x > SCREEN_WIDTH) {
                    spriteCloud2.active(false);
                }
            }

            if (!spriteCloud3.active()) {
                srand(time(0) * 30);
                float cloud3Height = (rand() % 200);
                spriteCloud3.setPosition(-500, cloud3Height);
                spriteCloud3.active(true);
                spriteCloud3.speed((rand() % 100) * 2);

            } else {
                auto cloud3XPos = deltaTime.asSeconds() * spriteCloud3.speed();
                spriteCloud3.setPosition(spriteCloud3.getPosition().x + cloud3XPos, spriteCloud3.getPosition().y);

                if (spriteCloud3.getPosition().x > SCREEN_WIDTH) {
                    spriteCloud3.active(false);
                }
            }

            if (!spriteLog.active()) {
                spriteLog.speed(4000);
                spriteLog.setPosition(810, 750);
            } else if (prevPlayerSide != playerSide) {
                spriteLog.speed(4000);
                spriteLog.setPosition(810, 750);
            } else {
                float logXMove = deltaTime.asSeconds() * spriteLog.speed();
                float logX = (playerSide == Side::LEFT) ? spriteLog.getPosition().x - logXMove : spriteLog.getPosition().x + logXMove;
                spriteLog.setPosition(logX, 750);

                if (spriteLog.getPosition().x > SCREEN_WIDTH || spriteLog.getPosition().x + 100 < 0) {
                    spriteLog.active(false);
                }
            }

            prevPlayerSide = playerSide;

            // Update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++) {
                float height = i * 150;

                if (branchPositions[i] == Side::LEFT) {
                    spriteBranches[i].setPosition(600, height);
                    // Flip the sprite round the other way
                    spriteBranches[i].setRotation(180);
                } else if (branchPositions[i] == Side::RIGHT) {
                    // Move the sprite to the right side
                    spriteBranches[i].setPosition(1330, height);
                    // Set the sprite rotation to normal
                    spriteBranches[i].setRotation(0);

                } else {
                    // Hide the branch
                    spriteBranches[i].setPosition(3000, height);
                }
            }

            if (playerSide == Side::LEFT) {
                spritePlayer.setScale(-1, 1);
                spritePlayer.setPosition(710, 700);
                spriteAxe.setScale(-1, 1);
                spriteAxe.setPosition(840, 810);
            } else if (playerSide == Side::RIGHT) {
                spritePlayer.setScale(1, 1);
                spritePlayer.setPosition(1200, 700);
                spriteAxe.setScale(1, 1);
                spriteAxe.setPosition(1080, 810);
            } else {
                spritePlayer.setPosition(-500, 0);
                spriteAxe.setPosition(-500, 0);
            }

            /* Check for collision */
            if (branchPositions[NUM_BRANCHES - 1] == playerSide) {
                isGameOver = true;
                soundDeath.play();
                textMessage.setString("Game Over");
                textMessage.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, game::origin::MIDDLE);

                if (playerSide == Side::LEFT) {
                    spriteRip.setPosition(550, 745);

                } else {
                    spriteRip.setPosition(1186, 748);
                }

                spritePlayer.setPosition(-500, 0);
                spriteAxe.setPosition(-500, 0);
                spriteLog.setPosition(-500, 0);
                spriteLog.active(false);
            }
        }

        /*
        *******************************************
        * Draw scene
        *******************************************
        */

        /* Clear everything from last frame*/
        window.clear();

        /* Draw our scene here*/
        window.draw(spriteBackground);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        window.draw(spriteTree);
        window.draw(spriteLog);
        window.draw(spriteBee);
        window.draw(textScore);
        window.draw(spriteAxe);
        window.draw(spritePlayer);

        /* Draw the branches */
        for (int i = 0; i < NUM_BRANCHES; i++) {
            window.draw(spriteBranches[i]);
        }

        window.draw(spriteRip);
        window.draw(rectTimeBar);

        if (isPaused || isGameOver) {
            window.draw(textMessage);
        }

        /* Render what we just drew*/
        window.display();
    }
    return 0;
}