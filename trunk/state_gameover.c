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

// player start picking state

// init the state
void InitState_GameOver()
{
	// initialize the text
	InitText();
	
	// draw it out
	PrintStr(10, 1, "Game Over!");
	
	if(wonPlayer == 0)
		PrintStr(10, 13, "Tie Game!");
	else if(wonPlayer == 1)
		PrintStr(8, 13, "Player 1 Won!");
	else if(wonPlayer == 2)
		PrintStr(8, 13, "Player 2 Won!");
	
	PrintStr(1, 26, "Press B to return to menu");
	
	// fade in
	FadeIn(1, true);
}

void DoState_GameOver()
{
	if(padReleased[startPlayer] & BTN_B)
		nextState = STATE_MAINMENU;
}