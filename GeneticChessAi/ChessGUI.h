#pragma once


class SDL_Window;
class SDL_Renderer;
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "thc.h"

class ChessGUI
{
public:
	ChessGUI(int size);
	~ChessGUI();


	void DrawBoardState(const thc::ChessPositionRaw* position);
	void HandleEvents();
	bool IsQuit();



private:

	SDL_Window* m_pWindow{ nullptr };
	SDL_Renderer* m_pRenderer{ nullptr };

	int m_Size;

	SDL_Color m_LightCol;
	SDL_Color m_DarkCol;

	SDL_Texture* m_pPieceSheet;
	int m_pieceSheetWidth;
	int m_pieceSheetHeight;


	bool m_Quit;


};

