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

// player start picking state

// init the state
void InitState_PlayerStart()
{
	// initialize the text
	InitText();
	
	// draw it out
	PrintStr(7, 11, "Player 1 starts!");
	
	// randomly select a start player
	wonPlayer = 0;
	startPlayer = rand() % 2;
	if(startPlayer == 1)
		SetTile(14, 11, '2');
	
	// let them know how to continue
	PrintStr(3, 13, "Press start to begin...");
	
	// fade in
	FadeIn(1, true);
}

void DoState_PlayerStart()
{
	if(padReleased[startPlayer] & BTN_START)
		nextState = STATE_PLAYING;
}