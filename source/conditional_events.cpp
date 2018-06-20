#include <random>
#include <iostream>

#include "conditional_events.h"
#include "blood_centre.h"
#include "patient.h"
#include "generators.h"
#include "seeds.h"

    
// constructors 

ConditionalEventBloodTransfusion::ConditionalEventBloodTransfusion(BloodCentre* blood_centre, ::TimeEventBloodExpired* te_blood_expired)    :
  blood_centre_(blood_centre) , te_blood_expired_(te_blood_expired)
{
}

ConditionalEventNormalOrder::ConditionalEventNormalOrder(BloodCentre* blood_centre, ::TimeEventNormalOrderArrived* te_normal_order_arrived)  :
  blood_centre_(blood_centre), te_normal_order_arrived_(te_normal_order_arrived)
{
  generator_ = new Generators(seeds[seeds_index]);
  seeds_index++; 
  seeds_index %= 20;
}

ConditionalEventEmergencyOrder::ConditionalEventEmergencyOrder(BloodCentre* blood_centre, ::TimeEventEmergencyOrderArrived* te_emergency_order_arrived) :
  blood_centre_(blood_centre), te_emergency_order_arrived_(te_emergency_order_arrived)
{
  generator_ = new Generators(seeds[seeds_index]);
  seeds_index++ ;
  seeds_index %= 20;

}

ConditionalEventResearchLevel::ConditionalEventResearchLevel(BloodCentre* blood_centre, TimeEventResearch* te_research)  :
  blood_centre_(blood_centre), te_research_(te_research)
{
}



bool ConditionalEventBloodTransfusion::ConditionMet() const
{
  //return !blood_centre->is_queue_empty() && blood_centre->get_amount_of_blood_in_depot() >= blood_centre->get_amount_of_blood_needed();
  //transfers only when full needed amount exists in depot

  return !blood_centre_->QueueEmpty() && blood_centre_->AmountOfBloodInDepot();

}

void ConditionalEventBloodTransfusion::Execute()
{
  /*
  const auto amount_of_blood_needed = blood_centre->get_amount_of_blood_needed();
  for(auto i=0; i<amount_of_blood_needed; i++)
  {
    blood_centre->utilize_blood();
  }             
  if(data_in_console) std::cout <<  blood_centre->get_system_time() << ". " << amount_of_blood_needed << " units transfused. There are " << blood_centre->get_amount_of_blood_in_depot() << " blood units in depot\n";
  blood_centre->remove_patient();  */
  auto units_donated = 0;    
   while(blood_centre_->AmountOfBloodNeeded() && blood_centre_->AmountOfBloodInDepot())
   {                                                                     // give patient as much blood as they need then remove that patient
                                                                         // if there aren't enough units - empty the depot and decrease patient's amount_of_blood_needed 
     blood_centre_->UtilizeBlood();
     blood_centre_->DonateBlood();
     units_donated++;
   }
   blood_centre_->stat_amount_of_blood_transfused += units_donated;
   if(data_in_console) std::cout << "\n" << blood_centre_->get_system_time() << ". " << units_donated << " units transfused. There are " << blood_centre_->AmountOfBloodInDepot() << " blood units in depot\n";
   if (blood_centre_->AmountOfBloodNeeded()){
     if(data_in_console) std::cout << blood_centre_->get_system_time() << ". Patient still needs "<< blood_centre_->AmountOfBloodNeeded()<<" units\n";
   }
   else    {
  
  blood_centre_->RemovePatient();
   }

   te_blood_expired_->schedule();

}


bool ConditionalEventNormalOrder::ConditionMet() const         
{
 
  return    !blood_centre_->OrderFlag(false) && blood_centre_->AmountOfBloodInDepot() <= blood_centre_->get_minimum_blood_level();

}
                  
void ConditionalEventNormalOrder::Execute()     //schedule normal order, block future normal orders
{
  if(data_in_console) std::cout << "\n" << blood_centre_->get_system_time() << ". Normal order for blood units sent\n";


  blood_centre_->stat_normal_orders_sent++;

  const auto time_from_rng = generator_->ExponentialDistribution(blood_centre_->get_normal_order_avg_time());


  te_normal_order_arrived_->schedule(blood_centre_->get_system_time()+time_from_rng);

  blood_centre_->set_order_flag(true, false);
}



bool ConditionalEventEmergencyOrder::ConditionMet() const
{
  return    !blood_centre_->OrderFlag(true) && !blood_centre_->AmountOfBloodInDepot() && !blood_centre_->QueueEmpty();

}

void ConditionalEventEmergencyOrder::Execute()       //schedule emergency order, block future emergency orders
{
  if(data_in_console) std::cout << "\n" << blood_centre_->get_system_time() << ". Emergency order for blood units sent\n";


  blood_centre_->stat_emergency_orders_sent++;
  blood_centre_->stat_last_emergency_order_sent = blood_centre_->get_system_time();

  const auto time_from_rng = generator_->NormalDistribution(blood_centre_->get_emergency_order_avg_time(), blood_centre_->get_emergency_order_time_var());

  te_emergency_order_arrived_->schedule(blood_centre_->get_system_time() + time_from_rng);

  blood_centre_->set_order_flag(true, true);
}



bool ConditionalEventResearchLevel::ConditionMet() const
{
  if(blood_centre_->get_research_flag())    //if research is already planned chenk if it should be aborted
  {
    if (blood_centre_->AmountOfBloodInDepot() <= blood_centre_->get_level_to_research()) 
    {       
      if(data_in_console) std::cout << "\n" << blood_centre_->get_system_time() << ". There is no longer enough blood for research\n";
      te_research_->schedule(-1);
      blood_centre_->set_research_flag(false);      
    }
    return false;
  }
 
  {
    return (blood_centre_->AmountOfBloodInDepot() > blood_centre_->get_level_to_research());
  }
}

void ConditionalEventResearchLevel::Execute()
{                                             
  te_research_->schedule(blood_centre_->get_system_time() + blood_centre_->get_time_to_research());
  blood_centre_->set_research_flag(true);

}


