#pragma once

#include <exception>
#include <cstddef>

namespace ft {

/**
 *		bad weak ptr
*/
class bad_weak_ptr : public std::exception {
public:
	const char*	what() const throw() {
		return "bad_weak_ptr: weak_ptr is expired or not valid";
	}
};

/**
 *		Default Deleter
*/
template <class T>
struct default_delete {
	void	operator()(T* _ptr) const {
		::delete _ptr;
	}
};

template <class T>
struct default_delete<T[]> {
	void	operator()(T* _ptr) const {
		::delete [] _ptr;
	}
};


/**
 *		Unique Pointer
*/
template <class T, class D = default_delete<T> >
class unique_ptr {
public:
	typedef T*	pointer;

private:
	pointer		m_Ptr;
	D			m_Deleter;

	unique_ptr(const unique_ptr&);
	unique_ptr&	operator=(const unique_ptr&);

public:
	explicit unique_ptr(pointer p = 0, const D& d = D()) : m_Ptr(p), m_Deleter(d) {}
	
	~unique_ptr() {
		m_Deleter(m_Ptr);
	}

	pointer&	operator*() const {
		return *m_Ptr;
	}

	pointer	operator->() const {
		return m_Ptr;
	}

	pointer	get() const {
		return m_Ptr;
	}

	void	reset(pointer ptr = 0) {
		m_Deleter(m_Ptr);
		m_Ptr = ptr;
	}
};

template <class T>
class	weak_ptr;

/**
 *		Shared Pointer
*/
template <class T, class D = default_delete<T> >
class	shared_ptr {
public:
	typedef T* pointer;

private:
	pointer		m_Ptr;
	D			m_Deleter;
	unsigned*	m_StrongCount;
	unsigned*	m_WeakCount;

	void	dispose() {
		if (--(*m_StrongCount) == 0) {
			m_Deleter(m_Ptr);
			if (*m_WeakCount == 0) {
				::delete m_StrongCount;
				::delete m_WeakCount;
			}
		}
	}

public:
	explicit shared_ptr(pointer p = 0, const D& d = D()) : m_Ptr(p), m_Deleter(d), m_StrongCount(::new unsigned(1)), m_WeakCount(::new unsigned(0)) {}

	explicit shared_ptr(const weak_ptr<T>& other) : m_Ptr(other.m_Ptr), m_Deleter(D()), m_StrongCount(other.m_StrongCount), m_WeakCount(other.m_WeakCount) {
		if (*m_StrongCount == 0) {
			throw bad_weak_ptr();
		}
		(*m_StrongCount)++;
	}

	shared_ptr(const shared_ptr& other) : m_Ptr(other.m_Ptr), m_Deleter(other.m_Deleter), m_StrongCount(other.m_StrongCount), m_WeakCount(other.m_WeakCount) {
		(*m_StrongCount)++;
	}

	shared_ptr&	operator=(const shared_ptr& other) {
		if (this != &other) {
			dispose();
			m_Ptr = other.m_Ptr;
			m_Deleter = other.m_Deleter;
			m_StrongCount = other.m_StrongCount;
			m_WeakCount = other.m_WeakCount;
			(*m_StrongCount)++;
		}
		return *this;
	}

	~shared_ptr() {
		dispose();
	}

	pointer&	operator*() const {
		return *m_Ptr;
	}

	pointer	operator->() const {
		return m_Ptr;
	}

	pointer	get() const {
		return m_Ptr;
	}

	unsigned	use_count() const {
		return *m_StrongCount;
	}

	friend class weak_ptr<T>;
};


/**
 *		Weak Pointer
*/
template <class T>
class	weak_ptr {
public:
	typedef T*	pointer;

private:
	pointer		m_Ptr;
	unsigned*	m_StrongCount;
	unsigned*	m_WeakCount;

public:
	weak_ptr() : m_Ptr(NULL), m_StrongCount(::new unsigned(0)), m_WeakCount(::new unsigned(1)) {}

	explicit weak_ptr(const shared_ptr<T>& other) : m_Ptr(other.m_Ptr), m_StrongCount(other.m_StrongCount), m_WeakCount(other.m_WeakCount) {
		(*m_WeakCount)++;
	}

	weak_ptr&	operator=(const weak_ptr& other) {
		if (this != &other) {
			if (--(*m_WeakCount) == 0 && *m_StrongCount == 0) {
				::delete m_StrongCount;
				::delete m_WeakCount;
			}
			m_Ptr = other.m_Ptr;
			m_StrongCount = other.m_StrongCount;
			m_WeakCount = other.m_WeakCount;
			(*m_WeakCount)++;
		}
		return *this;
	}

	weak_ptr& operator=(const shared_ptr<T>& other) {
		if (m_Ptr != other.m_Ptr) {
			if (--(*m_WeakCount) == 0 && *m_StrongCount == 0) {
				::delete m_StrongCount;
				::delete m_WeakCount;
			}
		}
		m_Ptr = other.m_Ptr;
		m_StrongCount = other.m_StrongCount;
		m_WeakCount = other.m_WeakCount;
		(*m_WeakCount)++;
		return *this;
	}

	~weak_ptr() {
		if (--(*m_WeakCount) == 0 && *m_StrongCount == 0) {
			::delete m_StrongCount;
			::delete m_WeakCount;
		}
	}

	pointer	lock() {
		if (*m_StrongCount) {
			return m_Ptr;
		}
		return NULL;
	}
};

}