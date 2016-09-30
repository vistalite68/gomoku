#ifndef AI_HPP
# define AI_HPP

# include "../Board.hpp"
# include "../heuristics/AbstractHeuristic.hpp"

class						AI
{
	public:
							AI(void);
							AI(const AI & rhs);
							AI(AbstractHeuristic *h, Board::Point &color);
		virtual				~AI(void);
		AI&					operator=(const AI & rhs);

		int					minimax(Board *node, int depth, bool player);
		int					minimaxAB(Board *node, int depth, int A, int B, bool player);
		int					negamax(Board *node, int depth, int A, int B, int player);

		int					nb_state = 0;

	private:
		AbstractHeuristic	*_h;
		Board::Point		_player_color;

		long long			_t_expansion;
		long long			_t_eval;
		long long			_t_vector_clear;
};


#endif
