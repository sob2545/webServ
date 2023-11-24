#include <string>
#include "BNFdata.hpp"

// TODO: URIvalue 매개변수를 std::string으로 변경
// TODO: 다른 곳에서 사용하는 함수만 BNFdata에 정의

namespace BNF {
	bool	PcharReserved(const std::string& inputURI, size_t& pos) {
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

	bool	Reserved(const std::string& inputURI, size_t& pos) {
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

	bool	Mark(const std::string& inputURI, size_t& pos) {
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

	bool	Unreserved(const std::string& inputURI, size_t& pos) {
		return (std::isalnum(static_cast<unsigned char>(inputURI.at(pos))) || Mark(inputURI, pos)) ? true : false;
	}

	bool	Escaped(const std::string& inputURI, size_t& pos) {
		if (inputURI.at(pos) == '%') {
			if (pos + 2 < inputURI.size()) {
				return (isxdigit(inputURI[pos + 1]) && isxdigit(inputURI[pos + 2])) ? true : false;
			}
			pos += 2;
		}
		return false;
	}


	bool	Pchar(const std::string& inputURI, size_t& pos) {
		if (Unreserved(inputURI, pos) || Escaped(inputURI, pos) || PcharReserved(inputURI, pos))
			return true;
		else
			return false;
	}

	void	compareOneCharacter(const std::string& inputURI, size_t& pos, const unsigned char toCmp) {
		if (inputURI.at(pos) != toCmp) {
			std::stringstream	msg;

			msg << "cannot find \"" << toCmp << "\"";
			throw errorMessageGenerator(inputURI, pos, msg.str());
		}
		pos++;
	}

	std::string	errorMessageGenerator(const std::string& inputURI, const int pos, const std::string& message) {
		std::stringstream	res;
		res << "Error:" << (pos + 1) << " \"" << inputURI << "\" " << message;
		return res.str();
	}


	bool	isValidSchemeSyntax(const std::string& inputURI, size_t& pos) {
		return (std::isalnum(static_cast<unsigned char>(inputURI.at(pos))) || inputURI.at(pos) == '+' || inputURI.at(pos) == '-' || inputURI.at(pos) == '.');
	}

	void	scheme(const std::string& inputURI, size_t& pos, URIvalue& uri) {
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

	std::string	domainlabel(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		std::string	label;

		if (pos >= inputURI.size() || !std::isalnum(static_cast<unsigned char>(inputURI.at(pos))))
			throw errorMessageGenerator(inputURI, pos, "is invalid domainlabel syntax");
		while (pos < inputURI.size() && (std::isalnum(static_cast<unsigned char>(inputURI.at(pos))) || inputURI.at(pos) == Mark::HYPHEN)) {
			label += inputURI.at(pos);
			pos++;
		}
		return label;
	}

	void	toplabel(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		const size_t	dotPos = uri.host.rfind(Mark::PERIOD);

		if (dotPos != std::string::npos && !std::isalpha(static_cast<unsigned char>(uri.host.at(dotPos + 1)))) {
			throw errorMessageGenerator(inputURI, dotPos + 1, "is invalid toplabel syntax");
		} else if (dotPos == std::string::npos && !std::isalpha(static_cast<unsigned char>(uri.host.at(0)))) {
			throw errorMessageGenerator(inputURI, (pos - uri.host.length()), "is invalid toplabel syntax");
		}
	}


	bool	hostname(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		uri.host += domainlabel(inputURI, pos, uri);
		while (pos < inputURI.size() && inputURI.at(pos) == Mark::PERIOD) {
			pos++;
			uri.host += '.';
			uri.host += domainlabel(inputURI, pos, uri);
		}
		toplabel(inputURI, pos, uri);
		return true;
	}

	bool	splitAddressNumber(const std::string& inputURI, size_t& pos, URIvalue& uri) {
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

	void	splitPort(const std::string& inputURI, size_t& pos, URIvalue& uri) {
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

	bool	IPv4address(const std::string& inputURI, size_t& pos, URIvalue& uri) {
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


	void	host(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		IPv4address(inputURI, pos, uri) || hostname(inputURI, pos, uri);
	}

	void	port(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		if (pos < inputURI.size() && inputURI.at(pos) == Reserved::COLON) {
			pos++;
			splitPort(inputURI, pos, uri);
		}
		else
			uri.port = 80;
	}

	void	server(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		host(inputURI, pos, uri);
		port(inputURI, pos, uri);
	}
	void	segment(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		size_t	startPos(pos);
		while (pos < inputURI.size() && pchar(inputURI, pos, uri)) {
			pos++;
		}
		uri.absPath.push_back(inputURI.substr(startPos, (pos - startPos)));
		while (pos < inputURI.size() && inputURI.at(pos) == Reserved::SEMICOLON) {
			startPos = pos;
			pos++;
			while (pos < inputURI.size() && pchar(inputURI, pos, uri)) {
				pos++;
			}
			uri.absPath.push_back(inputURI.substr(startPos, (pos - startPos)));
		}
	}

	void	pathSegments(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		segment(inputURI, pos, uri);

		while (pos < inputURI.size() && inputURI.at(pos) == Reserved::SLASH) {
			pos++;
			segment(inputURI, pos, uri);
		}
	}

	bool	absPath(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		if (pos >= inputURI.size() || inputURI.at(pos) != Reserved::SLASH) {
			std::cout << "abs path false\n";
			return false;
		}
		else {
			pos++;
			pathSegments(inputURI, pos, uri);
			return true;
		}
	}

	bool	netPath(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		compareOneCharacter(inputURI, pos, Reserved::SLASH);
		if ((pos < inputURI.size()) && (inputURI.at(pos) != Reserved::SLASH)) {
			--pos;
			return false;
		}
		pos++;
		server(inputURI, pos, uri);
		absPath(inputURI, pos, uri);
		return (true);
	}

	bool	Uric(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		return (pchar(inputURI, pos, uri) || inputURI.at(pos) == Reserved::SLASH || inputURI.at(pos) == Reserved::QUESTION_MARK);
	}



	void	query(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		if (pos < inputURI.size() && inputURI.at(pos) == Reserved::QUESTION_MARK) {
			pos++;
		}
		else {
			return ;
		}
		size_t	start(pos);
		std::string	key;
		while (pos < inputURI.size() && Uric(inputURI, pos, uri)) {
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

	void	Fragment(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		if (pos < inputURI.size() && inputURI.at(pos) == '#') {
			pos++;
		} else {
			return ;
		}
		const size_t	start(pos);
		while (pos < inputURI.size() && Uric(inputURI, pos, uri)) {
			pos++;
		}
		uri.fragment = inputURI.substr(start, pos - start);
	}

	void	hierPart(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		if (!netPath(inputURI, pos, uri) && !absPath(inputURI, pos, uri)) {
			throw errorMessageGenerator(inputURI, pos, "is invalid host syntax");
		}
		query(inputURI, pos, uri);
	}


	void	absoluteURI(const std::string& inputURI, size_t& pos, URIvalue& uri) {
		scheme(inputURI, pos, uri);
		compareOneCharacter(inputURI, pos, Reserved::COLON);
		hierPart(inputURI, pos, uri);
	}
}