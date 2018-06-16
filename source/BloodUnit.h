#pragma once
//#include "TimeEvents.h"

class BloodUnit
{
public:

  BloodUnit(int time_of_utilization);
  ~BloodUnit()= default;
  int get_time_of_utilization() const
  {
    return time_of_utilization_;
  };
private:
  const int time_of_utilization_;

  //TeBloodExpired* te_expire; // this unit's projected destruction event 
};

