#pragma once


class Generators
{
public:
  Generators();
  ~Generators()=default;

   int exponential_distribution(int avg_value) const;
   int geometric_distribution(double avg_value) const;
  int normal_distribution(int avg_value, double variance) const;

private:
  const int seed_;
  
};

