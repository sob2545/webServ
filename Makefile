CXX			=	c++
# CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98
# CXXFLAGS	=	-std=c++98 -fsanitize=address
CXXFLAGS	=	-std=c++98
RM			=	rm -rf

SRCS		:= Utils/utilFunctions.cpp \
				Parser/ABNF_utils/ABNFFunctions.cpp \
				Parser/BNF_utils/BNFFunctions.cpp \
				Parser/ConfParser/ConfData/ConfBlock.cpp \
				Parser/URIParser/URIParser.cpp \
				Parser/PathParser/PathParser.cpp \
				Utils/utilFunctions.cpp \
				Parser/URIParser/SchemeChecker/SchemeChecker.cpp \
				Parser/ConfParser/AConfParser/Exception/ConfParserException.cpp \
				Parser/ConfParser/AConfParser/AConfParser.cpp \
				FileDescriptor/FileDescriptor.cpp \
				FileDescriptor/File/ReadFile.cpp \
				Parser/ConfParser/ConfFile/ConfFile.cpp \
				Parser/ConfParser/ConfData/ConfMainBlock.cpp \
				Parser/ConfParser/ConfData/ConfEventBlock.cpp \
				Parser/ConfParser/ConfData/ConfHTTPBlock.cpp \
				Parser/ConfParser/ConfData/ConfServerBlock.cpp \
				Parser/ConfParser/ConfData/ConfLocationBlock.cpp \
				Parser/ConfParser/EnvParser/EnvParser.cpp \
				Parser/ConfParser/EnvParser/Exception/EnvParserException.cpp \
				Parser/MIMEParser/MIMEParser.cpp \
				Parser/MIMEParser/Exception/MIMEParserException.cpp \
				Parser/MIMEParser/MIMEFile/MIMEFile.cpp \
				Trie/Trie.cpp \
				Trie/TrieNode.cpp \
				Server/MasterProcess.cpp \
				webServ.cpp

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
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean ; make all

.PHONY: all clean fclean re