/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"
#define	VK_W 87
#define VK_S 83
#define VK_A 65
#define VK_D 68
Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	rng(std::random_device()()),
	brd( gfx ),
	snake({ brd.Getwidth()/2, brd.Getheight()/2 }),
	goal(rng,brd,snake,gfx)
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if (!gameisover) {
		
		++snakecounter;

		if (snakecounter >= rate) {
			snakecounter = 0;
			const Location next = snake.getnextloc(del_loc);
			if (!brd.IsInsideBoard(next) || snake.IsInTileExceptend(next)) {
				gameisover = true;
			}
			else {
				bool eating = (next == goal.getloc());
				if (eating) {
					snake.Grow();
				}
				if (wnd.kbd.KeyIsPressed(VK_RETURN)) {
					del_loc = { 0,0 };
				}
				snake.Move(del_loc);
				if (eating) {
					goal.Respawn(rng, brd, snake);
				}
			};

		}
	}
}

void Game::reset()
{
	snake.resetsnake(brd);
}

void Game::AI_move()
{
	float d = distance(snake.seg[0].loc, goal.getloc);
	Location possible[3];

	//snake's orientation check 1
	if (snake.seg[0].loc.x == (snake.seg[1].loc.x + 1)) {
		possible[0] = { 0, -1};
		possible[1] = { 0, 1 };
		possible[2] = { 1, 0 };
		
		const Location next0 = snake.getnextloc(possible[0]);
		const Location next1 = snake.getnextloc(possible[1]);
		const Location next2 = snake.getnextloc(possible[2]);

		//wall checks
			if (!brd.IsInsideBoard(next0)) {
				possible[0] = { 0,0 };
			}
			if (!brd.IsInsideBoard(next1)) {
				possible[1] = { 0, 0 };
			}
			if (!brd.IsInsideBoard(next2)) {
				possible[2] = { 0,0 };	//yesterday I left here and one thing is left
										// -> after every possible values is 0, one thing is
										//    for sure, the snake is gonna die so make any 
										//    random move
			}

		//Self collision test
			if(snake.seg[0].loc.y == )
	}

	//Snake's Orientation Check 2
}
float Game::distance(Location l1, Location l2)
{
	float X = (l2.x - l1.x)*(l2.x - l1.x);
	float Y = (l2.y - l1.y)*(l2.y - l1.y);
	float Z = X + Y;
	float d = sqrt( Z );
	return d;
}



void Game::ComposeFrame()
{
	brd.DrawBorder();
	snake.Draw(brd);
	goal.Draw(brd);
	
	if (gameisover) {
		SpriteCodex::DrawGameOver(400, 300, gfx);
		if(wnd.kbd.KeyIsPressed(VK_SPACE)) {
			gameisover = false;
			reset();
		}
	}

}
