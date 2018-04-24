
#include "BloodCentre.h"
#include <iostream>
 

BloodCentre::BloodCentre(const int r, const int n, const int z, const int t1, const int t2, const int p, const double w,
                         const int e, const int q, const int l, const int tu, const int tb, const int jb_min, const int jb_max) :

  system_time(0),

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

  std::cout << get_system_time()<<". Patient added to the queue\n";
  
  

  patients_queue_.push(patient);
}


