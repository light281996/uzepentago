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

// how to play state

void InitState_HowToPlay()
{
	// initialize the text
	InitText();
	
	// draw the text
	PrintStr(1, 1, "How To Play:");
	SetTile(2, 3, 20);
	PrintStr(4, 3, "This is a two-player game");
	PrintStr(4, 4, "so get a friend!");
	SetTile(2, 6, 20);
	PrintStr(4, 6, "First player places their");
	PrintStr(4, 7, "marble on an empty board");
	PrintStr(4, 8, "space");
	SetTile(2, 10, 20);
	PrintStr(4, 10, "That player then rotates");
	PrintStr(4, 11, "one of the sub-boards by");
	PrintStr(4, 12, "90 degrees. Turn is over");
	SetTile(2, 14, 20);
	PrintStr(4, 14, "Whoever can get 5 of");
	PrintStr(4, 15, "their marbles in a row");
	PrintStr(4, 16, "(straight or diagonal)");
	PrintStr(4, 17, "first, at any time, wins!");
	
	PrintStr(1, 26, "Press B to return...");
	
	// fade in
	FadeIn(1, true);
}

void DoState_HowToPlay()
{
	if((padReleased[0] | padReleased[1]) & BTN_B)
		nextState = STATE_MAINMENU;
}