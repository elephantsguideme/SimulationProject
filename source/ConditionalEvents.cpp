#include <random>
#include <iostream>

#include "ConditionalEvents.h"
#include "BloodCentre.h"
#include "Patient.h"
#include "Generators.h"

    
// constructors 

ce_blood_transfusion::ce_blood_transfusion(BloodCentre* blood_centre, ::te_blood_expired* te_blood_expired)    :
  blood_centre_(blood_centre) , te_blood_expired_(te_blood_expired)
{
}

ce_normal_order::ce_normal_order(BloodCentre* blood_centre, ::te_normal_order_arrived* te_normal_order_arrived)  :
  blood_centre_(blood_centre), te_normal_order_arrived_(te_normal_order_arrived)
{
}

ce_emergency_order::ce_emergency_order(BloodCentre* blood_centre, ::te_emergency_order_arrived* te_emergency_order_arrived) :
  blood_centre_(blood_centre), te_emergency_order_arrived_(te_emergency_order_arrived)
{
}

ce_research_level::ce_research_level(BloodCentre* blood_centre, te_research* te_research)  :
  blood_centre_(blood_centre), te_research_(te_research)
{
}



bool ce_blood_transfusion::condition_met() const
{
  //return !blood_centre->is_queue_empty() && blood_centre->get_amount_of_blood_in_depot() >= blood_centre->get_amount_of_blood_needed();
  //transfers only when full needed amount exists in depot

  return !blood_centre_->is_queue_empty() && blood_centre_->get_amount_of_blood_in_depot();

}

void ce_blood_transfusion::execute()
{
  /*
  const auto amount_of_blood_needed = blood_centre->get_amount_of_blood_needed();
  for(auto i=0; i<amount_of_blood_needed; i++)
  {
    blood_centre->utilize_blood();
  }             
  std::cout <<  blood_centre->get_system_time() << ". " << amount_of_blood_needed << " units transfused. There are " << blood_centre->get_amount_of_blood_in_depot() << " blood units in depot\n";
  blood_centre->remove_patient();  */
  auto units_donated = 0;    
   while(blood_centre_->get_amount_of_blood_needed() && blood_centre_->get_amount_of_blood_in_depot())
   {                                                                     // give patient as much blood as they need then remove that patient
                                                                         // if there aren't enough units - empty the depot and decrease patient's amount_of_blood_needed 
     blood_centre_->utilize_blood();
     blood_centre_->donate_blood();
     units_donated++;
   }
   std::cout << "\n" << blood_centre_->get_system_time() << ". " << units_donated << " units transfused. There are " << blood_centre_->get_amount_of_blood_in_depot() << " blood units in depot\n";
   if (blood_centre_->get_amount_of_blood_needed())
     std::cout << blood_centre_->get_system_time() << ". Patient still needs "<< blood_centre_->get_amount_of_blood_needed()<<" units\n";
   else    {
  
  blood_centre_->remove_patient();
   }

   te_blood_expired_->schedule();

}


bool ce_normal_order::condition_met() const         
{
 
  return    !blood_centre_->get_order_flag(false) && blood_centre_->get_amount_of_blood_in_depot() <= blood_centre_->get_minimum_blood_level();

}
                  
void ce_normal_order::execute()     //schedule normal order, block future normal orders
{
  std::cout << "\n" << blood_centre_->get_system_time() << ". Normal order for blood units sent\n";


  auto* generator = new Generators();
  const auto time_from_rng = generator->exponential_distribution(blood_centre_->get_normal_order_avg_time());

  generator->~Generators();
  

  te_normal_order_arrived_->schedule(blood_centre_->get_system_time()+time_from_rng);

  blood_centre_->set_order_flag(true, false);
}



bool ce_emergency_order::condition_met() const
{
  return    !blood_centre_->get_order_flag(true) && !blood_centre_->get_amount_of_blood_in_depot() && !blood_centre_->is_queue_empty();

}

void ce_emergency_order::execute()       //schedule emergency order, block future emergency orders
{
  std::cout << "\n" << blood_centre_->get_system_time() << ". Emergency order for blood units sent\n";


  auto* generator = new Generators();
  const auto time_from_rng = generator->normal_distribution(blood_centre_->get_emergency_order_avg_time(), blood_centre_->get_emergency_order_time_var());

  generator->~Generators();


  te_emergency_order_arrived_->schedule(blood_centre_->get_system_time() + time_from_rng);

  blood_centre_->set_order_flag(true, true);
}



bool ce_research_level::condition_met() const
{
  if(blood_centre_->get_research_flag())    //if research is already planned chenk if it should be aborted
  {
    if (blood_centre_->get_amount_of_blood_in_depot() <= blood_centre_->get_level_to_research()) 
    {       
      std::cout << "\n" << blood_centre_->get_system_time() << ". There is no longer enough blood for research\n";
      te_research_->schedule(-1);
      blood_centre_->set_research_flag(false);      
    }
    return false;
  }
 
  {
    return (blood_centre_->get_amount_of_blood_in_depot() > blood_centre_->get_level_to_research());
  }
}

void ce_research_level::execute()
{                                             
  te_research_->schedule(blood_centre_->get_system_time() + blood_centre_->get_time_to_research());
  blood_centre_->set_research_flag(true);

}


