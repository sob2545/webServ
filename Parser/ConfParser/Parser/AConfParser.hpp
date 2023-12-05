#pragma once

#include "../../../utils/utilABNFFunctions.hpp"
#include "../../URIParser/URIParser.hpp"

#include "../File/ConfFile.hpp"
#include "Exception/ConfParserException.hpp"
#include "ConfParserUtils.hpp"
#include <stack>
#include <string>

namespace	E_CONF {
	enum E_CONF {
		LBRACE = '{',
		RBRACE = '}',
	};
}

namespace CONF {
	class AConfParser {
	protected:
		typedef std::vector<std::string>	strVec;

		static std::stack<unsigned char>	m_BlockStack;

		// common util functions
		const bool		isMultipleDirective(const unsigned char& block_status, const unsigned short& directive_status);
		const bool		fileName(std::string& argument);
		void			absPathArgumentParser(strVec& argument);
		void			digitArgumentParser(std::string& argument);
		void			errorPageArgumentParser(std::string& argument);

		// void			indexArgumentParser(std::string& arugment);

		// common parsing functions
		const bool						contextLines();
		const bool						directives();
		virtual const unsigned short	directiveName();

		// virtual functions
		virtual const bool				context() = 0;
		virtual const unsigned short	directiveNameChecker(const std::string& name) = 0;

		virtual const std::string		argument(const unsigned short& status) = 0;
		virtual const bool				argumentChecker(const std::vector<std::string>& args, const unsigned short& status) = 0;


	private:
		AConfParser(const AConfParser& other);
		AConfParser& operator=(const AConfParser& other);

	public:
		AConfParser();
		virtual ~AConfParser();

		virtual void					initialize() = 0;
	};
}