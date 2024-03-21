/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "servo.h"

int leftServo = 17;
int rightServo = 16;
int left_trig = 5;
int left_echo = 6;
int right_trig = 3;
int right_echo = 4;
int left_widget = 0;
int right_widget = 0;
int red_led = 8;
int yellow_led = 7;
int green_led = 9;
int intersection = 0;
int counter = 0;
int direction = 0;
int finished_scan;
int loop_complete = 0;

void moveF();
void moveL();
void moveR();
void moveR_90();
void moveL_90();
void moveLeft_90();
void rightPath();
void final();

int main()                                    // Main function
{
  // Add startup code here.
  low(12);
  while (loop_complete < 4) 
  {
    lineFollower(30); 
    if(intersection == 1 && counter == 1)
    {
      high(12);
      high(red_led);
      servo_angle(leftServo, 970);
      servo_angle(rightServo, 970);
      pause(75);
      low(red_led);
      pause(10000);
      print("state = %d", input(14));
      if(input(15) == 1)
      {
        direction = 1;
      }
      else if(input(14) == 1)
      {
        direction = 2;
      }                 
    }      
    if(direction == 1)
    {
      rightPath(); 
    }   
    else if(direction == 2)
    {
      leftPath(); 
    }            
  }    
}


void moveF(int s) {
  servo_angle(leftServo, 970 + s);
  servo_angle(rightServo, 970 - s);                       
}  

void moveL_90() {
  int left_ir = input(2);
    while (left_ir ==1){
      int left_ir = input(2);
      if(left_ir == 1)
      {
        int left_ir = input(2);
        printf("HIIII");
        servo_angle(leftServo, 970);
        servo_angle(rightServo, 940);
      } 
      else
      {
        break;
      } 
    }                         
}  

void moveR_onTrack() {
  servo_angle(leftServo, 970);
  servo_angle(rightServo, 940);                       
}  

void moveR_90() {
  int right_ir = input(0);
  while (right_ir == 1){
    int right_ir = input(0);
    if(right_ir == 1)
    {
      int right_ir = input(0);
      servo_angle(leftServo, 1000);
      servo_angle(rightServo, 970);
    } 
    else
    {
      break;
     
    } 
  }    
}             
 
void moveL_onTrack() {  
  servo_angle(leftServo, 1000);
  servo_angle(rightServo, 970);                       
} 


void lineFollower(int speed){
  
  int right_ir = input(0);
  int center_ir = input(1);
  int left_ir = input(2);
    
  print("right_ir=%d\t",right_ir);
  print("center_ir=%d\t",center_ir);
  print("left_ir=%d\t",left_ir);
    
  if(right_ir ==0 && center_ir==1 && left_ir ==0 ) {                                                // right_ir: white, center_ir: black, left_ir: white
    moveF(speed);
    intersection = 0;
    print("intersection: %d\n" , intersection);
  }
  else if(right_ir ==1 && center_ir==1 && left_ir==0){
    moveL_onTrack();
    intersection = 0;
    print("intersection: %d\n" , intersection);
  }
  else if(right_ir ==1 && center_ir==0 && left_ir==0) {     
    moveL_onTrack();
    intersection = 0;
    print("intersection: %d\n" , intersection);
  }
  else if(right_ir ==0 && center_ir==1 && left_ir==1){
    moveR_onTrack();
    intersection = 0;
    print("intersection: %d\n" , intersection);
  }
  else if (right_ir ==0 && center_ir==0 && left_ir==1){
    moveR_onTrack();
    intersection = 0;
    print("intersection: %d\n" , intersection);
  }
  else if (right_ir ==1 && center_ir ==1 && left_ir ==1){
    intersection = 1;
    counter += 1;
    print("intersection: %d\n" , intersection);
    print("counter: %d\n" , counter);
  }
  else if (right_ir ==0 && center_ir==0 && left_ir==0){
     servo_angle(leftServo, 970);
     servo_angle(rightServo, 970);
     intersection = 0;
     print("intersection: %d\n" , intersection);
    }
}

int left_sensor(){
    low(left_trig);
    pulse_out(left_trig, 10);
    long tEcho = pulse_in(left_echo, 1);
    int cmDist = tEcho / 58;
    if (cmDist <= 12) {
      left_widget = 1;
    }
    else if (cmDist > 12) {
      left_widget = 0;
    }
    pause(20);
    return left_widget;  
}

int right_sensor(){
    low(right_trig);
    pulse_out(right_trig, 10);
    long tEcho = pulse_in(right_echo, 1);
    int cmDist = tEcho / 58;
    if (cmDist <= 12) {
      right_widget = 1;
    }
    else if (cmDist > 12) {
      right_widget = 0;
    }
    pause(20);
    return right_widget;  
}


void rightPath()
{
  
  if(intersection == 1 && counter == 1)
  {
    low(12);
    print("Lowering 12");
    moveR_90();
  }    
  else if(intersection == 1 && counter == 2)
  {
    moveL_90();
    int rw = 1;
    while (rw ==1){
      lineFollower(30);
      right_sensor();
      if (right_widget == 1)
      {
       moveF(30);
       pause(1000);
       servo_angle(leftServo, 1000);
       servo_angle(rightServo, 1000);
       pause(1400);
       servo_angle(leftServo, 970);
       servo_angle(rightServo, 970); 
       pause(1000);
       high(13);
       servo_angle(leftServo, 935);
       servo_angle(rightServo, 1005);
       pause(1400);
       servo_angle(leftServo, 970);
       servo_angle(rightServo, 970); 
       pause(3000);
       servo_angle(leftServo, 955);
       servo_angle(leftServo, 955);
       pause(1500);
       servo_angle(leftServo, 985);
       servo_angle(leftServo, 985);
       pause(750);
       servo_angle(leftServo, 970);
       servo_angle(rightServo, 970);
       pause(8000);
       servo_angle(leftServo, 1000);
       servo_angle(rightServo, 940);
       pause(1700);
       low(13);
       finished_scan = 1;
       break;
      }
    }
  }  
  
  else if (finished_scan == 1)
  {
     servo_angle(leftServo, 1000);
     servo_angle(rightServo, 1000);
     pause(1350);
     servo_angle(leftServo, 970);
     servo_angle(rightServo, 970);
     finished_scan = 0;
  }   
  else if(intersection == 1 && counter == 3)
  {
    moveR_90(); 
  }  
  else if(intersection == 1 && counter == 4)
  {
    moveF(30);
    pause(400); 
  } 
  else if(intersection == 1 && counter == 5)
  {
    moveR_90(); 
    int lw = 1;
    while (lw ==1){
      lineFollower(30);
      left_sensor();
      if (left_widget == 1)
      {
       moveF(30);
       pause(1000);
       servo_angle(leftServo, 940);
       servo_angle(rightServo, 940);
       pause(1400);
       servo_angle(leftServo, 970);
       servo_angle(rightServo, 970);
       pause(1000);
       high(13);
       servo_angle(leftServo, 935);
       servo_angle(rightServo, 1005);
       pause(1400);
       servo_angle(leftServo, 970);
       servo_angle(rightServo, 970); 
       pause(3000);
       servo_angle(leftServo, 985);
       servo_angle(leftServo, 985);
       pause(1500);
       servo_angle(leftServo, 955);
       servo_angle(leftServo, 955);
       pause(750);
       servo_angle(leftServo, 970);
       servo_angle(rightServo, 970);
       pause(8000);
       servo_angle(leftServo, 1000);
       servo_angle(rightServo, 940);
       pause(1700);
       low(13);
       finished_scan = 1;
       break;
      }
    } 
  }
  else if(intersection == 1 && counter == 6)
  {
    moveR_90(); 
  }
   else if(intersection == 1 && counter == 7)
  {
    high(red_led);
    pause(25);
    low(red_led);
    moveL_90(); 
    loop_complete += 1;
    counter = 0;
      if(loop_complete == 3 && counter == 0)
      {
        final(); 
      }        
  }      
}  


void leftPath()
{
  
  if(intersection == 1 && counter == 1)
  {
    low(12);
    print("lowered pin 12");
    moveL_90(); 
    
  }    
  else if(intersection == 1 && counter == 2)
  {
    moveR_90();
    int lw = 1;
    while (lw ==1){
      lineFollower(30);
      left_sensor();
      if (left_widget == 1)
      {
       moveF(30);
       pause(1000);
       servo_angle(leftServo, 940);
       servo_angle(rightServo, 940);
       pause(1400);
       servo_angle(leftServo, 970);
       servo_angle(rightServo, 970);
       pause(1000);
       high(13);
       servo_angle(leftServo, 935);
       servo_angle(rightServo, 1005);
       pause(1400);
       servo_angle(leftServo, 970);
       servo_angle(rightServo, 970); 
       pause(3000);
       servo_angle(leftServo, 985);
       servo_angle(leftServo, 985);
       pause(1500);
       servo_angle(leftServo, 955);
       servo_angle(leftServo, 955);
       pause(750);
       servo_angle(leftServo, 970);
       servo_angle(rightServo, 970);
       pause(8000);
       servo_angle(leftServo, 1000);
       servo_angle(rightServo, 940);
       pause(1700);
       low(13);
       finished_scan = 1;
       break;
      }
    }
  }  
  
  else if (finished_scan == 1)
  {
     servo_angle(leftServo, 940);
     servo_angle(rightServo, 940);
     pause(1200);
     servo_angle(leftServo, 970);
     servo_angle(rightServo, 970);
     finished_scan = 0;
  }   
  else if(intersection == 1 && counter == 3)
  {
    moveL_90(); 
  }  
  else if(intersection == 1 && counter == 4)
  {
    moveF(30);
    pause(400); 
  } 
  else if(intersection == 1 && counter == 5)
  {
    moveL_90(); 
    int rw = 1;
    while (rw ==1){
      lineFollower(30);
      right_sensor();
      if (right_widget == 1)
      {
       moveF(30);
       pause(1000);
       servo_angle(leftServo, 1000);
       servo_angle(rightServo, 1000);
       pause(1400);
       servo_angle(leftServo, 970);
       servo_angle(rightServo, 970); 
       pause(1000);
       high(13);
       servo_angle(leftServo, 935);
       servo_angle(rightServo, 1005);
       pause(1400);
       servo_angle(leftServo, 970);
       servo_angle(rightServo, 970); 
       pause(3000);
       servo_angle(leftServo, 955);
       servo_angle(leftServo, 955);
       pause(1500);
       servo_angle(leftServo, 985);
       servo_angle(leftServo, 985);
       pause(750);
       servo_angle(leftServo, 970);
       servo_angle(rightServo, 970);
       pause(8000);
       servo_angle(leftServo, 1000);
       servo_angle(rightServo, 940);
       pause(1700);
       low(13);
       finished_scan = 1;
       break;
      }
    } 
  }
  else if(intersection == 1 && counter == 6)
  {
    moveL_90(); 
  }
   else if(intersection == 1 && counter == 7)
  {
    high(red_led);
    pause(75);
    low(red_led);
    moveR_90(); 
    loop_complete += 1;
    counter = 0;
      if(loop_complete == 3 && counter == 0)
      {
        final(); 
      }
  }      
}

void final()
{
  int right_ir = input(0);
  int center_ir = input(1);
  int left_ir = input(2);
  while(input(0) == 1 || input(1) == 1 || input(2) == 1)
  {
    lineFollower(30);
  }    
  high(11);
  servo_angle(leftServo, 970);
  servo_angle(rightServo, 970);
  pause(10000);
  if(input(15) == 1)
  {
 //     pause(5000);
      while(input(10) == 0)
      {
        print("Rotating");
        servo_angle(leftServo, 980);
        servo_angle(rightServo, 980); 
      }             
      if(input(10) == 1)
      {
        while(input(0) == 0 && input(1) == 0 && input(2) == 0)
        {
          print("MoveF");
          moveF(20);
        }
        while(input(0) == 1 || input(1) == 1 || input(2) == 1)
        {
          servo_angle(leftServo, 970);
          servo_angle(rightServo, 970);
        }        
      }       
  }   
  
  
  else if(input(14) == 1)
  {
      pause(5000);
      while(input(10) == 0)
      {
        servo_angle(leftServo, 960);
        servo_angle(rightServo, 960);  
      }         
      if(input(10) == 1)
      {
        while(input(0) == 0 || input(1) == 0 || input(2) == 0)
        {
          moveF(20);
        }        
      }      
  } 
}