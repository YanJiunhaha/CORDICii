﻿
//
//  main.cpp
//  CORDICii
//
//  Created by 顏君翰 on 2017/9/27.
//  Copyright © 2017年 顏君翰. All rights reserved.
//
#include <stdio.h>
#include<stdlib.h>

#define Left false
#define Right true

const int Trivial_rotation[3][2] = { 0,1,-1,0,0,-1 };
const int Friend_Angle[2][2] = { 24,7,20,15 };
const int USR_CORDIC[2] = { 128,16 };
const int CORDIC[3] = { 32,64,128 };
const int Nano_CORDIC = 1024;
const int Nano_CORDIC_add = 8192;

//rotate c+js
void rotator(double &x, double &y, double c, double s, double r, bool dir) {
	double new_x, new_y;
	if (dir) {
		new_x = c*x + s*y;
		new_y = c*y - s*x;
	}
	else {
		new_x = c*x - s*y;
		new_y = c*y + s*x;
	}
	x = new_x / r; y = new_y / r;

}

int main(int argc, const char * argv[]) {
	double x = 100, y = 200;
	double SumAngle = 0, RAngle = 0;
	printf("stage0:%f,%f,%f,%f\n", x, y, RAngle, SumAngle);
	//stage1 trivial
	double x_temp = (x>0) ? x : 0 - x, y_temp = (y>0) ? y : 0 - y;
	bool xbiggery = (x_temp>y_temp) ? true : false;
	if (x>0) {
		if (y>0) {//第一象限
			if (!xbiggery) { rotator(x, y, Trivial_rotation[0][0], Trivial_rotation[0][1], 1, Right); RAngle = 90; }
		}
		else {//第四象限
			if (!xbiggery) { rotator(x, y, Trivial_rotation[2][0], Trivial_rotation[2][1], 1, Right); RAngle = 270; }
		}
	}
	else {
		if (y>0) {//第二象限
			if (xbiggery) { rotator(x, y, Trivial_rotation[1][0], Trivial_rotation[1][1], 1, Right); RAngle = 180; }
			else { rotator(x, y, Trivial_rotation[0][0], Trivial_rotation[0][1], 1, Right); RAngle = 90; }
		}
		else {//第三象限
			if (xbiggery) { rotator(x, y, Trivial_rotation[1][0], Trivial_rotation[1][1], 1, Right); RAngle = 180; }
			else { rotator(x, y, Trivial_rotation[2][0], Trivial_rotation[2][1], 1, Right); RAngle = 270; }
		}
	}SumAngle += RAngle; 
	printf("stage1:%f,%f,%f,%f\n", x, y, RAngle, SumAngle);RAngle = 0;
	//stage2 friend angle
	if (y > 0) {
		if (y / (20 / x) > 15) {
			rotator(x, y, Friend_Angle[1][0], Friend_Angle[1][1], 25, Right); RAngle = 36.8698976577;
			SumAngle += RAngle;
			printf("stage2:%f,%f,%f,%f\n", x, y, RAngle, SumAngle); RAngle = 0;
		}
		if (y / (24 / x) > 7) {
			rotator(x, y, Friend_Angle[0][0], Friend_Angle[0][1], 25, Right); RAngle = 16.2602046964;
			SumAngle += RAngle;
			printf("stage2:%f,%f,%f,%f\n", x, y, RAngle, SumAngle); RAngle = 0;
		}
	}
	else {
		y_temp = 0 - y;
		if (y_temp / (20 / x) > 15) {
			rotator(x, y, Friend_Angle[1][0], Friend_Angle[1][1], 25, Left); RAngle = -36.8698976577;
			SumAngle += RAngle;
			printf("stage2:%f,%f,%f,%f\n", x, y, RAngle, SumAngle); RAngle = 0;
		}
		y_temp = 0 - y;
		if (y_temp / (24 / x) > 7) {
			rotator(x, y, Friend_Angle[0][0], Friend_Angle[0][1], 25, Left); RAngle = -16.2602046964;
			SumAngle += RAngle;
			printf("stage2:%f,%f,%f,%f\n", x, y, RAngle, SumAngle); RAngle = 0;
		}
	}
	
	//stage3 USR CORDIC
	if (y>0) {
		rotator(x, y, USR_CORDIC[0], USR_CORDIC[1], 129, Right); RAngle = 7.12501637487;
	}
	else {
		rotator(x, y, USR_CORDIC[0], USR_CORDIC[1], 129, Left); RAngle = -7.12501637487;
	}SumAngle += RAngle; 
	printf("stage3:%f,%f,%f,%f\n", x, y, RAngle, SumAngle);RAngle = 0;

	//stage4 CORDIC
	if (y>0) {
		rotator(x, y, CORDIC[0], 1, 32, Right); RAngle = 1.78991060651;
	}
	else {
		rotator(x, y, CORDIC[0], 1, 32, Left); RAngle = -1.78991060651;
	}SumAngle += RAngle; 
	printf("stage4:%f,%f,%f,%f\n", x, y, RAngle, SumAngle);RAngle = 0;

	//stage5 CORDIC
	if (y>0) {
		rotator(x, y, CORDIC[1], 1, 64, Right); RAngle = 0.89517370903;
	}
	else {
		rotator(x, y, CORDIC[1], 1, 64, Left); RAngle = -0.89517370903;
	}SumAngle += RAngle; 
	printf("stage5:%f,%f,%f,%f\n", x, y, RAngle, SumAngle);RAngle = 0;

	//stage6 CORDIC
	if (y>0) {
		rotator(x, y, CORDIC[2], 1, 128, Right); RAngle = 0.44761417085;
	}
	else {
		rotator(x, y, CORDIC[2], 1, 128, Left); RAngle = -0.44761417085;
	}SumAngle += RAngle; 
	printf("stage6:%f,%f,%f,%f\n", x, y, RAngle, SumAngle);RAngle = 0;
	//stage7 nano Rotation
	for (int i = 8; i>0; i--) {
		if (y>0) {
			rotator(x, y, Nano_CORDIC, i, 1024, Right); RAngle = i*0.05595289191;
		}
		else {
			rotator(x, y, Nano_CORDIC, i, 1024, Left); RAngle = -i*0.05595289191;
		}SumAngle += RAngle; RAngle = 0;
	}
	printf("Angle=%f,R=%f\n", SumAngle, x);

	//stage8 nano Rotation
	for (int i = 8; i>0; i--) {
		if (y>0) {
			rotator(x, y, Nano_CORDIC_add, i, 8192, Right); RAngle = i*0.00699411368;
		}
		else {
			rotator(x, y, Nano_CORDIC_add, i, 8192, Left); RAngle = -i*0.00699411368;
		}SumAngle += RAngle; RAngle = 0;
	}
	printf("Angle=%f,R=%f\n", SumAngle, x);
	system("pause");
	return 0;
}