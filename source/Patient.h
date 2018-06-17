#pragma once
class Patient
{
public:
  explicit Patient(int amount_of_blood_needed, int time_of_arrival);
  ~Patient() = default;

  int get_amount_of_blood() const { return  amount_of_blood_needed_; }
  int get_time_of_arrival() const { return  time_of_arrival; }

  void DonateBlood() { amount_of_blood_needed_--; }

private:
  int amount_of_blood_needed_;               
  const int time_of_arrival;                                 
};

