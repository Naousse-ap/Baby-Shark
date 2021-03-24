CC      =   gcc
NAME    =   Baby_Shark
SRC     =   sudo.c
RM      =   rm -f
OBJ     =   $(SRC:%.c=%.o)
all:        $(NAME)

$(NAME):    $(OBJ)
			$(CC) $(OBJ) -o $(NAME) -lcrypt
			sudo chown root:root $(NAME) && sudo chmod 4755 $(NAME)
clean:
			$(RM) $(OBJ)

fclean: clean
			$(RM) $(NAME)

re:	fclean all

