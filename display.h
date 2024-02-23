#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <vector>
#include <array>
#include <string>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_audio.h>

#define NUMROWS 8
#define NUMCOLS 8

#define BOARDSIZE 862

using namespace std;

struct Piece {
    string name;
    int color;
};

class Display{     

    public :
    
    Display();
    void destroy();
    
    static void playSound(string sound);
    
    static vector<int> stringToVector(string move);
    static string vectorToString(vector<int> move);
        
	void reset();
    string getClick(int x, int y);
    
	void writeText(string text, int posX, int posY, SDL_Color color);

    void renderPresent(bool checked, int nbBlack, int nbWhite, int tie, float time, bool moved, vector<bool> possibleMoves, bool quit, int mouseX, int mouseY);
    
    vector<bool> movePiece(vector<int> move);
    int locatePieceIndex(const char* targetName);
	
    private :
        
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    TTF_Font* m_font;
    
    SDL_Texture *m_tile1Texture, *m_tile2Texture;
    vector<SDL_Rect> m_tilesDstRect;
    SDL_Texture* m_piecesTexture;
    vector<SDL_Rect> m_piecesSrcRect;
    SDL_Texture* m_dot, * m_take, * m_mouse;
    
    vector<int> m_positions, m_lighted;
    
    bool m_whiteTurn=true;
    
    vector<int> m_playerMove = {-1,-1,-1,-1};
    
    const char* m_pieceNames[6] = {"king", "queen", "bishop", "knight", "rook", "pawn"};
    
    array<array<Piece, NUMCOLS>, NUMROWS> m_board;
    array<array<Piece, NUMCOLS>, NUMROWS> m_initBoard {{
    {{{"rook", 1}, {"knight", 1}, {"bishop", 1}, {"queen", 1}, {"king", 1}, {"bishop", 1}, {"knight", 1}, {"rook", 1}}},
    {{{"pawn", 1}, {"pawn", 1}, {"pawn", 1}, {"pawn", 1}, {"pawn", 1}, {"pawn", 1}, {"pawn", 1}, {"pawn", 1}}},
    {{{"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}}},
    {{{"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}}},
    {{{"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}}},
    {{{"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}, {"", -1}}},
    {{{"pawn", 0}, {"pawn", 0}, {"pawn", 0}, {"pawn", 0}, {"pawn", 0}, {"pawn", 0}, {"pawn", 0}, {"pawn", 0}}},
    {{{"rook", 0}, {"knight", 0}, {"bishop", 0}, {"queen", 0}, {"king", 0}, {"bishop", 0}, {"knight", 0}, {"rook", 0}}}
}};


    
};

#endif

