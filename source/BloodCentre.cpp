
#include "BloodCentre.h"
#include <iostream>
 

BloodCentre::BloodCentre(const int r, const int n, const int z, const int t1, const int t2, const int p, const double w,
                         const int e, const int q, const int l, const int tu, const int tb, const int jb_min, const int jb_max) :

  system_time_(0),

kMinimumBloodLevel(r) ,
kNormalOrderAmount(n),
kNormalOrderAvgTime(z),
kExpirationTime1(t1),
kExpirationTime2(t2),
kPatientsAvgTime(p),
kNeededBloodAvgAmount(1.0/w),
kEmergencyOrderAmount(q),
kEmergencyOrderAvgTime(e),
kEmergencyOrderTimeVar(e*w*w),
kDonorsAvgTime(l),

kTimeToResearch(tu),
kLevelToResearch(tb),
kMinAmountToResearch(jb_min),
kMaxAmountToResearch(jb_max)


{
 
}

void BloodCentre::add_patient_to_queue(Patient* patient)
{
  patients_queue_.push(patient);
  std::cout << get_system_time()<<". Patient added to the queue. There are "<< patients_queue_.size() <<" patients in line\n";
  
  

  
}

void BloodCentre::add_blood_to_depot1(BloodUnit* blood_unit)
{
  blood_depot1_.push(blood_unit);
  //std::cout << get_system_time() << ". Blood added to the queue. There are " << blood_depot1_.size()+blood_depot2_.size() << " blood units in depot\n";

}

void BloodCentre::add_blood_to_depot2(BloodUnit* blood_unit)
{
  blood_depot2_.push(blood_unit);
  std::cout << get_system_time() << ". Blood added to the queue. There are " << blood_depot1_.size() + blood_depot2_.size() << " blood units in depot\n";
}

int BloodCentre::get_blood_utilization_time()     //returns nearest time of blood utilization 
{
   if(blood_depot1_.empty())
   {
     if(blood_depot2_.empty()) return(-1);
     return(blood_depot2_.front()->get_time_of_utilization());
   }

   if (blood_depot2_.empty())       return(blood_depot1_.front()->get_time_of_utilization());



  if(blood_depot1_.front()->get_time_of_utilization() < blood_depot2_.front()->get_time_of_utilization())
  {
    return(blood_depot1_.front()->get_time_of_utilization());
  }
  return(blood_depot2_.front()->get_time_of_utilization());
}

int BloodCentre::get_amount_of_blood_in_depot() const
{
  return  blood_depot1_.size() + blood_depot2_.size();
}

int BloodCentre::get_amount_of_blood_needed()
{
  if (!patients_queue_.empty())return patients_queue_.front()->get_amount_of_blood();
  return 0;
}

bool BloodCentre::is_queue_empty() const
{
  return patients_queue_.empty();
}

void BloodCentre::utilize_blood()      //destroys blood unit with lowest time_of_utilization value
{

  if (blood_depot1_.empty())
  {
    blood_depot2_.pop();
    return;
  }
  if (blood_depot2_.empty())
  {
    blood_depot1_.pop();
    return;
  }
  if (blood_depot1_.front()->get_time_of_utilization() < blood_depot2_.front()->get_time_of_utilization())
  {
    blood_depot1_.pop();
    return;
  }
  blood_depot2_.pop();
  
}

void BloodCentre::remove_patient()
{
  patients_queue_.pop();
  std::cout << get_system_time() << ". Patient removed from the queue. There are " << patients_queue_.size() << " patients in line\n";
}

void BloodCentre::donate_blood()
{
  patients_queue_.front()->donate_blood();

}

bool BloodCentre::get_order_flag(const bool emergency) const        //if flag is set then the order is due
                                                                    // if emergency = false then it is asking about normal order
{
  if (emergency)return emergency_order_flag_;
  return  normal_order_flag_;
}

void BloodCentre::set_order_flag(const bool value, const bool emergency)
{
  if (emergency) emergency_order_flag_ =value;
  normal_order_flag_ = value;
}


