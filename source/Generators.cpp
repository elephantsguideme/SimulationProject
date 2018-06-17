#include "generators.h"
#include <random>
#include <chrono>
#include <iostream>
#include <math.h>

Generators::Generators(int seed) : seed_(seed) 
// geting time-based random seed
{
    
}
Generators::Generators() : seed_(1)
// geting time-based random seed
{

}

double Generators::Uniform01Distribution() 
{
  
  seed_ = (a_ * seed_)%m_;
  
  double m_double = static_cast<double>(m_);
    
  return static_cast<double> (seed_/ m_double);
}




int Generators::ExponentialDistribution(const int avg_value) 
{
  
 double u = Uniform01Distribution();

  return -avg_value*log(u);
}

int Generators::GeometricDistribution(const double avg_value) 
{
  double u = Uniform01Distribution();
  int number_of_throws = 1;
  double probability = 1 / avg_value;
  while (u > probability) {
    u= Uniform01Distribution();
    number_of_throws++;
  }

  return number_of_throws;
}

double Generators::Normal01Distribution()
{
  double normal = 0;
     
  for (int i = 0; i < 12; i++) {
    normal += Uniform01Distribution();
  }
  normal -= 6;        

 



  return normal;
}


int Generators::NormalDistribution(const int avg_value, const double variance) 
{
  double normal = Normal01Distribution();
  double standard_dev = sqrt(variance);
  


  return static_cast<int>(normal*standard_dev+avg_value);
}

int Generators::UniformDistribution(const int lower_limit, const int upper_limit) 
{
  double u = Uniform01Distribution();
  //u = u * (upper_limit - lower_limit) + lower_limit;       // version 1
  u = u * (upper_limit + 1 - lower_limit) + lower_limit;

  return static_cast<int> (u);
}


