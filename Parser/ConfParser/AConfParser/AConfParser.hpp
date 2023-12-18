#pragma once

#include "../../ABNF_utils/ABNFFunctions.hpp"
#include "../../PathParser/PathParser.hpp"
#include "../../URIParser/URIParser.hpp"
#include "../ConfData/errorPageData/errorPageData.hpp"

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

	const int HTAB_SIZE = 4;
}

namespace CONF {
	class AConfParser {
	protected:
		typedef std::map<unsigned short, errorPageData>	errorPageMap;

		static std::stack<unsigned char>	m_BlockStack;

		// common util functions
		bool		isMultipleDirective(const unsigned char& block_status, const unsigned short& directive_status);
		bool		stringPathArgumentParser(std::string& argument);
		bool		absPathArgumentParser(std::string& argument);
		bool		digitArgumentParser(std::string& argument);

		void		fileName(std::string& argument);
		void		errorPageArgumentParser(std::string& argument);
		void		errorPageChecker(const std::vector<std::string>& args, errorPageMap& errorMap);
		void		argumentParser(std::string& argument);

		void		handleHtabSpace(const char& c);


		// common parsing functions
		bool						contextLines();
		bool						directives();
		virtual unsigned short		directiveName();

		// virtual functions
		virtual bool				context() = 0;
		virtual unsigned short		directiveNameChecker(const std::string& name) = 0;

		virtual const std::string	argument(const unsigned short& status) = 0;
		virtual bool				argumentChecker(const std::vector<std::string>& args, const unsigned short& status) = 0;


	private:
		AConfParser(const AConfParser& other);
		AConfParser& operator=(const AConfParser& other);

	public:
		AConfParser();
		virtual ~AConfParser();

		virtual void					initialize() = 0;
	};
}