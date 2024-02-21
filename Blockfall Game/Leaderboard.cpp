#include "Leaderboard.h"
#include "LeaderboardEntry.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {

    LeaderboardEntry* temp = head_leaderboard_entry;

    if (head_leaderboard_entry == nullptr || new_entry->score > temp->score) {
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
        return;
    }

    LeaderboardEntry* current = head_leaderboard_entry;

    while (current->next_leaderboard_entry != nullptr && current->next_leaderboard_entry->score > new_entry->score) {
        current = current->next_leaderboard_entry;
    }

    new_entry->next_leaderboard_entry = current->next_leaderboard_entry;
    current->next_leaderboard_entry = new_entry;



    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).
}

void Leaderboard::write_to_file(const string& filename) {
    std::ofstream leaderboard_file(filename);

    LeaderboardEntry* temp = head_leaderboard_entry;

    while(temp->next_leaderboard_entry != nullptr){
        leaderboard_file << temp->score << " " << temp->last_played << " " << temp->player_name << std::endl;
        temp = temp->next_leaderboard_entry;
    }

    leaderboard_file.close();
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions
}

void Leaderboard::read_from_file(const string& filename) {

    std::ifstream LeaderboardFile(filename);

    string myText;
    while (std::getline (LeaderboardFile, myText)) {
        istringstream iss(myText);
        vector<string> lines;

        do {
            string line;
            iss >> line;
            lines.push_back(line);
        } while (iss);

        lines.pop_back();

        unsigned long score_entry = std::stoul(lines[0]);
        unsigned long time_stamp_integer = std::stol(lines[1]);
        time_t time_stamp_entry = static_cast<time_t>(time_stamp_integer);
        string player_name_entry = lines[2];

        LeaderboardEntry* new_leader_board_entry = new LeaderboardEntry(score_entry,time_stamp_entry,player_name_entry);
        insert_new_entry(new_leader_board_entry);
    }


    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.
}


void Leaderboard::print_leaderboard() {
    LeaderboardEntry* temp = head_leaderboard_entry;
    int index = 1;
    while(temp!= nullptr){

        time_t last_played_time_stamp = temp->last_played;

        struct tm * timeinfo;
        char buffer[80];

        timeinfo = localtime(&last_played_time_stamp);
        strftime(buffer, sizeof(buffer), "%H:%M:%S/%d.%m.%Y", timeinfo);

        std::cout << index << ". " << temp->player_name << " " << temp->score << " " << buffer << std::endl;
        temp = temp->next_leaderboard_entry;
        index++;
    }

    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
}

Leaderboard::~Leaderboard() {

    LeaderboardEntry* temp = this->head_leaderboard_entry;
    LeaderboardEntry* next_block;

    while(temp != nullptr) {
        next_block = temp->next_leaderboard_entry;
        delete temp;
        temp = next_block;
    }

    // TODO: Free dynamically allocated memory used for storing leaderboard entries
}
