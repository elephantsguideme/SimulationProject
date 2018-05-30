#include "Generators.h"
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

double Generators::uniform_01_distribution() 
{
  
  seed_ = (a * seed_)%m;
  
  double m_double = static_cast<double>(m);
    
  return static_cast<double> (seed_/ m_double);
}




int Generators::exponential_distribution(const int avg_value) 
{
  
 double u = uniform_01_distribution();

  return -avg_value*log(u);
}

int Generators::geometric_distribution(const double avg_value) 
{
  double u = uniform_01_distribution();
  int number_of_throws = 1;
  double probability = 1 / avg_value;
  while (u > probability) {
    u= uniform_01_distribution();
    number_of_throws++;
  }

  return number_of_throws;
}

double Generators::normal_01_distribution()
{
  double normal = 0;
     
  for (int i = 0; i < 12; i++) {
    normal += uniform_01_distribution();
  }
  normal -= 6;        

 



  return normal;
}


int Generators::normal_distribution(const int avg_value, const double variance) 
{
  double normal = normal_01_distribution();
  double standard_dev = sqrt(variance);
  


  return static_cast<int>(normal*standard_dev+avg_value);
}

int Generators::uniform_distribution(const int lower_limit, const int upper_limit) 
{
  double u = uniform_01_distribution();
  //u = u * (upper_limit - lower_limit) + lower_limit;       // version 1
  u = u * (upper_limit + 1 - lower_limit) + lower_limit;

  return static_cast<int> (u);
}


