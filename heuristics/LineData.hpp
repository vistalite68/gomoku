#ifndef LINEDATA
# define LINEDATA

# include "../Board.hpp"
# include "../utilities/BoardUtilities.hpp"
# include "../utilities/AbstractLineData.hpp"

class				LineData : public AbstractLineData
{
	public:
		LineData(void);
		LineData(const LineData &obj);
		LineData &operator=(const LineData &p);
		virtual ~LineData(void);
		virtual void			init(const Board::Point &color, const Board *b);
	protected:
		void		_endOfSeries(void);
	private:
		int			_getStoneScore(void);
};

#endif
