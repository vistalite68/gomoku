#ifndef BOARD_HPP
# define BOARD_HPP

# include <vector>
# include <iostream>
# include "define.hpp"

class				Board
{


	public:
		enum class Point : char {EMPTY, BLACK, WHITE};
								Board(void);
								Board(const Board &p);
		virtual					~Board(void);
		Board&					operator=(const Board &p);

		std::vector<Point> const		&getBoard(void) const;
		bool					isMoveValid(int pos) const;
		bool					isWinningBoard(void) const;

	private:
		bool					_checkWinningLine(bool isRow) const;
		bool					_checkWinningDiag(bool down) const;
		bool					_checkWinningBackDiag(bool down) const;

		std::vector<Point>		_board;
};

#endif
