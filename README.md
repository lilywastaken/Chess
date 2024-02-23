# Chess

# A simulator for chess

This hand-made simulator allows you to play chess against stockfish.
Stockfish is directly linked to pipes to allow answers as fast as they can be. However, a little delay is simulated to make the computer move less "agressive".

Sound effects are identical to those in chess.com
You can adjust the time in the variable GAME_TIME at chess.cpp


To compile the code, make sure you've installed the following:

sudo apt install g++
sudo apt install libsdl2-image-2.0-0
sudo apt install libsdl2-mixer-dev
sudo apt install libsdl2-ttf-dev


And run the command:

g++ chess.cpp display.cpp audio.cpp stockfish.cpp input.cpp -o chess -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

