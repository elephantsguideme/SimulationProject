
#include <iostream>
#include <fstream>
#include <cstdio>
#include "blood_centre.h"
#include "time_events.h"
#include "conditional_events.h"
#include "generators.h"

double fisher[11] = {0,0,6.314, 2.920, 2.353, 2.312, 2.015, 1.943, 1.895, 1.860, 1.833}; //Fisher-t coefficients for 90% certainty + two zeros 



int main()
{             
  int total_patiients_arrived;
  char console_input;     
  

  std::cout << "Should all data be visible in console? [Y]es [N]o\n"; 
  std::cin >> console_input;
  data_in_console = console_input == 'y' || console_input == 'Y';
  
  bool step_by_step = false;
  if (data_in_console) {
    std::cout << "[S]tep-by-step or [C]ontinuous simualtion?\n";
    std::cin >> console_input;
    step_by_step = console_input == 's' || console_input == 'S';
  }

  std::cout << "Should system data be stored in a '.m' file? [Y]es [N]o\n";
  std::cin >> console_input;
  const bool statistics_in_m = console_input == 'y' || console_input == 'Y';

  int number_of_simulations=1;
  if (!data_in_console && !step_by_step && !statistics_in_m) {         // if to make sure multiple simulations will run smoothly
    std::cout << "number of simulations (1 to 10)\n";
    do
    std::cin >> number_of_simulations;
    while (number_of_simulations>10 || number_of_simulations<0);
  }

  
    double total_patients_arrived=0.0;                  // only ueed when there are a few simulations
    double total_patients_left = 0.0;
    double total_donors_arrived = 0.0;
    double total_max_queue = 0.0;
    double total_avg_time_in_queue = 0.0;
    double total_amount_of_blood_destroyed = 0.0;
    double total_amount_of_blood_transfused = 0.0;
    double total_amount_of_blood_for_research = 0.0;
    double total_blood_ratio = 0.0;
    double total_normal_orders = 0.0;
    double total_emergency_orders = 0.0;
    double total_emergency_orders_ratio = 0.0;

 
    double total_squared_avg_time_in_queue = 0.0;
 
    double total_squared_blood_ratio = 0.0;
   
    double total_squared_emergency_orders_ratio = 0.0;
 


  std::ofstream myfile;
  std::ofstream myfile2;

  if (statistics_in_m) {
    myfile.open("..\\source\\stats.m");
    myfile << "\%time, patientsInQueue, bloodInDepot, timeSinceLastEmergency, emergencyOrdersRatio, usedBloodRatio, avgTimeInQueue\n\n" <<
      "Stats=[";
  }
   myfile2.open("..\\source\\final_stats.txt");
   myfile2 << "*  avg_time  blood_ratio  order_ratio" << "\n";

  //initializing values given in assignment
  int r = 20; int n = 17;
  
  const int t1=300;  const int t2=500;  const int q = 12;          //values from assignment
  //const int t1 = 6000; const int t2 = 10000;  const int q = 50;    // values changed by me
  r=200, n = 100;

  const int z = 2000; const int p = 300;
  const double w = 0.19; const int e = 600; const double ew2 = 0.1;   const int l = 800; 

  const int tu = 300;

  const int tb = r+10; const int jb_min = 5; const int jb_max = 10;


  

      //int number_of_steps=5000;
      //int beginning_phase = 500;                  //simulation parameters
      int simulation_end_time = 1000000;


      for (int k = 1; k <= number_of_simulations; k++) {


  //initializing main object which contains all elements of the system
  auto* blood_centre = new BloodCentre(r,n,z,t1,t2,p,w,e,ew2,q,l,tu,tb,jb_min,jb_max/*,data_in_console*/);




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


      
     

      
                                   //main loop
                                    
  //for(int i=0; i<number_of_steps; i++ )       {      //   program goes through loop 'number_of_steps' times,
                                         // statistics ignore first 'beginning_phase' iterations

      while (blood_centre->get_system_time() <= simulation_end_time) {    //simulation lasts till given timestop

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

           if(statistics_in_m){            // writing system current state to .m file
    myfile << blood_centre->get_system_time() << ", " << blood_centre->PatientsInQueue() << ", " << blood_centre->AmountOfBloodInDepot() << ", "<<
     blood_centre->TimeSinceLastEmergencySent()<<", "<< blood_centre->EmergencyOrdersRatio() << ", " << blood_centre->UsedBloodRatio() << ", " << blood_centre->AverageTimeInQueue() <<";\n";

       }

    auto new_event_time = 0x7fffffff;           //updating the clock
    if (patient_arrival->get_event_time() < new_event_time) new_event_time = patient_arrival->get_event_time();
    if (blood_donated->get_event_time() < new_event_time) new_event_time = blood_donated->get_event_time();
    if (blood_expired->get_event_time() < new_event_time && blood_expired->get_event_time()>0) new_event_time = blood_expired->get_event_time();
   if (normal_order_arrived->get_event_time() < new_event_time  && normal_order_arrived->get_event_time()>0)new_event_time = normal_order_arrived->get_event_time();
   if (emergency_order_arrived->get_event_time() < new_event_time  && emergency_order_arrived->get_event_time()>0)new_event_time = emergency_order_arrived->get_event_time();
    if (research->get_event_time() < new_event_time  && research->get_event_time()>0)new_event_time = research->get_event_time();


    blood_centre->set_system_time(new_event_time);

    //if (i == beginning_phase)blood_centre->ZeroAllStats();

  


    if (step_by_step) std::cin >> console_input;

    }

  if (statistics_in_m) {
    myfile << "];";
    myfile.close();
  }


 std::cout << "\n\nStatistics "<<k<<"\n";

 std::cout << "number of patients arrived: " << blood_centre->stat_patients_arrived <<"\n";
 std::cout << "number of patients cured: " << blood_centre->stat_patients_left << "\n";
 std::cout << "maximum number of patients in queue: " << blood_centre->stat_max_number_of_patients_in_queue << "\n";
 std::cout << "average time spent in queue: " << blood_centre->AverageTimeInQueue() << "\n";
 std::cout << "number of donors arrived: " << blood_centre->stat_donors_arrived << "\n\n";

 std::cout << "number of blood units transfused: " << blood_centre->stat_amount_of_blood_transfused << "\n";
 std::cout << "number of blood units destroyed: " << blood_centre->stat_amount_of_blood_destroyed << "\n";
 std::cout << "number of blood units used for research: " << blood_centre->stat_amount_of_blood_used_for_research << "\n";
 std::cout << "used to all blood ratio: " << blood_centre->UsedBloodRatio() << "\n\n";

 std::cout << "number of normal orders sent: " << blood_centre->stat_normal_orders_sent << "\n";
 std::cout << "number of emergency orders sent: " << blood_centre->stat_emergency_orders_sent << "\n";
 std::cout << "emergency to all orders ratio: " << blood_centre->EmergencyOrdersRatio() << "\n";


 myfile2 << k<<"  "<<blood_centre->AverageTimeInQueue() << "  " << blood_centre->UsedBloodRatio() << "  " <<blood_centre->EmergencyOrdersRatio() <<"\n";


 total_patients_arrived += blood_centre->stat_patients_arrived;
 total_patients_left += blood_centre->stat_patients_left;
   total_max_queue += blood_centre->stat_max_number_of_patients_in_queue;
   total_avg_time_in_queue += blood_centre->AverageTimeInQueue();
   total_donors_arrived += blood_centre->stat_donors_arrived;

   total_amount_of_blood_destroyed += blood_centre->stat_amount_of_blood_destroyed;
   total_amount_of_blood_transfused += blood_centre->stat_amount_of_blood_transfused;
   total_amount_of_blood_for_research += blood_centre->stat_amount_of_blood_used_for_research;
   total_blood_ratio += blood_centre->UsedBloodRatio();

   total_normal_orders += blood_centre->stat_normal_orders_sent;
     total_emergency_orders += blood_centre->stat_emergency_orders_sent;
     total_emergency_orders_ratio += blood_centre->EmergencyOrdersRatio();

     total_squared_avg_time_in_queue += blood_centre->AverageTimeInQueue()*blood_centre->AverageTimeInQueue();
     total_squared_blood_ratio += blood_centre->UsedBloodRatio()*blood_centre->UsedBloodRatio();
     total_squared_emergency_orders_ratio += blood_centre->EmergencyOrdersRatio()*blood_centre->EmergencyOrdersRatio();

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

      }

        if(number_of_simulations>1){

          double time_certainty = sqrt(total_squared_avg_time_in_queue/ (number_of_simulations-1) - total_avg_time_in_queue / number_of_simulations * total_avg_time_in_queue / (number_of_simulations-1));                    //standard dev estimation
          double blood_certainty = sqrt(total_squared_blood_ratio/(number_of_simulations - 1) - total_blood_ratio / number_of_simulations * total_blood_ratio / (number_of_simulations - 1)) ;
          double orders_certainty = sqrt(total_squared_emergency_orders_ratio/ (number_of_simulations - 1) - total_emergency_orders_ratio / number_of_simulations * total_emergency_orders_ratio / (number_of_simulations - 1));

          time_certainty *= fisher[number_of_simulations] / sqrt(number_of_simulations);
            blood_certainty *= fisher[number_of_simulations] / sqrt(number_of_simulations);
            orders_certainty *= fisher[number_of_simulations] / sqrt(number_of_simulations);
            myfile2 << "Avg  " << total_avg_time_in_queue / number_of_simulations << "  " << total_blood_ratio / number_of_simulations << "  " << total_emergency_orders_ratio / number_of_simulations << "\n";
            myfile2 << "+-  " << time_certainty << "  " << blood_certainty << "  " << orders_certainty << "\n";

      std::cout << "\n\nStatistics summary (averages and 90% certainty range)\n";

      std::cout << "number of patients arrived: " << total_patients_arrived/number_of_simulations << "\n";
      std::cout << "number of patients cured: " << total_patients_left / number_of_simulations << "\n";
      std::cout << "maximum number of patients in queue: " << total_max_queue / number_of_simulations << "\n";
      std::cout << "average time spent in queue: " << total_avg_time_in_queue / number_of_simulations<< " +- " << time_certainty << "\n";
      std::cout << "number of donors arrived: " << total_donors_arrived / number_of_simulations << "\n\n";

      std::cout << "number of blood units transfused: " << total_amount_of_blood_transfused / number_of_simulations << "\n";
      std::cout << "number of blood units destroyed: " << total_amount_of_blood_destroyed / number_of_simulations << "\n";
      std::cout << "number of blood units used for research: " << total_amount_of_blood_for_research / number_of_simulations << "\n";
      std::cout << "used to all blood ratio: " <<total_blood_ratio / number_of_simulations << " +- " << blood_certainty << "\n\n";

      std::cout << "number of normal orders sent: " <<total_normal_orders / number_of_simulations << "\n";
      std::cout << "number of emergency orders sent: " << total_emergency_orders / number_of_simulations << "\n";
      std::cout << "emergency to all orders ratio: " <<total_emergency_orders_ratio / number_of_simulations << " +- " << orders_certainty << "\n";

      myfile2.close();
            }
       // uncorrelated seeds generation
     /*
    std::ofstream myfile;
    myfile.open("..\\source\\seeds.txt");
    auto* generator = new Generators(1);
    for (int j = 0; j < 200; j++) {
      for (int i = 0; i < 10000; i++)
      {
        auto amount_from_rng = generator->Uniform01Distribution();
      }
      myfile << generator->get_seed() << ", ";
    }
    generator->~Generators();
    myfile.close();
     */ 

      std::cin >> console_input;

    return 0;
}

