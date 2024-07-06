#ifndef BASE_CRYPT_BASE_H_
#define BASE_CRYPT_BASE_H_

#include "vars.hpp"
#include "stream_base.hpp"
#include "long_math.hpp"
#include "dependency_graph.hpp"
#include <map>
#include <vector>


enum class encoding : int {
	BASE64,
	RLE,
	HAMMING
};
enum class encryption : int {
	AES,
	XOR,
	CHACHA20
};
enum class asymmetric : int {
	EL_GAMAL,
	RSA
};
enum class signature : int {
	DSA,
	ECDSA
};
const asymmetric _ALL_ASYMMETRIC[] = {asymmetric::EL_GAMAL, asymmetric::RSA};
enum class asymmetric_exchange : int {
	DIFFIE_HELLMAN,
	EC_DIFFIE_HELLMAN
};


class encoding_t {
	const encoding type;
	const std::string name;
protected:
	encoding_t (encoding _type, const std::string& _name);
	void reorder ();
public:
	BLOCK_COPYING (encoding_t);
	virtual ~encoding_t ();
	virtual _size_t encode (istream_base& in, ostream_base& out) const = 0;
	virtual _size_t decode (istream_base& in, ostream_base& out) const = 0;
	std::string get_name () const;
	encoding get_type () const;
	static const encoding_t* get (const std::string& name);
	static const map_view<std::map<std::string, const encoding_t*>>& get_supported ();
	//static const std::vector<std::string>& get_supported ();
};

class encryption_t {
	const encryption type;
	const std::string name;
	const bool need, precounting;
	const _size_t keysize;
protected:
	encryption_t (encryption _type, const std::string& _name, bool _need, _size_t _key_size, bool use_precount);
public:
	BLOCK_COPYING (encryption_t);
	virtual ~encryption_t ();
	virtual _size_t encrypt (istream_base& in, ostream_base& out, const block_t& key, const block_t& precounted = block_t::empty) const = 0;
	virtual _size_t decrypt (istream_base& in, ostream_base& out, const block_t& key, const block_t& precounted = block_t::empty) const = 0;
	virtual block_t precount (const block_t& key) const = 0;
	std::string get_name () const;
	encryption get_type () const;
	bool need_fixed_key_size () const;
	bool need_precounting () const;
	_size_t key_size () const;//bytes
	static const encryption_t* get (const std::string& name);
	static const map_view<std::map<std::string, const encryption_t*>>& get_supported ();
	//static const std::vector<std::string>& get_supported ();
};

const long_number_t INVALID = -1;
class digit_signature_t {
	const signature type;
	const std::string name;
	const _size_t default_key;
protected:
	digit_signature_t (signature _type, const std::string& _name, _size_t _key_size);
public:
	BLOCK_COPYING (digit_signature_t);
	virtual ~digit_signature_t ();
	class sign_t {
	protected:
		sign_t () = default;
	public:
		NO_PUBLIC_CTOR (sign_t);
		virtual ~sign_t () = default;
		virtual bool verify (signature _alg) const = 0;
	};
	class public_key_t {
	protected:
		public_key_t () = default;
	public:
		NO_PUBLIC_CTOR (public_key_t);
		virtual ~public_key_t () = default;
		virtual bool check_sign (const long_number_t& m, const sign_t& s) const = 0;
	};
	class private_key_t {
	protected:
		private_key_t () = default;
	public:
		NO_PUBLIC_CTOR (private_key_t);
		virtual ~private_key_t () = default;
		virtual wrapper<sign_t> sign (const long_number_t& m) const = 0;
	};
	virtual std::pair<wrapper<private_key_t>, wrapper<public_key_t>> generate_keys () const = 0;

	std::string get_name () const;
	signature get_type () const;
	_size_t key_size () const;//bits
	static const digit_signature_t* get (const std::string& name);
	static const map_view<std::map<std::string, const digit_signature_t*>>& get_supported ();
};
class asymmetric_t {
	const asymmetric type;
	const std::string name;
	const _size_t default_key;
protected:
	asymmetric_t (asymmetric _type, const std::string& _name, _size_t _key_size);
public:
	BLOCK_COPYING (asymmetric_t);
	virtual ~asymmetric_t ();
	class sign_t {
	protected:
		sign_t () = default;
	public:
		NO_PUBLIC_CTOR (sign_t);
		virtual ~sign_t () = default;
		virtual bool verify (asymmetric _alg) const = 0;
	};
	class encrypted_t {
	protected:
		encrypted_t () = default;
	public:
		virtual ~encrypted_t () = default;
		virtual bool verify (asymmetric _alg) const = 0;
	};
	class public_key_t {
	protected:
		public_key_t () = default;
	public:
		NO_PUBLIC_CTOR (public_key_t);
		virtual ~public_key_t () = default;
		virtual bool check_sign (const long_number_t& m, const sign_t& s) const = 0;
		virtual wrapper<encrypted_t> encrypt (const long_number_t& m) const = 0;
	};
	class private_key_t {
	protected:
		private_key_t () = default;
	public:
		NO_PUBLIC_CTOR (private_key_t);
		virtual ~private_key_t () = default;
		virtual wrapper<sign_t> sign (const long_number_t& m) const = 0;
		virtual long_number_t decrypt (const encrypted_t& enc) const = 0;
	};

	virtual std::pair<wrapper<private_key_t>, wrapper<public_key_t>> generate_keys (_size_t key_size) const = 0;//bits

	std::string get_name () const;
	asymmetric get_type () const;
	_size_t default_key_size () const;//bits
	static const asymmetric_t* get (const std::string& name);
	static const map_view<std::map<std::string, const asymmetric_t*>>& get_supported ();
	//static const std::vector<std::string>& get_supported ();
};

class asymmetric_exchange_t {
	const asymmetric_exchange type;
	const std::string name;
	const _size_t key_size;
protected:
	asymmetric_exchange_t (asymmetric_exchange _type, const std::string& _name, _size_t _key_size);
public:
	BLOCK_COPYING (asymmetric_exchange_t);
	class message_t {
	protected:
		message_t () = default;
	public:
		NO_PUBLIC_CTOR (message_t);
		virtual ~message_t () = default;
		virtual bool verify (asymmetric_exchange _alg) const = 0;
		virtual bool operator== (const message_t&) const = 0;
		bool operator!= (const message_t& m) const {
			return !(*this == m);
		}
	};
	class public_key_t {
	protected:
		public_key_t () = default;
	public:
		NO_PUBLIC_CTOR (public_key_t);
		virtual ~public_key_t () = default;
		virtual bool verify (asymmetric_exchange _alg) const = 0;
	};
	class key_t {
		_size_t key_size;
	protected:
		key_t (_size_t _key_size) : key_size (_key_size) { }
	public:
		NO_PUBLIC_CTOR (key_t);
		virtual ~key_t () = default;
		virtual wrapper<message_t> gen_A () const = 0;
		virtual wrapper<message_t> gen_K (const message_t& other) const = 0;
		virtual wrapper<public_key_t> get_public () const = 0;
		_size_t size () const {
			return key_size;
		}
	};
	virtual ~asymmetric_exchange_t ();
	std::string get_name () const;
	asymmetric_exchange get_type () const;
	_size_t default_key_size () const;
	virtual wrapper<key_t> gen_key (_size_t key_size) const = 0;//bits
	virtual wrapper<key_t> gen_default_key () const = 0;
	virtual wrapper<key_t> gen_other (const message_t& A, const public_key_t& pub) const = 0;
	static const asymmetric_exchange_t* get (const std::string& name);
	//static const std::vector<std::string>& get_supported ();
	static const map_view<std::map<std::string, const asymmetric_exchange_t*>>& get_supported ();
};

#endif
