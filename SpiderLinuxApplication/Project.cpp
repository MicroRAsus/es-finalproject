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
#include <stack>

using namespace std;

void turnLeft(stack<bool>& rotationStack, CSpider Spider) { 
	Spider.RotatelLeft(6);
	rotationStack.push(true); //turn left push true
	printf("inside stack has a %d\n", (int)rotationStack.top());
}

// void turnRight(stack<bool> rotationStack, CSpider Spider) {
	// Spider.RotatelRight(6);
	// rotationStack.pop(); 
// }

int main(int argc, char *argv[]){

	printf("===== Group B Final Project =====\r\n");

	CSpider Spider;
	ADC adc;
	stack<bool> rotationStack;
	uint32_t sensorReading0 = 0;
	int counter = 0;

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
	while(1) {
		//Move forward until sensor reading
		while(sensorReading0 <= 1100 && rotationStack.empty()) {
			printf("Im in Loop 1\n");
			Spider.MoveBackward(1);
			for (int i = 0; i < 100; i++) { 
				sensorReading0 = adc.GetChannel(0);
				printf("Ch0 Sensor Reading: %u\r\n", sensorReading0);
			}
		  
			printf("Spider.Reset() Loop 1\n");
			Spider.Reset();
		}
		
		while (sensorReading0 <= 1100) {
			printf("Im in Loop 2\n");
			while(!rotationStack.empty() && counter < 50000) {
				printf("Attemping to move backwards\n");
				Spider.MoveBackward(1);
				printf("Finished moving backwards\n");
				for (int i = 0; i < 100; i++) { 
					sensorReading0 = adc.GetChannel(0);
					printf("Ch0 Sensor Reading: %u\r\n", sensorReading0);
				}

				printf("Spider.Reset() Loop 2\n");
				Spider.Reset();
				counter++;
			}
			Spider.RotatelRight(6);
			for (int i = 0; i < 100; i++) { 
				sensorReading0 = adc.GetChannel(0);
				printf("Ch0 Sensor Reading: %u\r\n", sensorReading0);
			}
			
			if (sensorReading0 <= 1100) {
				rotationStack.pop();
				counter = 0;
				break;
			}
			else {
				turnLeft(rotationStack, Spider);
				for (int i = 0; i < 100; i++) { 
					sensorReading0 = adc.GetChannel(0);
					printf("Ch0 Sensor Reading: %u\r\n", sensorReading0);
				}
			}
		}			

		//Rotate left if reading still high
		while (sensorReading0 > 1100) {
			printf("Im in Loop 3\n");
			turnLeft(rotationStack, Spider);
			printf("stack has a %d\n", (int)rotationStack.top());
			for (int i = 0; i < 100; i++) { 
				sensorReading0 = adc.GetChannel(0);
				printf("Ch0 Sensor Reading: %u\r\n", sensorReading0);
			}
			Spider.Reset();
			printf("Spider.Reset() Loop 3\n");
		}
		printf("end of all loop\n");
	}
	
	return 0;
}
