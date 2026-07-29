NAME = philos

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = philo/main.c \
			 philo/res.c \
				philo/parsing.c  \
				philo/philo.c


OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
