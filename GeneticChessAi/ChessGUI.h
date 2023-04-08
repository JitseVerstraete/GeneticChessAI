#pragma once


class SDL_Window;
class SDL_Renderer;
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "thc.h"
#include <thread>

class ChessGUI final
{
public:
	ChessGUI(int size);
	~ChessGUI();


	void HandleEvents();
	void DrawBoardState(const thc::ChessPositionRaw* position);
	bool IsQuit();



private:

	SDL_Window* m_pWindow{ nullptr };
	SDL_Renderer* m_pRenderer{ nullptr };

	thc::Square m_DraggedSquare{ thc::SQUARE_INVALID };


	thc::Move m_MoveTried;


	int m_Size;

	SDL_Color m_LightCol;
	SDL_Color m_DarkCol;

	SDL_Texture* m_pPieceSheet;
	int m_pieceSheetWidth;
	int m_pieceSheetHeight;


	bool m_Quit;




};

