#ifndef CRYPT_BASE_H
#define CRYPT_BASE_H

#include "vars.hpp"
#include "stream_base.hpp"
#include "long_math.hpp"
#include <map>
#include <vector>


enum class encoding : int {
	BASE64,
	RLE,
	HAMMING
};
enum class encryption : int {
	AES,
	XOR
};
enum class asymmetric : int {
	EL_GAMAL,
	RSA
};
const asymmetric _ALL_ASYMMETRIC[] = {asymmetric::EL_GAMAL, asymmetric::RSA};
enum class asymmetric_exchange : int {
	DIFFIE_HELLMAN
};


class encoding_t {
	encoding type;
	std::string name;
protected:
	encoding_t (encoding _type, const std::string& _name);
	void reorder ();
public:
	encoding_t& operator= (const encoding_t&) = delete;
	virtual ~encoding_t ();
	virtual _size_t encode (istream_base& in, ostream_base& out) const = 0;
	virtual _size_t decode (istream_base& in, ostream_base& out) const = 0;
	std::string get_name () const;
	encoding get_type () const;
	static const encoding_t* get (const std::string& name);
	static const std::vector<std::string>& get_supported ();
};

class encryption_t {
	encryption type;
	std::string name;
	bool need, precounting;
	_size_t keysize;
protected:
	encryption_t (encryption _type, const std::string& _name, bool _need, _size_t _key_size, bool use_precount);
public:
	virtual ~encryption_t ();
	encryption_t& operator= (const encryption_t&) = delete;
	virtual _size_t encrypt (istream_base& in, ostream_base& out, const block_t& key, const block_t& precounted = block_t::empty) const = 0;
	virtual _size_t decrypt (istream_base& in, ostream_base& out, const block_t& key, const block_t& precounted = block_t::empty) const = 0;
	virtual block_t precount (const block_t& key) const = 0;
	std::string get_name () const;
	encryption get_type () const;
	bool need_fixed_key_size () const;
	bool need_precounting () const;
	_size_t key_size () const;
	static const encryption_t* get (const std::string& name);
	static const std::vector<std::string>& get_supported ();
};

const long_number_t INVALID = -1;
class asymmetric_t {
	asymmetric type;
	std::string name;
protected:
	asymmetric_t (asymmetric _type, const std::string& _name);
public:
	asymmetric_t& operator= (const asymmetric_t&) = delete;
	virtual ~asymmetric_t ();
	class sign_t {
	public:
		sign_t () = default;
		virtual ~sign_t () = default;
		virtual bool verify (asymmetric _alg) const = 0;
	};
	class encrypted_t {
	public:
		encrypted_t () = default;
		virtual ~encrypted_t () = default;
		virtual bool verify (asymmetric _alg) const = 0;
	};
	class public_key_t {
	public:
		public_key_t () = default;
		virtual ~public_key_t () = default;
		virtual bool check_sign (const long_number_t& m, const sign_t& s) const = 0;
		virtual wrapper<encrypted_t> encrypt (const long_number_t& m) const = 0;
	};
	class private_key_t {
	public:
		private_key_t () = default;
		virtual ~private_key_t () = default;
		virtual wrapper<sign_t> sign (const long_number_t& m) const = 0;
		virtual long_number_t decrypt (const encrypted_t& enc) const = 0;
	};

	virtual std::pair<wrapper<private_key_t>, wrapper<public_key_t>> generate_keys (_size_t key_size) const = 0;

	std::string get_name () const;
	asymmetric get_type () const;
	static const asymmetric_t* get (const std::string& name);
	static const std::vector<std::string>& get_supported ();
};

class asymmetric_exchange_t {
	asymmetric_exchange type;
	std::string name;
protected:
	asymmetric_exchange_t (asymmetric_exchange _type, const std::string& _name);
public:
	asymmetric_exchange_t& operator= (const asymmetric_exchange_t&) = delete;
	class message_t {
	public:
		message_t () = default;
		virtual ~message_t () = default;
		virtual bool verify (asymmetric_exchange _alg) const = 0;
		virtual bool operator== (const message_t&) const = 0;
		bool operator!= (const message_t& m) const {
			return !(*this == m);
		}
	};
	class public_key_t {
	public:
		public_key_t () = default;
		virtual ~public_key_t () = default;
		virtual bool verify (asymmetric_exchange _alg) const = 0;
	};
	class key_t {
	protected:
		key_t () = default;
	public:
		virtual ~key_t () = default;
		virtual wrapper<message_t> gen_A () const = 0;
		virtual wrapper<message_t> gen_K (const message_t& other) const = 0;
		virtual wrapper<public_key_t> get_public () const = 0;
	};
	virtual ~asymmetric_exchange_t ();
	std::string get_name () const;
	asymmetric_exchange get_type () const;
	virtual wrapper<key_t> gen_key (_size_t key_size) const = 0;
	virtual wrapper<key_t> gen_other (const message_t& A, const public_key_t& pub) const = 0;
	static const asymmetric_exchange_t* get (const std::string& name);
	static const std::vector<std::string>& get_supported ();
};

#endif
