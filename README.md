[![Banner](https://github.com/chabad-source/esphometimer/blob/main/images/banner.png)](https://github.com/chabad-source/esphometimer)

[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/f17caa6e3d2946378de9beae9fc0ffe8)](https://www.codacy.com/gh/chabad-source/melachaplug/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=chabad-source/melachaplug&amp;utm_campaign=Badge_Grade)
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/donate/?hosted_button_id=Q9A7HG8NQEJRU)


# ESPHome Timer
Adds dynamic timer functionality to your esphome project. Set and adjust timers on the fly!

This project is using [ESPHome](https://esphome.io/).

## Features
-   [x] Features 20 updatable timers.
-   [x] Set repeatable timers based on the day of the week.
-   [x] Auto detect missed timers and restore the latest timers value.
-   [x] Includes quick override option.
-   [x] All running locally, no reliance on another server.


## Instructions

-   Copy the code and edit to your ESPs specifications.
-   Be sure to include your location and the correct time zone for your area.

### Timer Configuration

Each setting group is seperated by a `,`. The name of the setting must be written exactly as listed below, followed by `;` and the value.
- **Live**: Indicates whether the action is live or not. `0` = disbaled, `1` = enabled.
- **Mode**: Specifies the mode of operation. `0` = time, `1` = sunrise, `2` = sunset.
- **Time**: Represents the time at which the action occurs. time in 24 hour format with a `:` in between (example `18:30`). (not needed if mode isn't set to time `0`).
- **Repeat**: Determines if the action should repeat.  `0` = disbaled, `1` = enabled.
- **Days**: Specifies the days on which the action occurs. `-` or `0` to disable, any other character will enable that day of the week. Days are determined by position in the string, to repeat on sunday it looks like this `S------`, for sunday and monday `SM-----` etc.
- **Output**: Indicates the output of the action. TBD
- **Action**: Specifies the type of action.  `0` = off, `1` = on, `2` = toggle.
- **Offset**: Represents any time offset applied to the action. Same format as time. Prface with a `+` or `-` for positive or negative offset.

Timer Configuration example: `Live;1,Mode;0,Time;9:41,Repeat;1,Days;-MTWTF-,Output;1,Action;2,Offset;-00:01`

*Usage Instructions*
-   Mode 0 uses the time as the base. Mode 1 uses sunrise, Mode 2 uses sunset.
-   Action sets the action to do when timer runs. 0 = off 1 = on and 2 = toggle.
-   Output is the position of your relay, if you only have 1 relay use 0 (with two relays the first would be 0 and the second 1).
-   When using an offset, set "Timer Negative Offset" to on to make the offset be applied to before sunrise or sunset.
-   If "Repeat" is disabled the timer will only run once, after running the timer will automatically disable itself.
  
## Contribute 

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate/?hosted_button_id=Q9A7HG8NQEJRU) - or - [!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/rebbepod)
