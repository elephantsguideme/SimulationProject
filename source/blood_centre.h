#pragma once

#include <vector>
#include <queue>

#include "blood_unit.h"
#include "patient.h"


extern bool data_in_console;

class BloodCentre
{
public:
  BloodCentre(int r, int n, int z, int t1, int t2, int p, double w,
    int e,double ew2, int q, int l, int tu, int tb, int jb_min, int jb_max);
  //creating main object characterized with given constants 
  ~BloodCentre()=default;        

                                //getters
  int get_system_time() const { return system_time_; }

  int get_patients_avg_time() const { return kPatientsAvgTime; }
  int get_expiration_time1() const { return kExpirationTime1; }
  int get_expiration_time2() const { return kExpirationTime2; }
  double get_needed_blood_avg_amount() const { return kNeededBloodAvgAmount; }
  int get_donors_avg_time() const { return kDonorsAvgTime; }
  int get_minimum_blood_level() const { return kMinimumBloodLevel; }
  int get_normal_order_amount()const { return kNormalOrderAmount; }
  int get_normal_order_avg_time()const { return kNormalOrderAvgTime; }
  int get_emergency_order_amount()const { return kEmergencyOrderAmount; }
  int get_emergency_order_avg_time()const { return kEmergencyOrderAvgTime; }
  double get_emergency_order_time_var()const { return kEmergencyOrderTimeVar; }

   int get_time_to_research()const { return kTimeToResearch; }
   int get_level_to_research()const { return kLevelToResearch; }
   int get_min_to_research()const { return kMinAmountToResearch; }
   int get_max_to_research()const { return kMaxAmountToResearch; }
   bool get_research_flag() const { return research_flag_; }

   int BloodUtilizationTime() const;
   int AmountOfBloodInDepot() const;
   int AmountOfBloodNeeded() const;
   int PatientsInQueue() const;
   bool QueueEmpty() const;
   bool OrderFlag(bool emergency) const;

              //setters
  void set_system_time(const int next_system_time) { system_time_ = next_system_time; };
  void set_order_flag(bool value, bool emergency);
  void set_research_flag(const bool value) { research_flag_=value ; };

  //other functions
  void AddPatientToQueue(Patient* patient);
  void AddBloodToDepot1(BloodUnit* blood_unit);
  void AddBloodToDepot2(BloodUnit* blood_unit);
  
  void UtilizeBlood();
  void RemovePatient();
  void DonateBlood();
 

  //statistics
  int stat_amount_of_blood_transfused;
  int stat_amount_of_blood_destroyed;
  int stat_amount_of_blood_used_for_research;
      
  int stat_patients_arrived;
  int stat_patients_left;
  int stat_donors_arrived;
  int stat_max_number_of_patients_in_queue;
  int stat_total_time_spent_in_queue;
  int stat_normal_orders_sent;
  int stat_emergency_orders_sent;

  int stat_last_emergency_order_sent;
  
  double AverageTimeInQueue();
  double UsedBloodRatio();
  double EmergencyOrdersRatio();
  int TimeSinceLastEmergencySent();
  void ZeroAllStats();




private:
  int system_time_; // absolute system time (in "arbitrary time units")
  bool normal_order_flag_;
  bool emergency_order_flag_;
  bool research_flag_;

  const int kMinimumBloodLevel;
  const int kNormalOrderAmount;
  const int kNormalOrderAvgTime;
  const int kExpirationTime1;
  const int kExpirationTime2;
  const int kPatientsAvgTime;
  const double kNeededBloodAvgAmount;
  const int    kEmergencyOrderAmount;
  const int    kEmergencyOrderAvgTime;
  const double    kEmergencyOrderTimeVar;
  const int  kDonorsAvgTime;

  const int  kTimeToResearch;
  const int  kLevelToResearch;
  const int  kMinAmountToResearch;
  const int  kMaxAmountToResearch;


  std::queue<Patient*> patients_queue_;
  std::queue<BloodUnit*> blood_depot1_;
  std::queue<BloodUnit*> blood_depot2_;

};

