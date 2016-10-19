#ifndef MHEURISTIC_HPP
# define MHEURISTIC_HPP

# include "AbstractHeuristic.hpp"
# include <cstring>

class					MHeuristic : public AbstractHeuristic
{
	public:
						MHeuristic(void);
						MHeuristic(const MHeuristic & rhs);
		virtual			~MHeuristic(void);
		MHeuristic&	operator=(const MHeuristic & rhs);

		int				eval(Board *b, Board::Point color);
		static bool		coordValid(int x, int y);
		typedef struct		s_dir {
			int x;
			int	y;
		}					t_dir;
	private:
		enum class LineType : char {HORI, VERT, DIAG1, DIAG2};

		class				LineData
		{
			private:
				int				_nbCons = 0;
				Board::Point	_currentColor;
				Board::Point	_playerColor;
				int				_tot = 0;
				LineType		_dir;
				int				_interSpace = 0;
				bool			_lastIsSpace = false;
				bool			_startingSpace = false;
				bool			_endingSpace = false;
				const std::vector<Board::Point> *_grid;
				const Board		*_board;

				bool			_hasPlace(int pos);
				const MHeuristic::t_dir	_getDir(void) const;
				void			_endOfSeries(void);
				void			_display(void);
				static std::string		_getColor(const Board::Point &col);
				void					_addSpace(void);
				void					_addPointSameColor(void);
				void					_addPointOtherColor(const Board::Point &p, int pos);
			public:
				void			init(const Board::Point &color, const Board *b);
				void			addPoint(const Board::Point &color, int pos);
				int				getScore(void);
				void			setDir(LineType dir);
				void			endOfLine(void);
		};

		const std::vector<Board::Point>	*_b;
		LineData		_lineData;
		Board::Point	_color;
		Board::Point	_oppColor;

		void			_getD1Lines(void);
		void			_getD2Lines(void);
		void			_browseDLine(int startX, int startY, const t_dir &dir);
		void			_getHLines(void);
		void			_getVLines(void);
		void			_getLines(void);
};

#endif
