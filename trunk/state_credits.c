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

// credits state

void InitState_Credits()
{
	// initialize the text
	InitText();
	
	// draw the credits
	PrintStr(1, 1, "Pentago!\nInvented by Tomas Floden\nProduced by Mindtwister\n\nAll rights to the original\nauthors / creators.\n\nUzebox implementation by:\n\n Kenton Hamaluik\n\nProgramming by:\n\n Kenton Hamaluik\n\nGraphics by:\n\n Kenton Hamaluik\n\nCreated in March 2010\n\n\n     Licensed under GPLv3");
	
	PrintStr(1, 26, "Press B to return...");
	
	// fade in
	FadeIn(1, true);
}

void DoState_Credits()
{
	if((padReleased[0] | padReleased[1]) & BTN_B)
		nextState = STATE_MAINMENU;
}