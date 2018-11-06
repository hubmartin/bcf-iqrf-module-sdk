#include <application.h>

// LED instance
bc_led_t led;

// Button instance
bc_button_t button;

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    if (event == BC_BUTTON_EVENT_PRESS)
    {
        bc_led_set_mode(&led, BC_LED_MODE_TOGGLE);
    }

    // Logging in action
    bc_log_info("Button event handler - event: %i", event);
}

void iqrf_event_handler_climate(bc_module_iqrf_t *self, bc_module_iqrf_event_t event, void *param)
{
    uint8_t *pResponseData = self->dpa_message->Response.PData;
    uint8_t request_bit_mask;

    switch(event)
    {
        case BC_MODULE_IQRF_EVENT_PERIPHERAL_REQUEST:
            bc_log_debug("BC_MODULE_IQRF_EVENT_PERIPHERAL_REQUEST");
            // We implement 1 user peripheral
            self->dpa_message->EnumPeripheralsAnswer.UserPerNr = 1;
            FlagUserPer( self->dpa_message->EnumPeripheralsAnswer.UserPer, PNUM_STD_SENSORS );
            self->dpa_message->EnumPeripheralsAnswer.HWPID = HWPID_HARDWARIO_PRESENSCE_SENSOR;
            self->dpa_message->EnumPeripheralsAnswer.HWPIDver = 0xABCD;

            // Return the enumeration structure but do not modify _DpaDataLength
            self->return_data_length = sizeof( self->dpa_message->EnumPeripheralsAnswer );
            // Return TRUE
            self->return_flags = EVENT_RETURN_TRUE;
            break;

        case BC_MODULE_IQRF_EVENT_PERIPHERAL_INFO_REQUEST:
            bc_log_debug("BC_MODULE_IQRF_EVENT_PERIPHERAL_INFO_REQUEST");
            self->dpa_message->PeripheralInfoAnswer.PerT = PERIPHERAL_TYPE_STD_SENSORS;
            self->dpa_message->PeripheralInfoAnswer.PerTE = PERIPHERAL_TYPE_EXTENDED_READ_WRITE;
            // Set standard version
            self->dpa_message->PeripheralInfoAnswer.Par1 = 13;

            // Return the information structure but do not modify _DpaDataLength
            self->return_data_length = sizeof( self->dpa_message->PeripheralInfoAnswer );
            // Return TRUE
            self->return_flags = EVENT_RETURN_TRUE;
            break;

        case BC_MODULE_IQRF_EVENT_PCMD_STD_ENUMERATE:
            bc_log_debug("BC_MODULE_IQRF_EVENT_PCMD_STD_ENUMERATE");
            // 1st byte is sensor type
            self->dpa_message->Response.PData[0] = STD_SENSOR_TYPE_TEMPERATURE;
            self->dpa_message->Response.PData[1] = STD_SENSOR_TYPE_HUMIDITY;
            // Return just one sensor type
            self->return_data_length = *self->dpa_data_length = sizeof( self->dpa_message->Response.PData[0] );
            // Return TRUE
            self->return_flags = EVENT_RETURN_TRUE;
            break;

        case BC_MODULE_IQRF_EVENT_PCMD_STD_SENSORS_READ_VALUES:
            bc_log_debug("BC_MODULE_IQRF_EVENT_PCMD_STD_SENSORS_READ_VALUES");

            request_bit_mask = self->dpa_message->Request.PData[0];

            // Is my only sensor selected?
            if (request_bit_mask & 0x01)
            {
                uint32_t val = 1234;

                // Return sensor data
                *pResponseData++ = (val >> 0) & 0xFF;
                *pResponseData++ = (val >> 8) & 0xFF;
            }

            if (request_bit_mask & 0x02)
            {
                uint32_t val = 38 * 2;

                // Return sensor data
                *pResponseData++ = (val >> 0) & 0xFF;
            }
            // Returned data length
            self->return_data_length = *self->dpa_data_length = ( pResponseData - self->dpa_message->Response.PData );
            // Return TRUE
            self->return_flags = EVENT_RETURN_TRUE;
            break;

        case BC_MODULE_IQRF_EVENT_PCMD_STD_SENSORS_READ_TYPES_AND_VALUES:
            bc_log_debug("BC_MODULE_IQRF_EVENT_PCMD_STD_SENSORS_READ_TYPES_AND_VALUES");
            // Pointer to the response data
            request_bit_mask = self->dpa_message->Request.PData[0];
            // Is my only sensor selected?
            if (request_bit_mask & 0x01)
            {
                // Return also sensor type?
                *pResponseData++ = STD_SENSOR_TYPE_TEMPERATURE;

                volatile float float_val = 37.2f;

                int16_t integer = (int16_t)(float_val * 16);

                // Return sensor data
                *pResponseData++ = (integer >> 0) & 0xFF;
                *pResponseData++ = (integer >> 8) & 0xFF;

            }

            if (request_bit_mask & 0x02)
            {
                // Return also sensor type?
                *pResponseData++ = STD_SENSOR_TYPE_HUMIDITY;

                uint32_t val = 38 * 2;

                // Return sensor data
                *pResponseData++ = (val >> 0) & 0xFF;

            }

            // Returned data length
            self->return_data_length = *self->dpa_data_length = ( pResponseData - self->dpa_message->Response.PData );
            // Return TRUE
            self->return_flags = EVENT_RETURN_TRUE;
            break;

        default:
            break;
    }
}



void iqrf_event_handler(bc_module_iqrf_t *self, bc_module_iqrf_event_t event, void *param)
{
    uint8_t *pResponseData = self->dpa_message->Response.PData;

    switch(event)
    {
        case BC_MODULE_IQRF_EVENT_PERIPHERAL_REQUEST:
            bc_log_debug("BC_MODULE_IQRF_EVENT_PERIPHERAL_REQUEST");
            // We implement 1 user peripheral
            self->dpa_message->EnumPeripheralsAnswer.UserPerNr = 1;
            FlagUserPer( self->dpa_message->EnumPeripheralsAnswer.UserPer, PNUM_STD_SENSORS );
            self->dpa_message->EnumPeripheralsAnswer.HWPID = HWPID_HARDWARIO_PRESENSCE_SENSOR;
            self->dpa_message->EnumPeripheralsAnswer.HWPIDver = 0xABCD;

            // Return the enumeration structure but do not modify _DpaDataLength
            self->return_data_length = sizeof( self->dpa_message->EnumPeripheralsAnswer );
            // Return TRUE
            self->return_flags = EVENT_RETURN_TRUE;
            break;

        case BC_MODULE_IQRF_EVENT_PERIPHERAL_INFO_REQUEST:
            bc_log_debug("BC_MODULE_IQRF_EVENT_PERIPHERAL_INFO_REQUEST");
            self->dpa_message->PeripheralInfoAnswer.PerT = PERIPHERAL_TYPE_STD_SENSORS;
            self->dpa_message->PeripheralInfoAnswer.PerTE = PERIPHERAL_TYPE_EXTENDED_READ_WRITE;
            // Set standard version
            self->dpa_message->PeripheralInfoAnswer.Par1 = 13;

            // Return the information structure but do not modify _DpaDataLength
            self->return_data_length = sizeof( self->dpa_message->PeripheralInfoAnswer );
            // Return TRUE
            self->return_flags = EVENT_RETURN_TRUE;
            break;

        case BC_MODULE_IQRF_EVENT_PCMD_STD_ENUMERATE:
            bc_log_debug("BC_MODULE_IQRF_EVENT_PCMD_STD_ENUMERATE");
            // 1st byte is sensor type
            self->dpa_message->Response.PData[0] = STD_SENSOR_TYPE_BINARYDATA30; //STD_SENSOR_TYPE_BINARYDATA7;
            // Return just one sensor type
            self->return_data_length = *self->dpa_data_length = sizeof( self->dpa_message->Response.PData[0] );
            // Return TRUE
            self->return_flags = EVENT_RETURN_TRUE;
            break;

        case BC_MODULE_IQRF_EVENT_PCMD_STD_SENSORS_READ_VALUES:
            bc_log_debug("BC_MODULE_IQRF_EVENT_PCMD_STD_SENSORS_READ_VALUES");
            // Is my only sensor selected?
            if ( ( self->dpa_message->Request.PData[0] & 0x01 ) != 0 )
            {
                uint32_t val = 1234;

                // Return sensor data
                *pResponseData++ = (val >> 0) & 0xFF;
                *pResponseData++ = (val >> 8) & 0xFF;
                *pResponseData++ = (val >> 16) & 0xFF;
                *pResponseData++ = (val >> 24) & 0xFF;
            }
            // Returned data length
            self->return_data_length = *self->dpa_data_length = ( pResponseData - self->dpa_message->Response.PData );
            // Return TRUE
            self->return_flags = EVENT_RETURN_TRUE;
            break;

        case BC_MODULE_IQRF_EVENT_PCMD_STD_SENSORS_READ_TYPES_AND_VALUES:
            bc_log_debug("BC_MODULE_IQRF_EVENT_PCMD_STD_SENSORS_READ_TYPES_AND_VALUES");
            // Pointer to the response data
            //uint8_t *pResponseData2 = self->dpa_message->Response.PData;
            // Is my only sensor selected?
            if ( ( self->dpa_message->Request.PData[0] & 0x01 ) != 0 )
            {
                // Return also sensor type?
                *pResponseData++ = STD_SENSOR_TYPE_BINARYDATA30;

                uint32_t val = 1234;

                // Return sensor data
                *pResponseData++ = (val >> 0) & 0xFF;
                *pResponseData++ = (val >> 8) & 0xFF;
                *pResponseData++ = (val >> 16) & 0xFF;
                *pResponseData++ = (val >> 24) & 0xFF;

            }

            // Returned data length
            self->return_data_length = *self->dpa_data_length = ( pResponseData - self->dpa_message->Response.PData );
            // Return TRUE
            self->return_flags = EVENT_RETURN_TRUE;
            break;

        default:
            break;
    }
}

void application_init(void)
{
    // Initialize logging
    bc_log_init(BC_LOG_LEVEL_DUMP, BC_LOG_TIMESTAMP_ABS);

    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_ON);

    // Initialize button
    //bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    //bc_button_set_event_handler(&button, button_event_handler, NULL);

    bc_module_iqrf_init();
    bc_module_iqrf_set_event_handler(iqrf_event_handler_climate, NULL);

}

void application_task(void)
{
    // Logging in action
    //bc_log_debug("application_task run");

    // Plan next run this function after 1000 ms
    //bc_scheduler_plan_current_from_now(1000);
}
