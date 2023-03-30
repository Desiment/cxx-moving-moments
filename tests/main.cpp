#include <iostream>
#include <fstream>
#include "../sliders.hpp"

void _test_calc_origin_moments(const Sliders::Slice& test, std::size_t frame)
{

	std::ofstream rq;
	rq.open("raw.q");
	Sliders::Slice initial(frame, HPA::xreal(test.front()));
	Sliders::Moments moments(initial, 4);
	std::size_t position = 1;
	for (; position < test.size() + frame; position++)
	{
		for (int i = 0; i <= 4; i++)
			rq << moments.origin(i) <<",";
		rq << std::endl;

		if (position < test.size())
			moments.shift(test[position]);
		else
			moments.shift(test[test.size() - 1]);
	};
	for (int i = 0; i <= 4; i++)
		rq << moments.origin(i) <<",";
	rq << std::endl;
	rq.close();
}

void _test_calc_centred_moments(const Sliders::Slice& test, std::size_t frame)
{

	std::ofstream mq;
	mq.open("moments.q");
	Sliders::Slice initial(frame, HPA::xreal(test.front()));
	Sliders::Moments moments(initial, 4);
	std::size_t position = 1;
	for (; position < test.size() + frame; position++)
	{
		for (int i = 0; i <= 4; i++)
			mq << moments.centred(i) <<",";
		mq << std::endl;

		if (position < test.size())
			moments.shift(test[position]);
		else
			moments.shift(test[test.size() - 1]);
	};
	for (int i = 0; i <= 4; i++)
		mq << moments.centred(i) <<",";
	mq << std::endl;
	mq.close();
}

void _test_calc_stats(const Sliders::Slice& test, std::size_t frame)
{
	std::ofstream sq;
	sq.open("stats.q");
	Sliders::Slice initial(frame, HPA::xreal(test.front()));
	Sliders::Statistics stats(initial);
	std::size_t position = 1;
	for (; position < test.size() + frame; position++)
	{
		for (int i = 0; i <= 4; i++)
			sq << stats.quartile(HPA::xreal(i) / HPA::xreal(4)) <<",";
		sq << std::endl;

		if (position < test.size())
			stats.shift(test[position]);
		else
			stats.shift(test[test.size() - 1]);
	};
	for (int i = 0; i <= 4; i++)
		sq << stats.quartile(HPA::xreal(i) / HPA::xreal(4)) <<",";
	sq << std::endl;
	
	sq.close();
}

void _test_calc_means(const Sliders::Slice& test, std::size_t frame)
{
	std::ofstream qq;
	qq.open("means.q");
	Sliders::Slice initial(frame, HPA::xreal(test.front()));
	

	Sliders::Mean arithmetic(initial);
	
	Sliders::xlambda hlambda = [](HPA::xreal x){ return 1/x; };
	Sliders::Mean harmonic(initial, hlambda, hlambda);

	Sliders::xlambda glambda_d = [](HPA::xreal x){return log(x); };
	Sliders::xlambda glambda_i = [](HPA::xreal x){return exp(x); };
	Sliders::Mean geometric(initial, glambda_d, glambda_i);

	Sliders::xlambda slambda_d = [](HPA::xreal x){return x*x; };
	Sliders::xlambda slambda_i = [](HPA::xreal x){return sqrt(x); };
	Sliders::Mean squared(initial, slambda_d, slambda_i);

	std::size_t position = 1;
	std::vector<Sliders::Mean> means = {arithmetic, geometric, harmonic, squared};

	for (; position < test.size() + frame; position++)
	{
		for (auto& m: means)
			qq << m.value() <<",";
		qq << std::endl;

		if (position < test.size())
			for (auto& m: means)
				m.shift(test[position]);
		else
			for (auto& m: means)
				m.shift(test[test.size() - 1]);
	};
	
	for (auto& m: means)
		qq << m.value() <<",";
	qq << std::endl;
	
	qq.close();
}


int main()
{
	std::ifstream data;
	data.open("testdata");
	std::size_t size, frame;
	data >> size >> frame;
	
	Sliders::Slice test(size);
	for (std::size_t i = 0; i < size; i++)
		data >> test[i];
	data.close();
	
	_test_calc_origin_moments(test, frame);
	_test_calc_centred_moments(test, frame);
	_test_calc_stats(test, frame);
	_test_calc_means(test, frame);
	return 0;
}
