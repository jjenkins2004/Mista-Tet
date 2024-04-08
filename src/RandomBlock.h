#include <raylib.h>
#include <random>
#include <vector>

struct RandomBlock {
    public:
        //chooses from bag of 14 blocks
        int getBlock();

    private:
        //resets our bag and adds blocks back in when empty
        void resetBag();
        std::vector<int> blocks;
};