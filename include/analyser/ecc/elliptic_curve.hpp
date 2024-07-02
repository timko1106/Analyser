#ifndef ECC_ELLIPTIC_CURVE_H_
#define ECC_ELLIPTIC_CURVE_H_

#include "../base/vars.hpp"
#include "../base/dependency_graph.hpp"
#include <string>
#include "curve.hpp"
#include <map>

class elliptic_curve final : public curve {
	static std::map<std::string, wrapper<elliptic_curve>>& get_values ();
	const std::string name;
	const point G;
	elliptic_curve (const std::string& _name, const point& _G, const long_number_t& _p, const long_number_t& _a, const long_number_t& _b);
	static const elliptic_curve* register_curve (const std::string& _name, const point& g, const long_number_t& p, const long_number_t& a, const long_number_t& b);
public:
	BLOCK_COPYING (elliptic_curve);
	~elliptic_curve ();
	void deregister () const;
	static const elliptic_curve* get (const std::string& name);
	//Нужна случайная точка B? Умножаем G на случайное k. Скорее всего не infinity.
	point mul_generator (const long_number_t& k) const;
	std::string get_name () const;
	using ec_ptr = const elliptic_curve*;
	static const map_view<std::map<std::string, wrapper<elliptic_curve>>>& get_possibles ();
	static void register_all ();
	_ADD(ec);
	
	static ec_ptr secp256k1;
	static ec_ptr p_192;
	static ec_ptr p_224;
	static ec_ptr p_256;
	static ec_ptr p_384;
	static ec_ptr p_521;
};
_ADD_DEFINITION (elliptic_curve);

#endif
