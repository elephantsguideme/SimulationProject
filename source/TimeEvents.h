#pragma once
#include "Event.h"
#include "BloodCentre.h"
#include "Generators.h"
  
/**** different time events which will/may occur during simulation*****/


class te_blood_expired :
  public Event
{
public:
  explicit te_blood_expired(BloodCentre* blood_centre);
   ~te_blood_expired() = default;

  void execute() override;
  void schedule();
  int get_event_time() const { return event_time_; };

 
private:
  int event_time_;

  BloodCentre* blood_centre_;
};

class te_patient_arrival :         
  public Event
{
public:
  explicit te_patient_arrival(BloodCentre* blood_centre);
   ~te_patient_arrival() = default;

 void execute() override;
 void schedule(int next_event_time);
 int get_event_time() const { return event_time_; };

private:
  int event_time_;
  int random_seed_;

  Generators* generator_;
  Generators* generator2_;

 BloodCentre* blood_centre_;
 
  
  

 };


class te_blood_donated :        
  public Event
{
public:
  explicit te_blood_donated(BloodCentre* blood_centre, te_blood_expired* te_blood_expired);
   ~te_blood_donated() = default;

  void execute() override;
  void schedule(int next_event_time);
  int get_event_time() const { return event_time_; };

private:
  int event_time_;
  int random_seed_;

  Generators* generator_;
  BloodCentre* blood_centre_;
  te_blood_expired* te_blood_expired_;


  

};


class te_normal_order_arrived :
  public Event
{
public:
  explicit te_normal_order_arrived(BloodCentre* blood_centre, te_blood_expired* te_blood_expired);
  ~te_normal_order_arrived() = default;

  void execute() override;
  void schedule(int next_event_time);
  int get_event_time() const { return event_time_; };


  private:
    int event_time_;
  BloodCentre* blood_centre_;
  te_blood_expired* te_blood_expired_;

};
  
class te_emergency_order_arrived :
  public Event
{
public:
  explicit te_emergency_order_arrived(BloodCentre* blood_centre, te_blood_expired* te_blood_expired);
  ~te_emergency_order_arrived() = default;

  void execute() override;
  void schedule(int next_event_time);
  int get_event_time() const { return event_time_; };


private:
  int event_time_;

  BloodCentre * blood_centre_;
  te_blood_expired* te_blood_expired_;

};


class te_research :             
  public Event
{
public:
  explicit te_research(BloodCentre* blood_centre, te_blood_expired* te_blood_expired);
  ~te_research() = default;

  void execute() override;
  void schedule(int next_event_time);
  int get_event_time() const { return event_time_; };


private:
  int event_time_;
  int random_seed_;

  Generators* generator_;
  BloodCentre * blood_centre_;
  te_blood_expired* te_blood_expired_;

};


