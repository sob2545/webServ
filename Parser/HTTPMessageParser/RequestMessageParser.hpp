#pragma once

#include "../ABNF_utils/ABNFFunctions.hpp"
#include "../BNF_utils/BNFFunctions.hpp"
#include "../../Utils/Singleton.hpp"

#include "HTTPException/BadRequestException.hpp"
#include "HTTPException/MethodNotFoundException.hpp"
#include "HTTPException/LengthRequriedException.hpp"
#include "HTTPException/RequestEntityTooLongException.hpp"
#include "HTTPException/RequestURITooLongException.hpp"
#include "HTTPException/NotImplementedException.hpp"
#include "HTTPException/HTTPVersionNotSupportException.hpp"
#include "HTTPException/HTTPRequestParsingException.hpp"

#include "../URIParser/URIParser.hpp"

#include "ResponseRecipe.hpp"


namespace HTTP {
	class RequestMessageParser : public Singleton<RequestMessageParser> {
	public:
		typedef std::map<std::string, unsigned char>	MethodMap;
		typedef std::map<std::string, unsigned short>	HeaderMap;

	private:
		RequestMessageParser(const RequestMessageParser& other);
		RequestMessageParser& operator=(const RequestMessageParser& other);
	
	private:
		static std::map<std::string, unsigned char>		m_MethodMap;
		static std::map<std::string, unsigned short>	m_HeaderMap;

		static void		initMethodMap();
		static void		initHeaderMap();
		static bool		isDuplicatable(const unsigned short& status);
	
	private:
		static void				skipWSP(const std::string& message, std::size_t& pos);
		static void				token(const std::string& message, std::size_t& pos, std::string& argument);

		static const std::string	fieldContent(const std::string& message, std::size_t& pos);
		static unsigned short		fieldName(const std::string& message, std::size_t& pos);
		static void					fieldValue(const std::string& message, std::size_t& pos, std::vector<std::string>& args);
		static void					fieldLine(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos, bool& checkBit);
		static void					HTTPVersion(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos);
		static void					method(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos);

		// add Absolute-form / Authority-form
		static bool					OriginForm(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos);
		static void					requestTarget(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos);
		static void					requestLine(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos);

		static void					messageBody(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos);

	public:
		RequestMessageParser();
		~RequestMessageParser();

		static HTTP::ResponseRecipe	Parser(const std::string& HTTPMessage);		
	};
}
