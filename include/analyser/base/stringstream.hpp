#ifndef BASE_STRINGSTREAM_H_
#define BASE_STRINGSTREAM_H_

#include "stream_base.hpp"
#include <cstring>

constexpr _size_t DEFAULT_SIZE = 128;

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
	bool eof () const override;
	operator bool() const;
	_size_t buff_size() const override;
	_size_t used_size () const override;
	_size_t free_size () const override;
	const char* raw_view();
	char* eject ();
	virtual wrapper<base_pos> tellg () const override;
	virtual void seekg (const base_pos& to) override;
	//Изменение размера с СОХРАНЕНИЕМ старых оффсетов и данных. Если размер меньше - игнорирует
	void resize (_size_t new_size);
	//Индексы в 0
	void reset ();
	//Безвозвратное удаление массива.
	void reload (_size_t new_size);
	void own (char* buffer, _size_t buff_size);

	void destroy ();
};

class istringstream : public stringstream_base, public istream_base {
public:
	istringstream (char* buffer, _size_t buffer_size = FULL, bool use = false);
	istringstream (const char* buffer, _size_t buffer_size = FULL);
	virtual ~istringstream ();
	void seekg(const base_pos &p) override {
		stringstream_base::seekg (p);
	}
	wrapper<base_pos> tellg() const override {
		return stringstream_base::tellg ();
	}
	bool eof() const override final {
		return stringstream_base::eof ();
	}
	_size_t buff_size () const override final {
		return stringstream_base::buff_size ();
	}
	_size_t used_size () const override final {
		return stringstream_base::used_size ();
	}
	_size_t free_size () const override final {
		return stringstream_base::free_size ();
	}
	istream_base& operator>> (char& val) override;
	_size_t read (char* buffer, _size_t streamsize) override;
};

class ostringstream : public stringstream_base, public ostream_base {
public:
	ostringstream (_size_t buff_size = 0);
	~ostringstream ();
	void seekg(const base_pos &p) override {
		stringstream_base::seekg (p);
	}
	wrapper<base_pos> tellg() const override {
		return stringstream_base::tellg ();
	}
	bool eof() const override final {
		return stringstream_base::eof ();
	}
	_size_t buff_size () const override final {
		return stringstream_base::buff_size ();
	}
	_size_t used_size () const override final {
		return stringstream_base::used_size ();
	}
	_size_t free_size () const override final {
		return stringstream_base::free_size ();
	}
	ostream_base& operator<< (char val) override;
	void write (const char* buffer, _size_t streamsize) override;
};

#endif
