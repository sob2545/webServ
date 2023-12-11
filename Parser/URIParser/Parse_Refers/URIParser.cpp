#include "../SchemeChecker.hpp"
#include "URIParser.hpp"
#include "../Color.hpp"
#include <fstream>

std::string	errorMessageGenerator(const std::string& inputURI, const int pos, const std::string& message) {
	std::stringstream	res;
	res << "Error:" << (pos + 1) << " \"" << inputURI << "\" " << message;
	return res.str();
}

void	compareOneCharacter(const std::string& inputURI, std::size_t& pos, const unsigned char toCmp) {
	if (inputURI.at(pos) != toCmp) {
		std::stringstream	msg;

		msg << "cannot find \"" << toCmp << "\"";
		throw errorMessageGenerator(inputURI, pos, msg.str());
	}
	pos++;
}

bool	isValidSchemeSyntax(const std::string& inputURI, std::size_t& pos) {
	return (std::isalnum(static_cast<unsigned char>(inputURI.at(pos))) || inputURI.at(pos) == '+' || inputURI.at(pos) == '-' || inputURI.at(pos) == '.');
}

void	BNF_scheme(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	std::string	newScheme;

	if (std::isalpha(inputURI.at(pos))) {
		newScheme += inputURI[pos++];
		while (pos < inputURI.size() && isValidSchemeSyntax(inputURI, pos)) {
			newScheme += inputURI[pos];
			pos++;
		}
	}
	if (newScheme.empty() || !SchemeChecker::instance().isValidScheme(newScheme))
		throw errorMessageGenerator(inputURI, pos, "is invalid scheme syntax");

	uri.scheme = newScheme;
}

std::string	BNF_domainlabel(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	std::string	label;

	if (pos >= inputURI.size() || !std::isalnum(static_cast<unsigned char>(inputURI.at(pos))))
		throw errorMessageGenerator(inputURI, pos, "is invalid domainlabel syntax");
	while (pos < inputURI.size() && (std::isalnum(static_cast<unsigned char>(inputURI.at(pos))) || inputURI.at(pos) == Mark::HYPHEN)) {
		label += inputURI.at(pos);
		pos++;
	}
	return label;
}

void	BNF_toplabel(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	const std::size_t	dotPos = uri.host.rfind(Mark::PERIOD);

	if (dotPos != std::string::npos && !std::isalpha(static_cast<unsigned char>(uri.host.at(dotPos + 1)))) {
		throw errorMessageGenerator(inputURI, dotPos + 1, "is invalid toplabel syntax");
	} else if (dotPos == std::string::npos && !std::isalpha(static_cast<unsigned char>(uri.host.at(0)))) {
		throw errorMessageGenerator(inputURI, (pos - uri.host.length()), "is invalid toplabel syntax");
	}
}


bool	BNF_hostname(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	uri.host += BNF_domainlabel(inputURI, pos, uri);
	while (pos < inputURI.size() && inputURI.at(pos) == Mark::PERIOD) {
		pos++;
		uri.host += '.';
		uri.host += BNF_domainlabel(inputURI, pos, uri);
	}
	BNF_toplabel(inputURI, pos, uri);
	return true;
}

bool	splitAddressNumber(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	if (pos >= inputURI.size() || !std::isdigit(static_cast<unsigned char>(inputURI.at(pos))))
		return false;
	uri.host += inputURI.at(pos);
	pos++;
	while (pos < inputURI.size() && std::isdigit(static_cast<unsigned char>(inputURI.at(pos)))) {
		uri.host += inputURI.at(pos);
		pos++;
	}
	return true;
}

void	splitPort(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	std::string	portStr;
	if (pos >= inputURI.size() || !std::isdigit(static_cast<unsigned char>(inputURI.at(pos))))
		throw errorMessageGenerator(inputURI, pos, "is invalid port syntax");
	portStr += inputURI.at(pos);
	pos++;
	while (pos < inputURI.size() && std::isdigit(static_cast<unsigned char>(inputURI.at(pos)))) {
		portStr += inputURI.at(pos);
		pos++;
	}
	uri.port = static_cast<unsigned short>(std::atoi(portStr.c_str()));
}

bool	BNF_IPv4address(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	for (int i = 0; i < 3;) {
		if (!splitAddressNumber(inputURI, pos, uri))
			return false;
		if (pos >= inputURI.size() || inputURI.at(pos) != '.') {
			return false;
		} else if (pos < inputURI.size() && inputURI.at(pos) == '.') {
			uri.host += '.';
			i++;
			pos++;
		}
	}
	if (!splitAddressNumber(inputURI, pos, uri))
		return false;
	return true;
}


void	BNF_host(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	BNF_IPv4address(inputURI, pos, uri) || BNF_hostname(inputURI, pos, uri);
}

void	BNF_port(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	if (pos < inputURI.size() && inputURI.at(pos) == Reserved::COLON) {
		pos++;
		splitPort(inputURI, pos, uri);
	}
	else
		uri.port = 80;
}

void	BNF_server(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	BNF_host(inputURI, pos, uri);
	BNF_port(inputURI, pos, uri);
}

bool	BNF_PcharReserved(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	switch (inputURI.at(pos)) {
		case (Reserved::COLON):
		case (Reserved::AT_SIGN):
		case (Reserved::AMPERSAND):
		case (Reserved::EQUALS):
		case (Reserved::PLUS):
		case (Reserved::DOLLAR_SIGN):
		case (Reserved::COMMA):
			return true;
		default :
			return false;
	}
}

bool	BNF_Reserved(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	switch (inputURI.at(pos)) {
		case (Reserved::SEMICOLON):
		case (Reserved::SLASH):
		case (Reserved::QUESTION_MARK):
		case (Reserved::COLON):
		case (Reserved::AT_SIGN):
		case (Reserved::AMPERSAND):
		case (Reserved::EQUALS):
		case (Reserved::PLUS):
		case (Reserved::DOLLAR_SIGN):
		case (Reserved::COMMA):
			return true;
		default :
			return false;
	}
}

bool	BNF_Mark(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	switch (inputURI.at(pos)) {
		case (Mark::HYPHEN):
		case (Mark::UNDERSCORE):
		case (Mark::PERIOD):
		case (Mark::EXCLAMATION_MARK):
		case (Mark::TILDE):
		case (Mark::ASTERISK):
		case (Mark::SINGLE_QUOTE):
		case (Mark::LEFT_PARENTHESIS):
		case (Mark::RIGHT_PARENTHESIS):
			return true;
		default:
			return false;
	}
}

bool	BNF_Unreserved(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	return (std::isalnum(static_cast<unsigned char>(inputURI.at(pos))) || BNF_Mark(inputURI, pos, uri)) ? true : false;
}


bool	BNF_Escaped(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	if (inputURI.at(pos) == '%') {
		if (pos + 2 < inputURI.size()) {
			return (isxdigit(inputURI[pos + 1]) && isxdigit(inputURI[pos + 2])) ? true : false;
		}
		pos += 2;
	}
	return false;
}


bool	BNF_pchar(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	if (BNF_Unreserved(inputURI, pos, uri) || BNF_Escaped(inputURI, pos, uri) || BNF_PcharReserved(inputURI, pos, uri))
		return true;
	else
		return false;
}

void	BNF_segment(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	std::size_t	startPos(pos);
	while (pos < inputURI.size() && BNF_pchar(inputURI, pos, uri)) {
		pos++;
	}
	uri.absPath.push_back(inputURI.substr(startPos, (pos - startPos)));
	while (pos < inputURI.size() && inputURI.at(pos) == Reserved::SEMICOLON) {
		startPos = pos;
		pos++;
		while (pos < inputURI.size() && BNF_pchar(inputURI, pos, uri)) {
			pos++;
		}
		uri.absPath.push_back(inputURI.substr(startPos, (pos - startPos)));
	}
}

void	BNF_pathSegments(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	BNF_segment(inputURI, pos, uri);

	while (pos < inputURI.size() && inputURI.at(pos) == Reserved::SLASH) {
		pos++;
		BNF_segment(inputURI, pos, uri);
	}
}

bool	BNF_absPath(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	if (pos >= inputURI.size() || inputURI.at(pos) != Reserved::SLASH) {
		std::cout << "abs path false\n";
		return false;
	}
	else {
		pos++;
		BNF_pathSegments(inputURI, pos, uri);
		return true;
	}
}

bool	BNF_netPath(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	compareOneCharacter(inputURI, pos, Reserved::SLASH);
	if ((pos < inputURI.size()) && (inputURI.at(pos) != Reserved::SLASH)) {
		--pos;
		return false;
	}
	pos++;
	BNF_server(inputURI, pos, uri);
	BNF_absPath(inputURI, pos, uri);
	return (true);
}

bool	BNF_Uric(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	return (BNF_pchar(inputURI, pos, uri) || inputURI.at(pos) == Reserved::SLASH || inputURI.at(pos) == Reserved::QUESTION_MARK);
}



void	BNF_query(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	if (pos < inputURI.size() && inputURI.at(pos) == Reserved::QUESTION_MARK) {
		pos++;
	}
	else {
		return ;
	}
	std::size_t	start(pos);
	std::string	key;
	while (pos < inputURI.size() && BNF_Uric(inputURI, pos, uri)) {
		switch (inputURI.at(pos)) {
			case (Reserved::EQUALS):
				key = inputURI.substr(start, pos - start);
				start = pos + 1;
				break;
			case (Reserved::AMPERSAND):
				uri.query[key].push_back(inputURI.substr(start, pos - start));
				start = pos + 1;
				break;
			default:
				break;
		}
		pos++;
	}
	uri.query[key].push_back(inputURI.substr(start, pos - start));
}

void	BNF_Fragment(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	if (pos < inputURI.size() && inputURI.at(pos) == '#') {
		pos++;
	} else {
		return ;
	}
	const std::size_t	start(pos);
	while (pos < inputURI.size() && BNF_Uric(inputURI, pos, uri)) {
		pos++;
	}
	uri.fragment = inputURI.substr(start, pos - start);
}

void	BNF_hierPart(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	if (!BNF_netPath(inputURI, pos, uri) && !BNF_absPath(inputURI, pos, uri)) {
		throw errorMessageGenerator(inputURI, pos, "is invalid host syntax");
	}
	BNF_query(inputURI, pos, uri);
}


void	BNF_absoluteURI(const std::string& inputURI, std::size_t& pos, URIvalue& uri) {
	BNF_scheme(inputURI, pos, uri);
	compareOneCharacter(inputURI, pos, Reserved::COLON);
	BNF_hierPart(inputURI, pos, uri);
}

void	printURI(const std::string& inputURI, const URIvalue& uri, const std::size_t i) {
	std::cout << GREEN << "====== ["<< i << " Test: " << inputURI << "] ======" << RESET << std::endl;
	std::cout << "host: " << uri.host << std::endl;
	std::cout << "port: " << uri.port << std::endl;
	std::cout << "path: \n";
	for (std::size_t k = 0; k < uri.absPath.size(); ++k) {
		std::cout << uri.absPath[k] << "\n";
	}
	for (auto it = uri.query.begin(); it != uri.query.end(); ++it) {
		std::cout << it->first << ": ";
		for (auto v_it = it->second.begin(); v_it != it->second.end(); ++v_it) {
			std::cout << *v_it << ((v_it != it->second.end() - 1) ? ", " : "");
		}
		std::cout << "\n";
	}

	std::cout << "fragment: \n";
	std::cout << uri.fragment << std::endl;
	std::cout << RESET << std::endl;
}

void	parse(const std::string& inputURI, const std::size_t i) {
	// absoluteURI | relativeURI  [# fragment ]
	std::size_t		pos(0);
	URIvalue	uri;
	BNF_absoluteURI(inputURI, pos, uri);
	if (uri.absPath.size() && uri.absPath[uri.absPath.size() - 1].empty()) {
		uri.absPath.erase(uri.absPath.end() - 1);
	}
	BNF_Fragment(inputURI, pos, uri);

	printURI(inputURI, uri, i);
}

int main() {
	std::ifstream	file("test.txt");
	std::string		inputURI;

	if (!file.is_open()) {
		std::cerr << RED << "Error: cannot open file" << RESET << std::endl;
		return 1;
	}
	std::size_t	i(1);
	while (std::getline(file, inputURI)) {
		try {
			parse(inputURI, i);
		}
		catch (const std::string& errorMsg) {
			std::cerr << RED << i << "Test: "  << errorMsg << RESET << "\n" << std::endl;
		}
		i++;
	}
}
