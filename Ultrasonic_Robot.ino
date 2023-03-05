#include "VarSpeedServo.h"  
#include <NewPing.h>        

VarSpeedServo RU;  
VarSpeedServo RL;  
VarSpeedServo LU;  
VarSpeedServo LL;  

NewPing sonar(4,3,200); 
                                                    
const int vel = 20, vel_Back = 10;                   
const int delay_Forward = 750, delay_Back = 1000;    
                                                     
const int array_cal[4] = {90,90,90,90}; 
int RU_Degree = 0, LU_Degree = array_cal[2] + 5;

const int num1 = 6;
const int array_forward[num1][4] =  
{
    {0,-40,0,-20},        
    {30,-40,30,-20},
    {30,0,30,0},
    {0,20,0,40},
    {-30,20,-30,40},
    {-30,0,-30,0},
};

const int num2 = 5; 
const int array_turn[num2][4] =  
{     
    {-40,0,-20,0},
    {-40,30,-20,30},      
    {0,30,0,30},
    {30,0,30,0},
    {0,0,0,0},
};


#define RUN

void Servo_Init()
{
    RU.attach(9);   
    RL.attach(10);   
    LU.attach(11);    
    LL.attach(12);   
}

void Adjust()                            
{                                      
    for(RU_Degree = array_cal[0] - 5; RU_Degree <= array_cal[0]; RU_Degree += 1) {
        RU.write(RU_Degree);         
        LU.write(LU_Degree--);                
        delay(15);                    
    }
}

bool TooClose()
{
    int tooclose = 0;
    for(int a=0; a<5; a++) {  
        delay(50);
        int din = sonar.ping_in();
        if (din < 7 && din > 0) tooclose++;
    }
    if (tooclose < 5) return 1;   
    return 0;
}

void Forward()
{
    for(int x=0; x<num1; x++) {                    
        RU.slowmove (array_cal[0] + array_forward[x][0] , vel);    
        RL.slowmove (array_cal[1] + array_forward[x][1] , vel);
        LU.slowmove (array_cal[2] + array_forward[x][2] , vel);
        LL.slowmove (array_cal[3] + array_forward[x][3] , vel);
        delay(delay_Forward);
    }
}

void Backward()
{
    for(int z=0; z<4; z++) {    
        for(int y=0; y<num2; y++) {                  
            RU.slowmove (array_cal[0] + array_turn[y][0] , vel_Back);   
            RL.slowmove (array_cal[1] + array_turn[y][1] , vel_Back);
            LU.slowmove (array_cal[2] + array_turn[y][2] , vel_Back);
            LL.slowmove (array_cal[3] + array_turn[y][3] , vel_Back);
            delay(delay_Back); 
        }
    }
}

void setup()  
{
#ifdef INSTALL
    Servo_Init();
  
    RU.slowmove (90 , vel);
    RL.slowmove (90 , vel);
    LU.slowmove (90 , vel);
    LL.slowmove (90 , vel);
    while(1);
#endif

#ifdef CALIBRATION 
    Servo_Init();  
    Adjust();
    
    RL.slowmove (array_cal[1] , vel);
    LL.slowmove (array_cal[3] , vel);
    delay(2000);
    while(1);
#endif

#ifdef RUN 
    Servo_Init();
    Adjust(); 
       
    RL.slowmove (array_cal[1] , vel);
    LL.slowmove (array_cal[3] , vel);
    delay(2000);
#endif
}

void loop() 
{ 
    while(TooClose()) Forward();    
    Backward();    
}
