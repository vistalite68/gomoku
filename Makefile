CC= clang++
FLAGS=-Wall -Wextra  -std=c++14 -g
NAME_BIN= gomoku
NAME_NODE= ui_gomoku
NODE_FOLDER=ui

SRC= main.cpp \
	 players/AbstractPlayer.cpp \
	 players/STDINPlayer.cpp \
	 players/AIPlayer.cpp \
	 players/NetworkPlayer.cpp \
	 display/AbstractDisplay.cpp \
	 display/StdOutDisplay.cpp \
	 display/NetworkDisplay.cpp \
	 engine/GameLoop.cpp \
	 ai/AI.cpp \
	 heuristics/BadHeuristic.cpp \
	 heuristics/SimpleHeuristic.cpp \
	 heuristics/MHeuristic.cpp \
	 utilities/BrowseBoard.cpp \
	 utilities/AbstractLineData.cpp \
	 utilities/LineData.cpp \
	 utilities/BoardUtilities.cpp \
	 utilities/CheckForceMove.cpp \
	 utilities/HashUtilities.cpp \
	 network/Server.cpp \
	 Board.cpp

OBJ= $(SRC:.cpp=.o)

LIB=
INC=-isystem $(HOME)/.brew/include/

FRWK= 
.PHONY: clean fclean re all
.SILENT:
all:$(NAME_BIN) $(NAME_NODE)
	cp $(NAME_BIN) $(NODE_FOLDER)

$(NAME_NODE):
	(cd ui && npm install && bower install)

$(NAME_BIN): $(OBJ)
		$(CC) -o $(NAME_BIN) $(OBJ) $(LIB) $(FRWK) $(INC) $(FLAGS)
		echo "\t\xF0\x9F\x8F\x81   Compiling \033[35m$(NAME) \033[0mDONE!"

%.o: %.cpp
		echo "\t\xF0\x9F\x94\xA7   Building \033[34m $@ \033[0m" 
		$(CC) -c $(FLAGS) $(INC) -o $@ $<

clean:
		echo "\t\xF0\x9F\x92\xA3   Cleaning"
		rm -rf $(OBJ)

fclean: clean
		echo "\t\xF0\x9F\x9A\xBD   Full Clean"
		rm -rf $(NAME_BIN)

re: fclean all
