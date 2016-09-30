# include <chrono>
# include <iostream>
# include "Board.hpp"
# include "display/StdOutDisplay.hpp"
# include "engine/GameLoop.hpp"

int		main()
{
	GameLoop	game;

	game.launchGame();
/*	VHeuristic	h;
	h.display = true;
	Board	b;

	b.setMove(0, Board::Point::BLACK);
	b.setMove(1, Board::Point::BLACK);
	b.setMove(2, Board::Point::WHITE);
	b.setMove(3, Board::Point::BLACK);
	b.setMove(4, Board::Point::EMPTY);
	h.eval(&b, Board::Point::BLACK, true);*/
	return (0);
}
