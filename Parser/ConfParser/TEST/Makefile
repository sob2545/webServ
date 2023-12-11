CXX			=	c++
# CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98
CXXFLAGS	=	-std=c++98
RM			=	rm -rf

SRCS		:= ../../../utils/utilFunctions.cpp \
				../../../ABNF_utils/ABNFFunctions.cpp \
				../../../BNF_utils/BNFFunctions.cpp \
				../Data/ConfBlock.cpp \
				../../URIParser/URIParser.cpp \
				../../PathParser/PathParser.cpp \
				../../../utils/utilFunctions.cpp \
				../../URIParser/SchemeChecker/SchemeChecker.cpp \
				../Parser/Exception/ConfParserException.cpp \
				../Parser/AConfParser.cpp \
				../../../FileDescriptor/FileDescriptor.cpp \
				../../../FileDescriptor/File/ReadFile.cpp \
				../File/ConfFile.cpp \
				../Data/ConfMainBlock.cpp \
				../Data/ConfEventBlock.cpp \
				../Data/ConfHTTPBlock.cpp \
				../EnvParser/EnvParser.cpp \
				../EnvParser/Exception/EnvParserException.cpp \
				../../MIMEParser/MIMEParser.cpp \
				../../MIMEParser/Exception/MIMEParserException.cpp \
				../../MIMEParser/MIMEFile/MIMEFile.cpp \
				../../../Trie/Trie.cpp \
				../../../Trie/TrieNode.cpp \
				test.cpp
				
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