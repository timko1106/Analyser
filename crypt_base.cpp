#ifndef CRYPT_BASE_CPP
#define CRYPT_BASE_CPP

#include "crypt_base.hpp"

template<typename CLASS_T>
class storage {
	static std::map<std::string, CLASS_T*> str_obj;
	static std::vector<std::string> supported;
	storage () = delete;
public:
	static void add (const std::string& name, CLASS_T* ptr) {
		str_obj[name] = ptr;
		supported.push_back (name);
	}
	static void reorder (const std::string& name, CLASS_T* ptr) {
		if (str_obj.find (name) == str_obj.end ()) {
			supported.push_back (name);
		}
		str_obj[name] = ptr;
	}
	static const CLASS_T* get (const std::string& name) {
		return str_obj[name];
	}
	static const std::vector<std::string>& get_supported () {
		return supported;
	}
};
template<typename CLASS_T>
std::map<std::string, CLASS_T*> storage<CLASS_T>::str_obj {};
template<typename CLASS_T>
std::vector<std::string> storage<CLASS_T>::supported {};
#define MAKE_LIMITED(class_type, enum_type)\
enum_type class_type::get_type () const {\
	return type;\
}\
std::string class_type::get_name () const {\
	return name;\
}\
const class_type* class_type::get (const std::string& name) {\
	return storage<class_type>::get (name);\
}\
class_type::~class_type () { }\
const std::vector<std::string>& class_type::get_supported () {\
	return storage<class_type>::get_supported ();\
}


#define MAKE(class_type, enum_type)\
class_type::class_type (enum_type _type, const std::string& _name) : type (_type), name (_name) {\
	storage<class_type>::add (_name, this);\
}\
MAKE_LIMITED (class_type, enum_type)

void encoding_t::reorder () {
	storage<encoding_t>::reorder (name, this);
}
MAKE(encoding_t, encoding);
encryption_t::encryption_t (encryption _type, const std::string& _name, bool _need_fixed, _size_t _fixed_size, bool use_precount) : type (_type), name (_name), need (_need_fixed), precounting (use_precount), keysize (_fixed_size) {
	storage<encryption_t>::add (_name, this);
}
_size_t encryption_t::key_size () const {
	return keysize;
}
bool encryption_t::need_fixed_key_size () const {
	return need;
}
bool encryption_t::need_precounting () const {
	return precounting;
}
MAKE_LIMITED(encryption_t, encryption);
MAKE(asymmetric_t, asymmetric);
MAKE(asymmetric_exchange_t, asymmetric_exchange);
#undef MAKE


#endif
