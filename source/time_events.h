#pragma once
#include "event.h"
#include "blood_centre.h"
#include "generators.h"
  
/**** different time events which will/may occur during simulation*****/


class TimeEventBloodExpired :
  public Event
{
public:
  explicit TimeEventBloodExpired(BloodCentre* blood_centre);
   ~TimeEventBloodExpired() = default;

  void Execute() override;
  void schedule();
  int get_event_time() const { return event_time_; };

 
private:
  int event_time_;

  BloodCentre* blood_centre_;
};

class TimeEventPatientArrival :         
  public Event
{
public:
  explicit TimeEventPatientArrival(BloodCentre* blood_centre);
   ~TimeEventPatientArrival() = default;

 void Execute() override;
 void schedule(int next_event_time);
 int get_event_time() const { return event_time_; };

private:
  int event_time_;
  int random_seed_;

  Generators* generator_;
  Generators* generator2_;

 BloodCentre* blood_centre_;
 
  
  

 };


class TimeEventBloodDonated :        
  public Event
{
public:
  explicit TimeEventBloodDonated(BloodCentre* blood_centre, TimeEventBloodExpired* te_blood_expired);
   ~TimeEventBloodDonated() = default;

  void Execute() override;
  void schedule(int next_event_time);
  int get_event_time() const { return event_time_; };

private:
  int event_time_;
  int random_seed_;

  Generators* generator_;
  BloodCentre* blood_centre_;
  TimeEventBloodExpired* te_blood_expired_;


  

};


class TimeEventNormalOrderArrived :
  public Event
{
public:
  explicit TimeEventNormalOrderArrived(BloodCentre* blood_centre, TimeEventBloodExpired* te_blood_expired);
  ~TimeEventNormalOrderArrived() = default;

  void Execute() override;
  void schedule(int next_event_time);
  int get_event_time() const { return event_time_; };


  private:
    int event_time_;
  BloodCentre* blood_centre_;
  TimeEventBloodExpired* te_blood_expired_;

};
  
class TimeEventEmergencyOrderArrived :
  public Event
{
public:
  explicit TimeEventEmergencyOrderArrived(BloodCentre* blood_centre, TimeEventBloodExpired* te_blood_expired);
  ~TimeEventEmergencyOrderArrived() = default;

  void Execute() override;
  void schedule(int next_event_time);
  int get_event_time() const { return event_time_; };


private:
  int event_time_;

  BloodCentre * blood_centre_;
  TimeEventBloodExpired* te_blood_expired_;

};


class TimeEventResearch :             
  public Event
{
public:
  explicit TimeEventResearch(BloodCentre* blood_centre, TimeEventBloodExpired* te_blood_expired);
  ~TimeEventResearch() = default;

  void Execute() override;
  void schedule(int next_event_time);
  int get_event_time() const { return event_time_; };


private:
  int event_time_;
  int random_seed_;

  Generators* generator_;
  BloodCentre * blood_centre_;
  TimeEventBloodExpired* te_blood_expired_;

};


