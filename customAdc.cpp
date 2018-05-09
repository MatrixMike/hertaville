#include "adcHack.h"

int main(void){
    int DigitalResult = 0;
	adcHack adc(4,adcHack::ADC_RES_10); // initialize adcHack to 10-bit resolution 
    // & Comparator output to GPIO4
	int i = 20;
    adc.setResolution(adcHack::ADC_RES_10);// set resolution to 10-bit (just testing...)
    adc.setSettlingTimeUs(200); // set settling time
	while(i > 0){

		DigitalResult = adc.adcRead(); // perform conversion
		usleep(100); // every 100us...you change this to whatever value you like...just an interval
		printf("Digital value is %d and analog value is %0.4lf volts. \n",DigitalResult, (DigitalResult/1024.0)*3.3);
		//print out result
        i--; 
	}
    printf("settling time was %d us\n",adc.getSettlingTime()); //use getter for testing!
	return 0;
}
