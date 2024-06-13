#ifndef BASE_FILESTREAM_CPP_
#define BASE_FILESTREAM_CPP_

#include "../../../include/analyser/base/filestream.hpp"

ifstream::ifstream (const char* const name) : fs (name, std::ios_base::in) {
	beg = fs.tellg ();
	fs.seekg (0, std::ios_base::end);
	auto end = fs.tellg ();
	streamsize = (_size_t)(end - beg);
	fs.seekg (0, std::ios_base::beg);
}
ifstream::~ifstream () {
	fs.close ();
}
void ifstream::close () {
	fs.close ();
}
_size_t ifstream::read (char* dest, _size_t count) {
	fs.read (dest, (std::streamsize)count);
	_size_t len = (_size_t)fs.gcount ();
	if (len < count) {
		memset (dest + len, 0, count - len);
	}
	return len;
}
istream_base& ifstream::operator>> (char& val) {
	fs.get (val);
	return *this;
}
bool ifstream::eof () const {
	return fs.eof ();
}
bool ifstream::is_open () const {
	return fs.is_open ();
}
wrapper<base_pos> ifstream::tellg () const {
	base_pos* res = new base_pos{};
	res->byteoffset = (_size_t)(const_cast<std::fstream&>(fs).tellg() - beg);
	res->modifiers = EMPTY;
	return wrapper<base_pos>(res);
}
void ifstream::seekg (const base_pos& pos) {
	fs.seekg ((std::streamoff)pos.byteoffset + beg);
}
_size_t ifstream::buff_size () const {
	return streamsize;
}
_size_t ifstream::used_size () const {
	wrapper<base_pos> p = tellg ();
	_size_t used = p->byteoffset;
	return used;
}
_size_t ifstream::free_size () const {
	wrapper<base_pos> p = tellg ();
	_size_t free = buff_size () - p->byteoffset;
	return free;
}

ofstream::ofstream (const char* const name) : fs (name, std::ios_base::out), beg (fs.tellg ()) { }
ofstream::~ofstream () {
	fs.close ();
}
void ofstream::close () {
	fs.close ();
}
void ofstream::write (const char* dest, _size_t count) {
	fs.write (dest, (std::streamsize)count);
}
ostream_base& ofstream::operator<< (char val) {
	fs.put (val);
	return *this;
}
bool ofstream::eof () const {
	return fs.eof ();
}
bool ofstream::is_open () const {
	return fs.is_open ();
}
wrapper<base_pos> ofstream::tellg () const {
	wrapper<base_pos> res (new base_pos{});
	res->byteoffset = (_size_t)(const_cast<std::fstream&>(fs).tellg() - beg);
	res->modifiers = EMPTY;
	return res;
}
void ofstream::seekg (const base_pos& pos) {
	fs.seekg ((std::streamoff)pos.byteoffset + beg);
}
_size_t ofstream::buff_size () const {
	return FULL;
}
_size_t ofstream::free_size () const {
	return FULL;
}
_size_t ofstream::used_size () const {
	return FULL;
}

bool check_if_equal (const char* a, const char* b) {
	ifstream _a (a), _b (b);
	if (!_a.is_open () or !_b.is_open ()) {
		_a.close ();
		_b.close ();
		return false;
	}
	const _size_t BUFF_SIZE = 1024;
	char buff_1[BUFF_SIZE + 1]{}, buff_2[BUFF_SIZE + 1]{};
	memset (buff_1, 0, BUFF_SIZE);
	memset (buff_2, 0, BUFF_SIZE);
	bool ok = true;
	while (_a && _b) {
		_size_t read_a = _a.read (buff_1, BUFF_SIZE), read_b = _b.read (buff_2, BUFF_SIZE);
		if (read_a != read_b) {
			ok = false;
			break;
		}
		if (memcmp (buff_1, buff_2, BUFF_SIZE)) {
			ok = false;
			break;
		}
	}
	_a.close ();
	_b.close ();
	return ok;
}

#endif
