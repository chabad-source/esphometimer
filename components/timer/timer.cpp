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
*              ***** settings *****
*   *** Value set to '1' enabled '0' disabled ***
* 0 = Enabled
* 1-7 = Days (sun-sat)
* 8 = Repeat
* 9 = Negative Offset
* 10 = Time hour
* 11 = Time minute
* 12 = Output  {value '0' for the first position of switch in the 'relays' variable}
* 13 = Action  {'0' turn off, '1' turn on, '2' toggle}
* 14 = Offset hour
* 15 = Offset minute
* 16 = Mode    {'0' use time, '1' use sunrise, '2' use sunset} 
* sample "Live;1,Mode;0,Time;2:36,Repeat;1,Days;SMTWTFS,Output;1,Action;2,Offset;-0:30,"
****************************************************/

#include "timer.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace timer {

static const char *const TAG = "timer";

std::string TimerData::to_string() const {
    std::string result = str_sprintf("Live;%d,Mode;%d", this->live, this->mode);

    // Include time only if mode is 0
    if (this->mode == 0) {
        result += str_sprintf(",Time;%d:%02d", this->hour, this->minute);
    } else if (this->hour != 0 || this->minute != 0) {
    // Include offset if it's not zero
        result += str_sprintf(",Offset;%c%d:%02d", use_negative_offset ? '-' : '+', this->hour, this->minute);
    }

    result += str_sprintf(",Repeat;%d,Days;%c%c%c%c%c%c%c,Output;%d,Action;%d", this->repeat,
                          this->days.day.sun ? 'S' : '-',
                          this->days.day.mon ? 'M' : '-',
                          this->days.day.tue ? 'T' : '-',
                          this->days.day.wed ? 'W' : '-',
                          this->days.day.thu ? 'T' : '-',
                          this->days.day.fri ? 'F' : '-',
                          this->days.day.sat ? 'S' : '-',
                          this->output, this->action);

    return result;
}

void TimerData::reset() {
    this->valid = false;
    this->live = false;
    this->repeat = false;
    this->use_negative_offset = false;
    this->days.raw = 0;
    this->mode = 0;
    this->action = 0;
    this->output = 0;
    this->hour = 0;
    this->minute = 0;
    this->last_ran_timestamp = 0;
}

void TimerData::from_string(const std::string& settings) {
    this->reset(); // resets the timer
    uint16_t index = 0;
    while (index < settings.size()) {
        // Find the next key-value pair
        size_t delimiterPos = settings.find(',', index);
        if (delimiterPos == std::string::npos) {
            delimiterPos = settings.size();  // Handle the last key-value pair
        }
        std::string keyValuePair = settings.substr(index, delimiterPos - index);

        // Split key and value
        size_t keyDelimiterPos = keyValuePair.find(';');
        if (keyDelimiterPos == std::string::npos) {
            break;
        }
        std::string key = keyValuePair.substr(0, keyDelimiterPos);
        std::string value = keyValuePair.substr(keyDelimiterPos + 1);

        // Update struct settings based on the key-value pair
        if (key == "Live") {
            this->live = std::stoi(value);
        } else if (key == "Mode") {
            this->mode = std::stoi(value);
        } else if (key == "Time") {
            size_t colonPos = value.find(':');
            if (colonPos != std::string::npos) {
                this->hour = std::stoi(value.substr(0, colonPos));
                this->minute = std::stoi(value.substr(colonPos + 1));
            }
        } else if (key == "Repeat") {
            repeat = std::stoi(value);
        } else if (key == "Days") {
            for (size_t i = 0; i < value.size(); ++i) {
                if (value[i] != '-' && value[i] != '0') {
                    switch (i) {
                        case 0: this->days.day.sun = true; break;
                        case 1: this->days.day.mon = true; break;
                        case 2: this->days.day.tue = true; break;
                        case 3: this->days.day.wed = true; break;
                        case 4: this->days.day.thu = true; break;
                        case 5: this->days.day.fri = true; break;
                        case 6: this->days.day.sat = true; break;
                        default: break;
                    }
                }
            }
        } else if (key == "Output") {
            this->output = std::stoi(value);
        } else if (key == "Action") {
            this->action = std::stof(value);
        } else if (key == "Offset") {
            if (!value.empty()) {
                size_t colonPos = value.find(':');
                // check if offset starts with a + or - (or blank)
                if (value[0] == '-') {
                    use_negative_offset = true;
                    if (colonPos != std::string::npos) {
                        this->hour = std::stoi(value.substr(1, colonPos));
                    }
                } else if (value[0] == '+') {
                    use_negative_offset = false;
                    if (colonPos != std::string::npos) {
                        this->hour = std::stoi(value.substr(1, colonPos));
                    }
                } else {
                    use_negative_offset = false;
                    if (colonPos != std::string::npos) {
                        this->hour = std::stoi(value.substr(0, colonPos));
                    }
                }
                this->minute = std::stoi(value.substr(colonPos + 1));
            }
        }

        // Move to the next key-value pair
        index = delimiterPos + 1;
    }
    this->valid = true;
}

void Timer::setup() {
  ESP_LOGD(TAG, "setting up");
  this->choose_(0, true);
}

void Timer::loop() {
}

void Timer::dump_config() {
}

void Timer::set_num_timers(int count) {
  for (int i = 0; i < count; i++)
    this->timers_.push_back(TimerData());
}

void Timer::set_timer_text(text::Text *txt) {
  this->text_ = txt;
  txt->add_on_state_callback([this](std::string state) {
    if (!this->updating_)
      this->set_timer_text(state);
  });
}

void Timer::set_timer_select(select::Select *sel) {
  this->select_ = sel;
  sel->add_on_state_callback([this](std::string state, size_t index) {
    this->choose_(index, false);
  });
}

static void switch_output(switch_::Switch *sw, float action) {
  if (action == 0)
    sw->turn_off();
  else if (action == 1)
    sw->turn_on();
  else if (action == 2)
    sw->toggle();
  else
    ESP_LOGD(TAG, "invalid switch action %f", action);
}

void Timer::add_switch_output(switch_::Switch *sw) {
  using namespace std::placeholders;
  this->outputs_.push_back(std::move(std::bind(switch_output, sw, _1)));
}

void Timer::add_automation_output(Trigger<float> *trigger) {
  using namespace std::placeholders;
  this->outputs_.push_back(std::move(std::bind(&Trigger<float>::trigger, trigger, _1)));
}

void Timer::choose(int index) {
  this->choose_(index, true);
}

void Timer::set_timer_text(const std::string &value) {
  this->timers_[this->selected_timer_].from_string(value);
  this->updating_ = true;
  if (this->text_ != nullptr)
    this->text_->make_call().set_value(this->timers_[this->selected_timer_].to_string()).perform();
  this->updating_ = false;
}

void Timer::choose_(int index, bool update) {
  ESP_LOGD(TAG, "selecting timer %d", index);
  if (index >= this->timers_.size())
    return;
  if (index == this->selected_timer_)
    return;
  this->selected_timer_ = index;
  if (this->text_ != nullptr)
    this->text_->make_call().set_value(this->timers_[index].to_string()).perform();
  if (update && (this->select_ != nullptr))
    this->select_->make_call().set_index(index).perform();
}

void TimerSelect::control(const std::string &value) {
  this->publish_state(value);
}

void TimerText::control(const std::string &value) {
  this->publish_state(value);
}
}  // namespace timer
}  // namespace esphome
