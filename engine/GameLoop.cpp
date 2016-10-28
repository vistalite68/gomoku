#include "GameLoop.hpp"

GameLoop::GameLoop(void) :
	_p1(Parser::PlayerType::AI), _p2(Parser::PlayerType::HUMAN)
{
	this->_initServer();
	this->_createPlayers();
	this->_initDisplay();

}

GameLoop::GameLoop(Parser::PlayerType p1, Parser::PlayerType p2) :
	_p1(p1), _p2(p2)
{
	this->_initServer();
	this->_createPlayers();
	this->_initDisplay();

}

GameLoop::GameLoop(const GameLoop &obj) {
	*this = obj;
}

GameLoop::~GameLoop(void) {
	delete this->_players[0];
	delete this->_players[1];
	delete this->_display;
	delete this->_server;
}

GameLoop	&GameLoop::operator=(const GameLoop &p) {
	(void)p;
	return *this;
}

void		GameLoop::_createPlayers(void) {
	if (this->_p1 == Parser::PlayerType::AI)
		this->_players[0] = new AIPlayer("Black", Board::Point::BLACK);
	else
		this->_players[0] = new NetworkPlayer("Black", Board::Point::BLACK, this->_server);
	if (this->_p2 == Parser::PlayerType::AI)
		this->_players[1] = new AIPlayer("White", Board::Point::WHITE);
	else
		this->_players[1] = new NetworkPlayer("White", Board::Point::WHITE, this->_server);
}

void		GameLoop::_initServer(void) {
	this->_server = new Server();
}

void		GameLoop::_initDisplay(void) {
	this->_display = new NetworkDisplay(this->_server);
}

void		GameLoop::_getPlayerMove(AbstractPlayer &player) {
	int	pos;
	std::chrono::high_resolution_clock::time_point		start, end;

	while (1)
	{
		
		start = std::chrono::high_resolution_clock::now();
		pos = player.getMove(this->_board);
		end = std::chrono::high_resolution_clock::now();
		player.addTime(std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count());
		if (this->_board.isMoveValid(pos, player.getColor()))
		{
			this->_board.setMove(pos, player.getColor());
		//	this->_board.setLastMoves(pos);
			return ;
		}
		std::cout << "Move invalid" << std::endl;
	}
}

void			GameLoop::launchGame(void) {
	auto p = this->loop();

	std::cout << "Winner is: " << p->getName() << std::endl;
	this->_server->sendFinalStats(this->_players);
	this->_server->sendWinner(p->getColor());
}

void                printT(unsigned long int t)
{
	int             m, s, ms, us;

	m = (t / 60000000);
	s = (t / 1000000) % 60;
	ms = (t / 1000) % 1000;
	us = t % 1000;
	printf("Time taken for turn: %dm%ds%dms%dus\n\n", m, s, ms, us);
}

AbstractPlayer	*GameLoop::loop(void)
{
	MHeuristic		h;
	int				nbTurn = 1;
	
	this->_display->displayBoard(this->_board);
	while (1)
	{
		for (auto p : this->_players)
		{
			this->_getPlayerMove(*p);
			this->_display->displayBoard(this->_board);
			this->_server->sendLoopState(*p, nbTurn, p->getColor());
			if (this->_board.isWinningBoard())
			{
				for (auto p : this->_players)
				{
					p->dispAverage();
				}
				return (p);
			}
		}
		nbTurn++;
	}
}
