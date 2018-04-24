#include <random>
#include <iostream>

#include "TimeEvents.h"
#include "BloodCentre.h"
#include "Patient.h"

     /* constructors */



TePatientArrival::TePatientArrival(BloodCentre* blood_c) {
  blood_centre_ = blood_c;
}

TeBloodDonated::TeBloodDonated(BloodCentre* blood_c)
{
  BloodCentre* blood_centre = blood_c;
}







void TePatientArrival::Execute() {        //Patient arrives, amount of blood needed is generated, patient is put
                                          // on the end fo the waiting line, new patient's arrival is planned 
  auto patient_new = new Patient();   
  
  std::cout << blood_centre_->get_system_time()<<". New patient arrives\n";
  
  blood_centre_->add_patient_to_queue(patient_new);
                                                        
                                   
  std::default_random_engine rng;           //Planning next event
  const std::exponential_distribution<double> distribution(blood_centre_->get_patients_avg_time());
  const auto time_from_rng= static_cast<int>(distribution(rng));
  this->schedule(time_from_rng + blood_centre_->get_system_time()); 
}                               

void TePatientArrival::schedule(int next_event_time) {
  this->event_time_= next_event_time;
  std::cout << blood_centre_->get_system_time()<< ". Next patient will arrive at "<<next_event_time <<"\n";

}




void TeBloodDonated::Execute() {            

  std::cout << blood_centre_->get_system_time()<<". A unit of blood is given by donor\n";
  auto* blood_unit_ = new BloodUnit();

               /*
  std::default_random_engine rng;           //Planning next event
  std::exponential_distribution<double> distribution(blood_centre_->kDonorsAvgTime);
  auto time_from_rng = (int)distribution(rng);
  this->Schedule(time_from_rng + blood_centre_->get_system_time()); */
}

void TeBloodDonated::schedule(int next_event_time) {
  this->event_time_ = next_event_time;
  std::cout << blood_centre_->get_system_time() << ". Next donor will arrive at " << next_event_time << "\n";

}




