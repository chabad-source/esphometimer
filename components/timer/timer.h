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
#include <string>



namespace esphome {
namespace timer {


struct Timer {
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
    uint8_t mode : 2;
    uint8_t action : 2;
    uint8_t output : 2;
    uint8_t hour : 5;
    uint8_t minute : 6;
    uint8_t offset_hour : 5;
    uint8_t offset_minute : 6;
    time_t last_ran_timestamp;

    std::string to_string() const;
    void reset();
    void from_string(const std::string& settings);
} __attribute__((packed));

void doRelayAction(const uint8_t& i, const time_t& timestamp, const bool& set_relays);
bool dayMatches(const esphome::ESPTime& date, const struct Timer& timer);
bool isSameDay(std::time_t timestamp1, std::time_t timestamp2);
void checkForMissedTimer(const uint8_t& i);
time_t getTimerTimestamp(const struct Timer& timer);
void setTimerTimestamp(const uint8_t& i);
void setAllTimersTimestamp();
void onInterval();
void onSelect();
void onPressSave();

}  // namespace timer
}  // namespace esphome