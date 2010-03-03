/*	The Uzebox Implementation of Pentago
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

// main menu state

// globals
// the current player taking input
char currentPlayer; // 0 - player 1, 1 - player 2
// keep track of time for each player
char pHours[] = {0,0};
char pMins[] = {0,0};
char pSecs[] = {0,0};
char pSSecs[] = {0,0};
// the marbles
char tmpMarblesNoRot[3][3], tmpMarblesRot[3][3];
char marbles[6][6]; // 0: no marble, 1: player 1's marble, 2: player 2's marble
// current marble x, y
char marbleX, marbleY;
// super selectors
char superX, superY, selectedSuper;
// rotate selector
char rotateSelect;
// pause selector
char prePauseState;
char pauseSelect;
// the current state the playing state is in
#define PLAYINGSTATE_PLACEMARBLE	(0)
#define PLAYINGSTATE_SELECTSUPER	(1)
#define PLAYINGSTATE_SELECTROTATE	(2)
#define PLAYINGSTATE_PAUSE			(3)
char playingState, nextPlayingState = 0;

void TogglePlayer()
{
	SetTile(3 + (currentPlayer * 13), 27, 0);
	SetTile(4 + (currentPlayer * 13), 27, 0);
	if(currentPlayer == 0)
		currentPlayer = 1;
	else
		currentPlayer = 0;
	SetTile(3 + (currentPlayer * 13), 27, TILE_PLAYER_UNDERLINE);
	SetTile(4 + (currentPlayer * 13), 27, TILE_PLAYER_UNDERLINE);
}

char FirstClearSocket()
{
	// loop through the marbles to find the first available marble
	for(char y = 0; y < 6; y++)
		for(char x = 0; x < 6; x++)
			if(marbles[y][x] == 0)
				return ((y * 6) + x);
	
	// return -1 if there are no clear moves
	return -1;
}

char NextClearSocketLeft(char x, char y)
{
	for(char _x = x - 1; _x >= 0; _x--)
		if(marbles[y][_x] == 0)
			return _x;
	return -1;
}

char NextClearSocketRight(char x, char y)
{
	for(char _x = x + 1; _x < 6; _x++)
		if(marbles[y][_x] == 0)
			return _x;
	return -1;
}

char NextClearSocketUp(char x, char y)
{
	for(char _y = y - 1; _y >= 0; _y--)
		if(marbles[_y][x] == 0)
			return _y;
	return -1;
}

char NextClearSocketDown(char x, char y)
{
	for(char _y = y + 1; _y < 6; _y++)
		if(marbles[_y][x] == 0)
			return _y;
	return -1;
}

void UpdateTimers()
{
	pSSecs[currentPlayer]++;
	if(pSSecs[currentPlayer] >= 60)
	{
		pSSecs[currentPlayer] = 0;
		pSecs[currentPlayer]++;
		if(pSecs[currentPlayer] >= 60)
		{
			pSecs[currentPlayer] = 0;
			pMins[currentPlayer]++;
			if(pMins[currentPlayer] >= 60)
			{
				pMins[currentPlayer] = 0;
				pHours[currentPlayer]++;
				if(pHours[currentPlayer] > 99)
					pHours[currentPlayer] = 0;
					
				// update the hours tiles
				SetTile(6 + (currentPlayer * 13), 26, (pHours[currentPlayer] / 10) + TILE_NUMBERS_OFFSET);
				SetTile(7 + (currentPlayer * 13), 26, (pHours[currentPlayer] % 10) + TILE_NUMBERS_OFFSET);
			}
			
			// update the minutes tiles
			SetTile(9 + (currentPlayer * 13), 26, (pMins[currentPlayer] / 10) + TILE_NUMBERS_OFFSET);
			SetTile(10 + (currentPlayer * 13), 26, (pMins[currentPlayer] % 10) + TILE_NUMBERS_OFFSET);
		}
		
		// update the seconds tiles
		SetTile(12 + (currentPlayer * 13), 26, (pSecs[currentPlayer] / 10) + TILE_NUMBERS_OFFSET);
		SetTile(13 + (currentPlayer * 13), 26, (pSecs[currentPlayer] % 10) + TILE_NUMBERS_OFFSET);
	}
}

void ClearMarble(char x, char y)
{
	DrawMap2(4 + (x * 4), 3 + (y * 4), map_emptyBall);
}

void DrawMarble(char x, char y, char hl, char type)
{
	if(hl)
	{
		if(type == 0)
			DrawMap2(4 + (x * 4), 3 + (y * 4), map_blackBallHl);
		else
			DrawMap2(4 + (x * 4), 3 + (y * 4), map_whiteBallHl);
	}
	else
	{
		if(type == 0)
			DrawMap2(4 + (x * 4), 3 + (y * 4), map_blackBall);
		else
			DrawMap2(4 + (x * 4), 3 + (y * 4), map_whiteBall);
	}
}

void ClearSuperHighlight(char super)
{
	// get the x-y positions of the super to be unhighlighted
	char superX = super % 2;
	char superY = super / 2;
	
	// draw the corners
	SetTile(3 + (superX * 12), 2 + (superY * 12), MAP_BORDERTL);
	SetTile(14 + (superX * 12), 2 + (superY * 12), MAP_BORDERTR);
	SetTile(14 + (superX * 12), 13 + (superY * 12), MAP_BORDERBR);
	SetTile(3 + (superX * 12), 13 + (superY * 12), MAP_BORDERBL);
	
	// draw the lines
	Fill(4 + (superX * 12), 2 + (superY * 12), 10, 1, MAP_BORDERT); // top
	Fill(3 + (superX * 12), 3 + (superY * 12), 1, 10, MAP_BORDERL); // left
	Fill(14 + (superX * 12), 3 + (superY * 12), 1, 10, MAP_BORDERR); // right
	Fill(4 + (superX * 12), 13 + (superY * 12), 10, 1, MAP_BORDERB); // bottom
}

void DrawSuperHighlight(char super)
{
	// get the x-y positions of the super to be highlighted
	char superX = super % 2;
	char superY = super / 2;
	
	// draw the corners
	SetTile(3 + (superX * 12), 2 + (superY * 12), MAP_BORDERTLHL);
	SetTile(14 + (superX * 12), 2 + (superY * 12), MAP_BORDERTRHL);
	SetTile(14 + (superX * 12), 13 + (superY * 12), MAP_BORDERBRHL);
	SetTile(3 + (superX * 12), 13 + (superY * 12), MAP_BORDERBLHL);
	
	// draw the lines
	Fill(4 + (superX * 12), 2 + (superY * 12), 10, 1, MAP_BORDERTHL); // top
	Fill(3 + (superX * 12), 3 + (superY * 12), 1, 10, MAP_BORDERLHL); // left
	Fill(14 + (superX * 12), 3 + (superY * 12), 1, 10, MAP_BORDERRHL); // right
	Fill(4 + (superX * 12), 13 + (superY * 12), 10, 1, MAP_BORDERBHL); // bottom
}

void TakeMoveInput_Placing()
{
	static char tmp;
	if(padPressed[currentPlayer] & BTN_RIGHT)
	{
		if((tmp = NextClearSocketRight(marbleX, marbleY)) > -1)
		{
			ClearMarble(marbleX, marbleY);
			marbleX = tmp;
			DrawMarble(marbleX, marbleY, 1, currentPlayer);
		}
	}
	if(padPressed[currentPlayer] & BTN_LEFT)
	{
		if((tmp = NextClearSocketLeft(marbleX, marbleY)) > -1)
		{
			ClearMarble(marbleX, marbleY);
			marbleX = tmp;
			DrawMarble(marbleX, marbleY, 1, currentPlayer);
		}
	}
	if(padPressed[currentPlayer] & BTN_UP)
	{
		if((tmp = NextClearSocketUp(marbleX, marbleY)) > -1)
		{
			ClearMarble(marbleX, marbleY);
			marbleY = tmp;
			DrawMarble(marbleX, marbleY, 1, currentPlayer);
		}
	}
	if(padPressed[currentPlayer] & BTN_DOWN)
	{
		if((tmp = NextClearSocketDown(marbleX, marbleY)) > -1)
		{
			ClearMarble(marbleX, marbleY);
			marbleY = tmp;
			DrawMarble(marbleX, marbleY, 1, currentPlayer);
		}
	}
}

void RotateSuper(char super, char dir)
{
	// super - marble offsets
	char sX = (super % 2) * 3;
	char sY = (super / 2) * 3;
	
	// fill the temp buffer
	for(char y = sY; y < sY + 3; y++)
		for(char x = sX; x < sX + 3; x++)
			tmpMarblesNoRot[y - sY][x - sX] = marbles[y][x];
			
	// rotate the temp array
	if(dir == 1)
	{
		// counter clockwise
		// 1 2 3    3 6 9
		// 4 5 6 => 2 5 8
		// 7 8 9    1 4 7
		tmpMarblesRot[0][0] = tmpMarblesNoRot[0][2];
		tmpMarblesRot[0][1] = tmpMarblesNoRot[1][2];
		tmpMarblesRot[0][2] = tmpMarblesNoRot[2][2];
		tmpMarblesRot[1][0] = tmpMarblesNoRot[0][1];
		tmpMarblesRot[1][1] = tmpMarblesNoRot[1][1];
		tmpMarblesRot[1][2] = tmpMarblesNoRot[2][1];
		tmpMarblesRot[2][0] = tmpMarblesNoRot[0][0];
		tmpMarblesRot[2][1] = tmpMarblesNoRot[1][0];
		tmpMarblesRot[2][2] = tmpMarblesNoRot[2][0];
	}
	else
	{
		// clockwise
		// 1 2 3    7 4 1
		// 4 5 6 => 8 5 2
		// 7 8 9    9 6 3
		tmpMarblesRot[0][0] = tmpMarblesNoRot[2][0];
		tmpMarblesRot[0][1] = tmpMarblesNoRot[1][0];
		tmpMarblesRot[0][2] = tmpMarblesNoRot[0][0];
		tmpMarblesRot[1][0] = tmpMarblesNoRot[2][1];
		tmpMarblesRot[1][1] = tmpMarblesNoRot[1][1];
		tmpMarblesRot[1][2] = tmpMarblesNoRot[0][1];
		tmpMarblesRot[2][0] = tmpMarblesNoRot[2][2];
		tmpMarblesRot[2][1] = tmpMarblesNoRot[1][2];
		tmpMarblesRot[2][2] = tmpMarblesNoRot[0][2];
	}
			
	// restore the rotated buffer
	for(char y = sY; y < sY + 3; y++)
		for(char x = sX; x < sX + 3; x++)
			marbles[y][x] = tmpMarblesRot[y - sY][x - sX];
}

char FindWin()
{
	char ret = 0;
	// loop for both players
	for(char p = 1; p < 3; p++)
	{
		// look in the horizontal direction first
		// for each row
		for(char y = 0; y < 6; y++)
		{
			// count the number of successive marbles
			char nMarbles = 0;
			for(char x = 0; x < 6; x++)
			{
				if(marbles[y][x] == p)
					nMarbles++;
				else
					x = 7; // exit
			}
			
			// if we hit enough marbles in a row
			if(nMarbles >= 5)
				ret |= p;
		}
		
		// now look in the vertical direction
		// for each column
		for(char x = 0; x < 6; x++)
		{
			// count the number of successive marbles
			char nMarbles = 0;
			for(char y = 0; y < 6; y++)
			{
				if(marbles[y][x] == p)
					nMarbles++;
				else
					y = 7; // exit
			}
			
			// if we hit enough marbles in a row
			if(nMarbles >= 5)
				ret |= p;
		}
	}
	
	return ret;
}

void InitState_Playing()
{
	// set the screen tiles
	SetTileTable(tiles_screen);
	
	// clear the marbles table
	for(char x = 0; x < 6; x++)
		for(char y = 0; y < 6; y++)
			marbles[y][x] = 0;
			
	// set the current player
	currentPlayer = startPlayer;
	
	// reset the playingstate state
	nextPlayingState = 0;
	
	// set the inititial marble x, y
	marbleX = 0;
	marbleY = 0;
	
	// reset the timers
	pHours[0] = 0; pHours[1] = 0;
	pMins[0] = 0; pMins[1] = 0;
	pSecs[0] = 0; pSecs[1] = 0;
	pSSecs[0] = 0; pSSecs[1] = 0;
	
	// fill the bg
	Fill(0, 0, 30, 28, 0);
	
	// draw the super blocks
	DrawMap2(3, 2, map_emptySuper);
	DrawMap2(15, 2, map_emptySuper);
	DrawMap2(3, 14, map_emptySuper);
	DrawMap2(15, 14, map_emptySuper);
	
	// draw the title
	DrawMap2(11, 1, map_pentago);
	
	// draw the status'
	DrawMap2(3, 26, map_status);
	
	// fade in
	FadeIn(1, true);
}

void InitPlayState_PlaceMarble()
{
	// underline the current player
	SetTile(3 + (currentPlayer * 13), 27, TILE_PLAYER_UNDERLINE);
	SetTile(4 + (currentPlayer * 13), 27, TILE_PLAYER_UNDERLINE);
	
	// find the first open socket that we can place a marble into
	char firstAvailableSocket = FirstClearSocket();
	if(firstAvailableSocket < 0)
	{
		// couldn't find a free socket
		// game over!
		nextState = STATE_GAMEOVER;
		playingState = -1;
	}
	else
	{
		// calculate the marble's x and y position from its index
		marbleX = firstAvailableSocket % 6;
		marbleY = firstAvailableSocket / 6;
		
		// draw the marble
		DrawMarble(marbleX, marbleY, 1, currentPlayer);
	}
}

void DoPlayState_PlaceMarble()
{
	// update the timers
	UpdateTimers();
	// take movement input
	TakeMoveInput_Placing();
	
	// check to see if the player "placed" the marble
	if(padPressed[currentPlayer] & BTN_A)
	{
		// place the marble
		marbles[marbleY][marbleX] = currentPlayer + 1;
		DrawMarble(marbleX, marbleY, 0, currentPlayer);
		
		// load up the super selecting state
		nextPlayingState = PLAYINGSTATE_SELECTSUPER;
		
		// check for win
		int winner = FindWin();
		if(winner > 0)
		{
			// we found a winner, now find out if it was a tie
			if(winner == 3)
				wonPlayer = 0;
			else
				wonPlayer = winner; // set the winner
				
			// set the state
			nextState = STATE_GAMEOVER;
			nextPlayingState = -1;
		}
	}
	
	// check for pause button
	if(padPressed[currentPlayer] & BTN_START)
	{
		prePauseState = PLAYINGSTATE_PLACEMARBLE;
		nextPlayingState = PLAYINGSTATE_PAUSE;
	}
}

void InitPlayState_SelectSuper()
{
	// highlight the first super
	DrawSuperHighlight(0);
	
	// reset the super selector
	superX = 0;
	superY = 0;
}

void DoPlayState_SelectSuper()
{
	// update the timers
	UpdateTimers();
	
	// take input and move the highlighter
	if(padPressed[currentPlayer] & BTN_RIGHT && superX < 1)
	{
		ClearSuperHighlight((2 * superY) + superX);
		superX++;
		DrawSuperHighlight((2 * superY) + superX);
	}
	if(padPressed[currentPlayer] & BTN_LEFT && superX > 0)
	{
		ClearSuperHighlight((2 * superY) + superX);
		superX--;
		DrawSuperHighlight((2 * superY) + superX);
	}
	if(padPressed[currentPlayer] & BTN_DOWN && superY < 1)
	{
		ClearSuperHighlight((2 * superY) + superX);
		superY++;
		DrawSuperHighlight((2 * superY) + superX);
	}
	if(padPressed[currentPlayer] & BTN_UP && superY > 0)
	{
		ClearSuperHighlight((2 * superY) + superX);
		superY--;
		DrawSuperHighlight((2 * superY) + superX);
	}
	
	// take selection input
	if(padPressed[currentPlayer] & BTN_A)
	{
		// set the selected super and clear the highlight
		selectedSuper = (2 * superY) + superX;
		ClearSuperHighlight(selectedSuper);
		
		// load the rotate window
		nextPlayingState = PLAYINGSTATE_SELECTROTATE;
	}
	
	// check for pause button
	if(padPressed[currentPlayer] & BTN_START)
	{ 
		prePauseState = PLAYINGSTATE_SELECTSUPER;
		nextPlayingState = PLAYINGSTATE_PAUSE;
	}
}

void InitPlayState_SelectRotate()
{
	// draw the menu
	DrawMap2(10, 11, map_rotateMenu);
	
	// draw the selector
	rotateSelect = 0;
	SetTile(13, 14, TILE_LEFTARROW);
}

void DoPlayState_SelectRotate()
{
	// update the timers
	UpdateTimers();
	
	// take the selection input
	if(padPressed[currentPlayer] & BTN_RIGHT && rotateSelect == 0)
	{
		// set the selection
		rotateSelect = 1;
		
		// update the tiles
		SetTile(13, 14, 0);
		SetTile(16, 14, TILE_RIGHTARROW);
	}
	else if(padPressed[currentPlayer] & BTN_LEFT && rotateSelect == 1)
	{
		// set the selection
		rotateSelect = 0;
		
		// update the tiles
		SetTile(13, 14, TILE_LEFTARROW);
		SetTile(16, 14, 0);
	}
	
	// check for selection
	if(padPressed[currentPlayer] & BTN_A)
	{
		// rotate the selected super block
		RotateSuper(selectedSuper, rotateSelect);
		
		// reset the screen
		// draw the super blocks
		DrawMap2(3, 2, map_emptySuper);
		DrawMap2(15, 2, map_emptySuper);
		DrawMap2(3, 14, map_emptySuper);
		DrawMap2(15, 14, map_emptySuper);
		// draw the marbles
		for(char y = 0; y < 6; y++)
			for(char x = 0; x < 6; x++)
				if(marbles[y][x] > 0)
					DrawMarble(x, y, 0, marbles[y][x] - 1);
		
		// Change players
		TogglePlayer();
		
		// go back to marble placing state
		nextPlayingState = PLAYINGSTATE_PLACEMARBLE;
		
		// check for win
		int winner = FindWin();
		if(winner > 0)
		{
			// we found a winner, now find out if it was a tie
			if(winner == 3)
				wonPlayer = 0;
			else
				wonPlayer = winner; // set the winner
				
			// set the state
			nextState = STATE_GAMEOVER;
			nextPlayingState = -1;
		}
	}
	
	// check for pause button
	if(padPressed[currentPlayer] & BTN_START)
	{
		prePauseState = PLAYINGSTATE_SELECTROTATE;
		nextPlayingState = PLAYINGSTATE_PAUSE;
	}
}

void InitPlayState_Pause()
{
	// draw the menu
	DrawMap2(10, 11, map_pauseMenu);
	
	// draw the player number
	SetTile( 15, 13, currentPlayer + TILE_NUMBERS_OFFSET + 1);
	
	// draw the selector
	pauseSelect = 0;
	SetTile(11, 14, TILE_RIGHTARROW);
}

void DoPlayState_Pause()
{
	static int ret;
	ret = 0;

	// take the selection input
	if(padPressed[currentPlayer] & BTN_DOWN && pauseSelect == 0)
	{
		// set the selection
		pauseSelect = 1;
		
		// update the tiles
		SetTile(11, 14, 0);
		SetTile(12, 15, TILE_RIGHTARROW);
	}
	else if(padPressed[currentPlayer] & BTN_UP && pauseSelect == 1)
	{
		// set the selection
		pauseSelect = 0;
		
		// update the tiles
		SetTile(11, 14, TILE_RIGHTARROW);
		SetTile(12, 15, 0);
	}

	// go back
	if(padPressed[currentPlayer] & BTN_B || (padPressed[currentPlayer] & BTN_A && pauseSelect == 0))
	{
		// reset the screen
		// draw the super blocks
		DrawMap2(3, 2, map_emptySuper);
		DrawMap2(15, 2, map_emptySuper);
		DrawMap2(3, 14, map_emptySuper);
		DrawMap2(15, 14, map_emptySuper);
		// draw the marbles
		for(char y = 0; y < 6; y++)
			for(char x = 0; x < 6; x++)
				if(marbles[y][x] > 0)
					DrawMarble(x, y, 0, marbles[y][x] - 1);
	
		// change the state
		nextPlayingState = prePauseState;
	}
	
	// check for quit
	if(padPressed[currentPlayer] & BTN_A && pauseSelect == 1)
	{
		if(currentPlayer == 0)
			wonPlayer = 2;
		else
			wonPlayer = 1;
		nextState = STATE_GAMEOVER;
		playingState = -1;
	}
}

void DoState_Playing()
{
	// check for a new state
	if(nextPlayingState > -1)
	{
		// init the new state
		if(nextPlayingState == PLAYINGSTATE_PLACEMARBLE)
			InitPlayState_PlaceMarble();
		else if(nextPlayingState == PLAYINGSTATE_SELECTSUPER)
			InitPlayState_SelectSuper();
		else if(nextPlayingState == PLAYINGSTATE_SELECTROTATE)
			InitPlayState_SelectRotate();
		else if(nextPlayingState == PLAYINGSTATE_PAUSE)
			InitPlayState_Pause();
		
		// setup the state
		playingState = nextPlayingState;
		nextPlayingState = -1;
	}

	// handle the states
	if(playingState == PLAYINGSTATE_PLACEMARBLE)
		DoPlayState_PlaceMarble();
	else if(playingState == PLAYINGSTATE_SELECTSUPER)
		DoPlayState_SelectSuper();
	else if(playingState == PLAYINGSTATE_SELECTROTATE)
		DoPlayState_SelectRotate();
	else if(playingState == PLAYINGSTATE_PAUSE)
		DoPlayState_Pause();
}