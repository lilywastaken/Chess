#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <vector>

#define READ_END 0
#define WRITE_END 1

using namespace std;

string askStockfish(vector<string> moves, int fd1[2], int fd2[2]){
    
    char buffer[2048];
    
    string command = "position startpos moves ";
    
    for(int i=0; i<moves.size(); i++){
        command += moves[i];
        command += " ";
    }
    
    command += "\n";
    
    write(fd1[WRITE_END], command.c_str(), strlen(command.c_str()));
    write(fd1[WRITE_END], "setoption name Skill Level value 1\n", strlen("setoption name Skill Level value 1\n"));
    write(fd1[WRITE_END], "go depth 1\n", strlen("go depth 1\n"));

    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(fd2[READ_END], &read_set);

    const char* keyword = "bestmove ";
    bool keyword_found = false;

    while (!keyword_found) {
        int num_ready = select(fd2[READ_END] + 1, &read_set, NULL, NULL, NULL);
        if (num_ready > 0 && FD_ISSET(fd2[READ_END], &read_set)) {
            int num_read = read(fd2[READ_END], buffer, sizeof(buffer));
            buffer[num_read] = '\0';
            if (strstr(buffer, keyword) != NULL) {
                keyword_found = true;
                int index = strstr(buffer, keyword) - buffer;
                string bestMove = string(buffer + index + strlen(keyword), 5);
                if(bestMove[4]!='r'&&bestMove[4]!='k'&&bestMove[4]!='q'&&bestMove[4]!='b') bestMove = bestMove.substr(0, 4);
                return bestMove;
            }
        }
    }

    return "";
}

bool testNext(vector<string> moves, string nextMove, int fd1[2], int fd2[2]){
    
    char buffer[2048];
    
    string command = "position startpos moves ";
    string search = "go depth 1 searchmoves "+nextMove+"\n";
    
    for(int i=0; i<moves.size(); i++){
        command += moves[i];
        command += " ";
    }
    
    command += "\n";
    
    write(fd1[WRITE_END], command.c_str(), strlen(command.c_str()));
    write(fd1[WRITE_END], search.c_str(), strlen(search.c_str()));
    
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(fd2[READ_END], &read_set);
    
    const char* keyword = "bestmove ";
    bool keyword_found = false;

    while (!keyword_found) {
        int num_ready = select(fd2[READ_END] + 1, &read_set, NULL, NULL, NULL);
        if (num_ready > 0 && FD_ISSET(fd2[READ_END], &read_set)) {
            int num_read = read(fd2[READ_END], buffer, sizeof(buffer));
            buffer[num_read] = '\0';
            if (strstr(buffer, keyword) != NULL) {
                keyword_found = true;
                int index = strstr(buffer, keyword) - buffer;
                string bestMove = string(buffer + index + strlen(keyword), 5);
                if(bestMove[4]!='r'&&bestMove[4]!='k'&&bestMove[4]!='q'&&bestMove[4]!='b') bestMove = bestMove.substr(0, 4);
                if(bestMove=="(non") return false;
            }
        }
    }
    return true;
}

bool testCheck(vector<string> moves, int fd1[2], int fd2[2]){
    
    char buffer[2048];
    
    string command = "position startpos moves ";
    string search = "d\n";
    
    for(int i=0; i<moves.size(); i++){
        command += moves[i];
        command += " ";
    }
    
    command += "\n";
    
    write(fd1[WRITE_END], command.c_str(), strlen(command.c_str()));
    write(fd1[WRITE_END], search.c_str(), strlen(search.c_str()));
    
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(fd2[READ_END], &read_set);
    
    const char* keyword = "Checkers:";
    bool keyword_found = false;

    while (!keyword_found) {
        int num_ready = select(fd2[READ_END] + 1, &read_set, NULL, NULL, NULL);
        if (num_ready > 0 && FD_ISSET(fd2[READ_END], &read_set)) {
            int num_read = read(fd2[READ_END], buffer, sizeof(buffer));
            buffer[num_read] = '\0';
            if (strstr(buffer, keyword) != NULL) {
                keyword_found = true;
                int index = strstr(buffer, keyword) - buffer;
                string bestMove = string(buffer + index + strlen(keyword), 3);
                return isdigit(bestMove[2]);
            }
        }
    }
    return false;
}

bool testMove(vector<string> moves, int fd1[2], int fd2[2]){
    
    char buffer[2048];
    
    string command = "position startpos moves ";
    string search = "go wtime 1\n";
    
    for(int i=0; i<moves.size(); i++){
        command += moves[i];
        command += " ";
    }
    
    command += "\n";
    
    write(fd1[WRITE_END], command.c_str(), strlen(command.c_str()));
    write(fd1[WRITE_END], search.c_str(), strlen(search.c_str()));
    
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(fd2[READ_END], &read_set);
    
    const char* keyword = "bestmove ";
    bool keyword_found = false;

    while (!keyword_found) {
        int num_ready = select(fd2[READ_END] + 1, &read_set, NULL, NULL, NULL);
        if (num_ready > 0 && FD_ISSET(fd2[READ_END], &read_set)) {
            int num_read = read(fd2[READ_END], buffer, sizeof(buffer));
            buffer[num_read] = '\0';
            if (strstr(buffer, keyword) != NULL) {
                keyword_found = true;
                int index = strstr(buffer, keyword) - buffer;
                string bestMove = string(buffer + index + strlen(keyword), 5);
                if(bestMove[4]!='r'&&bestMove[4]!='k'&&bestMove[4]!='q'&&bestMove[4]!='b') bestMove = bestMove.substr(0, 4);
                if(bestMove=="(non") return true;
            }
        }
    }
    return false;
}


//position fen rnbqkbnr/pppp1ppp/8/8/8/4Q3/PPPP1PPP/RNB1KBNR b KQkq - 0 1








