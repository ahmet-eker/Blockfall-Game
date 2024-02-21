#include "BlockFall.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
    active_rotation = initial_block;
}

void BlockFall::read_blocks(const string &input_file) {
    string block_file(input_file);
    vector<string> string_blocks = read_block_files(block_file);
    vector<vector<bool>> block_shape;

    for(int i = 0; i < string_blocks.size(); i++){
        block_shape = string_to_vector(string_blocks[i]);
        insert_new_block(block_shape,initial_block);
    }
    Block* temp = initial_block;
    while(temp!= nullptr){
        create_circular_doubly_linked_list(temp);
        temp = temp->next_block;
    }

    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
}

void BlockFall::initialize_grid(const string &input_file) {
    vector<vector<int>> grid_2d;  // 2D game grid
    string grid_file_name_string(input_file);
    std::ifstream GridFile(grid_file_name_string);

    string myText;
    while (std::getline (GridFile, myText)) {
        vector<int> grid_row;
        if(myText.find_first_of("\r") != std::string::npos){
            myText.pop_back();
        }
        std::istringstream iss(myText);
        int cell;
        while (iss >> cell) {
            grid_row.push_back(cell);
        }
        grid_2d.push_back(grid_row);
    }

    grid = grid_2d;
    rows = grid_2d.size();
    cols = grid_2d[0].size();

    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main
}


BlockFall::~BlockFall() {
    Block* temp = this->initial_block;
    Block* next_block;

    while(temp != nullptr){
        next_block = temp->next_block;

        Block* temp_circular = temp->right_rotation;
        Block* next_block_circular;

        while(temp_circular != temp){
            next_block_circular = temp_circular->right_rotation;

            delete temp_circular;
            temp_circular = next_block_circular;
        }
        //temp_circular = nullptr;

        delete temp;
        temp = next_block;
    }
    //temp = nullptr;
    // TODO: Free dynamically allocated memory used for storing game blocks
}



bool BlockFall::is_matrix_equal(vector<vector<bool>> matrix1, vector<vector<bool>> matrix2) {
    bool equal = true;

    if(matrix1.size()!=matrix2.size() || matrix1[0].size()!=matrix2[0].size()){
        equal = false;
        return equal;
    }

    for (int i = 0; i < matrix1.size(); ++i) {
        for (int j = 0; j < matrix1[0].size(); ++j) {
            if(matrix1[i][j] != matrix2[i][j]){
                equal = false;
            }
        }
    }
    return equal;
}


void BlockFall::turn_right(vector<vector<bool>> &matrix) {
    vector<vector<bool>> transposed(matrix[0].size(), vector<bool>(matrix.size()));
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }

    for (auto& row : transposed) { // reverse rows
        std::reverse(row.begin(), row.end());
    }

    matrix = transposed;
}


void BlockFall::turn_left(vector<vector<bool>>&matrix) {
    vector<vector<bool>> transposed(matrix[0].size(), vector<bool>(matrix.size()));
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }

    matrix = transposed;

    std::vector<std::vector<bool>> copymatrix = matrix;

    for (size_t i = 0; i < matrix.size(); ++i) {
        matrix[i] = copymatrix[matrix.size()-1-i];
    }
}


int BlockFall::determine_how_many_rotation(Block* &block){
    vector<vector<bool>> shape_data = block->shape;
    vector<vector<bool>> no_rotation = shape_data;
    turn_right(shape_data);
    vector<vector<bool>> right_rotation = shape_data;
    turn_right(shape_data);
    vector<vector<bool>> mirror_rotation = shape_data;
    turn_right(shape_data);
    vector<vector<bool>> left_rotation = shape_data;


    if(is_matrix_equal(no_rotation,right_rotation)){
        return 1;
    } else if(is_matrix_equal(right_rotation,left_rotation)){
        return 2;
    } else if(!is_matrix_equal(no_rotation,mirror_rotation)){
        return 4;
    } else{
        return 0;
    }
}


void BlockFall::create_circular_doubly_linked_list(Block* &block){
    int number_of_rotations = determine_how_many_rotation(block);

    if(number_of_rotations == 1){

        block->left_rotation = block->right_rotation = block;

    } else if(number_of_rotations == 2){

        vector<vector<bool>> shape_data = block->shape;

        vector<vector<bool>> no_rotation = shape_data;
        turn_right(shape_data);
        vector<vector<bool>> right_rotation = shape_data;

        Block* right_block = new Block();

        block->right_rotation = right_block;
        block->left_rotation = right_block;

        right_block->next_block = block->next_block;
        right_block->shape = right_rotation;
        right_block->right_rotation = block;
        right_block->left_rotation = block;


    } else if(number_of_rotations==4){

        vector<vector<bool>> shape_data = block->shape;

        vector<vector<bool>> no_rotation = shape_data;
        turn_right(shape_data);
        vector<vector<bool>> right_rotation = shape_data;
        turn_right(shape_data);
        vector<vector<bool>> mirror_rotation = shape_data;
        turn_right(shape_data);
        vector<vector<bool>> left_rotation = shape_data;

        Block* right_block = new Block();
        Block* mirror_block = new Block();
        Block* left_block = new Block();

        block->right_rotation = right_block;
        block->left_rotation = left_block;

        right_block->next_block = block->next_block;
        right_block->shape = right_rotation;
        right_block->right_rotation = mirror_block;
        right_block->left_rotation = block;

        mirror_block->next_block = block->next_block;
        mirror_block->shape = mirror_rotation;
        mirror_block->right_rotation = left_block;
        mirror_block->left_rotation = right_block;

        left_block->next_block = block->next_block;
        left_block->shape = left_rotation;
        left_block->right_rotation = block;
        left_block->left_rotation = mirror_block;
    }
}


void BlockFall::print_matrix(const std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (bool element : row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
}


void BlockFall::print_matrix(const std::vector<std::vector<bool>>& matrix) {
    for (const auto& row : matrix) {
        for (bool element : row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
}

void BlockFall::print_matrix_symbol(const std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (bool element : row) {
            if(element){
                std::cout << occupiedCellChar;
            }else{
                std::cout << unoccupiedCellChar;
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\n" << std::endl;
}

void BlockFall::print_matrix_symbol(const std::vector<std::vector<bool>>& matrix) {
    for (const auto& row : matrix) {
        for (bool element : row) {
            if(element){
                std::cout << occupiedCellChar;
            }else{
                std::cout << unoccupiedCellChar;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}



void BlockFall::print_circular_linkedList(Block* head) {
    int index = 0;
    while (index < determine_how_many_rotation(head)) {
        print_matrix(head->shape);
        std::cout << std::endl;
        head = head->right_rotation;
        index++;
    }
    std::cout << std::endl;
}


vector<vector<bool>> BlockFall::string_to_vector(string block){ //  [10 11 01] input format returns vector<vector<bool>>

    vector<vector<bool>> block_shape;

    std::istringstream iss(block);
    std::vector<std::string> block_rows;

    std::string row;
    while (std::getline(iss, row, ' ')) {
        if (!row.empty()) {
            for(char a : row){
                if(a=='['){
                    row.erase(std::remove(row.begin(), row.end(), '['), row.end());
                } else if(a==']'){
                    row.erase(std::remove(row.begin(), row.end(), ']'), row.end());
                }
            }
            block_rows.push_back(row);
        }
    }

    for (const auto& row : block_rows) {
        vector<bool> row_vector;
        for(char member: row){
            if(member=='1'){
                row_vector.push_back(true);
            } else if(member =='0'){
                row_vector.push_back(false);
            }
        }
        block_shape.push_back(row_vector);
    }
    return block_shape;
}


vector<string> BlockFall::read_block_files(string block_file){ // input  file name  returns on format [101 111],[1],[11],[111],[1111],[11 11],[10 01],[10 10 11],[10 11 01],[111 010]

    vector<vector<int>> grid_2d;  // 2D game grid
    string block_file_name_string(block_file);
    std::ifstream BlockFile(block_file_name_string);

    string all_text = "";
    vector<string> all_blocks;
    string myText;
    bool temp = true;
    while (std::getline (BlockFile, myText)) {
        if(myText.find_first_of("\r") != std::string::npos){
            myText.pop_back();
        }

        if(myText[myText.size()-1]=='1' || myText[myText.size()-1]=='0'){
            all_text += myText + " ";
        } else if(myText[myText.size()-1]==']'){
            all_text += myText + ",";
        }
    }

    std::istringstream iss(all_text);
    vector<string> string_blocks;

    std::string string_block;
    while (std::getline(iss, string_block, ',')) {
        if (!string_block.empty()) {
            string_blocks.push_back(string_block);
        }
    }


    string power_up_string = string_blocks[string_blocks.size()-1];
    vector<vector<bool>> power_up_matrix = string_to_vector(power_up_string);
    this->power_up = power_up_matrix;

    string_blocks.pop_back();

    return string_blocks;
}


void BlockFall::insert_new_block(vector<vector<bool>> new_shape, Block* &initial_block){

    Block* new_block = new Block();

    Block* temp = initial_block;

    new_block->shape = new_shape;

    new_block->next_block = nullptr;

    if (initial_block == nullptr) {
        initial_block = new_block;
        return;
    }

    while (temp->next_block != nullptr) {
        temp = temp->next_block;
    }

    temp->next_block = new_block;
}


void BlockFall::print_linked_list(Block* &initial_block) {
    Block* temp = initial_block;
    while(temp!= nullptr){
        for (const auto &row : temp->shape) {
            for (bool value : row) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        temp = temp->next_block;
    }
}
