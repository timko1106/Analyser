#ifndef BASE_DEPENCENCY_GRAPH_CPP_
#define BASE_DEPENCENCY_GRAPH_CPP_

#include "../../include/analyser/base/dependency_graph.hpp"

sid_t& module_t::count () {
	static sid_t cnt = 0;
	return cnt;
}
std::vector<module_t*>& module_t::modules () {
	static std::vector<module_t*> _all{};
	return _all;
}
void module_t::dfs_topsort (sid_t i, module_t::topsort_vars& v) {
	v.visited[i] = true;
	for (sid_t u : modules ()[i]->dependencies) {
		if (!v.visited[u]) {
			dfs_topsort (u, v);
		}
	}
	v.order.push_back (i);
}
void module_t::run_fast () {
	if (!initialized) {
		initialized = true;
		init ();
	}
}
module_t::module_t (const std::string& _name, call_t addr, const depends_t& deps) : \
		init (addr), dependencies (deps),  initialized (false), \
	id (count ()++), name (_name) {
	modules ().push_back (this);
}
module_t::~module_t () { }
void module_t::run () {
	if (!initialized) {
		initialized = true;
		for (sid_t i : dependencies) {
			modules ()[i]->run ();
		}
		initialized = false;
		run_fast ();
	}
}
bool module_t::is_ready () const {
	return initialized;
}
module_t* module_t::get (sid_t id) {
	if (id >= modules ().size ()) {
		return nullptr;
	}
	return modules ()[id];
}
_size_t module_t::mod_count () {
	return modules ().size ();
}
void module_t::run_all () {
	topsort_vars v;
	v.order = {};
	v.visited = std::vector<char>(modules ().size ());
	for (sid_t i = 0; i < mod_count (); ++i) {
		v.visited[i] = modules ()[i]->initialized;
	}
	for (sid_t i = 0; i < mod_count (); ++i) {
		if (!v.visited[i]) {
			dfs_topsort (i, v);
		}
	}
	for (sid_t id : v.order) {
		printf ("Loading module with id: %llu and name %s\n", id, modules ()[id]->name.c_str ());
		modules ()[id]->run_fast ();
	}
}



#endif 
