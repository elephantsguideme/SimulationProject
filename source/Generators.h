#pragma once


class Generators
{
public:
  Generators(int seed);
  Generators();

  ~Generators()=default;

  double Uniform01Distribution();
  double Normal01Distribution();

  int get_seed() const { return seed_; }

   int ExponentialDistribution(int avg_value) ;
   int GeometricDistribution(double avg_value) ;
  int NormalDistribution(int avg_value, double variance) ;
  int UniformDistribution(int lower_limit, int upper_limit);

private:
  long long int seed_;
  const int m_ = 0x7fffffff;
  const int a_ = 16807;
};

