#ifndef BASE_VARS_H
#define BASE_VARS_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define __WINAPI_ENABLED__
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#define _CRT_SECURE_NO_WARNINGS
#include <locale>
#else
#define UNIX
#endif
#define VERBOSE 0
#define VERBOSE_DTORS 0
#define INCLUDE_ZERO_TERMINATOR true
#define UNSAFE_PRIME_ROOT 0
#define UNUSED [[maybe_unused]]

#include <cstddef>
#include <cstring>
#include <chrono>
#include <fstream>
#include <cstdio>
#include <thread>
#include <string>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>

using _size_t = unsigned long long;
template<typename container>
class map_view {
	const container* vals;
	using iter = typename container::const_iterator;
public:
	using key_t = typename container::key_type;
	using val_t = typename container::mapped_type;
	map_view (const container& _vals) : vals (&_vals) { }
private:
	class iterator {
		iter it;
	public:
		iterator (iter _it) : it (_it) { }
		iterator& operator++ () {
			++it;
			return *this;
		}
		const key_t& key () const {
			return it->first;
		}
		const val_t& value () const {
			return it->second;
		}
		const iterator operator* () const {
			return it;
		}
		operator const key_t& () const {
			return it->first;
		}
		bool operator!= (const iterator& o) const {
			return it != o.it;
		}
	};
public:
	iterator begin () const {
		return iterator (vals->begin ());
	}
	iterator end () const {
		return iterator (vals->end ());
	}
	_size_t size () const {
		return vals->size ();
	}
};
template<typename T>
class vector_view {
	const std::vector<T>* vals;
	using iter = typename std::vector<T>::const_iterator;
public:
	vector_view (const std::vector<T>& _vals) : vals (&_vals) { }
private:
	class iterator {
		iter it;
	public:
		iterator (iter _it) : it (_it) { }
		iterator& operator++ () {
			++const_cast<iter&>(it);
			return *this;
		}
		operator const T& () const {
			return *it;
		}
		bool operator!= (const iterator& o) const {
			return it != o.it;
		}
		const T& operator* () const {
			return *it;
		}
		iterator& operator+ (ssize_t shift) {
			it += shift;
			return *this;
		}
	};
public:
	iterator begin () const {
		return iterator (vals->begin ());
	}
	iterator end () const {
		return iterator (vals->end ());
	}
	_size_t size () const {
		return vals->size ();
	}
};

const _size_t FULL = 0xffffffffffffffff;
#define NO_PUBLIC_CTOR(name) \
	name (int) = delete;
#define MOVABLE(name) \
	NO_PUBLIC_CTOR (name) \
	name (const name&) = delete; \
	name& operator= (const name&) = delete;
#define BLOCK_COPYING(name) \
	MOVABLE (name) \
	name (name&&) = delete; \
	name& operator= (name&&) = delete;

template<typename T>
using wrapper = std::unique_ptr<T>;

//block_t не даёт никаких гарантий сохранности доверенного участка памяти. Всегда лучше делать полное ручное копирование.
class block_t {
	char *ptr = nullptr, *end = nullptr;
	bool owner = false;
public:
	block_t (_size_t size);
	explicit block_t (char* p, _size_t size, bool own = false);
	explicit block_t (const char* p = nullptr, _size_t size = 0);
	block_t (const block_t& other);
	block_t (block_t&& other);
	~block_t ();
	_size_t size () const;
	explicit operator char* ();
	explicit operator const char* () const;
	bool is_owner () const;
	char operator[] (_size_t idx) const;
	char& operator[] (_size_t idx);
	static const block_t empty;
	block_t& operator= (block_t&& val);
};

struct someinfo {
	std::thread::id __id;
	const char* file;
	const char* function;
	int line;
};

inline _size_t hash_combine (_size_t& seed) {
	return (_size_t)seed;
}

template <typename T, typename... Rest>
inline _size_t hash_combine (_size_t& seed, const T& v, Rest... rest) {
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return hash_combine (seed, rest...);
}

#define MAKE_HASHABLE(type, ...) \
	namespace std {\
		template<> struct hash<type> {\
			_size_t operator() (const type &t) const {\
				_size_t ret = 0;\
				ret = hash_combine (ret, __VA_ARGS__);\
				return ret;\
			}\
		};\
	};
MAKE_HASHABLE (someinfo, t.__id, t.file, t.function, t.line);

#define __FILE_GEN_ARGS __FILE__, __FUNCTION__, __LINE__

inline _size_t proxy_file_name_generator (const char* file, const char* function, int line) {
	_size_t __hash = std::hash<someinfo>{}({std::this_thread::get_id(), file, function, line});
	hash_combine (__hash, time (NULL));
	//printf ("%llu\n", __hash);
	return __hash;
}


//https://github.com/Iyamoto/iconv-lite
void cp1251_to_utf8 (const char* in, char* out, _size_t streamsize = FULL);

void utf8_to_cp1251 (const char* utf8, char* windows1251);

#endif
