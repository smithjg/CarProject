#include <Arduino.h>
#include "CommandMapping.h"

typedef struct 
{
    uint8_t drive_value;
    unsigned long command_reading;
}mapping_t;



#define MAX_COMMANDED_VALUE 2005
#define MIN_COMMANDED_VALUE 980

#define MAX_DRIVE_VALUE 40
#define MIN_DRIVE_VALUE 0


#define MAPPING_SECTORS (MAX_DRIVE_VALUE-MIN_DRIVE_VALUE)

#define MAPPING_PER_COMMAND_VALUE(x) (((x)/(MAX_COMMANDED_VALUE-MIN_COMMANDED_VALUE))*(MAPPING_SECTORS))

#define mapping_t_Default {(uint8_t)0,(unsigned long)0}

/// possibly to be used for a non linear result ?
static mapping_t current_result[MAPPING_SECTORS] = {
    {MAPPING_PER_COMMAND_VALUE(MIN_DRIVE_VALUE)},
    {MAPPING_PER_COMMAND_VALUE(MAX_DRIVE_VALUE)}    
    };

static uint8_t given_command_value_return_drive_value(unsigned long comanded_value)
{
    //uint8_t ret_val = (((comanded_value-MIN_DRIVE_VALUE)*MAPPING_SECTORS)/(MAX_COMMANDED_VALUE-MIN_COMMANDED_VALUE)) ;


    return 0;
}


unsigned long map_command_value_from_pwm_in(unsigned long pwmRead )
{
    return (unsigned long)given_command_value_return_drive_value(pwmRead);
}