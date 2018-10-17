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
	snake1({ brd.Getwidth() / 3, brd.Getheight() / 3 }),
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
	if (!gameisover1) {
		if (wnd.kbd.KeyIsPressed(VK_A)) {
			if (snake1.seg[0].getloc().y != snake1.seg[1].getloc().y) {
				del_loc1 = { -1,0 };
			};
		};
		if (wnd.kbd.KeyIsPressed(VK_D)) {
			if (snake1.seg[0].getloc().y != snake1.seg[1].getloc().y) {
				del_loc1 = { 1 , 0 };
			}
		};
		if (wnd.kbd.KeyIsPressed(VK_W)) {
			if (snake1.seg[0].getloc().x != snake1.seg[1].getloc().x) {
				del_loc1 = { 0,-1 };
			};

		}
		if (wnd.kbd.KeyIsPressed(VK_S)) {
			if (snake1.seg[0].getloc().x != snake1.seg[1].getloc().x) {
				del_loc1 = { 0, 1 };
			};
		}
		if (wnd.kbd.KeyIsPressed(VK_TAB)) {
			del_loc1 = { 0,0 };
		}
		++snakecounter1;

		if (snakecounter1 >= rate) {
			snakecounter1 = 0;
			const Location next = snake1.getnextloc(del_loc1);
			if (!brd.IsInsideBoard(next) || snake1.IsInTileExceptend(next)) {
				gameisover1 = true;
			}
			else {
				bool eating1 = (next == goal.getloc());
				if (eating1) {
					snake1.Grow(Colors::Yellow);
				}
				snake1.Move(del_loc1);
				if (eating1) {
					goal.Respawn(rng, brd, snake1);
				}
			};
		}
	}
	if (!gameisover) {
		if (wnd.kbd.KeyIsPressed(VK_LEFT)) {
			if (snake.seg[0].getloc().y != snake.seg[1].getloc().y) {
				del_loc = { -1,0 };
			};
		};
		if (wnd.kbd.KeyIsPressed(VK_RIGHT)) {
			if (snake.seg[0].getloc().y != snake.seg[1].getloc().y) {
				del_loc = { 1 , 0 };
			}
		};
		if (wnd.kbd.KeyIsPressed(VK_UP)) {
			if (snake.seg[0].getloc().x != snake.seg[1].getloc().x) {
				del_loc = { 0,-1 };
			};

		}
		if (wnd.kbd.KeyIsPressed(VK_DOWN)) {
			if (snake.seg[0].getloc().x != snake.seg[1].getloc().x) {
				del_loc = { 0, 1 };
			};
		}
		if (wnd.kbd.KeyIsPressed(VK_RETURN)) {
			del_loc = { 0,0 };
		}
		++snakecounter;

	}

	if (snakecounter >= rate) {
		snakecounter = 0;
		const Location next = snake.getnextloc(del_loc);
		if (!brd.IsInsideBoard(next) || snake.IsInTileExceptend(next)) {
			gameisover = true;
		}
		else {
			bool eating = (next == goal.getloc());
			if (eating) {
				snake.Grow(Colors::Red);
			}
			snake.Move(del_loc);
			if (eating) {
				goal.Respawn(rng, brd, snake);
			}
		};
	}
	are_snakes_colliding(snake, snake1);
}


void Game::reset(Snake &snake)
{
	snake.resetsnake(brd);
}
void Game::are_snakes_colliding(Snake & s1, Snake & s2)
{
	if (!((s1.nseg == 1) || (s2.nseg == 1)))
	{


		if (s1.seg[0].getloc() == s2.seg[0].getloc()) {
			gameisover = true;
			gameisover1 = true;
		}
		for (int j = 1; j < s2.nseg; j++) {
			if (s1.seg[0].getloc() == s2.seg[j].getloc()) {
				gameisover = true;
				break;
			}
		}
		for (int i = 0; i < s1.nseg; i++) {
			if (s2.seg[0].getloc() == s1.seg[i].getloc()) {
				gameisover1 = true;
				break;
			}
		}
	}
}
void Game::ComposeFrame()
{
	brd.DrawBorder();
	snake.Draw(brd);
	snake1.Draw(brd);
	goal.Draw(brd);

	if (gameisover) {
		if (wnd.kbd.KeyIsPressed(VK_SPACE)) {
			gameisover = false;
			reset(snake);
		}
	}
	if (gameisover1) {
		if (wnd.kbd.KeyIsPressed(VK_CONTROL)) {
			gameisover1 = false;
			reset(snake1);
		}
	}
	bool over = (gameisover && gameisover1);
	if (over == 1) {
		SpriteCodex::DrawGameOver(400, 300, gfx);
		if (wnd.kbd.KeyIsPressed(VK_DELETE)) {
			gameisover = false;
			gameisover1 = false;
			reset(snake);
			reset(snake1);
		}
	}
}

