#pragma once
#include "Event.h"
#include "BloodCentre.h"
#include "BloodUnit.h"


class time_event :       //class derived from Event, still won't have objects directly
  public Event
{
public:
  void Execute() override=0;
  virtual void  schedule()=0;   //function planning event's future excecution 
  int event_time;
  time_event() = default;
  virtual ~time_event() = default;
};


/**** different time events which will/may occur during simulation*****/


class te_patient_arrival :         
  public Event
{
public:
  te_patient_arrival(BloodCentre* blood_centre);
  virtual ~te_patient_arrival() = default;

 void Execute() override;
 void schedule(int next_event_time);
 int event_time;

 BloodCentre* blood_centre;
               
  
  

 };


class te_blood_donated :        
  public Event
{
public:
   te_blood_donated(BloodCentre* blood_centre);
  virtual ~te_blood_donated() = default;

  void Execute() override;
  void schedule(int next_event_time);
  int event_time;

  BloodCentre* blood_centre;


  

};


class te_blood_expired :         
  public Event
{
public:
   te_blood_expired(BloodCentre* blood_centre);
  virtual ~te_blood_expired() = default;

  void Execute() override;
  void schedule(int next_event_time);

  int event_time;
  BloodCentre* blood_centre;
  


  

};