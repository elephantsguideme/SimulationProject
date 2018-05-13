
#include <iostream>
#include <random>
#include "BloodCentre.h"
#include "TimeEvents.h"
#include "ConditionalEvents.h"


int main()
{
  int r = 20; int n = 17;
  //r=50, n = 50;
  /*const int z = 1000, const t1=1500,  const t2=2500*/
  const int z = 2000; const int t1 = 300; const int t2 = 500; const int p = 300;
  const double w = 0.19; const int e = 600; const int q = 12;   const int l = 800; // const int l = 800;
  //const int tu = 200; 
  const int tu = 300;

  const int tb = 30; const int jb_min = 5; const int jb_max = 10;
  

  auto* blood_centre = new BloodCentre(r,n,z,t1,t2,p,w,e,q,l,tu,tb,jb_min,jb_max);
  //initializing main object which contains all elements of the system
   

  
                           //initializing time and conditional events
  auto* blood_expired = new te_blood_expired(blood_centre);
  auto* patient_arrival = new te_patient_arrival(blood_centre);
  auto* blood_donated = new te_blood_donated(blood_centre, blood_expired);
  auto* normal_order_arrived = new te_normal_order_arrived(blood_centre, blood_expired);
  auto* emergency_order_arrived = new te_emergency_order_arrived(blood_centre, blood_expired);
  auto* research = new te_research(blood_centre, blood_expired);

  auto* blood_transfusion = new ce_blood_transfusion(blood_centre, blood_expired);
  auto* normal_order = new ce_normal_order(blood_centre, normal_order_arrived);
  auto* emergency_order = new ce_emergency_order(blood_centre, emergency_order_arrived);
  auto* research_level = new ce_research_level(blood_centre, research);

  blood_donated->Execute();
  patient_arrival->Execute();
        /*
  normal_order->Execute();
  normal_order_arrived->Execute();
  normal_order->Execute();
  normal_order_arrived->Execute();
  if (blood_transfusion->condition_met()) blood_transfusion->Execute();

  if (blood_transfusion->condition_met()) blood_transfusion->Execute();
  normal_order_arrived->Execute();
  if (blood_transfusion->condition_met()) blood_transfusion->Execute();
     */

      unsigned int command=1;      //number of simulation loops, 0 escapes main loop,
      
  while(command)       {
    

    //checking time events
    if (blood_centre->get_system_time() == patient_arrival->get_event_time())patient_arrival->Execute();
    if (blood_centre->get_system_time() == blood_donated->get_event_time())blood_donated->Execute();
    if (blood_centre->get_system_time() == normal_order_arrived->get_event_time())normal_order_arrived->Execute();
    if (blood_centre->get_system_time() == emergency_order_arrived->get_event_time())emergency_order_arrived->Execute();

    //checking contitional events
    bool change;
    do{
      change = false;
      if (blood_transfusion->condition_met()) {
        blood_transfusion->Execute();
        change = true;
      }
      if (normal_order->condition_met()) {
        normal_order->Execute();
        change = true;
      }
      if (emergency_order->condition_met()) {
        emergency_order->Execute();
        change = true;
      }
      if (research_level->condition_met()) {
        research_level->Execute();
        change = true;
      }

    } while (change);

    //checking time events which "should be checked last"
    if (blood_centre->get_system_time() == research->get_event_time())research->Execute();
    if (blood_centre->get_system_time() == blood_expired->get_event_time())blood_expired->Execute();


    auto new_event_time = 0x7fffffff;           //updating the clock
    if (patient_arrival->get_event_time() < new_event_time) new_event_time = patient_arrival->get_event_time();
    if (blood_donated->get_event_time() < new_event_time) new_event_time = blood_donated->get_event_time();
    if (blood_expired->get_event_time() < new_event_time && blood_expired->get_event_time()>0) new_event_time = blood_expired->get_event_time();
    if (normal_order_arrived->get_event_time() < new_event_time  && normal_order_arrived->get_event_time()>0)new_event_time = normal_order_arrived->get_event_time();
    if (emergency_order_arrived->get_event_time() < new_event_time  && emergency_order_arrived->get_event_time()>0)new_event_time = emergency_order_arrived->get_event_time();
    if (research->get_event_time() < new_event_time  && research->get_event_time()>0)new_event_time = research->get_event_time();

    
    blood_centre->set_system_time(new_event_time);  
    
    std::cout << "\n";
    if(command==1)std::cin>>command;  
    else command--;
     }

              //destructors
    patient_arrival -> ~te_patient_arrival();
    blood_donated  -> ~te_blood_donated();
    blood_expired   -> ~te_blood_expired();
    normal_order_arrived ->~te_normal_order_arrived();
    emergency_order_arrived ->~te_emergency_order_arrived();


    blood_transfusion   -> ~ce_blood_transfusion();
    normal_order ->~ce_normal_order();
    emergency_order ->~ce_emergency_order();
    return 0;
}

