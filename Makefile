NAME 			=	webserv

INC_PATH		=	includes/
OBJS_PATH 		=	obj/
SRCS_PATH 		=	src/
CC 				=	clang++
CFLAGS			=	-std=c++98 -Wall -Werror -Wextra -Wpedantic -fstandalone-debug
RM				=	rm -rf

INC_FILES 		=	colors.hpp defines.hpp

SRCS_FILES		=	main.cpp 

UTILS_FILES		=	utils.cpp string_utils.cpp
UTILS_HDRS		=	utils.hpp

PARSER_FILES	=	ConfigParser.cpp
PARSER_HDRS		=	config_fields.hpp ConfigParser.hpp
				
SERVER_FILES	= 	Server.cpp Socket.cpp
SERVER_HDRS		= 	Server.hpp Socket.hpp

UTILS_PATH		= 	$(SRCS_PATH)utils
SERVER_PATH		=	$(SRCS_PATH)server
PARSER_PATH		=	$(SRCS_PATH)parser

SRCS 			=	$(addprefix $(SRCS_PATH), $(SRCS_FILES))
SERVER_SRCS		=	$(addprefix $(SERVER_PATH), $(SERVER_FILES))
UTILS_SRCS		= 	$(addprefix $(UTILS_PATH), $(UTILS_FILES))
PARSER_SRCS		= 	$(addprefix $(PARSER_PATH), $(PARSER_FILES))

OBJS_FILES		= 	$(SRCS_FILES:.cpp=.o) $(UTILS_FILES:.cpp=.o) $(SERVER_FILES:.cpp=.o)\
					$(PARSER_FILES:.cpp=.o)

INCLUDES		=	$(addprefix $(INC_PATH), $(INC_FILES))	
OBJS 			=	$(addprefix $(OBJS_PATH), $(OBJS_FILES))

VPATH			=	$(SRCS_PATH) $(UTILS_PATH) $(SERVER_PATH) $(PARSER_PATH)

ALL_INCLUDES	= 	-I$(INC_PATH)\
					-I$(UTILS_PATH)\
					-I$(PARSER_PATH)\
					-I$(SERVER_PATH)\

$(OBJS_PATH)%.o: %.cpp
	@$(CC) $(CFLAGS) $(ALL_INCLUDES) -c $< -o $@

all:	CFLAGS += -O2
all:	$(NAME)

$(NAME):	$(OBJS_PATH) $(OBJS)
	@echo "\n\033[96m Compiling WebServ \033[0m\n"
	$(CC) $(OBJS) -o $(NAME)
	@echo "\\n\033[32;1m WebServ IS READY \033[0m \\n"

$(OBJS_PATH):
	@mkdir -p $(OBJS_PATH)
	@echo "\033[34;1m Created Object Directory \033[0m"
	
linux : CFLAGS += -g
linux :	$(OBJS_PATH) $(OBJS)
	@echo "\033[95m Compiling WebServ \033[0m"
	$(CC) $(OBJS) -o $(NAME)
	@echo "\\n\033[32;1m WebServ IS READY \033[0m \\n"

debug:	CFLAGS += -g
debug:	$(NAME)

clean:
	@$(RM) $(OBJS_FILES) $(OBJS_PATH) 
	@echo "\033[34;1m CLEAN DONE \033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\033[34;1m FCLEAN DONE \033[0m"

re: fclean all

.PHONY: all clean fclean re
