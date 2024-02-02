#pragma once

#include "Singleton.hpp"
#include <sys/semaphore.h>
#include <map>
#include <string>
#include <vector>

namespace ft {
	class Semaphore : public Singleton<Semaphore> {
	public:
		// server Fd로 해당되는 세마포어를 찾는다
		typedef std::map<int, sem_t*>		SemMap_t;
		typedef std::vector<std::string>	SemNameVector_t;

	private:
		const static std::string	m_SemName;
		static SemMap_t				m_SemMap;
		static SemNameVector_t		m_SemNameVector;

	private:
		Semaphore(const Semaphore& other);
		const Semaphore& operator=(const Semaphore& other);

	public:
		Semaphore();
		~Semaphore();
		
		static void	clear(const int& count);
		static void	setSemaphores(const int& count, const int& serverFd);
		static sem_t*	getSemaphore(const int& serverFd);
	};
}
