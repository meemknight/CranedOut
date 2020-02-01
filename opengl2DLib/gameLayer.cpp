#include "gameLayer.h"
#include "windowsPlatformLayer.h"
#include <sstream>
#include <Windows.h>


gl2d::Font f;
gl2d::Texture floorTexture;
gl2d::Texture gearTexture;

bool initGame(gl2d::Renderer2D &renderer)
{
	f.createFromFile("roboto_black.ttf");
	floorTexture.loadFromFile("floor.png");
	gearTexture.loadFromFile("gear.png");
	
	return true;
}

int gameWith = 800;
int gameHeigth = 700;

glm::vec2 players[2] = { {100, 100}, {400, 100} };
int playerSize = 40;

bool gameLoop(float deltaTime, gl2d::Renderer2D &renderer, int w, int h, platform::Window &wind)
{
	renderer.clearScreen();

#pragma region determinePlacement
	renderer.currentCamera.position.y = (gameHeigth - h) / 2.f;
	renderer.currentCamera.position.x = (gameWith - w) / 2.f;

	{
		float zoom = 1;
		if (gameWith < w)
		{
			zoom = (w) / (gameWith * 1.9);
		}
		else if (gameWith > w)
		{
			zoom = (w) / (gameWith * 0.9);
		}
		
		float Hzoom = 1;
		if (gameHeigth < h)
		{
			Hzoom = (h) / (gameHeigth * 2.0);
		}
		else if (gameHeigth > h)
		{
			Hzoom = (h) / (gameHeigth * 0.9);
		}

		if(gameWith*1.1 < w || gameHeigth*1.1 < h)
		{
			zoom = (w) / (gameWith * 1);
			Hzoom = (h) / (gameHeigth * 1);
			zoom = min(zoom, Hzoom);
		}else
		{
			zoom = min(zoom, Hzoom);
		}

		zoom = min(zoom, 3);
		zoom = max(zoom, 0.1);

		renderer.currentCamera.zoom = zoom;
		//renderer.currentCamera.zoom = platform::getPlayerMovement(0).y * 1 + 1;
	}
	
#pragma endregion

	
	
	//renderer.currentCamera.position.y += platform::getPlayerMovement(0).y * deltaTime * -200;
	//renderer.currentCamera.position.x += platform::getPlayerMovement(0).x * deltaTime * 200;

	///fps
	//renderer.renderText({ 0,100 }, std::to_string(1.f/deltaTime).c_str(), f, Colors_Red);
	
#pragma region player
	players[0] += platform::getPlayerMovement(0) * deltaTime * 200.f;
	players[1] += platform::getPlayerMovement(1) * deltaTime * 200.f;

	for(int i=0; i<2; i++)
	{
		if (players[i].x - (playerSize / 2) < 0)
		{
			players[i].x = playerSize / 2;
		}

		if (players[i].y - playerSize / 2 < 0)
		{
			players[i].y = playerSize / 2;
		}

		if (players[i].x + playerSize / 2 > gameWith)
		{
			players[i].x = gameWith - playerSize/2;
		}

		if (players[i].y + playerSize / 2 > gameHeigth)
		{
			players[i].y = gameHeigth - playerSize / 2;
		}
	}

	renderer.renderRectangle({ players[0].x - playerSize / 2, players[0].y - playerSize / 2 , playerSize , playerSize }, {}, 0, gearTexture);
	renderer.renderRectangle({ players[1].x - playerSize / 2, players[1].y - playerSize / 2 , playerSize , playerSize }, {}, 0, gearTexture);

#pragma endregion

#pragma region map
	renderer.render9Patch2({ 0, gameHeigth, gameWith, 200 }, 5, Colors_White, { 0,0 }, 0, floorTexture, DefaultTextureCoords, { 0,0.4,0.4,0 });
	renderer.render9Patch2({ -100,0, 100, gameHeigth + 200 }, 5, Colors_White, { 0,0 }, 0, floorTexture, DefaultTextureCoords, { 0,0.4,0.4,0 });
	renderer.render9Patch2({ gameWith ,0, 100, gameHeigth + 200 }, 5, Colors_White, { 0,0 }, 0, floorTexture, DefaultTextureCoords, { 0,0.4,0.4,0 });
#pragma endregion

	renderer.flush();

	return true;
}

void closeGame()
{
	//save files.
	//we don't have to clear resources like textures since os does that for us :P
}
