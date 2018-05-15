#pragma once

//class for all events

class Event
{
public:
  virtual void execute() = 0;  //will be overridden 
  Event() = default;
  virtual ~Event() = default;
};
