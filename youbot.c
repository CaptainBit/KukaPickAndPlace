/*
 * File:          youbot.c
 * Date:          October 30th 2018
 * Description:   Find the cube and place the three dices on the box
 * Author:        Isaac Fiset et Justin Roberge-Lavoie
 * Modifications: 
 */

#include "YoubotWebotsAllInclude.h"


void PickAndPlaceCube(YouBotManipulator* MyYouBotManipulator, YouBotBase* MyYouBotBase );
void FindCube (YouBotManipulator* MyYouBotManipulator, YouBotBase* MyYouBotBase);
void GetInPosition(YouBotManipulator* MyYouBotManipulator, YouBotBase* MyYouBotBase, float transversale);
int RobotInFrontFace(float* pData);
float FindNearDistance(float* pData);
int FindNearPoint(float* pData);

int main(int argc, char **argv)
{

 // Arm, base and gripper initialization
 YouBotBase *MyYouBotBase=0;
 reelYB_Init(MyYouBotBase);
 MyYouBotBase=reelYB_BaseInit();
 if(MyYouBotBase==0)
   return -1;
     
 YouBotManipulator *MyYouBotManipulator=0;
 MyYouBotManipulator=reelYB_ArmInit();
 if(MyYouBotManipulator==0)
     return -1;
  
 reelYB_GripperInit(MyYouBotManipulator);
  
 
  
 printf("Init done!\n");
 passive_wait(2.0);
 ///////////////////////////////////////////////////
 
 
 //Trouver le cube et se rapprocher de celui-ci
 FindCube(MyYouBotManipulator,MyYouBotBase);
 
 //Move close and rototate if needed 
 GetInPosition(MyYouBotManipulator, MyYouBotBase, 40.0f);
 
 //Pick dices and place them on top of the box
 PickAndPlaceCube(MyYouBotManipulator,MyYouBotBase );
 
 reelYB_ExitBase(MyYouBotBase);
 reelYB_ExitArm(MyYouBotManipulator);

  return 0;
}


//Function to pick and place the objects when the robot is in position
void PickAndPlaceCube(YouBotManipulator* MyYouBotManipulator, YouBotBase* MyYouBotBase)
{
  

  
  //Place to mid Plate
 reelYB_ArmSetAngle(MyYouBotManipulator,ArmPositionBackPlateMid, false);

 //Ungrip
 gripper_set_gap(0.05);
 
 //Set position to grip Mid
 reelYB_ArmSetAngle(MyYouBotManipulator,ArmPositionReadyToGripMid, false);
 
 passive_wait(2.0);
 
 //Grip 
 gripper_grip();
 
 passive_wait(2.0);
 
//LiftToLeft
 reelYB_ArmSetAngle(MyYouBotManipulator,ArmPositionDrop, false);
 
 passive_wait(2.0);
 
 //Ungrip
 gripper_set_gap(0.05);
 
 passive_wait(2.0);
 
 
 //Move to LeftPlate
 reelYB_ArmSetAngle(MyYouBotManipulator, ArmPositionBackPlateLeft, false);
 
 passive_wait(2.0);
  
  //Set position to grip Left
 reelYB_ArmSetAngle(MyYouBotManipulator,ArmPositionReadyToGripLeft, false);
 
 passive_wait(2.0);
  
  //Grip 
 gripper_grip();
 
 passive_wait(2.0);
  
 //LiftToLeft
 reelYB_ArmSetAngle(MyYouBotManipulator,ArmPositionDropLeft, false);
 
 passive_wait(3.0);
 
 //Ungrip
 gripper_set_gap(0.05);
 
 passive_wait(2.0);
 
 //Move to RightPlate
 reelYB_ArmSetAngle(MyYouBotManipulator, ArmPositionBackPlateRight, false);
  
  passive_wait(2.0);
  
  //Set position to grip Left
 reelYB_ArmSetAngle(MyYouBotManipulator,ArmPositionReadyToGripRight, false);
 
 passive_wait(2.0);
  
  //Grip 
 gripper_grip();
  
 passive_wait(2.0);
 
 //LiftToRight
 reelYB_ArmSetAngle(MyYouBotManipulator,ArmPositionDropRight, false);
 
 passive_wait(3.0);
 
 //Ungrip
 gripper_set_gap(0.05);
}

void FindCube (YouBotManipulator* MyYouBotManipulator, YouBotBase* MyYouBotBase)
{

 OpenLaser();
 
  //Data for laser
 float pData[LASERSIZE];
 
 GetLaserData(pData, LASERMINANGLE, LASERMAXANGLE);
 
 //Find the nearest point from the box
 int index = FindNearPoint(pData);
 
 //if doesnt find anything..
 if(index == LASERSIZE)
 {
   printf("No object found..Rotating...\n");
   //Rotating
   reelYB_MoveBaseAngular(MyYouBotBase, 50, 5, false,0);
   passive_wait(0.5);
   //Close the laser
   CloseLaser(); 
   //Recall the func
   FindCube(MyYouBotManipulator, MyYouBotBase);
   return;
   
 }else
 {
    //Angle of the box
   float delta = (index-85)*0.36;
   printf("%3.6f\n", delta);
   
   //if the box is left or right
   if(index < 340)
   {
     //Find the angle to rotate
     delta = 90 - delta;
     
     reelYB_MoveBaseAngular(MyYouBotBase, -delta, 5, false,0);
     passive_wait(0.5);
     
   }else
   {
     //Find the angle to rotate
     delta = delta - 90;
    
     reelYB_MoveBaseAngular(MyYouBotBase, delta, 5, false,0);
     passive_wait(0.5);
     
   }
   //Nearest distance of the box
   float distance = FindNearDistance(pData);
   printf("%3.6f\n", distance);
   
   passive_wait(0.5);
   
   //Move a little bit
   reelYB_MoveBaseLongitudinal(MyYouBotBase, (distance * 10), 10,false,0);
   passive_wait(0.5);
   CloseLaser();  
   if(distance > 0.4)
   {
     //if the cube isnt enough near....
     FindCube(MyYouBotManipulator, MyYouBotBase);
     
   } 
 }
 
 
 
 
}


//Find the most little distance
float FindNearDistance(float* pData)
{
  float distance = 99;
  int index = 0;
  while(index < LASERSIZE - 1)
  {
    if(pData[index] != 0.0f && pData[index] != 5.60f)
    {
      if(pData[index] < distance){
         distance = pData[index];
      }
    }
    index++;
    
  }
  return distance;
}


//Find the index of the most little
int FindNearPoint(float* pData)
{
  float distance = 99;
  int index = 0;
  int point = LASERSIZE;
  while(index < LASERSIZE - 1)
  {
    if(pData[index] != 0.0f && pData[index] != 5.60f)
    {
      if(pData[index] < distance){
         distance = pData[index];
         point = index;
      }
    }
    index++;
    
  }
  return point;
}




void GetInPosition(YouBotManipulator* MyYouBotManipulator, YouBotBase* MyYouBotBase, float transversale)
{
  passive_wait(1.0);
  
  float distance;
  int indice;
  float delta;
  float pData[LASERSIZE];
  
  
  OpenLaser();
  
  passive_wait(0.5);
  
 
  GetLaserData(pData, LASERMINANGLE, LASERMAXANGLE);
  passive_wait(0.5);
  
   //Check if robot in front of a face of the cube
  int bFront = RobotInFrontFace(pData);
  
  //if its not a face -> rotating to the left...
  if(bFront == 0){
  
   printf("rotating\n");
   
   //Before rotating.. move to the left
   reelYB_MoveBaseTransversal(MyYouBotBase, -transversale , 5, true , 0);
   passive_wait(0.5);
   
   GetLaserData(pData, LASERMINANGLE, LASERMAXANGLE);
   passive_wait(0.5);
   
   //Find the angle of the nearest angle
   indice = FindNearPoint(pData);
   delta = (indice-85)*0.36;
   
   printf("%3.6f\n", delta);
   passive_wait(0.5);
   
   
   //While the angle is not between 85 and 95
   while(delta < 85 || delta > 95){ 
     
     //Move a little bit to the left
     reelYB_MoveBaseTransversal(MyYouBotBase, -5 , 8.0, true , 0);
     passive_wait(0.5);
     
     //Change angle of the robot
     reelYB_MoveBaseAngular(MyYouBotBase, 5, 8.0, false,0);
     passive_wait(0.5);
     
     GetLaserData(pData, LASERMINANGLE, LASERMAXANGLE);
     passive_wait(0.5);
     
     
     //Find the new angle of the robot
     indice = FindNearPoint(pData);
     delta = (indice-85)*0.36;
     printf("%3.6f\n", delta);
     
   }
   //Be sure that the robot is in good position
   bFront = RobotInFrontFace(pData);
   if(bFront == 0){
     CloseLaser();
     GetInPosition(MyYouBotManipulator, MyYouBotBase, transversale * 0.75);
     return;
   }else{
    if(bFront == 2){
      reelYB_MoveBaseTransversal(MyYouBotBase, 20 , 5, true , 0);
    }else{
     if(bFront == 3){
      reelYB_MoveBaseTransversal(MyYouBotBase, -20 , 5, true , 0);
     }
    }
   }
   
  }else{
    if(bFront == 2){
      reelYB_MoveBaseTransversal(MyYouBotBase, 20 , 5, true , 0);
    }else{
     if(bFront == 3){
      reelYB_MoveBaseTransversal(MyYouBotBase, -20 , 5, true , 0);
    }
   }
  }
  
  passive_wait(0.5);
  GetLaserData(pData, LASERMINANGLE, LASERMAXANGLE);
  
  passive_wait(0.5);
  distance = FindNearDistance(pData);
  
  //Get to the box 
  while(distance > 0.30){
  
   printf("%3.6f\n", distance);
   reelYB_MoveBaseLongitudinal(MyYouBotBase,distance *10 , 10,false,0);
   passive_wait(0.5);
   
   GetLaserData(pData, LASERMINANGLE, LASERMAXANGLE);
   passive_wait(0.5);
   
   distance = FindNearDistance(pData);
  } 
  reelYB_MoveBaseLongitudinal(MyYouBotBase, ((0.30 - distance)*10), 5,false,0);
  //Last move to the box
  passive_wait(0.5);
  printf("Last move\n", distance);
  reelYB_MoveBaseLongitudinal(MyYouBotBase, 6, 5,false,0);
  passive_wait(0.5);
  return;
  
}


//Func that find if the robot is in good position from a face of the cube
int RobotInFrontFace(float* pData)
{
  int indice = FindNearPoint(pData);
  
  //Difference between the center and +-10 degree
  float difPositive = pData[indice + 50] - pData[indice];
  float difNegative = pData[indice - 50] - pData[indice];
  
  printf("Center : %3.6f\n", pData[indice]);
  printf("Difference positive : %3.6f\n",difPositive );
  printf("Difference negative :%3.6f\n",difNegative);
  
  //Angle of the robot (Debug only)
  float delta = (indice-85)*0.36;
  printf("%3.6f\n", delta);
  
  //If the difference is little -> face
  if(difPositive < 0.1 && difNegative < 0.1 ){
    return 1;
  }
  else{
   if(difPositive < 0.05){
    return 2;
   }else{
    if(difNegative < 0.05){
     return 3;
    }else{
     return 0;
    }
   }
  }
}




