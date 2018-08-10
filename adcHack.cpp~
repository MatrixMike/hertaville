#include "adcHack.h"

//constants that indicate desired ADC resolution
const int 	adcHack::ADC_RES_8; // use 8-bit resolution
const int 	adcHack::ADC_RES_10; //use 10-bit resolution (default)

//Error return code. Returned by setResolution() when erroneous value is used
const int   adcHack::ERR_RES;

/*********************************************************************************
 * Default Constructor - Default ADCHack setup. 10-bit  resolution, 60us settling
 *                       time, comparator output attached to GPIO4  
 * Parameters & Return Value - None
 * *******************************************************************************/
adcHack::adcHack(){
	inFromComparator.setPinDir(4,mmapGpio::INPUT);
    res = ADC_RES_10;
	sett = 60;
    dac.setFrequency(9370.0);
	dac.setCounts(1024);
	dac.setDutyCycleCount(512);
	dac.setMode(rpiPWM1::PWMMODE);

}

/***********************************************************************************
 * Overloaded Constructor 1 - Allows user to assign the GPIO pin attached to comparator 
 *                            output. 10-bit Resolution, 60us settling time
 *
 * Parameters - pinnum - unsigned int - GPIO number
 *
 * Return Value - None
 * *********************************************************************************/
adcHack::adcHack(unsigned int pinnum){
	inFromComparator.setPinDir(pinnum,mmapGpio::INPUT);
    res = ADC_RES_10;
    sett = 60;
    dac.setFrequency(9370.0);
	dac.setCounts(1024);
	dac.setDutyCycleCount(512);
	dac.setMode(rpiPWM1::PWMMODE);
}

/************************************************************************************************
 * Overloaded Constructor 2 - Allows user to assign the GPIO pin attached to comparator 
 *                            output as well as the ADC resolution. 60us settling time
 *
 * Parameters - pinnum     - unsigned int - GPIO number
 *              resolution - int          - ADC resolution: set to adcHack::ADC_RES_8 for 8-bit
 *                                                          set to adcHack::ADC_RES_10 for 10-bit
 *
 * Return Value - None
 * ***********************************************************************************************/
adcHack::adcHack(unsigned int pinnum, const int &resolution){
	inFromComparator.setPinDir(pinnum,mmapGpio::INPUT);
    res = resolution;
    sett = 69;
    if(res == ADC_RES_8){
        dac.setFrequency(37500.0);
	    dac.setCounts(256);
	    dac.setDutyCycleCount(128);
	    dac.setMode(rpiPWM1::PWMMODE);
    }

    else{
        dac.setFrequency(9370.0);
	    dac.setCounts(1024);
	    dac.setDutyCycleCount(512);
	    dac.setMode(rpiPWM1::PWMMODE);
    }

}

/***********************************************************************************
 * adcRead() - Perform AD conversion. Implements the successive approximation
 *             algorithm
 *
 * Parameters & Return Value - None
 * *********************************************************************************/
unsigned int adcHack::adcRead(){
    unsigned int accum = 0;
    
    if(res == ADC_RES_8){
	    for(int i=7; i>=0; i--){
		    accum += (1 << i);
		    dac.setDutyCycleCount(accum);
		    usleep(sett);
		    if(inFromComparator.readPin(4) == 0){
			    accum -= (1 << i);
		    }
	    }
    }
    else{ 
	    for(int i=9; i>=0; i--){
		    accum += (1 << i);
		    dac.setDutyCycleCount(accum);
		    usleep(sett);
		    if(inFromComparator.readPin(4) == 0){
			    accum -= (1 << i);
		    }
	    }
    }

	return accum;
}


/***********************************************************************************
 * setResolution - Setter..sets resolution
 * Parameters    - resolution - int - ADC resolution: set to adcHack::ADC_RES_8 for 8-bit
 *                                                    set to adcHack::ADC_RES_10 for 10-bit
 *
 * Return Value - int denoting if a valid ADC resolution value was specified. If ADC 
 *                resolution was valid, returns 0 else adcHack::ERR_RES
 * *********************************************************************************/
int adcHack::setResolution(const int &resolution){
    int retVal = 0;
    if (resolution == ADC_RES_8 || resolution == ADC_RES_10)
        res = resolution;
    else
        retVal = ERR_RES;

    return retVal;
}

/*************************************************************************************
 * setSettlingTimeUS - sets settling time in microseconds
 * Parameters - settle - settling time in microseconds ideally should be varied between 
 *                       50us and 500us. 
 *
 * ***********************************************************************************/
void adcHack::setSettlingTimeUs(unsigned int settle){
    sett = settle;
}
