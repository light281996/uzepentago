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

// main menu state

// global variables
char currentOpt = 0;

// init the state
void InitState_MainMenu()
{
	// initialize the text
	InitText();
	
	// draw the menu
	PrintStr(9, 8, "Pentago!");
	PrintStr(11, 10, "New Game");
	PrintStr(11, 12, "Music");
	PrintStr(11, 14, "How To Play");
	PrintStr(11, 16, "Credits");
	
	// draw the selector
	SetTile(10, 10 + (2 * currentOpt), 127);
	
	// draw the music indicator
	if(!musicOn)
		PrintStr(17, 12, "(Off)");
	else
		PrintStr(17, 12, "(On) ");
	
	// fade in
	FadeIn(1, true);
}

void DoState_MainMenu()
{
	// move the selector
	if((padPressed[0] | padPressed[1]) & BTN_DOWN && currentOpt < 3)
	{
		TriggerFx(4, 0xFF, true);
		SetTile(10, 10 + (2 * currentOpt), ' ');
		currentOpt++;
		SetTile(10, 10 + (2 * currentOpt), 127);
	}
	if((padPressed[0] | padPressed[1]) & BTN_UP && currentOpt > 0)
	{
		TriggerFx(4, 0xFF, true);
		SetTile(10, 10 + (2 * currentOpt), ' ');
		currentOpt--;
		SetTile(10, 10 + (2 * currentOpt), 127);
	}
	
	// take the selection input
	if((padPressed[0] | padPressed[1]) & BTN_A)
	{
		TriggerFx(5, 0xFF, true);
		if(currentOpt == 1)
		{
			// toggle the music
			musicOn = 1 - musicOn;
			
			// draw the music indicator
			if(!musicOn)
			{
				StopSong();
				PrintStr(17, 12, "(Off)");
			}
			else
			{
				StartSong(song_finalfantasy);
				PrintStr(17, 12, "(On) ");
			}
		}
		else
		{
			if(currentOpt == 0)
				nextState = STATE_PLAYERSTART;
			else
				nextState = currentOpt + 1;
		}
	}
}