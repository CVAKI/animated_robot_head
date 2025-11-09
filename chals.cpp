#include <Servo.h>
#include <NewPing.h>

const int eye_led = 13;
const int eye_mot_1= 2;//__MOT
const int eye_mot_2 = 11;//__MOT
const int eye_lid = 3;//_MOT
const int Mouth = 4;//_MOT
const int up_neck_left= 5;//_MOT
const int up_neck_right = 6;//_MOT
const int Side_neck= 7;//__MOT
const int ultra_trigg = 9;
const int ultra_echo = 10;
const int sound_left = A0;
const int sound_right = A1;

          Servo eye1, eye2,  eye_shut,  mouth, jaw_neck1, jaw_neck2, side_neck;
          NewPing sonar(ultra_trigg,ultra_echo,500);
          
          int pc =0;
          int speeed=30;
          int read_left=0,read_right=0;
          int position_degree;
          int alpha_left =528;
          int alpha_right =530;
          int intialposition =90;
          int min_position=60;
          int max_position=120;
          int timer =0;
          int thershold_l=0;
          int thershold_r=0;
          int cashe=90;

void setup()
  {
    eye1.attach(eye_mot_1); 
    eye2.attach(eye_mot_2); 
    eye_shut.attach(eye_lid);
    mouth.attach(Mouth);
    Serial.begin(9600);
    side_neck.attach(Side_neck);
    pinMode(eye_led,OUTPUT);
    pinMode(ultra_trigg,OUTPUT);
    pinMode(ultra_echo,INPUT);
  
  }
  //MAIN_()
void loop()
   {
       timer++;
       digitalWrite(eye_led,HIGH); //ROBOT__BOOT()
       pc=boot(pc);

       read_left= analogRead(sound_left);
       read_right= analogRead(sound_right);


      if (thershold_l<read_left)
          thershold_l=read_left;
      if (thershold_r<read_right)
          thershold_r = read_right;


       int sound_diff = read_left - read_right;
       int servo_position = intialposition + map(sound_diff,-100,100,-15,15);
           position_degree = constrain(servo_position,min_position,max_position);
           eye_motion(position_degree);




         if(thershold_l > alpha_left)
          {
                  eye_motion(min_position+10);
                  delay(1000);
                  cashe = normalization_neck(position_degree,speeed,cashe);     //  left neck work
                  thershold_l=0;
          }    

          if(thershold_r > alpha_right)
          {
                  eye_motion(max_position-10);
                  delay(1000);
                  cashe = normalization_neck(position_degree,speeed,cashe);      // right neck work
                  thershold_r=0;
          }  

      
       
       if (timer%250 == 0){eye_shut_motion(0); delay (500); eye_shut_motion(1); timer=0;}
       delay(50);


        
      
       
   }
//_END()
    int boot (int flag)
    {
      if(flag==0)
      {
         eye_shut.write(0);
         delay(500);
         eye_shut.write(90);
         delay(500);
         eye_shut.write(0);
         delay(500);
         eye_shut.write(90);
         delay(1000); 
          eye1.write(60);
          eye2.write(60);
          delay(1000);
        
          eye1.write(120);
          eye2.write(120);
          delay(1000);
         
          eye1.write(90);
          eye2.write(90);       
          return 1;          
      }
      return 1;
    }


      void eye_motion(int degree)
    {
      
         if (degree >= 60 && degree <= 120) 
      {
   
         eye1.write(degree);
         eye2.write(degree);

      } 
      else
      {
        Serial.println("ignored__??????");
      }

   }
 

  void eye_shut_motion(int choise)
 {
    
      if (choise==1)// open
     {    
      eye_shut.write(90);
      digitalWrite(eye_led, HIGH);
     } 
      else if(choise==0)// close
     {
      eye_shut.write(0);
      digitalWrite(eye_led, LOW);
     }
     else{pc++;}//=========================fals_data
  }

    void side_neck_motion(int degree)
  {
    if (degree >= 30 && degree <= 150) {
        side_neck.write(degree);
        if(degree>90)
        {Serial.print("object_decteed_on-RIGHT----{ ");}
          else        
        {Serial.print("object_decteed_on-LEFT-------{ ");}
  } 
    else 
  {
    Serial.println("TRASHED---&&");
  }

 
}


 
  int normalization_neck(int degree, int val, int temp)
  {
   int speedvalue=val;
   int lb,ub,eye,neck;
              if (degree >= 60 && degree <= 120)
           {

                  if (degree<90)
                  {
                       for (neck=120;neck>=degree;neck--)   //positive 90---60      1
                     {
                        side_neck.write(neck);
                        Serial.println("controll__1");
                        delay(speedvalue);
                      }
                  }


                  else if(degree>90)
                  {
                  
                         for (neck=90;neck<=degree;neck++)   //positive 90---120      2
                        {
                           side_neck.write(neck);
                           Serial.println("controll__2");
                           delay(speedvalue);
                        }
                  }


                  else if(degree==90)//==========================================================================================   3
                  {
                  
                        if (temp>90)
                      {
                            for (int i=temp+1;i<=90;i++)   //positive 90---60        3.1
                            {
                            side_neck.write(i);
                             Serial.println("controll__3.1");
                            delay(speedvalue);
                            }
                       }


                         else if(temp<90)
                       {
                  
                            for (int i=temp;neck>=90;i--)   //positive 90---120     3.2
                           {
                             side_neck.write(i);
                              Serial.println("controll__3.2");
                             delay(speedvalue);
                            }
                       }
                         
                  }
                  
              }        return degree;
        
   }


  int ultra(NewPing obj)
  {
     return ( obj.ping_cm());
  }
