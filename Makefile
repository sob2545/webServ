CXX			=	c++
# CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98
# CXXFLAGS	=	-std=c++98 -fsanitize=address
CXXFLAGS	=	-std=c++98 -D __DARWIN__=1
RM			=	rm -rf

# SRCS		:= $(wildcard ./FileDescriptor/*/*.cpp \
# 							./MainLoop/*/*.cpp \
# 							./Multiplexing/*/*.cpp \
# 							./Parser/*/*.cpp \
# 							./Trie/*/*.cpp \
# 							./Utils/*/*.cpp \
# 							webServ.cpp)
SRCS := $(shell find . -name '*.cpp' -not -path './File/*' -not -path './objs/*' -not -path './practice/*' -not -path './TEST/*')

OBJS_DIR	:= objs/

OBJS		:= $(SRCS:%.cpp=$(OBJS_DIR)%.o)

NAME		:= a.out

ifdef DEBUG
	CXXFLAGS	+=	-D DEBUG=1
endif

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJS_DIR)%.o : %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug:
	make DEBUG=1 all

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean ; make all

.PHONY: all clean fclean re