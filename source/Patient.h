#pragma once
class Patient
{
public:
  explicit Patient(int amount_of_blood_needed);
  ~Patient() = default;
  int get_amount_of_blood() const { return  k_amount_of_blood_needed_; };

private:
  const int k_amount_of_blood_needed_;
};

