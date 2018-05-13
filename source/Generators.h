#pragma once


class Generators
{
public:
  Generators();
  ~Generators()=default;

   int exponential_distribution(int avg_value) const;
   int geometric_distribution(double avg_value) const;
  int normal_distribution(int avg_value, double variance) const;
  int uniform_distribution(int lower_limit, int upper_limit) const;

private:
  const int seed_;
  
};

