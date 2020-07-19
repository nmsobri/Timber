const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const float TIMEBAR_WIDTH = 300;
const float TIMEBAR_HEIGHT = 60;

const float GAME_TIME = 15.0f;
const int NUM_BRANCHES = 6;

/* Player side, left or right */
enum class Side {
    LEFT,
    RIGHT,
    NONE
};

/* Function definition */
void updateBranches(int seed, std::array<Side, NUM_BRANCHES>& branchPositions) {
    // Move all the branches down one place
    for (int j = NUM_BRANCHES - 1; j > 0; j--) {
        branchPositions[j] = branchPositions[j - 1];
    }

    // Spawn a new branch at position 0
    srand((int)time(0) + seed);
    int r = (rand() % 5);

    switch (r) {
        case 0:
            branchPositions[0] = Side::LEFT;
            break;

        case 1:
            branchPositions[0] = Side::RIGHT;
            break;

        default:
            branchPositions[0] = Side::NONE;
            break;
    }
}