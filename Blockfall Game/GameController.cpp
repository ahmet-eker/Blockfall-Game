#include "GameController.h"
#include "BlockFall.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

bool GameController::play(BlockFall& game, const string& commands_file){

    std::ifstream commandFile(commands_file);

    std::string line;
    int status = 0;
    while (std::getline(commandFile, line) && status == 0) {
        if(line.find_first_of("\r") != std::string::npos){
            line.pop_back();
        }
        if(line=="PRINT_GRID"){
            print_grid(game);
        }else if(line=="ROTATE_RIGHT"){
            rotate_right(game);
        }else if(line=="ROTATE_LEFT"){
            rotate_left(game);
        }else if(line=="MOVE_RIGHT"){
            move_right(game);
        }else if(line=="MOVE_LEFT"){
            move_left(game);
        }else if(line=="DROP"){
            status = drop(game);
            if(status == -1){
                std::cout << "\nGAME OVER!" << std::endl;
                std::cout << "Next block that couldn't fit:" << std::endl;
                game.print_matrix_symbol(game.active_rotation->next_block->shape);

                std::cout << "Final grid and score:" << std::endl;
                print_grid(game);
                std::cout << "Leaderboard\n"
                             "-----------" << std::endl;

                game.leaderboard.print_leaderboard();
                break;
            }else if(status == 1){
                std::cout << "you won" << std::endl;

            }
        }else if(line=="GRAVITY_SWITCH"){
            gravity_switch(game);
        }else{
                std::cout << "Unknown command" << std::endl;
        }
    }



    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.

    return false;
}


bool GameController::check_for_collisions_entering_the_grid(Block* block, BlockFall &game){

    bool available = true;

    vector<vector<bool>> block_shape = block->shape;

    for (int i = 0; i < block_shape.size(); ++i) {
        for (int j = 0; j < block_shape[0].size(); ++j) {
            if(block_shape[i][j] == 1 && game.grid[i][j] == 1){
                available = false;
            }
        }
    }
    return available;
}

bool GameController::move_right_check_for_collisions(Block* block, BlockFall &game){

    int index_on_grid = block->index_on_grid;

    bool available = true;

    vector<vector<bool>> block_shape = block->shape;


    if(index_on_grid + block_shape[0].size() >= game.cols){
        return false;
    }

    for (int i = 0; i < block_shape.size(); ++i) {
        for (int j = 0; j < block_shape[i].size(); ++j) {
            if(block_shape[i][j] == 1 && game.grid[i][j+index_on_grid+1] == 1){
                available = false;
            }
        }
    }
    return available;

}

bool GameController::move_left_check_for_collisions(Block* block, BlockFall &game){

    int index_on_grid = block->index_on_grid;

    bool available = true;

    vector<vector<bool>> block_shape = block->shape;


    if(index_on_grid == 0){
        return false;
    }


    for (int i = 0; i < block_shape.size(); ++i) {
        for (int j = 0; j < block_shape[i].size(); ++j) {
            if(block_shape[i][j] == 1 && game.grid[i][j+index_on_grid-1] == 1){
                available = false;
            }
        }
    }
    return available;

}

bool GameController::rotate_right_check_for_collisions(Block* block, BlockFall &game){

    vector<vector<bool>> rotate_shape = block->shape;

    game.turn_right(rotate_shape);

    int index_on_grid = block->index_on_grid;

    bool available = true;


    //if(index_on_grid + rotate_shape[0].size() == game.cols){
    //    return false;
    //}


    for (int i = 0; i < rotate_shape.size(); ++i) {
        for (int j = 0; j < rotate_shape[0].size(); ++j) {
            if(rotate_shape[i][j] == 1 && game.grid[i][j+index_on_grid+1] == 1){
                available = false;
            }
        }
    }
    return available;

}

bool GameController::rotate_left_check_for_collisions(Block* block, BlockFall &game){

    vector<vector<bool>> rotate_shape = block->shape;

    game.turn_left(rotate_shape);

    int index_on_grid = block->index_on_grid;

    bool available = true;


    for (int i = 0; i < rotate_shape.size(); ++i) {
        for (int j = 0; j < rotate_shape[0].size(); ++j) {
            if(rotate_shape[i][j] == 1 && game.grid[i][j+index_on_grid-1] == 1){
                available = false;
            }
        }
    }
    return available;
}


void GameController::print_grid(BlockFall &game){


    std::cout << "\nScore: " << game.current_score << std::endl;
    std::cout << "High Score: " << 5600 <<std::endl;

    vector<vector<int>> copy_grid = game.grid;

    if(game.active_rotation != nullptr){
        vector<vector<bool>> block_shape = game.active_rotation->shape;
        int index_on_grid = game.active_rotation->index_on_grid;

        for (int i = 0; i < block_shape.size(); ++i) {
            for (int j = 0; j < block_shape[0].size(); ++j) {
                copy_grid[i][j+index_on_grid] = block_shape[i][j];
            }
        }
    }

    for (int i = 0; i < copy_grid.size(); ++i) {
        for (int j = 0; j < copy_grid[0].size(); ++j) {
            if(copy_grid[i][j] == 1){
                std::cout << occupiedCellChar;
            }else{
                std::cout << unoccupiedCellChar;
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\n";


}

void GameController::rotate_right(BlockFall &game){

    bool is_possible = rotate_right_check_for_collisions(game.active_rotation->right_rotation,game);

    if(is_possible){
        game.active_rotation = game.active_rotation->right_rotation;
    }
}

void GameController::rotate_left(BlockFall &game){

    bool is_possible = rotate_left_check_for_collisions(game.active_rotation->right_rotation,game);

    if(is_possible){
        game.active_rotation = game.active_rotation->left_rotation;
    }

}

void GameController::move_right(BlockFall &game){

    bool is_possible = move_right_check_for_collisions(game.active_rotation,game);

    if(is_possible){
        game.active_rotation->index_on_grid++;
        Block* temp = game.active_rotation->right_rotation;
        while(temp!= game.active_rotation){
            temp->index_on_grid = game.active_rotation->index_on_grid;
            temp = temp->right_rotation;
        }
    }

}

void GameController::move_left(BlockFall &game){

    bool is_possible = move_left_check_for_collisions(game.active_rotation,game);

    if(is_possible && game.active_rotation->index_on_grid != 0){
        game.active_rotation->index_on_grid--;
        Block* temp = game.active_rotation->right_rotation;
        while(temp!= game.active_rotation){
            temp->index_on_grid = game.active_rotation->index_on_grid;
            temp = temp->right_rotation;
        }
    }

}

bool GameController::find_powerups(BlockFall &game){

    bool temp_found;

    vector<vector<bool>> power_up_matrix = game.power_up;
    vector<vector<int>> copy_grid = game.grid;

    for (int i = 0; i < game.rows - power_up_matrix.size() + 1; ++i) {
        for (int j = 0; j < (game.cols - power_up_matrix[0].size()) + 1 ; j++) { //(game.cols - game.power_up[i].size())
            temp_found = true;
            for(int m = 0; m < power_up_matrix.size() && temp_found; ++m){
                for(int n = 0; n < power_up_matrix[0].size() && temp_found; ++n){
                    if(copy_grid[i+m][j+n] != int(power_up_matrix[m][n])){
                        temp_found = false;
                        break;
                    }
                }
                if(!temp_found){
                    break;
                }
            }
            if(temp_found){
                return true;
            }
        }
    }

    return false;


}

void GameController::clear_powerups_and_add_score(BlockFall &game){

    bool is_there = find_powerups(game);

    if(is_there){

        int cell_number = how_many_blocks_grid(game.grid);
        int added_score = 1000 + cell_number;
        game.current_score += added_score;

        for (int i = 1; i-1 < game.grid.size(); ++i) {
            for (int j = 0; j < game.grid[i].size(); ++j) {
                game.grid[i][j] = 0;
            }
        }
    }
}

int GameController::find_full_row(BlockFall &game){ // returns the full row, if not returns -1

    vector<vector<int>> copy_grid = game.grid;

    for (int i = 0; i < copy_grid.size(); ++i) {
        bool full_row = true;
        for (int j = 0; j < copy_grid[i].size(); ++j) {
            if(copy_grid[i][j] != 1){
                full_row = false;
                break;
            }
        }
        if (full_row) {
            return i;
        }
    }
    return -1;
}

void GameController::clear_full_rows_and_add_score(BlockFall &game){

    vector<vector<int>> copy_grid = game.grid;

    int row = find_full_row(game);
    if(row == -1){
        return;
    }

    std::cout << "Before clearing:" << std::endl;
    game.print_matrix_symbol(copy_grid);


    for (int j = 0; j < copy_grid[row].size(); ++j) {
        copy_grid[row][j] = 0;
    }


    for (int i = row-1; i >= 0; --i) {
        for (int j = 0; j < copy_grid[0].size(); ++j) {
            copy_grid[i+1][j] = copy_grid[i][j];
        }
    }

    game.grid = copy_grid;

    int column_number = game.cols;
    game.current_score += column_number;


}

void GameController::add_drop_score(BlockFall &game, int index_number){

    int cell_number = how_many_blocks(game.active_rotation->shape);

    int added_score = cell_number*(index_number);

    game.current_score += added_score;

}

int GameController::drop(BlockFall &game){

    vector<vector<int>> copy_grid = game.grid;
    vector<vector<bool>> block_matrix = game.active_rotation->shape;

    int index_on_grid = game.active_rotation->index_on_grid;
    int current_row = 0;

    while(current_row + block_matrix.size() < copy_grid.size()){
        bool collision = false;
        for (int i = 0; i < block_matrix.size(); ++i) {
            for(int j = 0; j < block_matrix[0].size(); j++){
                if (block_matrix[i][j] == 1 && copy_grid[current_row + i + 1][index_on_grid + j] == 1) {
                    collision = true;
                    break;
                }
            }
            if (collision) {
                break;
            }
        }
        if(!collision){
            current_row++;
        }else{
            break;
        }
    }


    for (int i = 0; i < block_matrix.size(); ++i) {
        for (int j = 0; j < block_matrix[0].size(); ++j) {
            if (block_matrix[i][j] == 1) {
                copy_grid[current_row + i][index_on_grid + j] = 1;
            }
        }
    }

    add_drop_score(game,current_row);

    if(find_powerups(game)){
        clear_powerups_and_add_score(game);
    }else{
        game.grid = copy_grid;
    }



    if(game.gravity_mode_on){
        apply_gravity(game);
    }

    clear_full_rows_and_add_score(game);


    if(game.active_rotation->next_block == nullptr){
        return 1;
    }

    if(!check_for_collisions_entering_the_grid(game.active_rotation->next_block,game)){
        return -1;
    }

    game.active_rotation = game.active_rotation->next_block;

    return 0;

}


int GameController::how_many_blocks(vector<vector<bool>> block_matrix){
    int count = 0;
    for (int i = 0; i < block_matrix.size(); ++i) {
        for (int j = 0; j < block_matrix[i].size(); ++j) {
            if (block_matrix[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}

int GameController::how_many_blocks_grid(vector<vector<int>> matrix){
    int count = 0;
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}

void GameController::gravity_switch(BlockFall &game){
    if(game.gravity_mode_on){
        game.gravity_mode_on = false;
    }else{
        game.gravity_mode_on = true;
        apply_gravity(game);
    }

    vector<vector<int>> copy_grid = game.grid;


    if(game.gravity_mode_on){
        apply_gravity(game);
    }

    game.grid = copy_grid;

}

void GameController::apply_gravity(BlockFall &game){

    vector<vector<int>> copy_grid = game.grid;

    for(int i = copy_grid.size() - 2; i >= 0; i--){
        for(int j = copy_grid[0].size() - 1; j >= 0; j--){
            int m = i;
            while(m+1<copy_grid.size()){
                if(copy_grid[m+1][j] == 0 && copy_grid[m][j] == 1){
                    copy_grid[m+1][j] = 1;
                    copy_grid[m][j] = 0;
                }
                m++;
            }
        }
    }

    game.grid = copy_grid;

}


