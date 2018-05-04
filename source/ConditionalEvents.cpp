#include <random>
#include <iostream>

#include "ConditionalEvents.h"
#include "BloodCentre.h"
#include "Patient.h"
#include "Generators.h"

    
// constructors 

ce_blood_transfusion::ce_blood_transfusion(BloodCentre* blood_centre, ::te_blood_expired* te_blood_expired)    :
  blood_centre(blood_centre) , te_blood_expired(te_blood_expired)
{
}





bool ce_blood_transfusion::condition_met() const
{
  return !blood_centre->is_queue_empty() && blood_centre->get_amount_of_blood_in_depot() >= blood_centre->get_amount_of_blood_needed();
}

void ce_blood_transfusion::Execute()
{
  const auto amount_of_blood_needed = blood_centre->get_amount_of_blood_needed();
  for(auto i=0; i<amount_of_blood_needed; i++)
  {
    blood_centre->utilize_blood();
  }
  std::cout <<  blood_centre->get_system_time() << ". " << amount_of_blood_needed << " unit(s) transfused. There are(is) " << blood_centre->get_amount_of_blood_in_depot() << " blood unit(s) in depot\n";
  blood_centre->remove_patient();
}
