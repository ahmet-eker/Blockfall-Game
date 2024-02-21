#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay

    bool check_for_collisions_entering_the_grid(Block* block, BlockFall &game);
    bool move_right_check_for_collisions(Block* block, BlockFall &game);
    bool move_left_check_for_collisions(Block* block, BlockFall &game);
    bool rotate_right_check_for_collisions(Block* block, BlockFall &game);
    bool rotate_left_check_for_collisions(Block* block, BlockFall &game);

    void print_grid(BlockFall &game);
    void rotate_right(BlockFall &game);
    void rotate_left(BlockFall &game);
    void move_right(BlockFall &game);
    void move_left(BlockFall &game);

    bool find_powerups(BlockFall &game);
    void clear_powerups_and_add_score(BlockFall &game);
    int find_full_row(BlockFall &game);
    void clear_full_rows_and_add_score(BlockFall &game);
    void add_drop_score(BlockFall &game,int index_number);
    int drop(BlockFall &game);
    int how_many_blocks(vector<vector<bool>> block_matrix);
    int how_many_blocks_grid(vector<vector<int>> block_matrix);
    void gravity_switch(BlockFall &game);
    void apply_gravity(BlockFall &game);
};


#endif //PA2_GAMECONTROLLER_H
