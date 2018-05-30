#pragma once


class Generators
{
public:
  Generators(int seed);
  Generators();

  ~Generators()=default;

  double uniform_01_distribution();
  double normal_01_distribution();

  int get_seed() const { return seed_; };

   int exponential_distribution(int avg_value) ;
   int geometric_distribution(double avg_value) ;
  int normal_distribution(int avg_value, double variance) ;
  int uniform_distribution(int lower_limit, int upper_limit);

private:
  long long int seed_;
  const int m = 0x7fffffff;
  const int a = 16807;
};

