
#include <iostream>

#include "BloodCentre.h"
#include "TimeEvents.h"

int main()
{
  int r = 20; int n = 17;
  const int z = 2000; const int t1 = 300; const int t2 = 500; const int p = 300;
  const double w = 0.19; const int e = 600; const int q = 12; const int l = 800;
  const int tu = 300; const int tb = 30; const int jb_min = 5; const int jb_max = 10;
  

  BloodCentre BloodC(r,n,z,t1,t2,p,w,e,q,l,tu,tb,jb_min,jb_max);      //initializing main object which contains all elements of the system
  BloodCentre* pBloodC = &BloodC;
  
  TePatientArrival* TePatientA = new TePatientArrival(pBloodC);
  TePatientA->Execute();
  TePatientA->Execute();

  std::cout << "Hello World\n";
    int x;
    std::cin>>x;
    return 0;
}

