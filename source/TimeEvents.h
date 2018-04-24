#pragma once
#include "Event.h"
#include "BloodCentre.h"
#include "BloodUnit.h"


class TimeEvent :       //class derived from Event, still won't have objects directly
  public Event
{
public:
  void Execute() override=0;
  virtual void  schedule()=0;   //function planning event's future excecution 
  int event_time_;
  TimeEvent() = default;
  virtual ~TimeEvent() = default;
};


/**** different time events which will/may occur during simulation*****/


class TePatientArrival :         
  public Event
{
public:
  TePatientArrival(BloodCentre* blood_c);
  virtual ~TePatientArrival() = default;

 void Execute() override;
 void schedule(int next_event_time);
 int event_time_;

 BloodCentre* blood_centre_;
               
  
  

 };


class TeBloodDonated :        
  public Event
{
public:
   TeBloodDonated(BloodCentre* blood_c);
  virtual ~TeBloodDonated() = default;

  void Execute() override;
  void schedule(int next_event_time);
  int event_time_;

  BloodCentre* blood_centre_;


  

};


class TeBloodExpired :         
  public Event
{
public:
   TeBloodExpired(BloodUnit* blood_u);
  virtual ~TeBloodExpired() = default;

  void Execute() override;
  int event_time;

  BloodUnit*     blood_unit_;    //pointer to the blood unit that will expire


  

};