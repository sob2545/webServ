#pragma once

#include "./ClientData/ResponseRecipe.hpp"
#include "../../FileDescriptor/Socket/ClientSocket.hpp"

class Client {
private:
	HTTP::ResponseRecipe	m_Data;
	ClientSocket			m_Socket;

private:
	Client(const Client& other);
	const Client&	operator=(const Client& other);

public:
	Client();
	~Client();
};
