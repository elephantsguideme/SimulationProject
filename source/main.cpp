
#include <iostream>
#include <fstream>
#include <random>
#include <cstdio>
#include "blood_centre.h"
#include "time_events.h"
#include "conditional_events.h"
#include "generators.h"


int main()
{
  //initializing values given in assignment
  int r = 20; int n = 17;
  r=200, n = 60;
  /*const int t1=300;  const int t2=500;  const int q = 12; const int tb = 30;*/          //values from assignment
  const int z = 2000; const int t1 = 6000; const int t2 = 10000; const int p = 300;
  const double w = 0.19; const int e = 600; const double ew2 = 0.1; const int q = 50;   const int l = 800; // const int l = 800;

  const int tu = 300;

  const int tb = 210; const int jb_min = 5; const int jb_max = 10;


  //initializing main object which contains all elements of the system
  auto* blood_centre = new BloodCentre(r,n,z,t1,t2,p,w,e,ew2,q,l,tu,tb,jb_min,jb_max);




                           //initializing time and conditional events
  auto* blood_expired = new TimeEventBloodExpired(blood_centre);
  auto* patient_arrival = new TimeEventPatientArrival(blood_centre);
  auto* blood_donated = new TimeEventBloodDonated(blood_centre, blood_expired);
  auto* normal_order_arrived = new TimeEventNormalOrderArrived(blood_centre, blood_expired);
  auto* emergency_order_arrived = new TimeEventEmergencyOrderArrived(blood_centre, blood_expired);
  auto* research = new TimeEventResearch(blood_centre, blood_expired);

  auto* blood_transfusion = new ConditionalEventBloodTransfusion(blood_centre, blood_expired);
  auto* normal_order = new ConditionalEventNormalOrder(blood_centre, normal_order_arrived);
  auto* emergency_order = new ConditionalEventEmergencyOrder(blood_centre, emergency_order_arrived);
  auto* research_level = new ConditionalEventResearchLevel(blood_centre, research);


  blood_donated->Execute();      //first donor and patient arrive
  patient_arrival->Execute();


      unsigned int command=1;

      int number_of_steps=2000;
      int beginning_phase = 500;

  for(int i=0; i<number_of_steps; i++ )       {    //main loop
                                         //program goes through loop 'number_of_steps' times, statistics ignore first 'beginning_phase' iterations

    //checking time events
    if (blood_centre->get_system_time() == patient_arrival->get_event_time())patient_arrival->Execute();
    if (blood_centre->get_system_time() == blood_donated->get_event_time())blood_donated->Execute();
    if (blood_centre->get_system_time() == normal_order_arrived->get_event_time())normal_order_arrived->Execute();
    if (blood_centre->get_system_time() == emergency_order_arrived->get_event_time())emergency_order_arrived->Execute();

    //checking contitional events
    bool change;
    do{
      change = false;
      if (blood_transfusion->ConditionMet()) {
        blood_transfusion->Execute();
        change = true;
      }
      if (normal_order->ConditionMet()) {
        normal_order->Execute();
        change = true;
      }
      if (emergency_order->ConditionMet()) {
        emergency_order->Execute();
        change = true;
      }
      if (research_level->ConditionMet()) {
        research_level->Execute();
        change = true;
      }

    } while (change);    //this is to prevent updating system time as long as conditional events are being executed

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

    if (i == beginning_phase)blood_centre->ZeroAllStats();

     }
 std::cout << "\n\nStatistics\n";
 std::cout << "number of patients arrived - " << blood_centre->stat_patients_arrived <<"\n";
 std::cout << "number of patients cured - " << blood_centre->stat_patients_left << "\n";
 std::cout << "maximum number of patients in queue - " << blood_centre->stat_max_number_of_patients_in_queue << "\n";
 double time = blood_centre->stat_total_time_spent_in_queue;
 time /= blood_centre->stat_patients_left;
 std::cout << "average time spent in queue - " << time << "\n";
 std::cout << "number of donors arrived - " << blood_centre->stat_donors_arrived << "\n\n";

 std::cout << "number of blood units transfused - " << blood_centre->stat_amount_of_blood_transfused << "\n";
 std::cout << "number of blood units destroyed - " << blood_centre->stat_amount_of_blood_destroyed << "\n";
 std::cout << "number of blood units used for research - " << blood_centre->stat_amount_of_blood_used_for_research << "\n";
 double ratio = blood_centre->stat_amount_of_blood_transfused+blood_centre->stat_amount_of_blood_used_for_research;
 ratio /= ratio + blood_centre->stat_amount_of_blood_destroyed;
 std::cout << "used to all blood ratio - " << ratio << "\n\n";

 std::cout << "number of normal orders sent - " << blood_centre->stat_normal_orders_sent << "\n";
 std::cout << "number of emergency orders sent - " << blood_centre->stat_emergency_orders_sent << "\n";
 ratio = blood_centre->stat_emergency_orders_sent;
 ratio /= ratio + blood_centre->stat_normal_orders_sent;
 std::cout << "emergency to all orders ratio - " << ratio << "\n";




 std::cin >> command;

              //destructors
    patient_arrival -> ~TimeEventPatientArrival();
    blood_donated  -> ~TimeEventBloodDonated();
    blood_expired   -> ~TimeEventBloodExpired();
    normal_order_arrived ->~TimeEventNormalOrderArrived();
    emergency_order_arrived ->~TimeEventEmergencyOrderArrived();
    research-> ~TimeEventResearch();

    blood_transfusion   -> ~ConditionalEventBloodTransfusion();
    normal_order ->~ConditionalEventNormalOrder();
    emergency_order ->~ConditionalEventEmergencyOrder();
    research_level ->~ConditionalEventResearchLevel();

    blood_centre -> ~BloodCentre();
    return 0;
}

