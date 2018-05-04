#pragma once

#include <vector>
#include <queue>

#include "BloodUnit.h"
#include "Patient.h"




class BloodCentre
{
public:
  BloodCentre(int r, int n, int z, int t1, int t2, int p, double w,
    int e, int q, int l, int tu, int tb, int jb_min, int jb_max);
  //creating main object characterized with given constants 
  ~BloodCentre()=default;        


  int get_system_time() const { return system_time; };
  int get_patients_avg_time() const { return kPatientsAvgTime; };
  int get_expiration_time1() const { return kExpirationTime1; };
  int get_expiration_time2() const { return kExpirationTime2; };

  double get_needed_blood_avg_amount() const { return kNeededBloodAvgAmount; };
  int get_donors_avg_time() const { return kDonorsAvgTime; };

  void add_patient_to_queue(Patient* patient);
  void add_blood_to_depot1(BloodUnit* blood_unit);
  void add_blood_to_depot2(BloodUnit* blood_unit);
  int get_blood_utilization_time();
  int get_amount_of_blood() const;
  void utilize_blood();

private:
  int system_time; // absolute system time (in "arbitrary time units")

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

