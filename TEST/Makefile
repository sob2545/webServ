CXX			=	c++
# CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98
# CXXFLAGS	=	-std=c++98 -fsanitize=address
CXXFLAGS	=	-std=c++98
RM			=	rm -rf

SRCS		:= ../utils/utilFunctions.cpp \
				../Parser/ABNF_utils/ABNFFunctions.cpp \
				../Parser/BNF_utils/BNFFunctions.cpp \
				../Parser/ConfParser/Data/ConfBlock.cpp \
				../Parser/URIParser/URIParser.cpp \
				../Parser/PathParser/PathParser.cpp \
				../utils/utilFunctions.cpp \
				../Parser/URIParser/SchemeChecker/SchemeChecker.cpp \
				../Parser/ConfParser/Parser/Exception/ConfParserException.cpp \
				../Parser/ConfParser/Parser/AConfParser.cpp \
				../FileDescriptor/FileDescriptor.cpp \
				../FileDescriptor/File/ReadFile.cpp \
				../Parser/ConfParser/File/ConfFile.cpp \
				../Parser/ConfParser/Data/ConfMainBlock.cpp \
				../Parser/ConfParser/Data/ConfEventBlock.cpp \
				../Parser/ConfParser/Data/ConfHTTPBlock.cpp \
				../Parser/ConfParser/Data/ConfServerBlock.cpp \
				../Parser/ConfParser/Data/ConfLocationBlock.cpp \
				../Parser/ConfParser/EnvParser/EnvParser.cpp \
				../Parser/ConfParser/EnvParser/Exception/EnvParserException.cpp \
				../Parser/MIMEParser/MIMEParser.cpp \
				../Parser/MIMEParser/Exception/MIMEParserException.cpp \
				../Parser/MIMEParser/MIMEFile/MIMEFile.cpp \
				../Trie/Trie.cpp \
				../Trie/TrieNode.cpp \
				test.cpp

OBJS_DIR	:= objs/

OBJS		:= $(SRCS:%.cpp=$(OBJS_DIR)%.o)

NAME		:= a.out


all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJS_DIR)%.o : %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS_DIR) FileDescriptor Parser/ Trie utils/

fclean: clean
	$(RM) $(NAME)

re: fclean ; make all

.PHONY: all clean fclean re