//
//  main.cpp
//  Final_P
//
//  Created by Longxiang Cui on 11/26/14.
//  Copyright (c) 2014 Longxiang Cui. All rights reserved.
//

// General model without physical detection
// Sensor value input from keyboard

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdlib.h>
//#include <unistd.h>
#include "mbed.h"
#include "HCSR04.h"




using namespace std;


//code that currently work 
DigitalOut myled1(LED1);
//DigitalOut myled2(LED2);
DigitalOut a_board(D11);
Serial pc(USBTX, USBRX);


int tBuf[50];

bool doorOpen(long doorD) {
    if ((doorD >= 50) || (doorD == -1)) {
        return 1;
    }
    return 0;
}

bool detecHuman(long distance) {
    if(distance < 80) {
        return 1;
    }
    return 0;
}



int main(int argc, const char * argv[]) {
    memset(tBuf, 0, 50);
    int start = 0;
    int end = 0;
    int count = 0;
    bool u_f;
    bool u_d;
    bool u_b;
    bool u_o;

    HCSR04 sensor(D3, D2);    // 1st sensor, detect door open or not
    HCSR04 sensor2(D9, D8);   // 2nd sensor, detect human at the front
    HCSR04 sensor3(D7, D6);   // 3rd sensor, detect human crossing door
    HCSR04 sensor4(D15, D14); // 4th sensor , detect human at the back


    pc.printf("hello world \r \n");



    while (1) {
        
        pc.printf("door open?\r \n");

        long doorD = sensor.distance();
        pc.printf("door open distance %d \r \n",doorD);
        u_o = doorOpen(doorD);
        if (u_o) {
            pc.printf("door is open, start detection\r \n");
        }
        



        //cin>>u_o;
        if (u_o == 1) {
            pc.printf("front sensor\r \n");
            long distanceF = sensor2.distance();
            pc.printf("front sensor distance %d \r \n",distanceF);
            u_f = detecHuman(distanceF);
            //cin>>u_f;
            if(u_f) {
                pc.printf("detected human at the front\r \n");
            }
            else {
                pc.printf("nobody at front\r \n");
            }


            pc.printf("door sensor");
            long distanceD = sensor3.distance();
            pc.printf("door sensor distance %d \r \n",distanceD);
            u_d = detecHuman(distanceD);
            //cin>>u_d;
            if(u_d) {
                pc.printf("detected human crossing\r \n");
            }
            else {
                pc.printf("nobody at door\r \n");
            }


            pc.printf("back sensor\r \n");
            long distanceB = sensor4.distance();
            pc.printf("back sensor distance %d \r \n",distanceB);
            u_b = detecHuman(distanceB);
            //cin>>u_b;
            if(u_b) {
                pc.printf("detected human at the back\r \n");
            }
            else {
                pc.printf("nobody at back\r \n");
            }

        }

        else {
            pc.printf("door is closed, continue next round\r \n");
            
            pc.printf("current count is %d \r \n",count);
            ++end;
            pc.printf("\r \n");
            pc.printf("\r \n");
            pc.printf("\r \n");
            
            wait_ms(10);
            continue;
        }
        
        
        if (end == 50) {
            end = 0;
            
        }
        
        if (end == start) {
            ++start;
        }
        tBuf[end] = 0;
        if (u_f == 1) { // front sensor
            tBuf[end] |= 1;
        }
        if (u_d == 1) { // detection sensor
            tBuf[end] |= 2;
        }
        if (u_b == 1) { // back sensor
            tBuf[end] |= 4;
        }
        
        //cout<<endl<<tBuf[end]<<endl<<endl;;
        
        if (tBuf[end] & 0x2) { // detected people
            pc.printf("detection mode\r \n");
            
            int bCount = 0;
            int nCount = 0;
            bool inFlag = 0;
            bool outFlag = 0;
            bool bFlag = 0;
            
            for (int i = end; bCount < 20; ++bCount) { // check previous condition
                if (i < 0) {
                    i = 49;
                }
                
                if(tBuf[i] & 0x1) { //cannot has two front reading.........
                    inFlag = 1;
                    if (tBuf[i] & 0x4) {
                        outFlag = 1;
                    }
                    break;
                }
                
                else if (tBuf[i] & 0x4) {
                    outFlag = 1;
                    if (tBuf[i] & 0x1) {
                        inFlag = 1;
                    }
                    break;
                }
                --i;
            }
            
            if (bCount >= 20) {
                pc.printf("ERROR: There's a ghost\r \n");
                continue;
            }
            
            while (nCount < 20) { // track the condition of next 2 seconds
                pc.printf("current count is %d \r \n",count);            
                ++end;
                pc.printf("\r \n");
                pc.printf("\r \n");
                pc.printf("\r \n");
                wait_ms(10);
                
                pc.printf("front sensor\r \n");
                long distanceF = sensor2.distance();
                pc.printf("front sensor distance %d \r \n",distanceF);
                u_f = detecHuman(distanceF);
                //cin>>u_f;
                if(u_f) {
                    pc.printf("detected human in the front\r \n");
                }
                else {
                    pc.printf("nobody at front\r \n");
                }


                pc.printf("door sensor\r \n");
                long distanceD = sensor3.distance();
                pc.printf("door sensor distance %d \r \n",distanceD);
                u_d = detecHuman(distanceD);
                //cin>>u_d;
                if(u_d) {
                    pc.printf("detected human crossing\r \n");
                }
                else {
                    pc.printf("nobody at door\r \n");
                }

                pc.printf("back sensor\r \n");
                long distanceB = sensor4.distance();
                pc.printf("back sensor distance %d \r \n",distanceB);
                u_b = detecHuman(distanceB);
                //cin>>u_b;
                if(u_b) {
                    pc.printf("detected human in the back\r \n");
                }
                else {
                    pc.printf("nobody at back\r \n");
                }


                if (end == 50) {
                    end = 0;
                    
                }
                
                if (end == start) {
                    ++start;
                }
                tBuf[end] = 0;
                
                if (u_b == 1) { // back sensor
                    tBuf[end] |= 4;
                }
                
                if (u_f == 1) { // front sensor
                    tBuf[end] |= 1;
                }
                
                
                if (u_d == 1) { // detection sensor
                    tBuf[end] |= 2;
                }
                
                if (inFlag == 1 && (tBuf[end] & 0x4)) { // person coming in
                    count++;
                    pc.printf("one person comes in\r \n");
                    pc.printf("current count is %d \r \n",count);
                    break;
                }
                
                else if (outFlag == 1 && (tBuf[end] & 0x1)) { // person going out
                    count--;
                    pc.printf("one person goes out\r \n");
                    pc.printf("current count is %d \r \n",count);
                    break;
                }
                
                else if (tBuf[end] & 0x2) { // person wait at the door
                    nCount++;
                }
                else { // person didn't go through
                    break;
                }
            }
            
        }
        
        pc.printf("current count is %d \r \n",count);
        ++end;
        pc.printf("\r \n");
        pc.printf("\r \n");
        pc.printf("\r \n");
        wait_ms(10);
        
        
    }
    
}
 


