#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

namespace rnd
{

  class Random
  {
    public:
      static Random& getInstance()
      {
	static rnd::Random instance;
	return instance;
      }

      float getUniformFloat()
      {
	return realDist(gen);
      }

      float getUniformFloat(const float min, const float max)
      {
	setUniformFloatRange(min, max);
	return realDist(gen);
      }

      void setUniformFloatRange(const float min, const float max)
      {
	std::uniform_real_distribution<float>::param_type p(min, max);
	realDist.param(p);
      }

    private:
      std::random_device rd;
      std::mt19937 gen;
      std::uniform_real_distribution<float> realDist;

      Random()
	: rd()
	, gen(rd())
      {
      }

      ~Random()
      {
      }

      Random(const Random&) = delete;
      Random& operator=(const Random&) = delete;
      Random(Random&&) = delete;
      Random& operator=(Random&&) = delete;
  };
}

#endif // RANDOM_HPP
