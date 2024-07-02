#ifndef BASE_DEPENDENCY_CT_GRAPH_H_
#define BASE_DEPENDENCY_CT_GRAPH_H_

#include "vars.hpp"
#include <vector>
#include <algorithm>
#include <string>

using sid_t = unsigned long long;
//Функция должна ничего не принимать и ничего не возвращать.
using call_t = void (*) ();
//Хотелось std::initializer_list, но у него не очень хорошо с lifetime.
using depends_t = std::vector<sid_t>;

struct module_t {
private:
	static sid_t& count ();
	static std::vector<module_t*>& modules ();
	struct topsort_vars {
		std::vector<char> visited;
		std::vector<sid_t> order;
	};
	static void dfs_topsort (sid_t i, topsort_vars& v);
	const call_t init;
	const depends_t dependencies;
	bool initialized;
	inline void run_fast ();
public:
	const sid_t id;
	const std::string name;
	module_t (const std::string& _name, call_t addr, const depends_t& deps);
	~module_t ();
	void run ();
	bool is_ready () const;
	static module_t* get (sid_t id);
	static _size_t mod_count ();
	static void run_all ();
};
template<typename T>
const module_t& owner () {
	static module_t m ("", nullptr, {});
	return m;
}

#define DEFAULT_NAME(name)__##name##_id__
#define CLASS_DEFAULT(name, class_name)class_name :: DEFAULT_NAME(name)
//Добавить в КЛАСС.
#define _ADD(name)\
	static const sid_t DEFAULT_NAME(name);
//Добавить в хедер ВНЕ КЛАССА
#define _ADD_DEFINITION(class_name)\
	template<>\
	const module_t& owner<class_name> ();
//В cpp файле сначала _ADD_LINK, потом _ADD_2
#define _ADD_2(name, class_name)\
	const sid_t CLASS_DEFAULT(name, class_name) = owner<class_name> ().id;
#define _ADD_LINK(name, method, class_name, ...)\
template<>\
const module_t& owner<class_name> () {\
	static module_t m (#class_name, class_name :: method, {__VA_ARGS__});\
	return m;\
}
#define _ADD_LINK_(name, method, class_name)\
template<>\
const module_t& owner<class_name> () {\
	static module_t m (#class_name, class_name :: method, {});\
	return m;\
}

#define _RUN()\
	module_t::run_all ()

#endif 
