#include "terasic_os.h"
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "CSpider.h"
#include "CSpiderLeg.h"
#include "CMotor.h"
#include "BtSppCommand.h"
#include "QueueCommand.h"
#include "PIO_LED.h"
#include "PIO_BUTTON.h"
#include "ADC.h"



int main(int argc, char *argv[]){

  printf("===== Group B Final Project =====\r\n");

	CSpider Spider;
	ADC adc;
	uint32_t sensorReading0 = 0;

	printf("Spider Init & Standup\r\n");
	if (!Spider.Init()){
		printf("Spilder Init failed\r\n");
	}else{
		if (!Spider.Standup())
			printf("Spilder Standup failed\r\n");
	}
	Spider.SetSpeed(50);

	Spider.WakeUp();
	//Consider keeping the 1100. Jim and Cole tested this value and it seems to work well.
	while(sensorReading0 <= 1100) {
	  //printf("Spider.MoveForward\n");
	  
	  Spider.MoveBackward(1);
	  for (int i = 0; i < 50; i++) { 
		sensorReading0 = adc.GetChannel(0);
		printf("Ch0 Sensor Reading: %u\r\n", sensorReading0);
	  }
	  
	  printf("Spider.Reset()\n");
	  Spider.Reset();
	}
  // Uncomment to use the ADC class for reading IR sensor
	// 
	// while(1) {
		// sensorReading0 = adc.GetChannel(0);
		// printf("Ch0 Sensor Reading: %u\r\n", sensorReading0);
	// }
	return 0;
}
