#include "ChessGUI.h"
#include <iostream>

#include <SDL_image.h>


ChessGUI::ChessGUI(int size)
	:m_Size(size), m_Quit(false)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to initialize the SDL2 library\n";
		throw;
	}

	m_pWindow = SDL_CreateWindow("Chess Board", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Size, m_Size, 0);
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

	m_pPieceSheet = IMG_LoadTexture(m_pRenderer, "Resources/Textures/ChessPieces.png");
	SDL_QueryTexture(m_pPieceSheet, nullptr, nullptr, &m_pieceSheetWidth, &m_pieceSheetHeight);


	m_DarkCol = { 150, 109, 83 };
	m_LightCol = { 227, 227, 227 };
}

ChessGUI::~ChessGUI()
{
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyTexture(m_pPieceSheet);
}

void ChessGUI::DrawBoardState(const thc::ChessPositionRaw* position)
{
	SDL_RenderClear(m_pRenderer);


	SDL_Rect dstRect{};
	dstRect.x = 0;
	dstRect.y = 0;
	dstRect.w = m_Size;
	dstRect.h = m_Size;

	int squareSize = m_Size / 8;




	SDL_SetRenderDrawColor(m_pRenderer, m_LightCol.r, m_LightCol.g, m_LightCol.b, m_LightCol.a);
	SDL_RenderFillRect(m_pRenderer, &dstRect);
	SDL_SetRenderDrawColor(m_pRenderer, m_DarkCol.r, m_DarkCol.g, m_DarkCol.b, m_DarkCol.a);

	//draw black squares
	for (int r{}; r < 8; ++r)
	{
		for (int c{}; c < 8; ++c)
		{
			if ((r + c) % 2 == 0)
			{

				dstRect.w = squareSize;
				dstRect.h = squareSize;
				dstRect.x = c * squareSize;
				dstRect.y = m_Size - ((r + 1) * squareSize);
				SDL_RenderFillRect(m_pRenderer, &dstRect);
			}
		}
	}

	SDL_Rect srcRect{};
	srcRect.w = m_pieceSheetWidth / 6;
	srcRect.h = m_pieceSheetHeight / 2;
	srcRect.x = 0;
	srcRect.y = 0;
	
	//draw pieces
	for (int i{}; i < 64; ++i)
	{
		char letter = position->squares[i];
		if (isspace(letter)) continue;

		switch (tolower(letter))
		{
		case 'k':
			srcRect.x = 0;
			break;
		case 'q':
			srcRect.x = 1 * (srcRect.w);
			break;
		case 'b':
			srcRect.x = 2 * (srcRect.w);
			break;
		case 'n':
			srcRect.x = 3 * (srcRect.w);
			break;
		case 'r':
			srcRect.x = 4 * (srcRect.w);
			break;
		case 'p':
			srcRect.x = 5 * (srcRect.w);
			break;
		default:
			break;
		}

		bool white = isupper(letter);
		if (white)
		{
			srcRect.y = 0;
		}
		else
		{
			srcRect.y = srcRect.h;
		}




		dstRect.y = (i / 8) * squareSize;
		dstRect.x = (i % 8) * squareSize;
		dstRect.w = squareSize;
		dstRect.h = squareSize;


		SDL_RenderCopy(m_pRenderer, m_pPieceSheet, &srcRect, &dstRect);
	}


	SDL_RenderPresent(m_pRenderer);
}

void ChessGUI::HandleEvents()
{
	SDL_Event e{};
	SDL_PollEvent(&e);
	if (e.type == SDL_QUIT)
	{
		m_Quit = true;
	}
}

bool ChessGUI::IsQuit()
{
	return m_Quit;
}
