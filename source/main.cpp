
#include <iostream>
#include <random>
#include "BloodCentre.h"
#include "TimeEvents.h"
#include "ConditionalEvents.h"


int main()
{
  int r = 20; int n = 17;
  const int z = 2000; const int t1 = 300; const int t2 = 500; const int p = 300;
  const double w = 0.19; const int e = 600; const int q = 12;   const int l = 80; // const int l = 800;
  const int tu = 300; const int tb = 30; const int jb_min = 5; const int jb_max = 10;
  

  auto* blood_centre = new BloodCentre(r,n,z,t1,t2,p,w,e,q,l,tu,tb,jb_min,jb_max);
  //initializing pointer to main object which contains all elements of the system
   

  auto* patient_arrival = new te_patient_arrival(blood_centre);
  auto* blood_donated = new te_blood_donated(blood_centre);
  auto* blood_expired = new te_blood_expired(blood_centre);

  auto* blood_transfusion = new ce_blood_transfusion(blood_centre, blood_expired);

  blood_donated->Execute();
  patient_arrival->Execute();
    
      int x;
      
  while(true)       {
    int new_event_time=1000000;
    if (blood_centre->get_system_time() == patient_arrival->event_time)patient_arrival->Execute();
    if (patient_arrival->event_time < new_event_time) new_event_time = patient_arrival->event_time;

    if (blood_centre->get_system_time() == blood_donated->event_time)blood_donated->Execute();
    if (blood_donated->event_time < new_event_time) new_event_time = blood_donated->event_time;

    if (blood_transfusion->condition_met()) blood_transfusion->Execute();
    blood_centre->set_system_time(new_event_time);
    std::cout << "\n";
    std::cin>>x;  
    
     }


    patient_arrival -> ~te_patient_arrival();
    blood_donated  -> ~te_blood_donated();
    blood_expired   -> ~te_blood_expired();
    blood_transfusion   -> ~ce_blood_transfusion();
    return 0;
}

