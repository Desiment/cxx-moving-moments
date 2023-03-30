#include "sliders.hpp"
#include <iostream>

namespace Sliders {

	// Helping stuff 
	int _alternate(int a) {
		return a % 2 ? -1 : 1;
	};

	int _xceil(HPA::xreal a) {
		return int(ceil(a)._2double() + 0.5);
	};

	HPA::xreal _binom(HPA::xreal n, int k) {
		HPA::xreal b = 1;
		for(int i = 1; i <= k; i++) {
			b *= (n + 1 - i) / i;
		};
		return b;
	};

	// Slider
	Slider::Slider() {};
	Slider::Slider(Slice values) : _values{values}, _size{values.size()} {};
	Slider::~Slider() {};

	HPA::xreal Slider::value() const { return _value; };


	// Mean
	Mean::Mean() {};
	Mean::Mean(Slice v, xlambda fdir, xlambda finv) : Slider(v), _dir_transform{fdir}, _inv_transform{finv} {
		_sum = 0.0;
		for (auto& x: _values)
			_sum += _dir_transform(x);
		_value = _inv_transform(_sum / _size);
	};
	Mean::~Mean() {};

	void Mean::shift(HPA::xreal next) {
		_sum += _dir_transform(next) - _dir_transform(_values.front());
		_value = _inv_transform(_sum / _size);
		_values.pop_front();
		_values.push_back(next);
	};

	// Moments
	Moments::Moments()  {};
	Moments::Moments(Slice v, int ordinality) : Slider(v), _max_ordinality{std::max(ordinality, 2)} {
		_raw_sums.resize(_max_ordinality + 1, 0);
		_origin_moments.resize(_max_ordinality + 1, 0);
		_centred_moments.resize(_max_ordinality + 1, 0);
				
		// Calculate origin moments and mean;
		for (int order = 0; order <= _max_ordinality; order++)
		{
			for (auto& x: v)
				_raw_sums[order] += x(order);
				_origin_moments[order] = _raw_sums[order] / _size;
		}
		_mean = _origin_moments[1];
	
		// Calculate centred moments
		for (int order = 0; order <= _max_ordinality; order++)
			for (int i = 0; i <= order; i++)
				_centred_moments[order] += _alternate(order - i) * _binom(order, i) * _origin_moments[i] * _mean(order - i);
			_value = _centred_moments[_max_ordinality]; 
	};
	Moments::~Moments() {};
			
	void Moments::shift(HPA::xreal next) {			
		// Recalculate raw moments
		for (int order = 0; order <= _max_ordinality; order++)
		{
			_raw_sums[order] = next(order) + _raw_sums[order] - (_values.front())(order);
			_origin_moments[order] = _raw_sums[order] / _size;
		};	
		_mean = _origin_moments[1];		

		// Recalculate centred moments
		for (int order = 0; order <= _max_ordinality; order++)
		{
			_centred_moments[order] = 0;
			for (int i = 0; i <= order; i++)
				_centred_moments[order] += _alternate(order - i) * _binom(order, i) * _origin_moments[i] * _mean(order - i);
		}
		_value = _centred_moments[_max_ordinality]; 
				
		// Update window
		_values.pop_front();
		_values.push_back(next);
	};
			
	HPA::xreal Moments::origin(int order)       const { return _origin_moments[order];  };
	HPA::xreal Moments::centred(int order)      const { return _centred_moments[order]; };
	HPA::xreal Moments::standardized(int order) const { return _centred_moments[order] / sqrt(_centred_moments[2](order)); };	
	
	Statistics::Statistics()  {};
	Statistics::Statistics(Slice v) : Slider(v) {
		for (auto& x: _values)
			_stats.insert(x);
		_value = 0;
	};
	Statistics::~Statistics() {};

	void Statistics::shift(HPA::xreal next) {
		_stats.erase(_stats.upper_bound(_values.front()));
		_stats.insert(next);

		//for (auto& t: _stats)
		//	std::cout << t << " ";
		//std::cout << '\n';
		_values.pop_front();
		_values.push_back(next);
	};

	order_statistics_tree Statistics::tree() const { return _stats; };	
	
	HPA::xreal Statistics::quartile(HPA::xreal percent) const {return *_stats.find_by_order(_xceil(percent * (_size - 1))); };
	HPA::xreal Statistics::statitistics(int k)          const {return *_stats.find_by_order(k - 1); };
	HPA::xreal Statistics::min()    const {return quartile(0.0); };
	HPA::xreal Statistics::max()    const {return quartile(1.0); };
	HPA::xreal Statistics::med()    const {return quartile(0.5); };
	HPA::xreal Statistics::firstQ() const {return quartile(0.25);}; 
	HPA::xreal Statistics::thirdQ() const {return quartile(0.75);};

};
