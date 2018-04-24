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

  void add_patient_to_queue(Patient* patient);



private:
  int system_time; // absolute system time (in arbitrary "time units")

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
  std::vector<BloodUnit*> blood_depot_;

};

