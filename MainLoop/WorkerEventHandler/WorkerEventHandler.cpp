#include "WorkerEventHandler.hpp"
#include "Exception/ClientCloseException.hpp"
#include <stdexcept>
#include <string>

// TODO: delete
#include <iostream>

WorkerEventHandler::WorkerEventHandler() {}

WorkerEventHandler::~WorkerEventHandler() {}

ft::shared_ptr<Client>	WorkerEventHandler::makeClient(const int& serverFd) {
	try {
		ft::shared_ptr<Client>	newClient(::new Client(serverFd));
		MultiplexHandler::addClientEvent(newClient->getFd(), E_EV::READ);

		return (newClient);
	} catch (SOCK::SocketException& e) {
		// 실질적으로 accept가 실패한 경우 -> error log에 남겨야됨
		return (ft::shared_ptr<Client>());
	}
#if defined (__RELEASE__)
	 catch (SemLockFail& e) {
		// 그냥 continue
		return (ft::shared_ptr<Client>());
	}
#endif
}

void	WorkerEventHandler::recvFromClient(ft::shared_ptr<Client>& currClient) {
	char	readData[MAX_READSIZE];
	bzero(&readData, sizeof(readData));

	const int	readSize = recv(currClient->getFd(), readData, MAX_READSIZE, 0);
	if (readSize <= 0) {
		// 해당 Client close + 서버 read-event 추가
		throw ClientCloseException();
	}
	const std::string	readDataStr(readData, readSize);
	currClient->setReadBuffer(currClient->getReadBuffer() + readDataStr);
	std::cout << BOLDBLUE << "client send data : " << currClient->getFd() << " " << currClient->getReadBuffer() << std::endl;
}

void	WorkerEventHandler::parseRequest(ft::shared_ptr<Client>& currClient) {
	// HTTP::RequestRecipe* tmpRecipe = &(currClient->getRequestRecipe());
	// const std::string&	restData = HTTP::RequestMessageParser::instance().Parser(*tmpRecipe, currClient->getReadBuffer());
	// std::cout << "recipe address: " << &(currClient->getRequestRecipe()) << std::endl;
	const std::string&	restData = HTTP::RequestMessageParser::instance().Parser(currClient->getRequestRecipe(), currClient->getReadBuffer());
	currClient->setReadBuffer(restData);

#ifdef DEBUG
std::cout << currClient.get()->getFd() << "recpie status: " << (int)currClient->getRequestRecipe().m_HTTPStatus << std::endl; 
std::cout << BOLDGREEN << "========= request data ===========\n";
switch(currClient->getRequestRecipe().m_Method) {
	case (E_HTTP::GET) :
		std::cout << "GET\n";
		break;
	case (E_HTTP::POST) :
		std::cout << "POST\n";
		break;
	case (E_HTTP::DELETE) :
		std::cout << "DELETE\n";
		break;
	case (E_HTTP::PUT) :
		std::cout << "PUT\n";
		break;
	case (E_HTTP::OPTIONS) :
		std::cout << "OPTION\n";
		break;
}
if (currClient->getRequestRecipe().m_HTTPStatus & E_HTTP::CONNECTION) {
	std::cout << "connection\n";
}
if (currClient->getRequestRecipe().m_HTTPStatus & E_HTTP::DATE) {
	std::cout << "date\n";
}
if (currClient->getRequestRecipe().m_HTTPStatus & E_HTTP::TRANSFER_ENCODING) {
	std::cout << "Transfer encoding\n";
}
if (currClient->getRequestRecipe().m_HTTPStatus & E_HTTP::HOST) {
	std::cout << "host\n";
	std::cout << currClient->getRequestRecipe().m_RequestTarget << std::endl;
}
if (currClient->getRequestRecipe().m_HTTPStatus & E_HTTP::CONTENT_LENGTHS) {
	std::cout << "content length\n";
}
if (currClient->getRequestRecipe().m_HTTPStatus & E_HTTP::CONTENT_TYPE) {
	std::cout << "content type\n";
}
std::cout << RESET << std::endl;


#endif

	const unsigned char&	clientStatus = currClient->getRequestRecipe().m_RecipeStatus;
	std::cout << BOLDYELLOW << "Client recipe status is " << (int)clientStatus << std::endl << RESET;
	switch (clientStatus) {
		// TODO: delete
		case 0:
		case E_HTTP::WRITE_SUCCESS:
		case E_HTTP::WRITE_FAIL:
			MultiplexHandler::addClientEvent(currClient->getFd(), E_EV::WRITE);
			break ;
		case E_HTTP::READ_BODY:
		case E_HTTP::READ_HEADER:
			MultiplexHandler::addClientEvent(currClient->getFd(), E_EV::READ);
	}
}

void	WorkerEventHandler::makeResponse(ft::shared_ptr<Client>& currClient) {
	const std::map<unsigned short, void*>::const_iterator it = currClient->getRequestRecipe().m_HeaderMap.find(E_HTTP::CONNECTION);
	if (it != currClient->getRequestRecipe().m_HeaderMap.end()) {
		if (*reinterpret_cast<bool*>(it->second)) {
			currClient->setCloseStatus();
		}
	} else if (currClient->getRequestRecipe().m_RecipeStatus & E_HTTP::WRITE_FAIL) {
		currClient->setCloseStatus();
	}
	std::string body;
		body = "HTTP/1.1 404 Not Found\r\n";
		body += "Connection: close\r\n";
		body += "Content-Type: text/html\r\n";
		body += "Content-length: 167\r\n\r\n";
		
		body += "<!DOCTYPE html>\r\n";
		body += "<html>\r\n";
		body += "<head>\r\n";
		body += "    <title>Not Found</title>\r\n";
		body += "</head>\r\n";
		body += "<body>\r\n";
		body += "    <h1>404 Not Found</h1>\r\n";
		body += "	<hr>\r\n";
		body += "	<p>Not found requested file</p>\r\n";
		body += "</body>\r\n";
		body += "</html>\r\n";

	currClient->setWriteBuffer(body);
}

void	WorkerEventHandler::sendToClient(ft::shared_ptr<Client>& currClient) {
	const std::string&	ClientBuffer = currClient->getWriteBuffer();
	const int	sendSize = send(currClient->getFd(), ClientBuffer.c_str(), ClientBuffer.size(), 0);
	if (sendSize <= 0) {
		throw ClientCloseException();
	}

	// TODO: 클라이언트를 close할지 read 이벤트로 다시 설정해줄지 선택하는 로직을 함수로 따로 구현하여 여기서 호출
	
	// close 확인
	// close or read
}