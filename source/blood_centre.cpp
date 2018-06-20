
#include "blood_centre.h"
#include <iostream>
 
bool data_in_console;


BloodCentre::BloodCentre(const int r, const int n, const int z, const int t1, const int t2, const int p, const double w,
  const int e, const double ew2, const int q, const int l, const int tu, const int tb, const int jb_min, const int jb_max) :



  system_time_(0),

  kMinimumBloodLevel(r),
  kNormalOrderAmount(n),
  kNormalOrderAvgTime(z),
  kExpirationTime1(t1),
  kExpirationTime2(t2),
  kPatientsAvgTime(p),
  kNeededBloodAvgAmount(1.0 / w),
  kEmergencyOrderAmount(q),
  kEmergencyOrderAvgTime(e),
  kEmergencyOrderTimeVar(ew2),
  kDonorsAvgTime(l),

  kTimeToResearch(tu),
  kLevelToResearch(tb),
  kMinAmountToResearch(jb_min),
  kMaxAmountToResearch(jb_max),


  stat_amount_of_blood_transfused(0),
  stat_amount_of_blood_destroyed(0),
  stat_amount_of_blood_used_for_research(0),
  stat_patients_arrived(0),
  stat_patients_left(0),
  stat_donors_arrived(0),
  stat_max_number_of_patients_in_queue(0),
  stat_total_time_spent_in_queue(0),
  stat_normal_orders_sent(0),
  stat_emergency_orders_sent(0),
  stat_last_emergency_order_sent(0)

{
 
}



void BloodCentre::AddPatientToQueue(Patient* patient)
{
  patients_queue_.push(patient);
  if(data_in_console) std::cout << get_system_time()<<". Patient added to the queue. There are "<< patients_queue_.size() <<" patients in line\n";
  if (patients_queue_.size() > stat_max_number_of_patients_in_queue)   stat_max_number_of_patients_in_queue = patients_queue_.size();
  

  
}

void BloodCentre::AddBloodToDepot1(BloodUnit* blood_unit)
{
  blood_depot1_.push(blood_unit);
  //if(data_in_console) std::cout << get_system_time() << ". Blood added to the queue. There are " << blood_depot1_.size()+blood_depot2_.size() << " blood units in depot\n";

}

void BloodCentre::AddBloodToDepot2(BloodUnit* blood_unit)
{
  

  
  blood_depot2_.push(blood_unit);
  if(data_in_console) std::cout << get_system_time() << ". Blood added to the queue. There are " << blood_depot1_.size() + blood_depot2_.size() << " blood units in depot\n";
  
}

int BloodCentre::BloodUtilizationTime() const    //returns nearest time of blood utilization 
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

int BloodCentre::AmountOfBloodInDepot() const
{
  return  blood_depot1_.size() + blood_depot2_.size();
}

int BloodCentre::AmountOfBloodNeeded()  const
{
  if (!patients_queue_.empty())return patients_queue_.front()->get_amount_of_blood();
  return 0;
}

int BloodCentre::PatientsInQueue() const
{
  return patients_queue_.size();
}

bool BloodCentre::QueueEmpty() const
{
  return patients_queue_.empty();
}

void BloodCentre::UtilizeBlood()      //destroys blood unit with lowest time_of_utilization value
{
  BloodUnit *temp;      //prevents memory leaks

  if (blood_depot1_.empty())
  {
    temp = blood_depot2_.front();
    
    blood_depot2_.pop();
    delete temp;
    return;
  }

  if (blood_depot2_.empty())
  {
    temp = blood_depot1_.front();
    blood_depot1_.pop();
    delete temp;
    return;
  }

  if (blood_depot1_.front()->get_time_of_utilization() < blood_depot2_.front()->get_time_of_utilization())
  {
    temp = blood_depot1_.front();
    blood_depot1_.pop();
    delete temp;
    return;
  }       

  temp = blood_depot2_.front();
  blood_depot2_.pop();
  delete temp;
  
}

void BloodCentre::RemovePatient()
{
  stat_patients_left++;
  auto *temp = patients_queue_.front();  //prevents memory leaks
  stat_total_time_spent_in_queue += get_system_time() - temp->get_time_of_arrival();
  patients_queue_.pop();
  delete temp;

  if(data_in_console) std::cout << get_system_time() << ". Patient removed from the queue. There are " << patients_queue_.size() << " patients in line\n";
}

void BloodCentre::DonateBlood()
{
  patients_queue_.front()->DonateBlood();

}

double BloodCentre::AverageTimeInQueue()
{
  double time = stat_total_time_spent_in_queue;
  if (stat_patients_left) time /= stat_patients_left; // "if" to prevent division by zero
  return time;
}

double BloodCentre::UsedBloodRatio()
{
  

  double ratio = stat_amount_of_blood_transfused + stat_amount_of_blood_used_for_research;
  if (stat_amount_of_blood_transfused + stat_amount_of_blood_used_for_research)ratio /= ratio + stat_amount_of_blood_destroyed;   // "if" to prevent division by zero
  return ratio;
}

double BloodCentre::EmergencyOrdersRatio()
{

   
  double ratio = stat_emergency_orders_sent;
  if (stat_normal_orders_sent) ratio /= ratio + stat_normal_orders_sent; // "if" to prevent division by zero
  return ratio;
}

int BloodCentre::TimeSinceLastEmergencySent()
{
  return get_system_time()-stat_last_emergency_order_sent;
}

void BloodCentre::ZeroAllStats()
{
  stat_amount_of_blood_transfused = 0;
    stat_amount_of_blood_destroyed = 0;
    stat_amount_of_blood_used_for_research = 0;
    stat_patients_arrived = 0;
    stat_patients_left = 0;
    stat_donors_arrived = 0;
    stat_max_number_of_patients_in_queue = 0;
    stat_total_time_spent_in_queue = 0;
    stat_normal_orders_sent = 0;
    stat_emergency_orders_sent = 0;
}

bool BloodCentre::OrderFlag(const bool emergency) const        //if flag is set then the order is due
                                                                    // if emergency = false then it is asking about normal order
{
  if (emergency)return emergency_order_flag_;
  return  normal_order_flag_;
}

void BloodCentre::set_order_flag(const bool value, const bool emergency)
{
  if (emergency) emergency_order_flag_ =value;
  else normal_order_flag_ = value;
}


