#pragma once

#include "../../utils/utilFunctions.hpp"
#include <map>
#include <vector>

namespace	MIME {
	typedef std::vector<std::string>	extensionName;
	class	mime {
	private:
		std::map<std::string, extensionName >	types;

		mime(const mime& other);
		mime&	operator=(const mime& other);
		void	parseMime(const std::string& file);
	
	public:
		mime(const std::string& file);
		~mime();

		const extensionName&	operator[](const std::string& key) const;
		const bool				isValidMime(const std::string& key, const std::string& to_find_extension) const;

	};
}
