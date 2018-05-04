#include <random>
#include <iostream>

#include "TimeEvents.h"
#include "BloodCentre.h"
#include "Patient.h"
#include "Generators.h"

    
// constructors 


te_patient_arrival::te_patient_arrival(BloodCentre* blood_centre) : blood_centre(blood_centre)
{
  
}

te_blood_donated::te_blood_donated(BloodCentre* blood_centre) : blood_centre(blood_centre)
{
  
}

te_blood_expired::te_blood_expired(BloodCentre* blood_centre) : blood_centre(blood_centre)
{
}






void te_patient_arrival::Execute() {        //Patient arrives, amount of blood needed is generated, patient is put
                                          // on the end fo the waiting line, new patient's arrival is planned 
  
  auto* generator = new Generators();
  const auto amount_from_rng = generator->geometric_distribution(blood_centre->get_needed_blood_avg_amount());
  generator->~Generators();


  const auto patient_new = new Patient(amount_from_rng);
  
  std::cout << "\n" << blood_centre->get_system_time()<<". New patient arrives\n";
  std::cout << blood_centre->get_system_time() << ". They need "<< amount_from_rng<<" blood unit(s)\n";
  blood_centre->add_patient_to_queue(patient_new);
                                                        
                              
  generator = new Generators();
  const auto time_from_rng = generator->exponential_distribution(blood_centre->get_patients_avg_time());
  
  generator->~Generators();

  this->schedule(time_from_rng + blood_centre->get_system_time()); 
}                               

void te_patient_arrival::schedule(const int next_event_time) {
  this->event_time= next_event_time;
  std::cout << blood_centre->get_system_time()<< ". Next patient will arrive at "<<next_event_time <<"\n";

}




void te_blood_donated::Execute() {               //a blood unit is given, it's utilization is planned, 
                                                //it is added to depot, new donor's arrival is planned

  std::cout <<"\n"<< blood_centre->get_system_time()<<". A unit of blood is given by donor\n";
  
  
  auto* blood_unit = new BloodUnit(blood_centre->get_expiration_time2()  +blood_centre->get_system_time());
  std::cout << blood_centre->get_system_time() << ". Unused unit will get utilized at "<< blood_centre->get_expiration_time2() + blood_centre->get_system_time()<<"\n";
  blood_centre->add_blood_to_depot2(blood_unit);

  auto* generator = new Generators();
   const auto time_from_rng = generator->exponential_distribution(blood_centre->get_donors_avg_time());
   generator->~Generators();

  this->schedule(time_from_rng + blood_centre->get_system_time()); 
}

void te_blood_donated::schedule(const int next_event_time) {
  this->event_time = next_event_time;
  std::cout << blood_centre->get_system_time() << ". Next donor will arrive at " << next_event_time << "\n";

}


void te_blood_expired::Execute()     //destroys all expired blood units, checks new utilization time
{
  auto units_utilized=0;
                     while(blood_centre->get_blood_utilization_time()== blood_centre->get_system_time())
                     {
                       blood_centre->utilize_blood();
                       units_utilized++;
                     }
 std::cout <<"\n"<< blood_centre->get_system_time() << ". "<< units_utilized <<" unit(s) utilized. There are(is) " << blood_centre->get_amount_of_blood_in_depot() << " blood units in depot\n";
 this->schedule();
}

void te_blood_expired::schedule()
{
  this->event_time = blood_centre->get_blood_utilization_time();
  if (event_time < 0)std::cout << blood_centre->get_system_time() << ". Blood depot is empty\n";    // get_blood_utilization_time() returns -1
  else std::cout << blood_centre->get_system_time() << ". Next blood will be utilized at " << event_time << "\n";

}




