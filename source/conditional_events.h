#pragma once
#include "event.h"
#include "blood_centre.h"
#include "time_events.h"
#include "generators.h"



/**** different conditional events which will/may occur during simulation*****/

class ConditionalEventBloodTransfusion :
  public Event
{
public:
  explicit ConditionalEventBloodTransfusion(BloodCentre* blood_centre, TimeEventBloodExpired* te_blood_expired);
  ~ConditionalEventBloodTransfusion() = default;

  bool ConditionMet() const;
  void Execute() override;
  
private:
  BloodCentre* blood_centre_;
  TimeEventBloodExpired* te_blood_expired_;



};


class ConditionalEventNormalOrder :
  public Event
{
public:
  explicit ConditionalEventNormalOrder(BloodCentre* blood_centre, TimeEventNormalOrderArrived* te_normal_order_arrived);
  ~ConditionalEventNormalOrder() = default;

  bool ConditionMet() const;
  void Execute() override;

private:
  Generators * generator_;
  BloodCentre* blood_centre_;
  TimeEventNormalOrderArrived* te_normal_order_arrived_;

};


class ConditionalEventEmergencyOrder :
  public Event
{                                                                      
public:
  explicit ConditionalEventEmergencyOrder(BloodCentre* blood_centre, TimeEventEmergencyOrderArrived* te_emergency_order_arrived);
  ~ConditionalEventEmergencyOrder() = default;

  bool ConditionMet() const;
  void Execute() override;

private:
  Generators * generator_;
  BloodCentre* blood_centre_;
  TimeEventEmergencyOrderArrived* te_emergency_order_arrived_;

};


class ConditionalEventResearchLevel :
  public Event
{
public:
  explicit ConditionalEventResearchLevel(BloodCentre* blood_centre, TimeEventResearch* te_research);
  ~ConditionalEventResearchLevel() = default;

  bool ConditionMet() const;
  void Execute() override;

private:
  BloodCentre * blood_centre_;
  TimeEventResearch* te_research_;

};