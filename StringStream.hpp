#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H

#include "StreamBase.hpp"
#include <cstring>

//stringstream по умолчанию копирует чужой буффер (не доверяем).
class stringstream_base : public iostream_base {
protected:
	char* buff = nullptr;
	char* end = nullptr;
	char* curr = nullptr;
public:
	stringstream_base (char* buffer, _size_t buff_size = FULL, bool use = false);
	stringstream_base (const char* buffer = nullptr, _size_t buff_size = FULL);
	virtual ~stringstream_base ();
	virtual bool eof () const override;
	operator bool() const;
	_size_t buff_size() const override;
	const char* raw_view();
	char* eject ();
	virtual base_pos* tellg () const override;
	virtual void seekg (const base_pos& to) override;
};

class istringstream : public stringstream_base, public istream_base {
public:
	istringstream (char* buffer, _size_t buffer_size = FULL, bool use = false);
	virtual ~istringstream ();
	virtual void seekg(const base_pos &p) override;
	virtual base_pos* tellg() const override;
	virtual bool eof() const override;
	virtual _size_t buff_size() const override;
	istream_base& operator>> (char& val) override;
	void read (char* buffer, _size_t streamsize) override;
};

class ostringstream : public stringstream_base, public ostream_base {
public:
	ostringstream (size_t buff_size);
	virtual ~ostringstream ();
	virtual void seekg(const base_pos &p) override;
	virtual base_pos* tellg() const override;
	virtual bool eof() const override;
	virtual _size_t buff_size() const override;
	ostream_base& operator<< (char val) override;
	void write (const char* buffer, _size_t streamsize) override;
};

#endif