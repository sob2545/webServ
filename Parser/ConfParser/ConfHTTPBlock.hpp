#pragma once

#include "../../utils/utilFunctions.hpp"
#include "ConfServerBlock.hpp"
#include <map>
#include <vector>

namespace   CONF {
	class httpBlock {
	private:
		bool							autoindex;
		std::string						root;
		std::string						default_type;
		std::string						error_page;
		std::string						access_log;
		strVec							index;
		std::map<std::string, serverBlock>	server_block;

		httpBlock(const httpBlock& other);
		httpBlock&	operator=(const httpBlock& other);

	public:
		httpBlock();
		~httpBlock();

		const serverBlock&	operator[](const std::string& key);
		const bool			isAutoindex() const;
		const std::string&	getRoot() const;
		const std::string&	getDefaultType() const;
		const std::string&	getErrorPage() const;
		const std::string&	getAccessLog() const;
		const strVec&		getIndexes() const;
		const bool			isValidIndex(const std::string& index_page) const;
	};
}
