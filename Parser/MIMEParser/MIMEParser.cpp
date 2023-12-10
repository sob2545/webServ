#include "MIMEParser.hpp"
#include <string>

// TODO: throw MIME template으로 변경하여 사용할 것

/*
	rulelist      = 1title *WSP 1typelist

	title         = 5ALPHA

	typelist      = "{" *[LF | WSP] *type 1 *[LF | WSP] "}"

	type          = typename *WSP 1*extension ";"

	typename      = group "/" elements

	extension     = 1*[ALPHA | DIGIT]

	group         = 1*ALPHA

	elements      = 1*(ALPHA | DIGIT] * PCHAR

	PCHAR         = ALPHA | DIGIT | "+" | "-" | "."
*/

void	errorCode(const std::string& fileContent, size_t& Pos) {
	const size_t	fileSize = fileContent.size();

	while (Pos < fileSize && ABNF::isWSP(fileContent, Pos)) {
		Pos++;
		ABNF::isC_nl(fileContent, Pos);
	}
	(Pos != fileSize) ? throw ConfParserException(fileContent.substr(Pos), "invalid MIME type file!") : 0;
}


bool	isMIMETypeName(const char& c) {
	return (std::isalpha(c)
			|| std::isdigit(c)
			|| c == E_MIME::HYPHEN
			|| c == E_MIME::SLASH
			|| c == E_MIME::PERIOD
			|| c == E_MIME::PLUS);
}

void	element(const std::string& fileName, const std::string& typeName, size_t& Pos, MIME::TypeMap& MIMEType) {
	const size_t	fileSize = fileName.size();
	while (Pos < fileSize && (ABNF::isWSP(fileName, Pos) || fileName[Pos] == E_MIME::LF)) {
		Pos++;
	}
	if (fileName[Pos] == E_MIME::RBRACE) {
		return ;
	}

	std::string	elementName;
	while (Pos < fileSize && std::isalnum(fileName[Pos])) {
		elementName += fileName[Pos];
		Pos++;
	}
	(elementName.empty()) ? throw ConfParserException(elementName, "invalid type of MIME Type element!") : 0;
	MIMEType[typeName].push_back(elementName);
}

const std::string	MIME::type(const std::string& fileName, size_t& Pos) {
	const size_t	fileSize = fileName.size();

	std::string	typeName;
	while (Pos < fileSize && isMIMETypeName(fileName[Pos])) {
		typeName += fileName[Pos];
		Pos++;
	}
	(typeName.empty()) ? throw ConfParserException(typeName, "invalid type of MIME Type typename!") : 0;
	return typeName;
}

bool	line(const std::string& fileContent, size_t& Pos, MIME::TypeMap& MIMEType) {
	if (fileContent[Pos] == E_MIME::SEMICOLON
			|| fileContent[Pos] == E_MIME::LF
			|| fileContent[Pos] == E_MIME::RBRACE) {
		return false;
	}

	const std::string& typeName = MIME::type(fileContent, Pos);
	while (Pos < fileContent.size() && fileContent[Pos] != E_MIME::SEMICOLON) {
		element(fileContent, typeName, Pos, MIMEType);
	}
	return (ABNF::isC_nl(fileContent, Pos));
}

void	typelist(const std::string& fileContent, size_t& Pos, MIME::TypeMap& MIMEType) {
	const size_t	fileSize = fileContent.size();

	while (Pos < fileSize) {
		while (Pos < fileSize && ABNF::isWSP(fileContent, Pos)) {
			Pos++;
		}
		if (line(fileContent, Pos, MIMEType) || ABNF::isC_nl(fileContent, Pos)) {
		} else {
			return ;
		}
	}
}

void	typeBlock(const std::string& fileContent, size_t& Pos, MIME::TypeMap& MIMEType) {
	const size_t	fileSize = fileContent.size();

	while (Pos < fileSize && ABNF::isWSP(fileContent, Pos)) {
		Pos++;
	}
	if (fileContent[Pos] != E_MIME::LBRACE) {
		throw ConfParserException("{", "invalid type of MIME Type typelist!");
	}
	Pos++;
	typelist(fileContent, Pos, MIMEType);
	if (fileContent[Pos] != E_MIME::RBRACE) {
		throw ConfParserException("}", "invalid type of MIME Type typelist!");
	}
	Pos++;
	errorCode(fileContent, Pos);
}

void	title(const std::string& fileContent, size_t& Pos) {
	const size_t	fileSize = fileContent.size();

	while (Pos < fileSize && ABNF::isWSP(fileContent, Pos)) {
		Pos++;
	}
	std::string	typeName;
	while (Pos < fileSize && std::isalpha(fileContent[Pos])) {
		typeName += fileContent[Pos];
		Pos++;
	}
	(typeName != "types") ? throw ConfParserException(typeName, "invalid type of MIME Type title!") : 0;
}

void	MIME::Parser(const std::string& fileName, TypeMap& MIMEType) {
	ReadFile			file(fileName);
	const std::string&	fileContent = file.getFileContent();
	size_t				Pos(0);

	title(fileContent, Pos);
	typeBlock(fileContent, Pos, MIMEType);
}