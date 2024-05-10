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

Configure your timer settings with ease using the following guidelines. Each setting group is separated by a comma `,`. The name of the setting must be written exactly as listed below, followed by a semicolon `;` and the corresponding value.

- **Live**: Indicates whether the action is live or not. `0` for disabled, `1` for enabled.
- **Mode**: Specifies the mode of operation. `0` for time, `1` for sunrise, `2` for sunset.
- **Time**: Represents the time at which the action occurs in a 24-hour format (`HH:MM`). Not needed if the mode isn't set to time (`0`).
- **Repeat**: Determines if the action should repeat. `0` for disabled, `1` for enabled.
- **Days**: Specifies the days on which the action occurs. Use `-` or `0` to disable, any other character will enable that day of the week. Days are determined by their position in the string; for example, to repeat on Sunday, it looks like this: `S------`. For Sunday and Monday, use `SM-----`, and so on.
- **Output**: Indicates the output of the action. (To Be Determined)
- **Action**: Specifies the type of action. `0` for off, `1` for on, `2` for toggle.
- **Offset**: Represents any time offset applied to the action in the same format as time. Prefix with a `+` or `-` for positive or negative offset.

**Example**:

`Live;1,Mode;0,Time;9:41,Repeat;1,Days;-MTWTF-,Output;1,Action;2,Offset;-00:01`

  
## Contribute 

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate/?hosted_button_id=Q9A7HG8NQEJRU) - or - [!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/rebbepod)
