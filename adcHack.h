/*********************************************************************
 * adcHack Class. This class is intended to allow the Raspberry Pi, with 
 * the aid of a few external components to perform analog to digital
 * conversion. The class is capable of performing 10-bit ADC conversions
 * with a sample rate of a bit over 700 sps as well as 8-bit ADC conversion
 * at a plausibly faster sample rate.
 *
 * The adcHack Class utilizes the PWM1 peripheral as a PWM digital to analog 
 * convertor (DAC) that outputs a PWM signal on GPIO18. This PWM signal is 
 * converted to an analog value with the aid of a passive RC low pass filter
 * (R=1kohm an C = 10nF). This analog value is then routed to the negative 
 * input terminal of a comparator chip, whereas the analog signal to be measured is 
 * routed to the positive input terminal of the comparator chip.
 *
 * 
 * Since we are using a PWM DAC, the ADC's reference voltage is the same as the PWM's 
 * maximum  signal level i.e. 3.3v. We will also assume that we are using the 10bit 
 * version of the ADC. The adcHack class uses the PWM DAC to generate analog voltages 
 * representing the value for bit 9 = 512 Digital value = 512/1023*3.3v = 1.65V at the 
 * comparator's negative terminal. If the analog input is larger than the analog value 
 * for bit 9, bit 9 is set in an accumulator else its cleared in an accumulator. The PWM 
 * DAC then generates an analog voltage representing bit 8 + the value in the accumulator. 
 * If the analog input is larger that the analog value of bit 8 + accumulator then bit 8 is 
 * set in the accumulator and so forth. By the time we finish evaluating bit zero, the 
 * value in the accumulator is the digital representation of the analog input. This is known 
 * as the successive approximation algorithm. The comparator is used to compare the various analog 
 * voltages this output must be connected to a re-assignable GPIO input pin. This GPIO pin is 
 * controlled by the mmapGpio class whereas the PWM1 Peripheral is controlled by the rpiPWM1 class.
 *
 *                                                ___ 3.3V
 *                                                 |
 *                                                 |
 *                                                 \
 *                                     |\          /
 *__ Analog Voltage in_________________|+\         \ 10Kohm Pull-up Resistor
 *                                     |  \        /
 *                                     |   \_______\___________________________GPIO4(can be re-assigned in the adcHack class)
 *                      1Kohm          |   /
 * __RPI_GPIO18________/\/\/\__________|- /
 *                              |      | / Comparator (LM311)
 *                            -----    |/
 *                       10nF -----
 *                              |
 *                              |
 *                            ----- GND
 *                             ---
 *                              -   
 *
 * The rpiPWM1 class configures the PWM frequency to 9370 Hz for 10-bit resolution and 37500 for 
 * 8-bit resolution. In both cases the rpiPWM1::PWMMODE is used. This has the effect of distributing 
 * the ON time across the period making the average effective frequency of the PWM waveforms be over 
 * 3MHz in both cases. The RC filter has a cutoff frequency of 1/2*Pi*R*C =15.913KHz. This cutoff 
 * frequency is much lower than the effective PWM frequency (approx 3MHz). The settling time for the
 * RC filter is 5*R*C + comparator settling time = 50.2uS. The class offers the user the ability to 
 * finetune the built-in settling time compensation delay. Making it shorter than the default, will 
 * result in faster conversion at the cost of accuracy.
 *
 **********************************************************************************/
#ifndef ADCHACK_H
    #define ADCHACK_H
#include "mmapGpio.h"
#include "rpiPWM1.h"
#include <stdio.h>
#include <math.h>
class adcHack{

public:
	adcHack();// default constructor 10-bit resolution Comparator output mapped to GPIO4
    adcHack(unsigned int pinnum);// overloaded constructor 1. 10-bit resolution, Comparator output can be reassigned to 'pinnum'
	adcHack(unsigned int pinnum, const int &resolution);// overloaded constructor. Let's user choose resolution & Comparator pin
	unsigned int adcRead();// This method performs the ADC conversion
    int setResolution(const int &resolution); // setter for setting/changing the resolution between adcHack::ADC_RES_8 & adcHack::ADC_RES_10
    void setSettlingTimeUs(unsigned int settle);// setter for changing the settling time. Default value is 55
    
    unsigned int getSettlingTime() const {return sett;}// getters for settling time and adcresolution
    int getAdcResolution() const {return res;} 
    
    static int const ADC_RES_8 = 0; // const indicating 8-bit resolution
    static int const ADC_RES_10 = 1; // const indicating 10-bit resolution
    static int const ERR_RES = 1; // error code for setting erroneous resolution value

private:
	mmapGpio inFromComparator; // mmapGpio object to control GPIO used connected to comparator output
	rpiPWM1 dac; // rpiPWM1 object to control the PWM1 peripheral
    int res; // resolution variable
    unsigned int sett; // settling time variable
};
#endif
