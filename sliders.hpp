#include <deque>
#include <vector>
#include <numeric>
#include <xreal.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp> 

/* List of calcluated features:
 * mean  --- sample mean
 * dev   --- deviation from sample mean
 * mad   --- sample mean absolute deviation
 * std   --- sample standart deviation
 * sem   --- standart error of the mean
 * skew  --- sample skewnes
 * kurt  --- sample kurtosis
 * gmean --- geomtric mean
 * hmean --- harmonic mean
 * kstat --- k-th statistics (1, 2, 3 and 4)
*/


namespace Sliders {
	
	typedef std::deque<HPA::xreal> Slice;
	typedef std::function<HPA::xreal(HPA::xreal)> xlambda;
	typedef __gnu_pbds::tree<
		HPA::xreal,
		__gnu_pbds::null_type,
		std::less_equal<HPA::xreal>,
		__gnu_pbds::rb_tree_tag,
		__gnu_pbds::tree_order_statistics_node_update> order_statistics_tree;
	
	const xlambda lambdaId = [](HPA::xreal x){return x;};
	/*
	 * General interface for calculating sum functions over interval with moving interval;
	 * The shift() methods should erase left value and add a new one to the right side;
	*/
	class Slider
	{
		public:
			Slider();
			Slider(Slice values);
			~Slider();

			HPA::xreal value() const;
			virtual void shift(HPA::xreal next) = 0;

		protected:
			Slice   _values;
			std::size_t _size;
			HPA::xreal  _value;
	};

	/* 
	 * Quasi-arithmetic mean slider
	 *
	 * Quasi arithmetic mean of X[1], ... X[n] is defined as:
	 *      M_f(X[1], ..., X[n]) := f^{-1}( (f(X[1]) + ... + f(X[n]))/n )
	 * where f some monotone function. For example:
	 * - If f(x) = x, then M is arithmetic mean;
	 * - If f(x) = 1/x, then M is harmonic mean;
	 * - If f(x) = log(x) then M is geometric mean;
	 * You can check out Wikipedia for properties and examples.
	 * Note that user is the one how controls arithmetics errors:
	 * e.g. you can not calculate geometric mean with negative number
	 * but objects of this class does not know about domains of f and f^{-1}.
	 * 
	 * Complexity:
	 * - Time: Initilization is O(n * m) where O(m) is complexity of calculating f
	 * - Time: Recalculation works in O(m)
	 * - Memory: O(n)
	 */

	class Mean : public Slider
	{
		public:
			Mean();
			Mean(Slice v, xlambda fdir = lambdaId, xlambda finv = lambdaId);
			~Mean();

			void shift(HPA::xreal next);
		private:
			HPA::xreal _sum;
			xlambda _dir_transform, _inv_transform;
	};
	
	/*
	 * Momentum slider. Given the maxim ordinality of momentum, say K, calculates
	 * ALL moments of order 0,..., K. The value of slicer is centred K-th momentum .
	 * Note that if you initilaize with K less than 2, it will be set to 2.
	 *
	 * Complexity:
	 * Time: initilization O(n * K^3)
	 * Time: recalculation O(K^3)
	 * Memory: O(n)
	*/

	class Moments : public Slider
	{
		public:
			Moments();
			Moments(Slice v, int ordinality = 2);
			~Moments();

			void shift(HPA::xreal next);
			
			HPA::xreal origin(int order) const;       
			HPA::xreal centred(int order) const;      
			HPA::xreal standardized(int order) const;
	
		private:
			int _max_ordinality;
			HPA::xreal _mean;
			std::vector<HPA::xreal> _raw_sums;
			std::vector<HPA::xreal> _origin_moments;
			std::vector<HPA::xreal> _centred_moments;
	};
	

	/*
	 * Slider for k-th order statistics. 
	 *
	 * Complexity:
	 * Time: Initilization O(n * log n)
	 * Time: Recalulate    O(log n)
	 * Time: Querry        O(log n)
	 * Memory:             O(n) // not sure
	 */
	class Statistics : public Slider
	{
		public:
			Statistics();
			~Statistics();
			Statistics(Slice v);

			void shift(HPA::xreal next);

			HPA::xreal quartile(HPA::xreal percent) const;
			HPA::xreal statitistics(int k) const;
			HPA::xreal min() const;
			HPA::xreal max() const;
			HPA::xreal med() const;
			HPA::xreal firstQ() const; 
			HPA::xreal thirdQ() const;
			
			order_statistics_tree tree() const;
		private:
			order_statistics_tree _stats;
	};
};
