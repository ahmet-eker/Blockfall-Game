#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>

using namespace std;

class Block {
public:

    vector<vector<bool>> shape; // Two-dimensional vector corresponding to the block's shape
    Block * right_rotation = nullptr; // Pointer to the block's clockwise neighbor block (its right rotation)
    Block * left_rotation = nullptr; // Pointer to the block's counter-clockwise neighbor block (its left rotation)
    Block * next_block = nullptr; // Pointer to the next block to appear in the game
    int index_on_grid = 0;

    bool operator==(const Block& other) const {
        bool equal = true;
        for (int i = 0; i < this->shape.size(); ++i) {
            for (int j = 0; j < this->shape[0].size(); ++j) {
                if(this->shape[i][j] != other.shape[i][j]){
                    equal = false;
                }
            }
        }

        if(equal){
            return true;
        }
        // TODO: Overload the == operator to compare two blocks based on their shapes
        return false;
    }

    bool operator!=(const Block& other) const {
        bool not_equal = false;
        for (int i = 0; i < this->shape.size(); ++i) {
            for (int j = 0; j < this->shape[0].size(); ++j) {
                if(this->shape[i][j] != other.shape[i][j]){
                    not_equal = true;
                }
            }
        }

        if(not_equal){
            return true;
        }
        // TODO: Overload the != operator to compare two blocks based on their shapes
        return false;
    }
};


#endif //PA2_BLOCK_H
