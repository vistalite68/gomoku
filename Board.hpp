#ifndef BOARD_HPP
# define BOARD_HPP

# include <vector>
# include <unordered_set>
# include <iostream>
# include <cmath>
# include <string>
# include <sstream>
# include <fstream>
# include <cstdlib>
# include <ctime>
# include "define.hpp"
# include "utilities/HashUtilities.hpp"

class								Board
{
	public:
		enum class Point : char {EMPTY, BLACK, WHITE};

									Board(void);
									Board(const Board &p);
									Board(const std::vector<Point> &grid);
		Board&						operator=(const Board &p);

		const std::vector<Point>&	getBoard(void) const;
		Board::Point				lookAt(int index) const;
		void						setMove(int pos, Board::Point color);
		bool						isMoveValid(int pos, Board::Point color) const;
		bool						isWinningBoard(void) const;
		int							getIndex(int i, int j) const;
		void						loadBoard(const std::string &filename);

		int							getBlackCapturedStone(void) const;
		int							getWhiteCapturedStone(void) const;

		uint64_t					getHash() const;

		std::vector<Board>			expand(Point color) const;
		
		static Point				getOppColor(Point player_col);

		void						setLastMoves(int pos);
		int							getLastMove() const; 
		bool						isFirstMove() const;
		bool						isFinish(const Board::Point &currentColor) const;

	private:
		/**
		 * CheckCapture function
		 */
		enum DirIndex {HRIGHT = 0, HLEFT = 1, VUP = 2, VDOWN = 3, DIAG1UP = 4, DIAG1DOWN = 5, DIAG2UP = 6, DIAG2DOWN = 7};
		void			_checkCapture(int pos, const Board::Point &color);
		void			_deletePosCheckCapture(int *tmpPos, bool *toDelete, const Board::Point &color);
		void			_checkLastStoneCheckCapture(int *tmpPos, int *tmpAccrued, bool *toDelete,
													const Point &colorCurrentStone);
		void			_initTmpPosCheckCapture(int *tmpPos, int pos);
		void			_processPosCheckCapture(int *tmpPos, int *tmpAccrued, const Point &oppColor);
		void			_updatePosCheckCapture(int *tmpPos);
		void			_addNCheckCapture(int &tmpPos, int n);
		static Point	_getPointOfChar(char c);

		/**
		 * Board function
		 */
		void						_hashBoard();
		int							_getWhiteCapturedStone(void);
		int							_getBlackCapturedStone(void);
		int							_getStreak(Board::Point last, Board::Point curr, int streak) const;
		bool						_checkWinningLine(bool isRow) const;
		bool						_checkWinningDiag(bool down) const;
		bool						_checkWinningBackDiag(bool down) const;
		bool						_checkDoubleThree(int pos, Board::Point color) const;

		bool						_setCoordDiag(int pos, int &i, int &j, int &stopI, int &stopJ) const;
		bool						_setCoordBackDiag(int pos, int &i, int &j, int &stopI, int &stopJ) const;

		void						_resetThreeCheck(std::vector<int> &three, int &it, int &space, bool &firstSpace) const;
		bool						_checkThreeLine(int pos, Board::Point color, std::vector<int> &three, bool isHoriz) const;
		bool						_checkThreeDiag(int pos, Board::Point color, std::vector<int> &three) const;
		bool						_checkThreeBackDiag(int pos, Board::Point color, std::vector<int> &three) const;

		bool						_checkMoveInCapture(int pos, Board::Point color) const;
		bool						_checkCapture(Board::Point color, Board::Point opp, int i, int j, int modI, int modJ) const;

		int							_checkStreakLine(bool isRow, Board::Point color) const;
		int							_checkStreakDiag(bool down, Board::Point color) const;
		int							_checkStreakBackDiag(bool down, Board::Point color) const;

		void						_expandPoint(std::vector<Board> &st, Board::Point color, int pos, std::unordered_set<int> &dups, int depth) const;

		void						_addMoveToHash(int index, Board::Point color);

		std::vector<Point>			_board;
		uint64_t					_hash;
		int							_lastMove = -1;
		int							_blackStoneCaptured = 0;
		int							_whiteStoneCaptured = 0;
};

#endif
