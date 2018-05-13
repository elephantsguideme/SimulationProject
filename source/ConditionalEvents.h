#pragma once
#include "Event.h"
#include "BloodCentre.h"
#include "TimeEvents.h"



/**** different conditional events which will/may occur during simulation*****/

class ce_blood_transfusion :
  public Event
{
public:
  explicit ce_blood_transfusion(BloodCentre* blood_centre, te_blood_expired* te_blood_expired);
  ~ce_blood_transfusion() = default;

  bool condition_met() const;
  void Execute() override;
  
private:
  BloodCentre* blood_centre_;
  te_blood_expired* te_blood_expired_;



};


class ce_normal_order :
  public Event
{
public:
  explicit ce_normal_order(BloodCentre* blood_centre, te_normal_order_arrived* te_normal_order_arrived);
  ~ce_normal_order() = default;

  bool condition_met() const;
  void Execute() override;

private:
  BloodCentre* blood_centre_;
  te_normal_order_arrived* te_normal_order_arrived_;

};


class ce_emergency_order :
  public Event
{                                                                      
public:
  explicit ce_emergency_order(BloodCentre* blood_centre, te_emergency_order_arrived* te_emergency_order_arrived);
  ~ce_emergency_order() = default;

  bool condition_met() const;
  void Execute() override;

private:
  BloodCentre* blood_centre_;
  te_emergency_order_arrived* te_emergency_order_arrived_;

};


class ce_research_level :
  public Event
{
public:
  explicit ce_research_level(BloodCentre* blood_centre, te_research* te_research);
  ~ce_research_level() = default;

  bool condition_met() const;
  void Execute() override;

private:
  BloodCentre * blood_centre_;
  te_research* te_research_;

};