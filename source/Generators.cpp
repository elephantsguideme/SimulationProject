#include "Generators.h"
#include <random>
#include <chrono>


Generators::Generators() : seed_(std::chrono::system_clock::now().time_since_epoch().count()) 
// geting time-based random seed
{
    
}

int Generators::exponential_distribution(const int avg_value) const
{
  
  std::default_random_engine rng(this->seed_);  //   constructing a random number generator engine from a seed

  const std::exponential_distribution<double> distribution(1.0 / avg_value);
  const auto value_from_rng = static_cast<int>(distribution(rng));

  return value_from_rng;
}

int Generators::geometric_distribution(const double avg_value) const
{
  std::default_random_engine rng(this->seed_);  //   constructing a random number generator engine from a seed

  const std::geometric_distribution<int> distribution(1.0 / avg_value);
  const auto value_from_rng = distribution(rng);

  return value_from_rng;
}

int Generators::normal_distribution(const int avg_value, const double variance) const
{
  std::default_random_engine rng(this->seed_);  //   constructing a random number generator engine from a seed

  std::normal_distribution<double> distribution(avg_value, sqrt(variance));   // standard deviation = sqrt(variance)

   const auto value_from_rng = static_cast<int> (distribution(rng));


  return value_from_rng;
}


