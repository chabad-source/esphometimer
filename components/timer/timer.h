/****
Copyright (c) 2024 RebbePod

This library is free software; you can redistribute it and/or modify it 
under the terms of the GNU Lesser GeneralPublic License as published by the Free Software Foundation; 
either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,but WITHOUT ANY WARRANTY; 
without even the impliedwarranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
See the GNU Lesser General Public License for more details. 
You should have received a copy of the GNU Lesser General Public License along with this library; 
if not, write tothe Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA, 
or connect to: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
****/

/***************************************************
*      ***** Timer Format Cheat Guide *****
* **************************************************
*              ***** Switches *****
*   *** Value set to '1' enabled '0' disabled ***
* 0 = Enabled
* 1-7 = Days (sun-sat)
* 8 = Repeat
* 9 = Negative Offset
*               ***** Numbers *****
* 10 = Time hour
* 11 = Time minute
* 12 = Output  {value '0' for the first position of switch in the 'relays' variable}
* 13 = Action  {'0' turn off, '1' turn on, '2' toggle}
* 14 = Offset hour
* 15 = Offset minute
* 16 = Mode    {'0' use time, '1' use sunrise, '2' use sunset} 
* sample "Live;1,Mode;0,Time;2:36,Repeat;1,Days;SMTWTFS,Output;1,Action;2,Offset;-0:30,"
****************************************************/

#pragma once
#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/components/button/button.h"
#include "esphome/components/select/select.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/text/text.h"

#include <string>
#include <vector>


namespace esphome {
namespace timer {


struct TimerData {
    bool valid : 1;
    bool live : 1;
    bool repeat : 1;
    bool use_negative_offset : 1;
    union {
        struct {
            bool sun : 1;
            bool mon : 1;
            bool tue : 1;
            bool wed : 1;
            bool thu : 1;
            bool fri : 1;
            bool sat : 1;
        } day;
        uint8_t raw;
    } days;
    uint8_t mode;
    uint8_t output;
    float action;
    uint8_t hour;
    uint8_t minute;
    time_t last_ran_timestamp;

    std::string to_string() const;
    void reset();
    void from_string(const std::string& settings);
} __attribute__((packed));

class Timer : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::DATA; }
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_num_timers(int count);
  void add_switch_output(switch_::Switch *sw);
  void add_automation_output(Trigger<float> *trigger);
  void set_timer_text(text::Text *txt);
  void set_timer_select(select::Select *sel);

  void choose(int index);
  void set_timer_text(const std::string &value);

 protected:
  std::vector<TimerData> timers_;
  std::vector<std::function<void(float)>> outputs_;
  text::Text *text_{nullptr};
  select::Select *select_{nullptr};
  int selected_timer_{-1};
  bool updating_{false};

  void choose_(int index, bool update);
};

class TimerText : public Component, public text::Text {
 public:
  void control(const std::string &value);
};

class TimerSelect : public Component, public select::Select {
 public:
  void control(const std::string &value);
};

class TimerSaveButton : public Component, public button::Button {
 public:
  void press_action() {}
};

}  // namespace timer
}  // namespace esphome
