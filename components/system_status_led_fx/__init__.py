# components/system_status_led_fx/__init__.py
import esphome.codegen as cg
import esphome.config_validation as cv

CODEOWNERS = ["@Flo-R1der"]

CONFIG_SCHEMA = cv.Schema({})

async def to_code(config):
    cg.add_global(cg.RawStatement('#include "esphome/components/system_status_led_fx/system_status_led_effect.h"'))
    