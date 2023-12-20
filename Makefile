NAME = ircserv

SRCS = $(wildcard srcs/*.cpp)

OBJS_D = objs

OBJS = $(SRCS:%.cpp = /%.o)

CC = c++

FLAGS = -Wall -Wextra  -std=c++98 -g -fsanitize=address

RM = rm -rf

##### COLORS #####

WHITE	= \033[0;37m
BLACK	= \033[0;30m
RED		= \033[0;31m
YELLOW	= \033[0;33m
BLUE	= \033[0;34m
GREEN	= \033[0;32m
DEFAULT = \033[0m

##### BOLD COLORS #####

BRED	= \033[31;1m
BGREEN	= \033[32;1m
BYELLOW	= \033[33;1m
BBLUE	= \033[34;1m
BMAGENTA	= \033[35;1m
BCYAN	= \033[36;1m
BWHITE	= \033[37;1m

##### ACTIONS #####

%.o:%.cpp
	@mkdir -p $(OBJS_D)
	$(CC) $(FLAGS) -o $@ -c $<

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(BLUE)Compiling $(YELLOW)$(NAME)$(BLUE) . . . . . . .$(DEFAULT)"
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)Successfully built --> 😈😈😈 $(YELLOW)$(NAME)$(DEFAULT) 😈😈😈"

clean: 
	@$(RM) $(OBJS_D)
	@echo "$(BYELLOW)Objects $(BRED)REMOVED$(DEFAULT)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(BYELLOW)$(NAME) $(BRED)REMOVED$(DEFAULT)"

re: fclean all

do:
	@echo "🕐 make re ongoing.... 🕐"
	@make re
	@echo "$(GREEN) ------> Running on port 6666 with "pass" as password $(DEFAULT)"	
	@./ircserv 6666 pass

.PHONY: all clean re fclean

god:
	git status
	git add .
	git status
	git commit -m "😈 Random Makefile Commit 😈"
	git status
