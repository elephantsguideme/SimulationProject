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
  

  BloodCentre* blood_centre;
  te_blood_expired* te_blood_expired;



};