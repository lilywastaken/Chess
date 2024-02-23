#include "display.h"

Display::Display()
{
    m_board=m_initBoard;
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_ShowCursor(SDL_DISABLE);
    
    m_window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1248, 848, SDL_WINDOW_SHOWN);   
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    
    TTF_Init();
    m_font = TTF_OpenFont("resources/font.ttf", 48);
        
    m_lighted = {-1,-1,-1,-1};
    
    // Load images
    SDL_Surface* tile1Surface = IMG_Load("resources/tile1.png");
    SDL_Surface* tile2Surface = IMG_Load("resources/tile2.png");
    SDL_Surface* piecesSurface = IMG_Load("resources/pieces.png");
    SDL_Surface* dotSurface = IMG_Load("resources/dot.png");
    SDL_Surface* takeSurface = IMG_Load("resources/take.png");
    SDL_Surface* mouseSurface = IMG_Load("resources/mouse.png");
        
    // Convert tiles to texture
    m_tile1Texture = SDL_CreateTextureFromSurface(m_renderer, tile1Surface);
    m_tile2Texture = SDL_CreateTextureFromSurface(m_renderer, tile2Surface);
    SDL_Rect tileRect;
    tileRect.w = tile1Surface->w;
    tileRect.h = tile1Surface->h;
    
    for (int row = 0; row < NUMROWS; row++) {
        for (int col = 0; col < NUMCOLS; col++) {
            m_tilesDstRect.push_back({col*tileRect.w, row*tileRect.h, tileRect.w, tileRect.h});
        }
    }
    
    // Convert pieces to texture
    m_piecesTexture = SDL_CreateTextureFromSurface(m_renderer, piecesSurface);
    SDL_Rect pieceRect;
    pieceRect.w = piecesSurface->w/6;
    pieceRect.h = piecesSurface->h/2;
        
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 6; col++) {
            m_piecesSrcRect.push_back({col*pieceRect.w, row*pieceRect.h, pieceRect.w, pieceRect.h});
        }
    }
    
    // Convert circle to texture
    m_dot = SDL_CreateTextureFromSurface(m_renderer, dotSurface);
    m_take = SDL_CreateTextureFromSurface(m_renderer, takeSurface);
    m_mouse = SDL_CreateTextureFromSurface(m_renderer, mouseSurface);
}

void Display::destroy(){
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    TTF_Quit();
}

vector<int> Display::stringToVector(string move){
    vector<int> moveInt = {8-(move[1]-'0'),move[0]-'a',8-(move[3]-'0'),move[2]-'a'};
    if(move.size()==5) moveInt.push_back(1);
    return moveInt;
}

string Display::vectorToString(vector<int> move) {
  string result;
  result += char('a' + move[1]);
  result += char('8' - move[0]);
  result += char('a' + move[3]);
  result += char('8' - move[2]);
  return result;
}

void Display::reset(){

    m_board=m_initBoard;
    m_whiteTurn=true;
    m_playerMove={-1,-1,-1,-1};
    m_lighted={-1,-1,-1,-1};
}

string Display::getClick(int x, int y){

	string move = "";
    int posX=x/106, posY=y/106;
    if(posX<0 || posX>7 || posY<0 || posY>7){
        m_playerMove={-1,-1,-1,-1};
        return "";
    }
    if(m_playerMove[0]==-1){
        if(m_board[posY][posX].color==0) m_playerMove[0] = posY, m_playerMove[1] = posX;
        move = vectorToString(m_playerMove);
    }    
    else{
        if(m_board[posY][posX].color!=0){
            m_playerMove[2] = posY, m_playerMove[3] = posX;
            move = vectorToString(m_playerMove);
            if(m_board[m_playerMove[0]][m_playerMove[1]].name=="pawn" && m_playerMove[2]==0) move+="q";
            m_playerMove[2] = -1, m_playerMove[3] = -1;
        }
        else{
        	if(m_board[posY][posX].color==0) m_playerMove[0] = posY, m_playerMove[1] = posX;
       		move = vectorToString(m_playerMove);
        }
    }
    return move;
}

vector<bool> Display::movePiece(vector<int> move){

    bool eat = m_board[move[2]][move[3]].color!=-1;
    bool castle = false;
    
    // Castle
    if(m_board[move[0]][move[1]].name=="king"){
		if(move[1]-move[3]==2 && m_whiteTurn){
		    m_board[7][3] = m_board[7][0];
		    m_board[7][0] = {"", -1};
		    castle = true;
		}
		
		if(move[1]-move[3]==-2 && m_whiteTurn){
		    m_board[7][5] = m_board[7][7];
		    m_board[7][7] = {"", -1};
		    castle = true;
		}
		
		if(move[1]-move[3]==2 && !m_whiteTurn){
		    m_board[0][3] = m_board[0][0];
		    m_board[0][0] = {"", -1};
		    castle = true;
		}
		
		if(move[1]-move[3]==-2 && !m_whiteTurn){
		    m_board[0][5] = m_board[0][7];
		    m_board[0][7] = {"", -1};
		    castle = true;
		}
	}
    
    // En passant
    if(m_board[move[0]][move[1]].name=="pawn" && move[1]!=move[3] && m_board[move[2]][move[3]].name==""){
        m_board[move[0]][move[3]] = {"", -1};
    }
    
    m_lighted = move;
    m_board[move[2]][move[3]] = m_board[move[0]][move[1]];
    m_board[move[0]][move[1]] = {"", -1};
    
    if(move.size()==5) m_board[move[2]][move[3]] = {"queen", !m_whiteTurn};
    
    m_whiteTurn=!m_whiteTurn;
    m_playerMove={-1,-1,-1,-1};
    
    return vector<bool> {eat,castle};
        
}

int Display::locatePieceIndex(const char* targetName){
    for (int i=0; i<6; i++) {
        if (strcmp(m_pieceNames[i], targetName) == 0) return i;
    }
    return -1;
}

void Display::writeText(string text, int posX, int posY, SDL_Color color){
    SDL_Surface* surface = TTF_RenderText_Solid(m_font, text.c_str(), color);
	SDL_Texture* texture;
	texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	int textWidth = surface->w;
	int textHeight = surface->h;
	SDL_Rect textposition = {posX, posY, textWidth, textHeight};
	SDL_RenderCopy(m_renderer, texture, NULL, &textposition);
	
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void Display::renderPresent(bool checked, int nbBlack, int nbWhite, int tie, float time, bool moved, vector<bool> possibleMoves, bool quit, int mouseX, int mouseY){

    SDL_RenderClear(m_renderer);
        
    for (int row = 0; row < NUMROWS; row++) {
        for (int col = 0; col < NUMCOLS; col++){
        
        
            if((m_lighted[0]==row && m_lighted[1]==col) || (m_lighted[2]==row && m_lighted[3]==col)){
                SDL_SetTextureColorMod(m_tile1Texture, 0, 255, 0);
                SDL_SetTextureColorMod(m_tile2Texture, 0, 255, 0);
            }
            
            if(m_playerMove[0]==row && m_playerMove[1]==col && !(moved || int(time)==0 || quit)){
            	SDL_SetTextureColorMod(m_tile1Texture, 0, 255, 255);
                SDL_SetTextureColorMod(m_tile2Texture, 0, 255, 255);
            }
            
            
            int tileID=row*8+col;
            int color = m_board[row][col].color;
            
            if((row+col)%2) SDL_RenderCopy(m_renderer, m_tile1Texture, NULL, &m_tilesDstRect[tileID]);
            else SDL_RenderCopy(m_renderer, m_tile2Texture, NULL, &m_tilesDstRect[tileID]);
            
            SDL_SetTextureColorMod(m_tile1Texture, 255, 255, 255);
            SDL_SetTextureColorMod(m_tile2Texture, 255, 255, 255);
        
            string pieceName = m_board[row][col].name;
                            
            int pieceIndex = locatePieceIndex(pieceName.c_str());
            if(pieceIndex!=-1){
                int pieceToDisplay = pieceIndex+6*color;
                
                SDL_Rect pieceDstRect = {(col*106)-2, (row*106)+1, int(m_piecesSrcRect[pieceToDisplay].w), int(m_piecesSrcRect[pieceToDisplay].h)};
                
                if(pieceName=="king" && checked && ((color==0 && m_whiteTurn) || (color==1 && !m_whiteTurn))){
                    SDL_SetTextureColorMod(m_piecesTexture, 255, 0, 0);
                    SDL_RenderCopy(m_renderer, m_piecesTexture, &m_piecesSrcRect[pieceToDisplay], &pieceDstRect);
                    SDL_SetTextureColorMod(m_piecesTexture, 255, 255, 255);
                }
                
                else SDL_RenderCopy(m_renderer, m_piecesTexture, &m_piecesSrcRect[pieceToDisplay], &pieceDstRect);
                
            }
            
            if(possibleMoves[tileID]){
            	if(color==1) SDL_RenderCopy(m_renderer, m_take, NULL, &m_tilesDstRect[tileID]);
            	else SDL_RenderCopy(m_renderer, m_dot, NULL, &m_tilesDstRect[tileID]);
            }
            
        }
    }
    
    ////////////////////////////////////////////////////////////
    
    string timeleft = "Time left : "+to_string(int(time));
    
	if(int(time)==0) writeText(timeleft, 862, 0, {255,0,0,255});
	else writeText(timeleft, 862, 0, {255,255,0,255});
    
    vector<string> textList;
    textList.push_back("Black wins : "+to_string(nbBlack));
    textList.push_back("White wins : "+to_string(nbWhite));
    textList.push_back("Ties : "+to_string(tie));
    
    int posY=100;
    for(string text : textList){
    	writeText(text, 862, posY, {255,255,255,255});
		posY+=60;
	}
	
	if(moved || int(time)==0 || quit){
    	writeText("END OF GAME", 862, 300, {0,0,255,255});
		
		if(int(time)==0 || m_whiteTurn || quit) writeText("BLACK WINS", 862, 360, {255,0,0,255});
		else writeText("BLACK WINS", 862, 360, {0,255,0,255});
	}
	else if(moved && !checked) writeText("TIE", 862, 360, {255,255,0,255});
			
    ////////////////////////////////////////////////////////////
	
	
	SDL_Rect mouseRect = {mouseX-58, mouseY-58, 106, 106};
    SDL_RenderCopy(m_renderer, m_mouse, NULL, &mouseRect);
	    
    SDL_RenderPresent(m_renderer);
}







