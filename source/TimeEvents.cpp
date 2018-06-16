#include <iostream>

#include "TimeEvents.h"
#include "BloodCentre.h"
#include "Patient.h"
#include "Generators.h"
#include "seeds.h"


// constructors 

         


te_blood_expired::te_blood_expired(BloodCentre* blood_centre) : event_time_(-1), blood_centre_(blood_centre) 
{
}


te_patient_arrival::te_patient_arrival(BloodCentre* blood_centre)  : event_time_(-1), blood_centre_(blood_centre), random_seed_(seeds[seeds_index]) 
{
  generator_ = new Generators(random_seed_);
  seeds_index++ ;
  generator2_ = new Generators(seeds[seeds_index]);
  seeds_index++ ;
}

te_blood_donated::te_blood_donated(BloodCentre* blood_centre, ::te_blood_expired* te_blood_expired) : event_time_(-1), 
blood_centre_(blood_centre), te_blood_expired_(te_blood_expired), random_seed_(seeds[seeds_index])
{
  generator_ = new Generators(random_seed_);
  seeds_index++ ;
  
}

te_normal_order_arrived::te_normal_order_arrived(BloodCentre* blood_centre, te_blood_expired* te_blood_expired)    : event_time_(-1), blood_centre_(blood_centre), te_blood_expired_(te_blood_expired)
{
}
te_emergency_order_arrived::te_emergency_order_arrived(BloodCentre* blood_centre, te_blood_expired* te_blood_expired) : event_time_(-1), blood_centre_(blood_centre), te_blood_expired_(te_blood_expired)
{
}

te_research::te_research(BloodCentre* blood_centre, te_blood_expired* te_blood_expired) : event_time_(-1), blood_centre_(blood_centre), te_blood_expired_(te_blood_expired), random_seed_(seeds[seeds_index])
{
  generator_ = new Generators(random_seed_);
  seeds_index++ ;
  
}





void te_patient_arrival::execute() {        //Patient arrives, amount of blood needed is generated, patient is put
                                          // on the end fo the waiting line, new patient's arrival is planned 
  
  
  const auto amount_from_rng = generator_->geometric_distribution(blood_centre_->get_needed_blood_avg_amount());
  
  const auto patient_new = new Patient(amount_from_rng, blood_centre_->get_system_time());

  blood_centre_->stat_patients_arrived++;


  
  std::cout << "\n" << blood_centre_->get_system_time()<<". New patient arrives\n";
  std::cout << blood_centre_->get_system_time() << ". They need "<< amount_from_rng<<" blood units\n";
  blood_centre_->add_patient_to_queue(patient_new);
   
                              
  
  const auto time_from_rng = generator2_->exponential_distribution(blood_centre_->get_patients_avg_time());
  
  

  this->schedule(time_from_rng + blood_centre_->get_system_time()); 
}                               

void te_patient_arrival::schedule(const int next_event_time) {
  event_time_= next_event_time;
  std::cout << blood_centre_->get_system_time()<< ". Next patient will arrive at "<<next_event_time <<"\n";

}





void te_blood_donated::execute() {               //a blood unit is given, it's utilization is planned, 
                                                //it is added to depot, new donor's arrival is planned

  std::cout <<"\n"<< blood_centre_->get_system_time()<<". A unit of blood is given by donor\n";
  
  blood_centre_->stat_donors_arrived++;

  auto* blood_unit = new BloodUnit(blood_centre_->get_expiration_time2() +blood_centre_->get_system_time());
  std::cout << blood_centre_->get_system_time() << ". Unused unit will get utilized at "<< blood_centre_->get_expiration_time2() + blood_centre_->get_system_time()<<"\n";
  
  blood_centre_->add_blood_to_depot2(blood_unit);
 
  te_blood_expired_->schedule();
 


 
   const auto time_from_rng = generator_->exponential_distribution(blood_centre_->get_donors_avg_time());
   

  schedule(time_from_rng + blood_centre_->get_system_time()); 
}

void te_blood_donated::schedule(const int next_event_time) {
  event_time_ = next_event_time;
  std::cout << blood_centre_->get_system_time() << ". Next donor will arrive at " << next_event_time << "\n";

}






void te_blood_expired::execute()     //destroys all expired blood units, checks new utilization time
{
  auto units_utilized=0;
                     while(blood_centre_->get_blood_utilization_time()== blood_centre_->get_system_time())
                     {
                       blood_centre_->utilize_blood();
                       units_utilized++;
                     }
                     blood_centre_->stat_amount_of_blood_destroyed += units_utilized;
 std::cout <<"\n"<< blood_centre_->get_system_time() << ". "<< units_utilized <<" units utilized. There are " << blood_centre_->get_amount_of_blood_in_depot() << " blood units in depot\n";
 this->schedule();
}

void te_blood_expired::schedule()
{

  this->event_time_ = blood_centre_->get_blood_utilization_time();
  
  if (event_time_ < 0)std::cout << blood_centre_->get_system_time() << ". Blood depot is empty\n";    // get_blood_utilization_time() returns -1
  else std::cout << blood_centre_->get_system_time() << ". Next unit will be destroyed at " << event_time_ << "\n";

}




void te_normal_order_arrived::execute()           //normal order arrives, blood utilization is planned, 
                                                //units are added to depot, flag is cleared
{                                                   
  
  std::cout << "\n" << blood_centre_->get_system_time() << ". Normal order with " << blood_centre_->get_normal_order_amount() << " units of blood arrived\n";
  std::cout << blood_centre_->get_system_time() << ". Unused units will get utilized at " << blood_centre_->get_expiration_time1() + blood_centre_->get_system_time() << "\n";

  for(auto i=0;i< blood_centre_->get_normal_order_amount(); i++)       
  {
    auto blood_unit=new BloodUnit(blood_centre_->get_expiration_time1() + blood_centre_->get_system_time());
    blood_centre_->add_blood_to_depot1(blood_unit);  
  }
    std::cout << blood_centre_->get_system_time() << ". Blood added to the queue. There are " <<blood_centre_->get_amount_of_blood_in_depot() << " units in depot \n";
   
  te_blood_expired_->schedule();

  blood_centre_->set_order_flag(false, false);
  event_time_ = -1;
}

void te_normal_order_arrived::schedule(const int next_event_time)
{
  event_time_ = next_event_time;
  std::cout << blood_centre_->get_system_time() << ". Order will arrive at "<< event_time_ <<"\n";
}




void te_emergency_order_arrived::execute()           //emergency order arrives, blood utilization is planned, 
                                                  //units are added to depot, flag is cleared
{

  std::cout << "\n" << blood_centre_->get_system_time() << ". Emergency order with " << blood_centre_->get_emergency_order_amount() << " units of blood arrived\n";
  std::cout << blood_centre_->get_system_time() << ". Unused units will get utilized at " << blood_centre_->get_expiration_time1() + blood_centre_->get_system_time() << "\n";

  for (auto i = 0;i< blood_centre_->get_emergency_order_amount(); i++)
  {
    auto* blood_unit = new BloodUnit(blood_centre_->get_expiration_time1() + blood_centre_->get_system_time());
    blood_centre_->add_blood_to_depot1(blood_unit);
  }
  std::cout << blood_centre_->get_system_time() << ". Blood added to the queue. There are " << blood_centre_->get_amount_of_blood_in_depot() << " units in depot \n";

  te_blood_expired_->schedule();

  blood_centre_->set_order_flag(false, true);
  event_time_ = -1;
}

void te_emergency_order_arrived::schedule(const int next_event_time)
{
  event_time_ = next_event_time;
  std::cout << blood_centre_->get_system_time() << ". Order will arrive at " << event_time_ << "\n";
}



void te_research::execute()  //some blood units are used for research, blood utilization time changes, flag is cleared,
{
  
  const auto amount_from_rng = generator_->uniform_distribution(blood_centre_->get_min_to_research(), blood_centre_->get_max_to_research());

  int amount;
  for(amount=0; amount<amount_from_rng; amount++)
  {
    blood_centre_->utilize_blood();
  }
  blood_centre_->stat_amount_of_blood_used_for_research += amount;
  std::cout << blood_centre_->get_system_time() << ". " << amount << " units used for research \n"; 
  std::cout << blood_centre_->get_system_time() << ". There are "<< blood_centre_->get_amount_of_blood_in_depot() << " units in depot \n";

  te_blood_expired_->schedule();

  blood_centre_->set_research_flag(false);
  event_time_ = -1;
}

void te_research::schedule(const int next_event_time)
{
  event_time_ = next_event_time;
  if(event_time_>0)
  std::cout << blood_centre_->get_system_time() << ". Blood is planned to be used for research at " << event_time_ << "\n";

}
