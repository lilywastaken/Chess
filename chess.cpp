#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <vector>

#include "display.h"
#include "audio.h"
#include "stockfish.h"
#include "input.h"

#define READ_END 0
#define WRITE_END 1
#define GAME_TIME 1200

using namespace std;

int main(int argc, char* argv[]){

    int fd1[2], fd2[2];
    pid_t pid;
    char command[256];
    string token;

    sprintf(command, "%s", "stockfish");

    pipe(fd1);
    pipe(fd2);

    pid = fork();

    if (pid == 0) {
        close(fd1[WRITE_END]);close(fd2[READ_END]);
        dup2(fd1[READ_END], STDIN_FILENO);dup2(fd2[WRITE_END], STDOUT_FILENO);
        execl(command, "", NULL);
    }
    
    else {
    
    // Main code
    close(fd1[READ_END]);
    close(fd2[WRITE_END]);
        
    Uint32 loopStart(0), loopEnd(0), timeLasted(0), waitBeacon(0), blackBeacon(0);
    vector<string> moves;
    
    Input input;
    Display display;
    Audio audio;
    SDL_Init(SDL_INIT_AUDIO);
    audio.playSound("start");
    
    int blackWin=0, whiteWin=0, tie=0;
    float time=GAME_TIME;
    vector<bool> possibleMoves(64, false);
    
    bool whiteTurn=true, checked=false, moved=false, waitNext=false;
    pair<int, int> mouse;
    bool clicking;
    
    while(!input.endInput()){
    
        loopStart = SDL_GetTicks();
       
        input.updateEvents();
        if(input.getKey(SDL_SCANCODE_ESCAPE)) break;
		mouse = input.getMouse();
        clicking = input.getClick();
        
		display.renderPresent(checked, blackWin, whiteWin, tie, time, moved, possibleMoves, false, mouse.first, mouse.second);
        
        if(waitNext){
        	waitNext = (SDL_GetTicks()-waitBeacon<5000);
        	if(!waitNext){
        	    audio.playSound("start");
        	    moved=false;
		        checked=false;
		        whiteTurn=true;
		        moves={};
		        display.reset();
		        time = GAME_TIME;
        	}
        }
        
        else{
        
        	//cout << mouse.first << ":" << mouse.second << endl;
		           
        	bool quit = input.getKey(SDL_SCANCODE_G);
		    if((int(time)==0 || moved || quit) && waitNext==false){
				if(!checked && int(time)!=0 && !quit){
					tie++;
		        	audio.playSound("stalemate");
				}
				else{
		        	audio.playSound("gameover");
					if(whiteTurn || int(time)==0 || quit) blackWin++;
					else whiteWin++;
				}
				
        		possibleMoves.assign(64, false);
		    	
		        waitNext=true;
		    	waitBeacon = SDL_GetTicks();
			}
		            
		    if(!whiteTurn){
		    	if(SDL_GetTicks()-blackBeacon>300){
				    string move = askStockfish(moves, fd1, fd2);
				    //cout << ">>>> " << move << endl;
					moves.push_back(move);
				    vector<bool> result = display.movePiece(Display::stringToVector(move));
				    checked = testCheck(moves, fd1, fd2);
					moved = testMove(moves, fd1, fd2);
				    whiteTurn=!whiteTurn;
		        	if(int(time)!=0 && !moved){
						if(checked) audio.playSound("check");
						else if(result[1]) audio.playSound("castle");
						else if(result[0]) audio.playSound("take");
						else audio.playSound("move");
					}
				}
		    }
		    
		    else{
		        if(clicking){
		            string move = display.getClick(mouse.first, mouse.second);
		            if (move == "") possibleMoves.assign(64, false);
		            if (move[3] == '9'){
		                possibleMoves.clear();
						for (int i = 7; i >= 0; i--) {
							for (int j = 0; j < 8; j++) {
								char file = 'a' + j;
								char rank = '1' + i;
								string caseString(1, file);
								caseString += rank;
								string possibility = move.substr(0, 2) + caseString;
		                		possibleMoves.push_back(testNext(moves, possibility, fd1, fd2));
		                	}
		                }
		            }
				    else if (move!="" && testNext(moves, move, fd1, fd2)){
		                //cout << "PLAY " << move << endl;
						moves.push_back(move);
		                vector<bool> result = display.movePiece(Display::stringToVector(move));
				        checked = testCheck(moves, fd1, fd2);
				        moved = testMove(moves, fd1, fd2);
						whiteTurn=!whiteTurn;
		        		if(int(time)!=0 && !moved){
							if(checked) audio.playSound("check");
							else if(result[1]) audio.playSound("castle");
							else if(result[0]) audio.playSound("take");
							else audio.playSound("move");
						}
        				possibleMoves.assign(64, false);
				    }
		        }
		        blackBeacon = SDL_GetTicks();
		    }
		    
		    audio.checkChannels();    
	    	time-=0.01;
		}
		              
	    loopEnd = SDL_GetTicks();
	    timeLasted = loopEnd - loopStart;
	    if((int)timeLasted < 10) SDL_Delay(10 - (int)timeLasted);
    }
        
    // End main code
    close(fd1[WRITE_END]);
    close(fd2[READ_END]);
        
    return 0;
    }
}


