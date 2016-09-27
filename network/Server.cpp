#include "Server.hpp"

Server::Server(void) {
	this->_initServer();
}

void	Server::_initServer() {
	struct sockaddr_in serv_addr; 

	char sendBuff[1025];

	this->_listenFd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(4203); 

	bind(this->_listenFd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

	listen(this->_listenFd, 10); 
	this->_getClient();
}

int		Server::_getClientColor(Board::Point &color) {
	switch (color) {
		case (Board::Point::BLACK):
			return (0);
		case (Board::Point::WHITE):
			return (1);
		case (Board::Point::EMPTY):
			return (2);
	}
	return (2);
}

void			Server::_sendMsg(const char *str) {
	int		ret;

	//const char *str = "toto";
	size_t size = strlen(str);
	int fd = open("toto", O_WRONLY|O_CREAT|O_TRUNC);
	while (size > 0)
	{
		std::cout << "here" << std::endl << std::flush;
		std::cout << "Str: " << str << " write size: " << size << std::endl << std::flush;
		printf("p: %p", str);
		fflush(0);
		if ((ret = write(fd, str, size)) < 0)
		{
			std::cout << "TOTO" << std::endl << std::flush;
			this->_lostCo = true;
			return ;
		}
		std::cout << "end" << std::endl << std::flush;
		size -= ret;
		str += ret;
		std::cout << "then" << std::endl << std::flush;
	}

}

std::vector<std::string>	*Server::_splitString(std::string &str) {
	std::vector<std::string> *strings = new std::vector<std::string>;
	std::istringstream f(str);
	std::string s;    
	while (getline(f, s, '-')) {
		strings->push_back(s);
	}
	return (strings);
}

void		Server::sendBoard(const Board &board) {
	std::string		str = "DISP-";

	auto rawBoard = board.getBoard();
	std::cout << "Before" << std::endl << std::flush;
	for (auto &k: rawBoard)
	{
		str += std::to_string(this->_getClientColor(k));
	}
	std::cout << "Here" << std::endl << std::flush;
	str = "toto";
	this->_sendMsg(str.c_str());
	std::cout << "After" << std::endl << std::flush;
	std::cout << str << std::endl;
}

int			Server::getMove(Board::Point &color)
{
	int		iColor = this->_getClientColor(color);
	std::string	msg = "GMOV-";
	std::string	*res;

	msg += std::to_string(iColor);
	std::cout << msg << std::endl;
	this->_sendMsg(msg.c_str());
	res = this->_getMsg(10);
	std::cout << "res = " << *res << std::endl;
	auto arr = this->_splitString(*res);
	if (arr->size() < 3 || arr->front() != std::string("SMOV"))
		return (-1);
	return (std::stoi(arr->back()));
}

std::string		*Server::_getMsg(size_t size = 0) {
	std::string		msg;
	int				ret = 0;
	char			*res;
	char			*p_res;

	res = new char[size + 1]();
	p_res = res;
	while (size > 0)
	{
		if ((ret = read(this->_connfd, p_res, size)) < 0)
		{
			this->_lostCo = true;
			return nullptr;
		}
		size -= ret;
		p_res += ret;
	}
	return new std::string(res);
}

void	Server::_getClient() {
	std::cout << "waiting for client connection" << std::endl;
	this->_connfd = accept(this->_listenFd, (struct sockaddr*)NULL, NULL);
	std::string	*res = this->_getMsg(sizeof(this->ACCEPT_CO));
	std::cout << *res << std::endl;
	if (*res == this->ACCEPT_CO)
	{
		this->_lostCo = false;
		std::cout << "Client successfully connect" << std::endl;
		return ;
	}
	std::cout << "Client fail to connect" << std::endl;
}

Server::Server(const Server &obj) {
	*this = obj;
}

Server::~Server(void) {
	close(this->_connfd);
	close(this->_listenFd);
}

Server    &Server::operator=(const Server &p) {
	(void)p;
	return *this;
}

