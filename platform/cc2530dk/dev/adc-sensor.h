/*
 * Copyright (c) 2011, George Oikonomou - <oikonomou@users.sourceforge.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *         Header file for ADC sensors on the SmartRF05EB.
 *
 *         Sensors will be off by default, unless turned on explicitly
 *         in contiki-conf.h
 *
 * \author
 *         George Oikonomou - <oikonomou@users.sourceforge.net>
 */

#ifndef __ADC_SENSOR_H__
#define __ADC_SENSOR_H__

#include "cc253x.h"
#include "contiki-conf.h"
#include "lib/sensors.h"

/*_____________IoTClinic1_________________
defines for eternal input for ADC conversation*/
#define ADC_EXT_1_PIN      0
#define ADC_EXT_2_PIN	   1
#define ADC_EXT_3_PIN	   6
#define ADC_EXT_4_PIN	   7
#define ADC_EXT_BATT_PIN	4

#define ADC_EXT_1		(1<<ADC_EXT_1_PIN)
#define ADC_EXT_2		(1<<ADC_EXT_2_PIN)
#define ADC_EXT_3		(1<<ADC_EXT_3_PIN)
#define ADC_EXT_4		(1<<ADC_EXT_4_PIN)
#define ADC_EXT_BATT	        (1<<ADC_EXT_BATT_PIN)

#define ADC_TEMP		0x100
#define ADC_VDDC		0x200
//_____________IoTClinic1_________________



/* ADC Sensor Types */
#define ADC_SENSOR "ADC"

#define ADC_SENSOR_TYPE_TEMP    0
#define ADC_SENSOR_TYPE_VDD     4
/*_____________IoTClinic2_________________
*/
#define ADC_SENSOR_TYPE_EXT_1	5
#define ADC_SENSOR_TYPE_EXT_2	6
#define ADC_SENSOR_TYPE_EXT_3	7   // dont use
#define ADC_SENSOR_TYPE_EXT_4	8   // dont use
#define ADC_SENSOR_TYPE_EXT_BATT	9
//_____________IoTClinic2_________________
#ifdef ADC_SENSOR_CONF_ON
#define ADC_SENSOR_ON ADC_SENSOR_CONF_ON
#endif /* ADC_SENSOR_CONF_ON */

#if ADC_SENSOR_ON
extern const struct sensors_sensor adc_sensor;
#define   ADC_SENSOR_ACTIVATE() adc_sensor.configure(SENSORS_ACTIVE, 1)
/*_____________IoTClinic3_________________
*/
#define VDDC_SENSOR_ACTIVATE()	     adc_sensor_exteral_activate(ADC_VDDC)
#define VDDC_SENSOR_DEACTIVATE()     adc_sensor_exteral_deactivate(ADC_VDDC)
#define VDDC_SENSOR_GET()	     get_adc_value(ADC_SENSOR_TYPE_VDD)

#define TEMP_SENSOR_ACTIVATE()       adc_sensor_exteral_activate(ADC_TEMP)
#define TEMP_SENSOR_DEACTIVATE()     adc_sensor_exteral_deactivate(ADC_TEMP)
#define TEMP_SENSOR_GET()            get_adc_value(ADC_SENSOR_TYPE_TEMP)

#define BATT_SENSOR_ACTIVATE()    adc_sensor_exteral_activate(ADC_EXT_BATT)
#define BATT_SENSOR_DEACTIVATE() adc_sensor_exteral_deactivate(ADC_EXT_BATT)
#define BATT_SENSOR_GET()         get_adc_value(ADC_SENSOR_TYPE_EXT_BATT)

#define EXT1_SENSOR_ACTIVATE()   adc_sensor_exteral_activate(ADC_EXT_1)
#define EXT1_SENSOR_DEACTIVATE() adc_sensor_exteral_deactivate(ADC_EXT_1)
#define EXT1_SENSOR_GET()        get_adc_value(ADC_SENSOR_TYPE_EXT_1)

#define EXT2_SENSOR_ACTIVATE() adc_sensor_exteral_activate(ADC_EXT_2)
#define EXT2_SENSOR_DEACTIVATE() adc_sensor_exteral_deactivate(ADC_EXT_2)
#define EXT2_SENSOR_GET()       get_adc_value(ADC_SENSOR_TYPE_EXT_2)
void adc_sensor_exteral_activate ( unsigned int );
void adc_sensor_exteral_deactivate ( unsigned int );
float get_adc_value(unsigned int);

//_____________IoTClinic3_________________
#else
#define   ADC_SENSOR_ACTIVATE()
#endif /* ADC_SENSOR_ON */

/* Battery - SmartRF stuff */
#ifdef BATTERY_SENSOR_CONF_ON
#define BATTERY_SENSOR_ON BATTERY_SENSOR_CONF_ON
#endif /* BATTERY_SENSOR_CONF_ON */

/* Temperature - Available on all devices */
#ifdef TEMP_SENSOR_CONF_ON
#define TEMP_SENSOR_ON TEMP_SENSOR_CONF_ON
#endif /* TEMP_SENSOR_CONF_ON */

/* Supply Voltage (VDD / 3) - Available on all devices*/
#ifdef VDD_SENSOR_CONF_ON
#define VDD_SENSOR_ON VDD_SENSOR_CONF_ON
#endif /* VDD_SENSOR_CONF_ON */

#endif /* __ADC_SENSOR_H__ */
