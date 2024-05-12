import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import button, select, switch, text
from esphome.const import (
    CONF_ID,
    CONF_MODE,
    CONF_OUTPUTS,
    CONF_THEN,
)

"""
timer:
  quantity: 2
  text_input:
    name: "Timer Configuration"
  timer_select:
    name: "Select Timer"
  save_button:
    name: Timer Save
  disable_switch:
    name: Disable Timers
  outputs:
    - type: switch
      id: relay_1
    - type: automation
      then:
        - stuff
  #optional
  names:
    - "Timer 1"
    - "Lamp"
"""

timer_ns = cg.esphome_ns.namespace("timer")
TimerComponent = timer_ns.class_("TimerComponent", cg.Component)
TimerText = timer_ns.class_("TimerText", text.Text, cg.Component)
TimerSelect = timer_ns.class_("TimerSelect", select.Select, cg.Component)
TimerSaveButton = timer_ns.class_("TimerSaveButton", button.Button, cg.Component)
TimerDisableSwitch = timer_ns.class_("TimerDisableSwitch", switch.Switch, cg.Component)

CONF_QUANTITY = "quantity"
CONF_TEXT_INPUT = "text_input"
CONF_TIMER_SELECT = "timer_select"
CONF_SAVE_BUTTON = "save_button"
CONF_DISABLE_SWITCH = "disable_switch"
CONF_NAMES = "names"
CONF_SWITCH = "switch"
CONF_AUTOMATION = "automation"

def validate_names(config):
    if CONF_NAMES not in config:
        return config
    if len(config[CONF_NAMES]) != config[CONF_QUANTITY]:
        raise cv.Invalid("number of names must be the same as the timer quantity")
    return config

OUTPUT_SCHEMA = cv.typed_schema(
    {
        CONF_SWITCH: {
            cv.GenerateID(CONF_ID): cv.use_id(switch.Switch),
        },
        CONF_AUTOMATION: {
            cv.Required(CONF_THEN): automation.validate_automation(single=True),
        }
    }
)

CONFIG_SCHEMA = cv.All(
    cv.COMPONENT_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(TimerComponent),
            cv.Required(CONF_QUANTITY): cv.positive_int,
            cv.Required(CONF_TEXT_INPUT): text.TEXT_SCHEMA.extend(
                {
                    cv.GenerateID(): cv.declare_id(TimerText),
                    cv.Optional(CONF_MODE, default="TEXT"): cv.enum(text.TEXT_MODES, upper=True),
                }
            ),
            cv.Required(CONF_TIMER_SELECT): select.select_schema(TimerSelect),
            cv.Required(CONF_SAVE_BUTTON): button.button_schema(TimerSaveButton),
            cv.Required(CONF_DISABLE_SWITCH): switch.switch_schema(TimerDisableSwitch),
            cv.Required(CONF_OUTPUTS): cv.ensure_list(OUTPUT_SCHEMA),
            cv.Optional(CONF_NAMES): cv.ensure_list(cv.string),
        }
    ),
    validate_names,
)

async def to_code(config):
    pass

