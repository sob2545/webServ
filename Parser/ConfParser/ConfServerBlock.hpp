#pragma once

#include "../../utils/utilFunctions.hpp"
#include "ConfLocationBlock.hpp"
#include <map>
#include <vector>

namespace   CONF {
	class serverBlock {
	private: 
		bool									autoindex;
		unsigned short							port;
		std::string								root;
		strVec									server_name;
		std::string								error_page;
		std::string								access_log;
		strVec									index;
		std::map<std::string, locationBlock>	location_block;

		serverBlock(const serverBlock& other);

	public:
		serverBlock(const std::string& file, size_t pos);
		~serverBlock();

		serverBlock& 			operator=(const serverBlock& other);
		const locationBlock&	operator[](const std::string& key);
		const bool				isAutoIndex() const;
		const unsigned short&	getPort() const;
		const strVec&			getServerNames() const;
		const strVec&			getIndexes() const;
		const bool				isValidServerName(const std::string& key) const;
		const bool				isValidIndex(const std::string& key) const;
		const std::string&		getErrorPage() const;
		const std::string&		getAccessLog() const;
	};
}
