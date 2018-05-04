#pragma once
#include "Event.h"
#include "BloodCentre.h"

  
/**** different time events which will/may occur during simulation*****/


class te_patient_arrival :         
  public Event
{
public:
  explicit te_patient_arrival(BloodCentre* blood_centre);
   ~te_patient_arrival() = default;

 void Execute() override;
 void schedule(int next_event_time);
 int event_time;

 BloodCentre* blood_centre;
               
  
  

 };


class te_blood_donated :        
  public Event
{
public:
  explicit te_blood_donated(BloodCentre* blood_centre);
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
  explicit te_blood_expired(BloodCentre* blood_centre);
  virtual ~te_blood_expired() = default;

  void Execute() override;
  void schedule();

  int event_time;
  BloodCentre* blood_centre;
  


  

};