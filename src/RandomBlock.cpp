#include "RandomBlock.h"

int RandomBlock::getBlock() {
    if (blocks.empty()) {
        resetBag();
    }
    int rand = GetRandomValue(0, blocks.size()-1);
    int num = blocks[rand];
    std::vector<int>::iterator it = blocks.begin()+rand;
    blocks.erase(it);
}

void RandomBlock::resetBag() {
    for (int i = 2; i < 16; i++) {
        blocks.push_back(i/2);
    }
}