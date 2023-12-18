#include "MIMEParser.hpp"
#include "./MIMEFile/MIMEFile.hpp"
#include "Exception/MIMEParserException.hpp"
#include <cstddef>
#include <string>

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

template <typename T>
void	errorCode(const std::string& fileContent) {
	const std::size_t	fileSize = fileContent.size();
	std::size_t*			Pos = MIME::MIMEFile::getInstance()->Pos();

	while (Pos[E_INDEX::FILE] < fileSize && ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])) {
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
		ABNF::isC_nl(fileContent, Pos[E_INDEX::FILE]);
	}
	(Pos[E_INDEX::FILE] != fileSize) ? throw T(fileContent.substr(Pos[E_INDEX::FILE]), "invalid MIME type file!") : 0;
}


bool	isMIMETypeName(const char& c) {
	return (std::isalpha(c)
			|| std::isdigit(c)
			|| c == E_MIME::HYPHEN
			|| c == E_MIME::SLASH
			|| c == E_MIME::PERIOD
			|| c == E_MIME::PLUS);
}

template <typename T>
void	element(const std::string& fileName, const std::string& typeName, MIME::TypeMap& MIMEType) {
	const std::size_t	fileSize = fileName.size();
	std::size_t*			Pos = MIME::MIMEFile::getInstance()->Pos();

	while (Pos[E_INDEX::FILE] < fileSize && (ABNF::isWSP(fileName, Pos[E_INDEX::FILE]) || fileName[Pos[E_INDEX::FILE]] == E_MIME::LF)) {
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
	if (fileName[Pos[E_INDEX::FILE]] == E_MIME::RBRACE) {
		return ;
	}

	std::string	elementName;
	while (Pos[E_INDEX::FILE] < fileSize && std::isalnum(fileName[Pos[E_INDEX::FILE]])) {
		elementName += fileName[Pos[E_INDEX::FILE]];
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
	(elementName.empty()) ? throw T(elementName, "invalid type of MIME Type element!") : 0;
	MIMEType[typeName].push_back(elementName);
}

template <typename T>
const std::string	MIME::type(const std::string& fileName, std::size_t& Pos) {
	const std::size_t	fileSize = fileName.size();

	std::string	typeName;
	while (Pos < fileSize && isMIMETypeName(fileName[Pos])) {
		typeName += fileName[Pos];
		Pos++;
	}
	(typeName.empty()) ? throw T(typeName, "invalid type of MIME Type typename!") : 0;
	return typeName;
}

template <typename T>
bool	line(const std::string& fileContent, MIME::TypeMap& MIMEType) {
	std::size_t*	Pos = MIME::MIMEFile::getInstance()->Pos();

	if (fileContent[Pos[E_INDEX::FILE]] == E_MIME::SEMICOLON
			|| fileContent[Pos[E_INDEX::FILE]] == E_MIME::LF
			|| fileContent[Pos[E_INDEX::FILE]] == E_MIME::RBRACE) {
		return false;
	}

	const std::size_t	startPos = Pos[E_INDEX::FILE];
	const std::string& typeName = MIME::type<T>(fileContent, Pos[E_INDEX::FILE]);
	Pos[E_INDEX::COLUMN] += Pos[E_INDEX::FILE] - startPos;

	while (Pos[E_INDEX::FILE] < fileContent.size() && fileContent[Pos[E_INDEX::FILE]] != E_MIME::SEMICOLON) {
		element<T>(fileContent, typeName, MIMEType);
	}
	return (ABNF::isC_nl(fileContent, Pos[E_INDEX::FILE]));
}

template <typename T>
void	typelist(const std::string& fileContent, MIME::TypeMap& MIMEType) {
	const std::size_t	fileSize = fileContent.size();
	std::size_t*			Pos = MIME::MIMEFile::getInstance()->Pos();

	while (Pos[E_INDEX::FILE] < fileSize) {
		while (Pos[E_INDEX::FILE] < fileSize && ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])) {
			Pos[E_INDEX::FILE]++;
			Pos[E_INDEX::COLUMN]++;
		}
		if (line<T>(fileContent, MIMEType) || ABNF::isC_nl(fileContent, Pos[E_INDEX::FILE])) {
			Pos[E_INDEX::LINE]++;
			Pos[E_INDEX::COLUMN] = 1;
		} else {
			return ;
		}
	}
}

template <typename T>
void	typeBlock(const std::string& fileContent, MIME::TypeMap& MIMEType) {
	const std::size_t	fileSize = fileContent.size();
	std::size_t*			Pos = MIME::MIMEFile::getInstance()->Pos(); 

	while (Pos[E_INDEX::FILE] < fileSize && ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])) {
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
	if (fileContent[Pos[E_INDEX::FILE]] != E_MIME::LBRACE) {
		throw T("{", "invalid type of MIME Type typelist!");
	}
	Pos[E_INDEX::FILE]++;
	Pos[E_INDEX::COLUMN]++;
	typelist<T>(fileContent, MIMEType);
	if (fileContent[Pos[E_INDEX::FILE]] != E_MIME::RBRACE) {
		throw T("}", "invalid type of MIME Type typelist!");
	}
	Pos[E_INDEX::FILE]++;
	Pos[E_INDEX::COLUMN]++;
	errorCode<T>(fileContent);
}

template <typename T>
void	title(const std::string& fileContent) {
	const std::size_t	fileSize = fileContent.size();
	std::size_t*			Pos = MIME::MIMEFile::getInstance()->Pos();

	while (Pos[E_INDEX::FILE] < fileSize && ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])) {
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
	std::string	typeName;
	while (Pos[E_INDEX::FILE] < fileSize && std::isalpha(fileContent[Pos[E_INDEX::FILE]])) {
		typeName += fileContent[Pos[E_INDEX::FILE]];
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
	(typeName != "types") ? throw T(typeName, "invalid type of MIME Type title!") : 0;
}

template <typename T>
void	MIME::Parser(const std::string& fileName, TypeMap& MIMEType) {
	MIME::MIMEFile::InitInstance(fileName);
	const std::string&	fileContent = MIME::MIMEFile::getInstance()->getFileContent();

	title<T>(fileContent);
	typeBlock<T>(fileContent, MIMEType);
}

template void				MIME::Parser<MIMEParserException>(const std::string&, TypeMap&);
template const std::string	MIME::type<ConfParserException>(const std::string& fileName, std::size_t& Pos);
