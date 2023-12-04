#pragma once

#include "../../../utils/utilABNFFunctions.hpp"
#include "../../URIParser/URIParser.hpp"

#include "../ConfFile/ConfFile.hpp"
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
	class ConfParser {
	protected:
		typedef std::vector<std::string>	strVec;

		static std::stack<unsigned char>	m_BlockStack;

		// common util functions
		const bool		fileName(std::string& argument);
		void			absPathArgumentParser(strVec& argument);
		void			digitArgumentParser(std::string& argument);
		void			errorPageArgumentParser(std::string& argument);

		const std::string&	fileContent() const;
		const std::string&	fileName() const;
		size_t*				Pos();
		// void			indexArgumentParser(std::string& arugment);

		// common parsing functions
		const bool		contextLines();
		const bool		directives();

		// virtual functions
		virtual const bool				context() = 0;
		virtual const unsigned short	directiveName();
		virtual const unsigned short	directiveNameChecker() = 0;

		virtual const std::string		argument(const unsigned short& status) = 0;
		virtual const bool				argumentChecker(const std::vector<std::string>& args, const unsigned short& status) = 0;


	private:
		ConfParser(const ConfParser& other);
		ConfParser& operator=(const ConfParser& other);

	public:
		ConfParser();
		virtual ~ConfParser();

		virtual void					initialize() = 0;
	};
}