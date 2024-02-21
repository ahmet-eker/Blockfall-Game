#ifndef BLOCKFALL_H
#define BLOCKFALL_H

#define occupiedCellChar "██"
#define unoccupiedCellChar "▒▒"

#include <vector>
#include <string>

#include "Block.h"
#include "LeaderboardEntry.h"
#include "Leaderboard.h"

using namespace std;

class BlockFall {
public:

    BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name,
              const string &player_name);
    virtual ~BlockFall();

    int rows;  // Number of rows in the grid
    int cols;  // Number of columns in the grid
    vector<vector<int>> grid;  // 2D game grid
    vector<vector<bool>> power_up; // 2D matrix of the power-up shape
    Block * initial_block = nullptr; // Head of the list of game blocks. Must be filled up and initialized after a call to read_blocks()
    Block * active_rotation = nullptr; // Currently active rotation of the active block. Must start with the initial_block
    bool gravity_mode_on = false; // Gravity mode of the game
    unsigned long current_score = 0; // Current score of the game
    string leaderboard_file_name; // Leaderboard file name, taken from the command-line argument 5 in main
    string player_name; // Player name, taken from the command-line argument 6 in main
    Leaderboard leaderboard;

    void initialize_grid(const string & input_file); // Initializes the grid using the command-line argument 1 in main
    void read_blocks(const string & input_file); // Reads the input file and calls the read_block() function for each block;

    vector<string> read_block_files(string block_file);
    vector<vector<bool>> string_to_vector(string block);
    bool is_matrix_equal(vector<vector<bool>> matrix1, vector<vector<bool>> matrix2);
    int determine_how_many_rotation(Block* &block);
    void turn_right(vector<vector<bool>> &matrix);
    void turn_left(vector<vector<bool>>&matrix);
    void insert_new_block(vector<vector<bool>> new_shape, Block* &initial_block);
    void create_circular_doubly_linked_list(Block* &block);
    void print_matrix(const std::vector<std::vector<int>>& matrix);
    void print_matrix(const std::vector<std::vector<bool>>& matrix);
    void print_circular_linkedList(Block* head);
    void print_linked_list(Block* &initial_block);
    void print_matrix_symbol(const std::vector<std::vector<int>>& matrix);
    void print_matrix_symbol(const std::vector<std::vector<bool>>& matrix);
};



#endif // BLOCKFALL_H



