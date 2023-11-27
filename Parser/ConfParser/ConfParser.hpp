#pragma once

#include "../../utils/utilFunctions.hpp"
#include <map>
#include <vector>

namespace	MIME {
	typedef std::vector<std::string>	extensionName;
	struct	mime {
		std::map<std::string, extensionName >	types;
	};
}

namespace   CONF {
	enum SpecialLetter{
		CR = '\r',
		LF = '\n',
		HTAB = '\t',
		SP = ' ',
		UNDERSCORE = '_',
		DQUOTE = '\"',
		SEMICOLON = ';'
	};

	typedef	std::vector<std::string>	strVec;

	struct location {
		bool							autoindex;
		bool							internal;
		std::string						root;
		std::string						error_page;
		std::string						access_log;
		strVec							index;
	};

	struct server {
		bool							autoindex;
		unsigned short					port;
		std::string						root;
		strVec							server_name;
		std::string						error_page;
		std::string						access_log;
		strVec							index;
		std::map<std::string, location>	server_block;
	};

	struct http {
		bool							autoindex;
		std::string						root;
		std::string						default_type;
		std::string						error_page;
		std::string						access_log;
		strVec							index;
		std::map<std::string, server>	server_block;
	};

	struct events {
		unsigned int	worker_connections;
	};

	struct main {
		bool								daemon;
		unsigned int					    worker_process;
		unsigned long						time_resolution;
		std::string							error_log;
		std::map<std::string, std::string>	env;
		events								event_block;
		http								http_block;

	};

	CONF::main	Parser(const std::string& file);
}

