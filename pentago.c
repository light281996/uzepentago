/*
	The Uzebox Implementation of Pentago
	Copyright 2010, Kenton Hamaluik

	This file is part of The Uzebox Implementation of Pentago.

	The Uzebox Implementation of Pentago is free software: you can redistribute
	it and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Uzebox Implementation of Pentago is distributed in the hope that it
	will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with The Uzebox Implementation of Pentago.  If not, see
	<http://www.gnu.org/licenses/>.
*/

#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <uzebox.h>

// graphics
#include "mammoth.c"
#include "atascii.c"
#include "screen.c"

// sounds
#include "patches.c"

// type definitions
#define STATE_MAINMENU		(1)
#define STATE_PLAYERSTART	(2)
#define STATE_HOWTOPLAY		(3)
#define STATE_CREDITS		(4)
#define STATE_PLAYING		(5)
#define STATE_GAMEOVER		(6)

// globals
char gameState = 0;
char nextState = STATE_MAINMENU;
char musicOn = 0;

// text renderer
#include "text.c"

// joypad states
int padHeld[2] = {0,0};
int padPressed[2] = {0,0};
int padReleased[2] = {0,0};

// global variables passed between states
char startPlayer;
char wonPlayer;

// include the states
#include "state_mainmenu.c"
#include "state_playerstart.c"
#include "state_playing.c"
#include "state_gameover.c"
#include "state_howtoplay.c"
#include "state_credits.c"

// intro screen
void DoIntro()
{
	
	// random number generator seed
	int rndSeed = 0;
	
	SetTileTable(tiles_mammoth);
	DrawMap2(0, 0, map_mammoth);
	
	// game loop
	char quit = 0;
	while(!quit)
	{
		// wait for v sync
		WaitVsync(1);
		
		// update the seed
		rndSeed += 7;
		
		if(ReadJoypad(0) & BTN_START)
			quit = 1;
	}
	
	// seed the prng
	srand(rndSeed);
}

int main()
{
	// clear the video
	ClearVram(); 
	
	// init the music
	InitMusicPlayer(patches);

	// show the intro screen
	DoIntro();
	
	// joystick previous states
	int padPrev[2] = {0,0};
	
	// now the game loop
	while(1)
	{
		// wait for v sync
		WaitVsync(1);
		
		// load the next state if we need to
		if(nextState > 0)
		{
			// set the new game state and reset the next state
			gameState = nextState;
			nextState = 0;
			
			// fade out
			FadeOut(1, true);
			
			// init the state
			if(gameState == STATE_MAINMENU)
				InitState_MainMenu();
			else if(gameState == STATE_PLAYING)
				InitState_Playing();
			else if(gameState == STATE_HOWTOPLAY)
				InitState_HowToPlay();
			else if(gameState == STATE_CREDITS)
				InitState_Credits();
			else if(gameState == STATE_PLAYERSTART)
				InitState_PlayerStart();
			else if(gameState == STATE_GAMEOVER)
				InitState_GameOver();
		}
		
		// handle the input
		padHeld[0] = ReadJoypad(0);
		padPressed[0] = padHeld[0] & (padHeld[0] ^ padPrev[0]);
		padReleased[0] = padPrev[0] & (padHeld[0] ^ padPrev[0]);
		padPrev[0] = padHeld[0];
		padHeld[1] = ReadJoypad(1);
		padPressed[1] = padHeld[1] & (padHeld[1] ^ padPrev[1]);
		padReleased[1] = padPrev[1] & (padHeld[1] ^ padPrev[1]);
		padPrev[1] = padHeld[1];
		
		// handle the states
		if(gameState == STATE_MAINMENU)
			DoState_MainMenu();
		else if(gameState == STATE_PLAYING)
			DoState_Playing();
		else if(gameState == STATE_HOWTOPLAY)
			DoState_HowToPlay();
		else if(gameState == STATE_CREDITS)
			DoState_Credits();
		else if(gameState == STATE_PLAYERSTART)
			DoState_PlayerStart();
		else if(gameState == STATE_GAMEOVER)
			DoState_GameOver();
	}
}
