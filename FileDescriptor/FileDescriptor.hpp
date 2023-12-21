#pragma once

#include <unistd.h>
#include <fcntl.h>

class FileDescriptor {
protected:
	int	m_Fd;

private:
	FileDescriptor(const FileDescriptor& other);
	FileDescriptor& operator=(const FileDescriptor& other);

public:
    FileDescriptor(const int fd);
	virtual ~FileDescriptor();

	const int&	getFd() const;
	void		setNonBlocking();
};