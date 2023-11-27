#include "utilFunctions.hpp"

void	compareOneCharacter(const std::string& inputURI, size_t& pos, const unsigned char toCmp) {
	if (inputURI.at(pos) != toCmp) {
		std::stringstream	msg;

		msg << "cannot find \"" << toCmp << "\"";
		throw errorMessageGenerator(inputURI, pos, msg.str());
	}
	pos++;
}


/**
 *  ======================== Error Message Function ========================
 *  @brief				errorMessageGenerator
 *  @param inputURI:	입력받은 URI
 *  @param pos: 		에러가 발생한 위치
 *  @param message:		에러 메시지
 *  @return:			에러 메시지
*/
std::string	errorMessageGenerator(const std::string& inputURI, const int pos, const std::string& message) {
	std::stringstream	res;

	res << "Error:" << (pos + 1) << " \"" << inputURI << "\" " << message;
	return res.str();
}
