#ifndef AI_HPP
# define AI_HPP

# include <sys/time.h>
# include <boost/cstdint.hpp>
# include <boost/random.hpp>
# include <unordered_map>
# include "../Board.hpp"
# include "../heuristics/AbstractHeuristic.hpp"
# include "../utilities/CheckForceMove.hpp"
# include "../utilities/BrowseBoard.hpp"

using time_point = std::chrono::high_resolution_clock::time_point;

namespace			TTUtility
{
	enum Flag {EXACT = 3, LOWERBOUND = 2, UPPERBOUND = 1};

	typedef struct		s_ttEntry
	{
		int				value;
		int				depth;
		int				age;
		Flag			flag;
	}					t_ttEntry;
}

class						AI
{
	public:
							AI(void);
							AI(const AI & rhs);
							AI(AbstractHeuristic *h, Board::Point &color, int aiLevel);
		virtual				~AI(void);
		AI&					operator=(const AI & rhs);

		void				setInitialDepth(int depth);

		bool				hashComp(const Board &a, const Board &b);

//		int					minimax(Board *node, int depth, bool player);
//		int					minimaxAB(Board *node, int depth, int A, int B, bool player);
		int					negamax(Board &node, int depth, int A, int B, int player);
		int					ID(const Board & board, Board::Point color);

		int					nb_state = 0;

		void				resetTimer(void);
		void				startTimer();
		void				addTime(long long &dur);
		time_point				getTime();
		int					getMaxDepth() const;
		long long			getInt(time_point start, time_point end);
		void				printTime(long long dur, std::string str);
		void				showTime();

		int					getHistorySize();
		int					getTTSize();
		int64_t				getLastTime() const;


	private:
		const std::vector<Board>	_expandNode(Board &node, int player);
		void				_initBaseHashTable(void);
		void				_updateHistory(Board &node, int depth);

		AbstractHeuristic	*_h;
		Board::Point		_player_color;
		int					_initial_depth;

		uint64_t			_baseHashTable[GRID_SIZE][2];
		std::unordered_map<uint64_t, int>	_historyTable;
		std::unordered_map<uint64_t, TTUtility::t_ttEntry>	_transpositionTable;


		long long			_t_expansion;
		long long			_t_eval;
		long long			_t_vector_clear;
		time_point			_timer;
		CheckForceMove		*_lineData;
		BrowseBoard			_browseBoard;
		int64_t				_timeToCalc;
		int					_maxDepth = 0;
		int64_t				_lastTime = 0;
};

#endif
