# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rabougue <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/10 19:16:51 by rabougue          #+#    #+#              #
#    Updated: 2017/11/24 04:44:32 by rabougue         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BLUE = \033[34m
WHITE = \033[37m

##################################_COMPILATION_#################################
NAME	= BackupNX
CC		= clang
FLAG	= -g -Wall -Werror -Wextra
SRCS	=	./source/main.c\
			./source/file.c\
			./source/linked_list.c\

LINKER = 

OBJS	= $(SRCS:.c=.o)

INCLUDE	= -I ./include

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAG) -o $(NAME) $(OBJS) $(LINKER)

%.o : %.c $(RELINK) ./Makefile
	@printf " ✅                                                              \r"
	@printf "✅  $(notdir $<)\r"
	@$(CC) -c $(FLAG) $< -o $@ $(INCLUDE)

clean:
	@printf "                                                               \r"
	@printf "✅  clean done ! (Client)\n"
	@rm -f $(OBJS)

fclean:
	@printf "                                                               \r"
	@printf "✅  fclean done ! (Client)\n"
	@rm -f $(NAME) $(OBJS)

re: fclean all


