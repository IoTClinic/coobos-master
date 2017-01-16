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
 *         ADC sensor module for TI SmartRF05EB devices.
 *
 * \author
 *         George Oikonomou - <oikonomou@users.sourceforge.net>
 */
#include "sfr-bits.h"
#include "cc253x.h"
#include "adc-sensor.h"

#if ADC_SENSOR_ON

/*_____________IoTClinic1_________________
VDDC,TEMP,ADCP0-7,ADCP0-6,ADCP0-5,ADCP0-4,ADCP0-3,ADCP0-2,ADCP0-2,ADCP0-1,ADCP0-0*/
static int EnableFlag=0;
//_____________IoTClinic1_________________

/*---------------------------------------------------------------------------*/
/*_____________IoTClinic2_________________
*/
static int
value(int type)
{
  uint16_t reading;
  /*
   * For single-shot AD conversions, we may only write to ADCCON3[3:0] once
   * (This write triggers the conversion). We thus use the variable 'command'
   * to store intermediate steps (reference, decimation rate, input channel)
   */
  uint8_t command;

  /* 1.25V ref, max decimation rate */
  
  command = ADCCON3_EDIV1 | ADCCON3_EDIV0;

  /* Clear the Interrupt Flag */
  ADCIF = 0;

  /* Depending on the desired reading, append the input bits to 'command' and
   * enable the corresponding input channel in ADCCFG if necessary */
  switch(type) {

  case ADC_SENSOR_TYPE_TEMP:
    if(EnableFlag & ADC_TEMP)
      command |= ADCCON3_ECH3 | ADCCON3_ECH2 | ADCCON3_ECH1;
    else
      return -1;  
  break;

  case ADC_SENSOR_TYPE_VDD:
    if(EnableFlag & ADC_VDDC)    
      command |= ADCCON3_ECH3 | ADCCON3_ECH2 | ADCCON3_ECH1 | ADCCON3_ECH0;
    else
      return -1;
    break;


  case ADC_SENSOR_TYPE_EXT_1:
	if(EnableFlag & ADC_EXT_1)
	    command |= ADC_EXT_1_PIN ;
	else 
	    return -1;
    break;

  case ADC_SENSOR_TYPE_EXT_2:
 	if(EnableFlag & ADC_EXT_2)
	    command |= ADC_EXT_2_PIN ;
	else 
	    return -1;
    break;

  case ADC_SENSOR_TYPE_EXT_3:
 	if(EnableFlag & ADC_EXT_3)
	    command |= ADC_EXT_3_PIN ;
	else 
	    return -1;
    break;

  case ADC_SENSOR_TYPE_EXT_4:
    	if(EnableFlag & ADC_EXT_4)
	    command |= ADC_EXT_4_PIN ;
	else 
	    return -1;
    break;

  case ADC_SENSOR_TYPE_EXT_BATT:
    	if(EnableFlag & ADC_EXT_BATT)
	    command |= ADC_EXT_BATT_PIN ;
	else 
	    return -1;
    break;


  default:
    /* If the sensor is not present or disabled in conf, return -1 */
    return -1;
  }

  /* Writing in bits 3:0 of ADCCON3 will trigger a single conversion */
  ADCCON3 = command;

  /*
   * When the conversion is complete, the ADC interrupt flag is set. We don't
   * use an ISR here, we just wait on the flag and clear it afterwards.
   */
  while(!ADCIF);

  /* Clear the Interrupt Flag */
  ADCIF = 0;

  reading = ADCL;
  reading |= (((uint8_t) ADCH) << 8);
  /* 12-bit decimation rate: 4 LS bits are noise */
  reading >>= 4;

  return reading;
}
//_____________IoTClinic2_________________
/*---------------------------------------------------------------------------*/
static int
status(int type)
{
  return 1;
}
/*---------------------------------------------------------------------------*/
static int
configure(int type, int value)
{
  switch(type) {
  case SENSORS_HW_INIT:
#if TEMP_SENSOR_ON
    /* Connect temperature sensor to the SoC */
    ATEST = 1;
    TESTREG0 = 1;
#endif
    APCFG = 0; /* Disables Input Channels */
    break;
  }
  return 1;
}

/*_____________IoTClinic3_________________
*/
void adc_sensor_exteral_activate ( unsigned int pin)
{
 if(pin & 0xFF)
  APCFG |=pin;
 EnableFlag |=pin;
 

}
void adc_sensor_exteral_deactivate ( unsigned int pin)
{
 if(pin & 0xFF)
  APCFG &= ~(pin);
 EnableFlag &= ~pin;
}

float get_adc_value(unsigned int type)
{
  static struct sensors_sensor *sensor;
  int digitValue=0;
  float analogeValue=0;
  sensor = sensors_find(ADC_SENSOR); // return NULL if adc sensor not available
  if(sensor)
   {
    digitValue = sensor->value (type) ;
     if(digitValue != -1)
     {
	switch (type)
	   {
              case ADC_SENSOR_TYPE_TEMP:
	       /*
	       * Temperature:
	       * Using 1.25V ref. voltage (1250mV).
	       * Typical AD Output at 25°C: 1480
	       * Typical Co-efficient     : 4.5 mV/°C
	       *
	       * Thus, at 12bit decimation (and ignoring the VDD 
	       * co-efficient as well
	       * as offsets due to lack of calibration):
	       *
	       *          AD - 1480
	       * T = 25 + ---------
	       *              4.5
	       */
       	       analogeValue =(float) 25 + ((digitValue - 1480) / 4.5);
	       return analogeValue;
	      break; 
    
           case ADC_SENSOR_TYPE_VDD:	   
	      /*
	       * Power Supply Voltage.
	       * Using 1.25V ref. voltage.
	       * AD Conversion on VDD/3
	       *
	       * Thus, at 12bit resolution:
	       *
	       *          ADC x 1.25 x 3
	       * Supply = -------------- V
	       *               2047
	       */
	       analogeValue = (float) digitValue * 3.75 / 2047;
	       return analogeValue;
           break;
		
           case ADC_SENSOR_TYPE_EXT_BATT:
	      /*
	       * Battry Voltage.
	       * Using 1.25V ref. voltage.
	       * AD Conversion on VDD/3
	       *
	       * Thus, at 12bit resolution:
	       *
	       *          ADC x 1.25 x 3
	       * Supply = -------------- V
	       *               2047
	       */
	       analogeValue =  digitValue * 3.75 / 2047;
	       return analogeValue;
           break;

	   case ADC_SENSOR_TYPE_EXT_1:
	   case ADC_SENSOR_TYPE_EXT_2:
	   case ADC_SENSOR_TYPE_EXT_3:
	   case ADC_SENSOR_TYPE_EXT_4:
	      /*
	       * external Voltage.
	       * Using 1.25V ref. voltage.
	       * ADC EXTERNAL 
	       *
	       * Thus, at 12bit resolution:
	       *
	       *          ADC x 1.25 
	       * Supply = -------------- V
	       *               2047
	       */
           analogeValue =  digitValue * 1.25 / 2047;
	   return analogeValue;
	   break;
           }
     }
     else
       return -1;
   }
  else 
    return -1;
}

//_____________IoTClinic3_________________

/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(adc_sensor, ADC_SENSOR, value, configure, status);
#endif /* ADC_SENSOR_ON */
