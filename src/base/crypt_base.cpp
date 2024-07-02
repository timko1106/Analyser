#ifndef BASE_CRYPT_BASE_CPP_
#define BASE_CRYPT_BASE_CPP_

#include "../../include/analyser/base/crypt_base.hpp"

template<typename CLASS_T>
class storage {
	static std::map<std::string, const CLASS_T*>& str_obj () {
		static std::map<std::string, const CLASS_T*> _m{};
		return _m;
	}
	storage () = delete;
public:
	storage (int) = delete;
	~storage () = delete;
	static void add (const std::string& name, const CLASS_T* ptr) {
		str_obj ()[name] = ptr;
	}
	static void reorder (const std::string& name, const CLASS_T* ptr) {
		str_obj ()[name] = ptr;
	}
	static const CLASS_T* get (const std::string& name) {
		auto it = str_obj ().find (name);
		return it == str_obj ().end () ? nullptr : it->second;
	}
	static const map_view<std::map<std::string, const CLASS_T*>>& get_supported () {
		static map_view<std::map<std::string, const CLASS_T*>> _view {str_obj ()};
		return _view;
	}
};
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
const map_view<std::map<std::string, const class_type*>>& class_type::get_supported () {\
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
encryption_t::encryption_t (encryption _type, const std::string& _name, \
		bool _need_fixed, _size_t _fixed_size, bool use_precount) : \
	type (_type), name (_name), need (_need_fixed), \
	precounting (use_precount), keysize (_fixed_size) {
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
MAKE_LIMITED(asymmetric_t, asymmetric);
asymmetric_t::asymmetric_t (asymmetric _type, const std::string& _name, _size_t key_size) : type (_type), name (_name), default_key (key_size) {
	storage<asymmetric_t>::add (name, this);
}
_size_t asymmetric_t::default_key_size () const {
	return default_key;
}
MAKE_LIMITED(asymmetric_exchange_t, asymmetric_exchange);
asymmetric_exchange_t::asymmetric_exchange_t (asymmetric_exchange _type, const std::string& _name, _size_t _key_size) : type (_type), name (_name), key_size (_key_size) {
	storage<asymmetric_exchange_t>::add (name, this);
}
_size_t asymmetric_exchange_t::default_key_size () const {
	return key_size;
}
#undef MAKE


#endif
