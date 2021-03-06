#include "Board.hpp"

Board::Board(void) : _board(GRID_SIZE, Board::Point::EMPTY), _hash(0)
{
}

Board::Board(const std::vector<Point> &grid) : _board(grid)
{
	this->_hashBoard();
}

Board::Board(const Board &obj)
{
	*this = obj;
}

Board::Point		Board::_getPointOfChar(char c)
{
	switch (c)
	{
		case 'X':
			return Board::Point::BLACK;
		case 'O':
			return Board::Point::WHITE;
	}
	return Board::Point::EMPTY;
}

void					Board::loadBoard(const std::string &filename)
{
	std::string line;
	std::ifstream infile(filename) ;
	std::vector<Board::Point>		grid;

	if ( infile ) {
		while ( getline( infile , line ) ) {
			for (auto c : line)
			{
				if (c == 'X' || c == 'O' || c == '_')
					grid.push_back(this->_getPointOfChar(c));
			}
		}
	} else {
		std::cerr << "Unable to open the files" << std::endl;
	}
	infile.close();
	if (grid.size() != GRID_SIZE)
	{
		std::cerr << grid.size() << std::endl;
		std::cerr << "Bad size of grid for file " << filename << ", board not loaded" << std::endl;
		return;
	}
	this->_board = grid;
}

Board    								&Board::operator=(const Board &p)
{
	this->_board = p.getBoard();
	this->_lastMove = p.getLastMove();
	this->_hash = p.getHash();
	this->_blackStoneCaptured = p.getBlackCapturedStone();
	this->_whiteStoneCaptured = p.getWhiteCapturedStone();
	return *this;
}

const std::vector<Board::Point>&		Board::getBoard(void) const
{
	return (const_cast<const std::vector<Board::Point>&>(this->_board));
}

uint64_t								Board::getHash() const
{
	return (this->_hash);
}

void									Board::setLastMoves(int pos)
{
	this->_lastMove = pos;
}

int										Board::getLastMove() const
{
	return this->_lastMove;
}

bool									Board::isFirstMove() const
{
	if (this->_lastMove == -1)
		return true;
	return false;
}

Board::Point							Board::lookAt(int index) const
{
	return (this->_board[index]);
}

Board::Point							Board::getOppColor(Point player_color)
{
	if (player_color == Point::WHITE)
		return Point::BLACK;
	return Point::WHITE;
}

void									Board::_addNCheckCapture(int &tmpPos, int n)
{
	if (tmpPos < 0)
		return;
	tmpPos += n;
	if (tmpPos >= GRID_SIZE)
		tmpPos = -1;
}

void									Board::_updatePosCheckCapture(int *tmpPos)
{
	this->_addNCheckCapture(tmpPos[DirIndex::HRIGHT], 1);
	if (tmpPos[DirIndex::HRIGHT] % GRID_LENGTH == 0)
	{
		tmpPos[DirIndex::HRIGHT] = -1;
	}
	this->_addNCheckCapture(tmpPos[DirIndex::HLEFT], -1);
	if (tmpPos[DirIndex::HLEFT] % GRID_LENGTH == (GRID_LENGTH - 1))
	{
		tmpPos[DirIndex::HLEFT] = -1;
	}
	this->_addNCheckCapture(tmpPos[DirIndex::VUP], -GRID_LENGTH);
	this->_addNCheckCapture(tmpPos[DirIndex::VDOWN], GRID_LENGTH);
	this->_addNCheckCapture(tmpPos[DirIndex::DIAG1UP], -(GRID_LENGTH + 1));
	this->_addNCheckCapture(tmpPos[DirIndex::DIAG1DOWN], GRID_LENGTH + 1);
	this->_addNCheckCapture(tmpPos[DirIndex::DIAG2UP], -(GRID_LENGTH - 1));
	this->_addNCheckCapture(tmpPos[DirIndex::DIAG2DOWN], (GRID_LENGTH - 1));
}

void									Board::_processPosCheckCapture(int *tmpPos, int *tmpAccrued, const Point &oppColor)
{
	for (int i = 0; i < 8; i++)
	{
		if (tmpPos[i] < 0)
			continue ;
		if (this->_board[tmpPos[i]] == oppColor)
			tmpAccrued[i]++;
	}
}

void									Board::_initTmpPosCheckCapture(int *tmpPos, int pos)
{
	for (int i = 0; i < 8; i++)
	{
		tmpPos[i] = pos;
	}
}

void									Board::_checkLastStoneCheckCapture(int *tmpPos, int *tmpAccrued, bool *toDelete, const Point &color)
{
	for (int i = 0; i < 8; i++)
	{
		if (tmpAccrued[i] != 2)
			continue ;
		if (this->_board[tmpPos[i]] == color)
			toDelete[i] = true;
	}
}

void									Board::_deletePosCheckCapture(int *tmpPos, bool *toDelete, const Board::Point &colorCurrentStone)
{
	for (int i = 0; i < 8; i++)
	{
		if (toDelete[i])
		{
			this->_board[tmpPos[i]] = Board::Point::EMPTY;
			this->_addMoveToHash(tmpPos[i], colorCurrentStone);
			if (colorCurrentStone == Board::Point::WHITE)
				this->_whiteStoneCaptured++;
			else
				this->_blackStoneCaptured++;
		}
	}
}

void									Board::_checkCapture(int pos, const Board::Point &color)
{
	int			tmpPos[8];
	int			tmpAccrued[8] = {};
	bool		toDelete[8] = {};
	const Point	oppColor = Board::getOppColor(color);

	bzero(toDelete, sizeof(bool) * 8);
	this->_initTmpPosCheckCapture(tmpPos, pos);
	for (int i = 0; i < 2; i++)
	{
		this->_updatePosCheckCapture(tmpPos);
		this->_processPosCheckCapture(tmpPos, tmpAccrued, oppColor);
	}
	this->_updatePosCheckCapture(tmpPos);
	this->_checkLastStoneCheckCapture(tmpPos, tmpAccrued, toDelete, color);
	this->_initTmpPosCheckCapture(tmpPos, pos);
	for (int i = 0; i < 2; i++)
	{
		this->_updatePosCheckCapture(tmpPos);
		this->_deletePosCheckCapture(tmpPos, toDelete, oppColor);
	}
}

int										Board::getWhiteCapturedStone(void) const
{
	return this->_whiteStoneCaptured;
}

int										Board::getBlackCapturedStone(void) const
{
	return this->_blackStoneCaptured;
}
void									Board::setMove(int pos, Board::Point color)
{
	if (pos < 0 || pos >= GRID_SIZE)
		return ;
	this->_board[pos] = color;
	this->_addMoveToHash(pos, color);
	this->setLastMoves(pos);
	this->_checkCapture(pos, color);
}

bool									Board::isMoveValid(int pos, Board::Point color) const
{
	if (pos < 0 || pos >= GRID_SIZE)
		return (false);
	else if (this->_board[pos] != Board::Point::EMPTY)
		return (false);
	else if (this->_checkMoveInCapture(pos, color))
		return (false);
	else if (this->_checkDoubleThree(pos, color))
		return (false);
	return (true);
}

bool									Board::isWinningBoard(void) const
{
	if (this->_checkWinningLine(true))
		return (true);
	if (this->_checkWinningLine(false))
		return (true);
	if (this->_checkWinningDiag(true))
		return (true);
	if (this->_checkWinningDiag(false))
		return (true);
	if (this->_checkWinningBackDiag(true))
		return (true);
	if (this->_checkWinningBackDiag(false))
		return (true);
	if (this->_blackStoneCaptured >= 10)
	{
		std::cout << "Black stone are all captured" << std::endl;
		return (true);
	}
	if (this->_whiteStoneCaptured >= 10)
	{
		std::cout << "White stone are all captured" << std::endl;
		return (true);
	}
	return (false);
}

int							Board::getIndex(int i, int j) const
{
	int						index;
	index = j * GRID_LENGTH + i;
	if (index < 0)
		index = 0;
	else if (index >= GRID_SIZE)
		index = GRID_SIZE - 1;
	return (index);
}

void						Board::_addMoveToHash(int index, Board::Point color)
{
	int col = ((color == Board::Point::BLACK) ? 0 : 1);
	this->_hash = this->_hash ^ HashUtilities::getBaseHashTableEntry(index, col);
}

/*
 *		CHECK FOR WINNING BOARD FUNCTIONS
 */

int							Board::_getStreak(Board::Point last, Board::Point curr, int streak) const
{
	if (curr == Board::Point::EMPTY || curr != last)
		return (0);
	else if (curr == last)
		return (streak + 1);
	return (streak);
}

bool						Board::_checkWinningLine(bool isRow) const
{
	int						i, j, index;
	int						streak;
	Board::Point			curr, last;

	i = 0;
	last = Board::Point::EMPTY;
	while (i < GRID_LENGTH)
	{
		j = 0;
		streak = 0;
		while (j < GRID_LENGTH)
		{
			if (isRow)
				index = this->getIndex(j, i);
			else
				index = this->getIndex(i, j);
			curr = this->_board[index];
			if ((streak = this->_getStreak(last, curr, streak)) == 4)
				return (true);
			last = curr;
			j++;
		}
		i++;
	}
	return (false);
}

bool						Board::_checkWinningBackDiag(bool down) const
{
	int						a, i, j, index;
	int						streak;
	Board::Point			curr, last;

	a = 0;
	if (down)
		a++;
	while (a < GRID_LENGTH - 1)
	{
		i = a;
		j = 0;
		streak = 0;
		last = Board::Point::EMPTY;
		while (i < GRID_LENGTH)
		{
			if (down)
				index = this->getIndex(j, i);
			else
				index = this->getIndex(i, j);
			curr = this->_board[index];
			if ((streak = this->_getStreak(last, curr, streak)) == 4)
				return (true);
			last = curr;
			i++;
			j++;
		}
		a++;
	}
	return (false);
}

bool						Board::_checkWinningDiag(bool down) const
{
	int						a, i, j, mod, index;
	int						streak = 0;
	Board::Point			curr, last;

	a = GRID_LENGTH - 1;
	while (a > 0)
	{
		i = a;
		j = 0;
		if (down) {
			mod = (GRID_LENGTH - 1 - a);
			j += mod;
			i += mod;
		}
		streak = 0;
		last = Board::Point::EMPTY;
		while (i > 0 && j < GRID_LENGTH)
		{
			index = this->getIndex(i, j);
			curr = this->_board[index];
			if ((streak = this->_getStreak(last, curr, streak)) == 4)
				return (true);
			last = curr;
			i--;
			j++;
		}
		a--;
	}
	return (false);
}

/*
 *		CHECK FOR DOUBLE THREE FUNCTIONS
 */

bool					Board::_checkDoubleThree(int pos, Board::Point color) const
{
	std::vector<int>	three(3, -1);
	std::vector<int>	three2(3, -1);

	if (this->_checkThreeLine(pos, color, three, false))
	{
		for (int i = 0 ; i < 3 ; i++)
		{
			if (three[i] == pos)
				continue ;
			if (this->_checkThreeLine(three[i], color, three2, true))
				return (true);
			if (this->_checkThreeBackDiag(three[i], color, three2))
				return (true);
			if (this->_checkThreeDiag(three[i], color, three2))
				return (true);
		}
	}

	if (this->_checkThreeLine(pos, color, three, true))
	{
		for (int i = 0 ; i < 3 ; i++)
		{
			if (three[i] == pos)
				continue ;
			if (this->_checkThreeLine(three[i], color, three2, false))
				return (true);
			if (this->_checkThreeBackDiag(three[i], color, three2))
				return (true);
			if (this->_checkThreeDiag(three[i], color, three2))
				return (true);
		}
	}

	if (this->_checkThreeBackDiag(pos, color, three))
	{
		for (int i = 0 ; i < 3 ; i++)
		{
			if (three[i] == pos)
				continue ;
			if (this->_checkThreeLine(three[i], color, three2, true))
				return (true);
			if (this->_checkThreeLine(three[i], color, three2, false))
				return (true);
			if (this->_checkThreeDiag(three[i], color, three2))
				return (true);
		}
	}

	if (this->_checkThreeDiag(pos, color, three))
	{
		for (int i = 0 ; i < 3 ; i++)
		{
			if (three[i] == pos)
				continue ;
			if (this->_checkThreeLine(three[i], color, three2, true))
				return (true);
			if (this->_checkThreeLine(three[i], color, three2, false))
				return (true);
			if (this->_checkThreeBackDiag(three[i], color, three2))
				return (true);
		}
	}
	return (false);
}

void					Board::_resetThreeCheck(std::vector<int> &three, int &it, int &space, bool &firstSpace) const
{
	three.clear();
	it = 0;
	space = 0;
	firstSpace = false;
}

bool					Board::_checkThreeLine(int pos, Board::Point color, std::vector<int> &three, bool isHoriz) const
{
	int					it = 0;
	int					space = 0;
	int					i, j, index;
	int					stop;
	Board::Point		curr, last;
	bool				firstSpace = false;

	if (isHoriz)
	{
		i = pos / GRID_LENGTH;
		j = pos % GRID_LENGTH - 3;
		stop = (pos % GRID_LENGTH) + 5;
	}
	else
	{
		i = pos % GRID_LENGTH;
		j = pos / GRID_LENGTH - 3;
		stop = (pos / GRID_LENGTH) + 5;
	}
	if (j < -2)
		return (false);
	else if (j <= 0)
		j = 1;
	if (stop >= GRID_LENGTH)
		stop = GRID_LENGTH;
	if (isHoriz)
		index = this->getIndex(j - 1, i);
	else
		index = this->getIndex(i, j - 1);
	last = this->_board[index];
	while (j < stop)
	{
		if (isHoriz)
			index = this->getIndex(j, i);
		else
			index = this->getIndex(i, j);
		if (index != pos)
			curr = this->_board[index];
		else
			curr = color;
		if (it == 3 && curr == Board::Point::EMPTY && firstSpace)
			return (true);
		else if (last == color && curr == Board::Point::EMPTY && space < 2)
			space++;
		else if (((last == Board::Point::EMPTY && curr == color) || (last == color && curr == color)) && space < 2)
		{
			if (it == 0 && last == Board::Point::EMPTY)
				firstSpace = true;
			if (it < 3)
				three[it++] = index;
			else
				this->_resetThreeCheck(three, it, space, firstSpace);
		}
		else if ((last == Board::Point::EMPTY && curr == Board::Point::EMPTY) || (curr != last))
			this->_resetThreeCheck(three, it, space, firstSpace);
		last = curr;
		j++;
	}
	return (false);
}

bool					Board::_setCoordDiag(int pos, int &i, int &j, int &stopI, int &stopJ) const
{
	int					k;
	int					posI, posJ;

	posI = pos % GRID_LENGTH;
	posJ = pos / GRID_LENGTH;
	i = posI;
	j = posJ;
	stopI = posI;
	stopJ = posJ;
	if (i == 0 || j == 0 || i == (GRID_LENGTH - 1) || j == (GRID_LENGTH - 1))
		return (false);
	k = 0;
	while (k < 3 && (i > 1 && j > 1))
	{
		i--;
		j--;
		k++;
	}
	k = 0;
	while (k < 5 && (stopI < GRID_LENGTH && stopJ < GRID_LENGTH))
	{
		stopI++;
		stopJ++;
		k++;
	}
	return (true);
}

bool					Board::_checkThreeDiag(int pos, Board::Point color, std::vector<int> &three) const
{
	int					it = 0;
	int					space = 0;
	int					i, j, index;
	int					stopI, stopJ;
	Board::Point		curr, last;
	bool				firstSpace = false;

	if (!(this->_setCoordDiag(pos, i, j, stopI, stopJ)))
		return (false);
	index = this->getIndex(i - 1, j - 1);
	last = this->_board[index];
	while (i < stopI && j < stopJ)
	{
		index = this->getIndex(i, j);
		if (index != pos)
			curr = this->_board[index];
		else
			curr = color;
		if (it == 3 && curr == Board::Point::EMPTY && firstSpace)
			return (true);
		else if (last == color && curr == Board::Point::EMPTY && space < 2)
			space++;
		else if (((last == Board::Point::EMPTY && curr == color) || (last == color && curr == color)) && space < 2)
		{
			if (it == 0 && last == Board::Point::EMPTY)
				firstSpace = true;
			if (it < 3)
				three[it++] = index;
			else
				this->_resetThreeCheck(three, it, space, firstSpace);
		}
		else if ((last == Board::Point::EMPTY && curr == Board::Point::EMPTY) || (curr != last))
			this->_resetThreeCheck(three, it, space, firstSpace);
		last = curr;
		i++;
		j++;
	}
	return (false);
}

bool					Board::_setCoordBackDiag(int pos, int &i, int &j, int &stopI, int &stopJ) const
{
	int					k;
	int					posI, posJ;

	posI = pos % GRID_LENGTH;
	posJ = pos / GRID_LENGTH;
	i = posI;
	j = posJ;
	stopI = posI;
	stopJ = posJ;
	if (i == 0 || j == 0 || i == (GRID_LENGTH - 1) || j == (GRID_LENGTH - 1))
		return (false);
	k = 0;
	while (k < 3 && (i < (GRID_LENGTH - 2) && j > 1))
	{
		i++;
		j--;
		k++;
	}
	k = 0;
	while (k < 5 && (stopI > 0 && stopJ < GRID_LENGTH))
	{
		stopI--;
		stopJ++;
		k++;
	}
	return (true);
}

bool					Board::_checkThreeBackDiag(int pos, Board::Point color, std::vector<int> &three) const
{
	int					it = 0;
	int					space = 0;
	int					i, j, index;
	int					stopI, stopJ;
	Board::Point		curr, last;
	bool				firstSpace = false;

	if (!(this->_setCoordBackDiag(pos, i, j, stopI, stopJ)))
		return (false);
	index = this->getIndex(i - 1, j - 1);
	last = this->_board[index];
	while (i >= stopI && j < stopJ)
	{
		index = this->getIndex(i, j);
		if (index != pos)
			curr = this->_board[index];
		else
			curr = color;
		if (it == 3 && curr == Board::Point::EMPTY && firstSpace)
			return (true);
		else if (last == color && curr == Board::Point::EMPTY && space < 2)
			space++;
		else if (((last == Board::Point::EMPTY && curr == color) || (last == color && curr == color)) && space < 2)
		{
			if (it == 0 && last == Board::Point::EMPTY)
				firstSpace = true;
			if (it < 3)
				three[it++] = index;
			else
				this->_resetThreeCheck(three, it, space, firstSpace);
		}
		else if ((last == Board::Point::EMPTY && curr == Board::Point::EMPTY) || (curr != last))
			this->_resetThreeCheck(three, it, space, firstSpace);
		last = curr;
		i--;
		j++;
	}
	return (false);
}

/*
 *			CHECK FOR MOVE IN CAPTURE
 */

bool					Board::_checkCapture(Board::Point color, Board::Point opp, int i, int j, int modI, int modJ) const
{
	int					index1, index2, index3;

	index1 = this->getIndex(i - 1 * modI, j - 1 * modJ);
	index2 = this->getIndex(i + 1 * modI, j + 1 * modJ);
	index3 = this->getIndex(i + 2 * modI, j + 2 * modJ);
	if (this->_board[index1] == opp && this->_board[index2] == color && this->_board[index3] == opp)
		return (true);
	index1 = this->getIndex(i - 2 * modI, j - 2 * modJ);
	index2 = this->getIndex(i - 1 * modI, j - 1 * modJ);
	index3 = this->getIndex(i + 1 * modI, j + 1 * modJ);
	if (this->_board[index1] == opp && this->_board[index2] == color && this->_board[index3] == opp)
		return (true);
	return (false);
}

bool					Board::_checkMoveInCapture(int pos, Board::Point color) const
{
	int					i, j;
	Board::Point		opp;

	if (color == Board::Point::WHITE)
		opp = Board::Point::BLACK;
	else
		opp = Board::Point::WHITE;
	i = pos % GRID_LENGTH;
	j = pos / GRID_LENGTH;

	if (this->_checkCapture(color, opp, i, j, 1, 1))
		return (true);
	if (this->_checkCapture(color, opp, i, j, 1, -1))
		return (true);
	if (this->_checkCapture(color, opp, i, j, 1, 0))
		return (true);
	if (this->_checkCapture(color, opp, i, j, 0, -1))
		return (true);
	return (false);
}

/*
 *			EXPAND FUNCTIONS
 */

bool					Board::isFinish(const Board::Point &currentColor) const
{
	for (int i = 0; i < GRID_SIZE; i++)
	{
		if (this->isMoveValid(i, currentColor))
			return false;
	}
	return true;
}

std::vector<Board>		Board::expand(Point color) const
{
	std::vector<Board>	st;
	std::unordered_set<int>		dups;
	int							set = 0;
	int							x, y;

	for (int pos = 0 ; pos < GRID_SIZE ; pos++)
	{
		if (this->_board[pos] != Board::Point::EMPTY)
		{
			this->_expandPoint(st, color, pos, dups, 2);
			set++;
		}
	}
	if (set == 0 && this->_lastMove == -1)
	{
		x = (std::rand() % (GRID_LENGTH - 4)) + 2;
		y = (std::rand() % (GRID_LENGTH - 4)) + 2;
		st.push_back(*this);
		(st.back()).setMove(x + y * GRID_LENGTH, color);
	}
	else if (set == 0)
	{
		for (int pos = 0; pos < GRID_SIZE; pos++)
		{
			if (this->isMoveValid(pos, color))
			{
				st.push_back(*this);
				st.back().setMove(pos, color);
				break ;
			}
		}
	}
	return st;
}

void				Board::_expandPoint(std::vector<Board> &st, Board::Point color, int pos, std::unordered_set<int> &dups, int depth) const
{
	int				i, j, index;
	int				m, n;

	i = pos % GRID_LENGTH - depth;
	j = pos / GRID_LENGTH - depth;
	m = pos % GRID_LENGTH + depth + 1;
	n = pos / GRID_LENGTH + depth + 1;
	
	if (i < 0)
		i = 0;
	if (j < 0)
		j = 0;
	if (m > GRID_LENGTH)
		m = GRID_LENGTH;
	if (n > GRID_LENGTH)
		n = GRID_LENGTH;
	while (i < m)
	{
		for (int jj = j ; jj < n ; jj++)
		{
			index = this->getIndex(i, jj);
			if (dups.find(index) == dups.end())
			{
				if (this->isMoveValid(index, color))
				{
					st.push_back(*this);
					(st.back()).setMove(index, color);
					dups.insert(index);
				}
			}
		}
		i++;
	}
}

void		Board::_hashBoard()
{
	int			j;
	Board::Point	p;
	this->_hash = 0;

	for (int i = 0 ; i < GRID_SIZE ; i++)
	{
		if ((p = this->_board[i]) != Board::Point::EMPTY)
		{
			j = ((p == Board::Point::BLACK) ? 0 : 1);
			this->_hash = this->_hash ^ HashUtilities::getBaseHashTableEntry(i, j);
		}
	}
}
