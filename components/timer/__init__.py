import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import select, switch, text, time
from esphome.const import (
    CONF_ID,
    CONF_MODE,
    CONF_OUTPUTS,
    CONF_THEN,
    CONF_TIME_ID,
    CONF_TRIGGER_ID,
    CONF_TYPE,
)

AUTO_LOAD = [ "select", "switch", "text" ]

"""
timer:
  quantity: 3
  text_input:
    name: "Timer Configuration"
  timer_select:
    name: "Select Timer"
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
Timer = timer_ns.class_("Timer", cg.Component)
TimerText = timer_ns.class_("TimerText", text.Text, cg.Component)
TimerSelect = timer_ns.class_("TimerSelect", select.Select, cg.Component)
TimerDisableSwitch = timer_ns.class_("TimerDisableSwitch", switch.Switch, cg.Component)
OutputTrigger = timer_ns.class_(
    "OutputTrigger", automation.Trigger.template(cg.float_)
)


CONF_QUANTITY = "quantity"
CONF_TEXT_INPUT = "text_input"
CONF_TIMER_SELECT = "timer_select"
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
        CONF_AUTOMATION: automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(automation.Trigger.template(cg.float_)),
            },
            single=True,
        ),
    }
)

CONFIG_SCHEMA = cv.All(
    cv.COMPONENT_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(Timer),
            cv.GenerateID(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
            cv.Required(CONF_QUANTITY): cv.positive_int,
            cv.Required(CONF_TEXT_INPUT): text.TEXT_SCHEMA.extend(
                {
                    cv.GenerateID(): cv.declare_id(TimerText),
                    cv.Optional(CONF_MODE, default="TEXT"): cv.enum(text.TEXT_MODES, upper=True),
                }
            ),
            cv.Required(CONF_TIMER_SELECT): select.select_schema(TimerSelect),
            cv.Required(CONF_DISABLE_SWITCH): switch.switch_schema(TimerDisableSwitch),
            cv.Required(CONF_OUTPUTS): cv.ensure_list(OUTPUT_SCHEMA),
            cv.Optional(CONF_NAMES): cv.ensure_list(cv.string),
        }
    ),
    validate_names,
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    time_ = await cg.get_variable(config[CONF_TIME_ID])
    cg.add(var.set_time(time_))
    numTimers = config[CONF_QUANTITY]
    cg.add(var.set_num_timers(numTimers));

    if CONF_NAMES in config:
        names = config[CONF_NAMES]
    else:
        names = [f'Timer{n + 1}' for n in range(numTimers)]

    if CONF_TEXT_INPUT in config:
        txt = await text.new_text(config[CONF_TEXT_INPUT])
        cg.add(var.set_timer_text(txt))

    if CONF_TIMER_SELECT in config:
        sel = await select.new_select(config[CONF_TIMER_SELECT], options=names)
        cg.add(var.set_timer_select(sel))

    for conf in config[CONF_OUTPUTS]:
        if conf[CONF_TYPE] == CONF_SWITCH:
            sw = await cg.get_variable(conf[CONF_ID])
            cg.add(var.add_switch_output(sw))
        else:
            trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
            await automation.build_automation(trigger, [(cg.float_, "action")], conf)
            cg.add(var.add_automation_output(trigger))

