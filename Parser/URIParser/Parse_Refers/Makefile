CXX			=	c++
# CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98
CXXFLAGS	=	-std=c++98
RM			=	rm -rf

SRCS		:= $(wildcard *.cpp)
OBJS_DIR	:=	objs/

OBJS		:=	$(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))
NAME		:= a.out


all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJS_DIR)%.o : %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -c -o $@

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean ; make all

.PHONY: all clean fclean re