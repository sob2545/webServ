#include "Semaphore.hpp"
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/fcntl.h>
#include <sys/semaphore.h>

ft::Semaphore::SemMap_t			ft::Semaphore::m_SemMap;
ft::Semaphore::SemNameVector_t	ft::Semaphore::m_SemNameVector;
const std::string				ft::Semaphore::m_SemName = "webServSem";

ft::Semaphore::Semaphore() {}

ft::Semaphore::~Semaphore() {
	const int	semCount = m_SemNameVector.size();

	for (ft::Semaphore::SemMap_t::const_iterator it = m_SemMap.begin(); it != m_SemMap.end(); ++it) {
		sem_close(it->second);
	}
	for (int semIndex(0); semIndex < semCount; ++semIndex) {
		sem_unlink(m_SemNameVector[semIndex].c_str());
	}
}

void	ft::Semaphore::setSemaphores(const int& count, const int& serverFd) {
	std::stringstream	exSemName;
	exSemName << m_SemName << count;
	const std::string	currSemName = exSemName.str();

	sem_unlink(currSemName.c_str());
	m_SemNameVector.push_back(currSemName);

	sem_t*	newSem = sem_open(currSemName.c_str(), O_CREAT, 0644, 1);
	if (newSem == SEM_FAILED) {
		// TOOD: implement
		// throw SemaphoreException();
		throw std::runtime_error("error");
	}
	m_SemMap.insert(std::make_pair(serverFd, newSem));
}

sem_t*	ft::Semaphore::getSemaphore(const int &serverFd) {
	SemMap_t::const_iterator	it = m_SemMap.find(serverFd);
	if (it == m_SemMap.end()) {
		// TOOD: implement
		// throw NoSemaphoreException();
		throw std::runtime_error("error");
	}
	return (it->second);
}
