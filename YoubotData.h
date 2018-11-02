#ifndef YOUBOT_DATA_H
#define  YOUBOT_DATA_H

#ifdef WEBOTS_APPLICATION
double ArmAngleReadyPosition[5] = {170.571341,70.579036,-139.999496,10.138310,170.064502};
double ArmAngleFoetalPosition[5] = { 0.579808, 0.579808, -0.9,1.26761,6.33803};

//Mine

//Premier cube
double ArmPositionBackPlateMid[5]= {0.0,0.92,0.42,1.78,0.0};
double ArmPositionReadyToGripMid[5] = {-0.029775, 0.838603, 0.652011, 1.372149, 0.045185};



//Position to drop
double ArmPositionDrop[5] = {-0.009837,0.011039,-0.779790,-1.264426,0.059525};
double ArmPositionDropLeft[5] = {0.324167, -0.286438, -0.425846, -1.210001, -0.00000};
double ArmPositionDropRight[5] = {-0.314867, -0.286438, -0.425846,-1.210001, -0.00000 };

//Deuxième cube
double ArmPositionBackPlateLeft[5]={-0.294342,0.735480,0.611345,1.611970,0.008079};
double ArmPositionReadyToGripLeft[5] = {-0.294342,0.836343,0.611338,1.611969,0.008079};

//TroisièmeCube
double ArmPositionBackPlateRight[5]={0.280208,0.731633,0.621347,1.611970,0.494565};
double ArmPositionReadyToGripRight[5] = {0.304899, 0.990205, 0.299547, 1.765083, 0.371668};

#endif

#ifdef REELYOUBOT_APPLICATION   
double ArmAngleReadyPosition[5] = {170.571341,70.579036,-139.999496,10.138310,170.064502};
double ArmAngleFoetalPosition[5] = { 0.579808, 0.579808, -0.9,1.26761,6.33803};
double ArmPositionCalibrate[5]= {1005.000000,1005.000000,1005.000000,1005.000000,5005.000000};
double ArmPositionReady[5]= {302335,126052,181068,23004,81462} ;

#endif                     
                      
#endif
