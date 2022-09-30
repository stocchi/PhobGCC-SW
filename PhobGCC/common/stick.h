#ifndef STICKCAL_H
#define STICKCAL_H



#include <curveFitting.h>
#include <eigen.h>
#include <Eigen/LU>
#include <ADC.h>
#include <VREF.h>

#include "structsAndEnums.h"
#include "filter.h"

ADC *adc = new ADC();

//////values used for calibration
const int _noOfNotches = 16;
const int _noOfCalibrationPoints = _noOfNotches * 2;
const int _noOfAdjNotches = 12;
float _ADCScale = 1;
float _ADCScaleFactor = 1;
const float _maxStickAngle = 0.4886921906;//28 degrees; this is the max angular deflection of the stick.

//origin values, useful for writing readable stick positions
const int _intOrigin = 127;
const float _floatOrigin = 127.5;

bool _calAStick = true; //determines which stick is being calibrated (if false then calibrate the c-stick)
//                                                         right                     notch 1                   up right                  notch 2                   up                        notch 3                   up left                   notch 4                   left                      notch 5                   down left                 notch 6                   down                      notch 7                   down right                notch 8
//                                                         0            1            2            3            4            5            6            7            8            9            10           11           12           13           14           15           16           17           18           19           20           21           22           23           24           25           26           27           28           29           30           31

const float _defaultCalPointsX[_noOfCalibrationPoints] =  {0.3010610568,0.3603937084,0.3010903951,0.3000194135,0.3005567843,0.3471911134,0.3006904343,0.3009976295,0.3000800899,0.300985051, 0.3001020858,0.300852804, 0.3008746305,0.2548450139,0.3001434092,0.3012600593,0.3011594091,0.2400535218,0.3014621077,0.3011248469,0.3010860944,0.2552106305,0.3002197989,0.3001679513,0.3004438517,0.300486505, 0.3002766984,0.3012828579,0.3014959877,0.346512936, 0.3013398149,0.3007809916};
const float _defaultCalPointsY[_noOfCalibrationPoints] =  {0.300092277, 0.3003803475,0.3002205792,0.301004752, 0.3001241394,0.3464200104,0.3001331245,0.3011881186,0.3010685972,0.3606900641,0.3001520488,0.3010662947,0.3008837105,0.3461478452,0.3011732026,0.3007367683,0.3011345742,0.3000566197,0.3006843288,0.3009673425,0.3011228978,0.2547579852,0.3011177285,0.301264851, 0.3002376991,0.2403885431,0.3006540818,0.3010588401,0.3011093054,0.2555000655,0.300080276, 0.3008482317};

//                                                         right        up          left          down         up right     up left      down left    down right   notch 1      notch 2      notch 3      notch 4      notch 5      notch 6      notch 7      notch 8
//                                                         right        notch 1      up right     notch 2      up           notch 3      up left      notch 4      left         notch 5      down left    notch 6      down         notch 7      down right   notch 8
//                                                         0            1            2            3            4            5            6            7            8            9            10           11           12           13           14           15
float _aNotchAngles[_noOfNotches] =                       {0,           M_PI/8.0,    M_PI*2/8.0,  M_PI*3/8.0,  M_PI*4/8.0,  M_PI*5/8.0,  M_PI*6/8.0,  M_PI*7/8.0,  M_PI*8/8.0,  M_PI*9/8.0,  M_PI*10/8.0, M_PI*11/8.0, M_PI*12/8.0, M_PI*13/8.0, M_PI*14/8.0, M_PI*15/8.0};
float _measuredNotchAngles[_noOfNotches];
int _aNotchStatus[_noOfNotches] =                         {3,           1,           2,           1,           3,           1,           2,           1,           3,           1,           2,           1,           3,           1,           2,           1};
int _cNotchStatus[_noOfNotches] =                         {3,           1,           2,           1,           3,           1,           2,           1,           3,           1,           2,           1,           3,           1,           2,           1};
float _cNotchAngles[_noOfNotches];

//Defaults
//                                                         right        notch 1      up right     notch 2      up           notch 3      up left      notch 4      left         notch 5      down left    notch 6      down         notch 7      down right   notch 8
//                                                         0            1            2            3            4            5            6            7            8            9            10           11           12           13           14           15
const int _calOrder[_noOfCalibrationPoints] =             {0, 1,        8, 9,       16, 17,       24, 25,      4, 5,        12, 13,      20, 21,      28, 29,      2, 3,        6, 7,        10, 11,      14, 15,      18, 19,      22, 23,      26, 27,      30, 31};
const float _notchAngleDefaults[_noOfNotches] =           {0,           M_PI/8.0,    M_PI*2/8.0,  M_PI*3/8.0,  M_PI*4/8.0,  M_PI*5/8.0,  M_PI*6/8.0,  M_PI*7/8.0,  M_PI*8/8.0,  M_PI*9/8.0,  M_PI*10/8.0, M_PI*11/8.0, M_PI*12/8.0, M_PI*13/8.0, M_PI*14/8.0, M_PI*15/8.0};
const int _notchStatusDefaults[_noOfNotches] =            {3,           1,           2,           1,           3,           1,           2,           1,           3,           1,           2,           1,           3,           1,           2,           1};
//                                                         up right     up left      down left    down right   notch 1      notch 2      notch 3      notch 4      notch 5      notch 6      notch 7      notch 8
const int _notchAdjOrder[_noOfAdjNotches] =               {2,           6,           10,          14,          1,           3,           5,           7,           9,           11,          13,          15};
const float _notchAdjustStretchLimit = 0.3;
const int _cardinalNotch = 3;
const int _secondaryNotch = 2;
const int _tertiaryNotchActive = 1;
const int _tertiaryNotchInactive = 0;
const int _fitOrder = 3; //fit order used in the linearization step

//these are the linearization coefficients
float _aFitCoeffsX[_fitOrder+1];
float _aFitCoeffsY[_fitOrder+1];
float _cFitCoeffsX[_fitOrder+1];
float _cFitCoeffsY[_fitOrder+1];

//these are the notch remap parameters
float _aAffineCoeffs[_noOfNotches][6]; //affine transformation coefficients for all regions of the a-stick
float _cAffineCoeffs[_noOfNotches][6]; //affine transformation coefficients for all regions of the c-stick
float _aBoundaryAngles[_noOfNotches]; //angles at the boundaries between regions of the a-stick (in the plane)
float _cBoundaryAngles[_noOfNotches]; //angles at the boundaries between regions of the c-stick (in the plane)

float linearize(float point, float coefficients[]){
	return (coefficients[0]*(point*point*point) + coefficients[1]*(point*point) + coefficients[2]*point + coefficients[3]);
};

void print_mtxf(const Eigen::MatrixXf& X){
	int i, j, nrow, ncol;
	nrow = X.rows();
	ncol = X.cols();
	Serial.print("nrow: "); Serial.println(nrow);
	Serial.print("ncol: "); Serial.println(ncol);
	Serial.println();
	for (i=0; i<nrow; i++)
	{
		for (j=0; j<ncol; j++)
		{
			Serial.print(X(i,j), 6);   // print 6 decimal places
			Serial.print(", ");
		}
		Serial.println();
	}
	Serial.println();
};

/*
 * calcStickValues computes the stick x/y coordinates from angle.
 * This requires weird trig because the stick moves spherically.
 */
void calcStickValues(float angle, float* x, float* y){
	*x = 100*atan2f((sinf(_maxStickAngle)*cosf(angle)),cosf(_maxStickAngle))/_maxStickAngle;
	*y = 100*atan2f((sinf(_maxStickAngle)*sinf(angle)),cosf(_maxStickAngle))/_maxStickAngle;
};

/*
 * Convert the x/y coordinates (actually angles on a sphere) to an azimuth
 * We first convert to a 3D coordinate and then drop to 2D, then arctan it
 * This does the opposite of calcStickValues, ideally.
 */
void angleOnSphere(const float x, const float y, float& angle){
	float xx = sinf(x*_maxStickAngle/100) * cosf(y*_maxStickAngle/100);
	float yy = cosf(x*_maxStickAngle/100) * sinf(y*_maxStickAngle/100);
	angle = atan2f(yy, xx);//WHY IS THIS BACKWARDS
	if(angle < 0){
		angle += 2*M_PI;
	}
};

/*
 * stripCalPoints removes the notches from un-cleaned cal points
 * this is so we can get the original values of the notches after the affine transform.
 * there need to be _noOfCalibrationPoints values in the inputs and outputs.
 */
void stripCalPoints(float calPointsX[], float calPointsY[], float strippedPointsX[], float strippedPointsY[]){
	for(int i=0; i < _noOfCalibrationPoints; i++){
		//start off by just copying them wholesale
		strippedPointsX[i] = calPointsX[i];
		strippedPointsY[i] = calPointsY[i];
		if((i+1)%4 == 0){//non-cardinal non-diagonal notch (every fourth starting at index 3)
			strippedPointsX[i] = calPointsX[0];//set equal to origin
			strippedPointsY[i] = calPointsY[0];
		}
	}
};

/*
 * computeStickAngles
 * write all the stick angles into the notch angles array array
 * inputs need to be length _noOfNotches+1
 * outputs need to be length _noOfNotches
 */
void computeStickAngles(float xInput[], float yInput[], float stickAngles[]){
	Serial.println("Computed stick angles:");
	for(int i=0; i < _noOfNotches; i++){
		if(i%2 == 0){//cardinal or diagonal
			stickAngles[i] = _notchAngleDefaults[i];
		} else {
			angleOnSphere(xInput[i+1], yInput[i+1], stickAngles[i]);
		}
	}
};
//sets notches to measured values if absent
void cleanNotches(float notchAngles[], float measuredNotchAngles[], int notchStatus[]){
	for(int i=0; i < _noOfNotches; i++){
		if(notchStatus[i] == _tertiaryNotchInactive){
			notchAngles[i] = measuredNotchAngles[i];
		}
	}
};

/*******************
	notchRemap
	Remaps the stick position using affine transforms generated from the notch positions
*******************/
void notchRemap(float xIn, float yIn, float* xOut, float* yOut, float affineCoeffs[][6], float regionAngles[], int regions){
	//determine the angle between the x unit vector and the current position vector
	float angle = atan2f(yIn,xIn);

	//unwrap the angle based on the first region boundary
	if(angle < regionAngles[0]){
		angle += M_PI*2;
	}

	//go through the region boundaries from lowest angle to highest, checking if the current position vector is in that region
	//if the region is not found then it must be between the first and the last boundary, ie the last region
	//we check GATE_REGIONS*2 because each notch has its own very small region we use to make notch values more consistent
	//int region = regions*2-1;
	int region = regions-1;
	for(int i = 1; i < regions; i++){
		if(angle < regionAngles[i]){
			region = i-1;
			break;
		}
	}

	//Apply the affine transformation using the coefficients found during calibration
	*xOut = affineCoeffs[region][0]*xIn + affineCoeffs[region][1]*yIn + affineCoeffs[region][2];
	*yOut = affineCoeffs[region][3]*xIn + affineCoeffs[region][4]*yIn + affineCoeffs[region][5];

	if((abs(*xOut)<5) && (abs(*yOut)>95)){
		*xOut = 0;
	}
	if((abs(*yOut)<5) && (abs(*xOut)>95)){
		*yOut = 0;
	}

	if((abs(*xOut)<3) && (abs(*yOut)<3)) {
		*xOut = 0;
		*yOut = 0;
	}
};

/*
 * transformCalPoints
 * remaps the cleaned calibration points from raw measurements to output coordinates
 * This seems redundant but we're feeding it coordinates without non-diagonal notches
 */
void transformCalPoints(float xInput[], float yInput[], float xOutput[], float yOutput[], float fitCoeffsX[], float fitCoeffsY[], float affineCoeffs[][6], float boundaryAngles[]){
	for(int i=0; i < _noOfNotches+1; i++){
		float xValue = linearize(xInput[i], fitCoeffsX);
		float yValue = linearize(yInput[i], fitCoeffsY);
		float outX;
		float outY;
		notchRemap(xValue, yValue, &outX, &outY, affineCoeffs, boundaryAngles, _noOfNotches);
		xOutput[i] = outX;
		yOutput[i] = outY;
	}
};

/*******************
	cleanCalPoints
	take the x and y coordinates and notch angles collected during the calibration procedure,
	and generate the cleaned (non-redundant) x and y stick coordinates and the corresponding x and y notch coordinates
*******************/
void cleanCalPoints(const float calPointsX[], const float calPointsY[], const float notchAngles[], float cleanedPointsX[], float cleanedPointsY[], float notchPointsX[], float notchPointsY[], int notchStatus[]){

	Serial.println("The raw calibration points (x,y) are:");
	for(int i = 0; i< _noOfCalibrationPoints; i++){
		Serial.print(calPointsX[i], 4);
		Serial.print(",");
		Serial.println(calPointsY[i], 4);
	}

	Serial.println("The notch angles are:");
	for(int i = 0; i< _noOfNotches; i++){
		Serial.println(notchAngles[i], 4);
	}

	notchPointsX[0] = 0;
	notchPointsY[0] = 0;
	cleanedPointsX[0] = 0;
	cleanedPointsY[0] = 0;

	Serial.println("The notch points are:");
	for(int i = 0; i < _noOfNotches; i++){
		//add the origin values to the first x,y point
		cleanedPointsX[0] += calPointsX[i*2];
		cleanedPointsY[0] += calPointsY[i*2];

		//copy the cal point into the cleaned list
		cleanedPointsX[i+1] = calPointsX[i*2+1];
		cleanedPointsY[i+1] = calPointsY[i*2+1];

		//convert notch angles to x/y coords (weird since the stick moves spherically)
		calcStickValues(notchAngles[i], notchPointsX+i+1, notchPointsY+i+1);
		notchPointsX[i+1] = round(notchPointsX[i+1]);
		notchPointsY[i+1] = round(notchPointsY[i+1]);

		Serial.print(notchPointsX[i+1]);
		Serial.print(",");
		Serial.println(notchPointsY[i+1]);
	}

	//remove the largest and smallest two origin values to remove outliers
	//first, find their indices
	int smallestX = 0;
	int smallX = 0;
	int largeX = 0;
	int largestX = 0;
	int smallestY = 0;
	int smallY = 0;
	int largeY = 0;
	int largestY = 0;
	for (int i = 0; i < _noOfNotches; i++){
		if (calPointsX[i*2] < calPointsX[smallestX]){//if it's the new smallest
			smallX = smallestX;//shuffle the old smallest to small
			smallestX = i*2;//record the new smallest index
		} else if (calPointsX[i*2] < calPointsX[smallX]){//if it's the new second-smallest
			smallX = i*2;//record the new small index
		}
		if (calPointsX[i*2] > calPointsX[largestX]){//if it's the new largest
			largeX = largestX;//shuffle the old largest to large
			largestX = i*2;//record the new largest index
		} else if (calPointsX[i*2] > calPointsX[largeX]){//if it's the new second-largest
			largeX = i*2;//record the new large index
		}
		if (calPointsY[i*2] < calPointsY[smallestY]){
			smallY = smallestY;
			smallestY = i*2;
		} else if (calPointsY[i*2] < calPointsY[smallY]){
			smallY = i*2;
		}
		if (calPointsY[i*2] > calPointsY[largestY]){
			largeY = largestY;
			largestY = i*2;
		} else if (calPointsY[i*2] > calPointsY[largeY]){
			largeY = i*2;
		}
	}
	//subtract the smallest and largest values
	cleanedPointsX[0] -= calPointsX[smallestX];
	cleanedPointsX[0] -= calPointsX[smallX];
	cleanedPointsX[0] -= calPointsX[largeX];
	cleanedPointsX[0] -= calPointsX[largestX];
	cleanedPointsY[0] -= calPointsY[smallestY];
	cleanedPointsY[0] -= calPointsY[smallY];
	cleanedPointsY[0] -= calPointsY[largeY];
	cleanedPointsY[0] -= calPointsY[largestY];

	//divide by the total number of calibration steps/2 to get the average origin value
	//except it's minus 4 steps since we removed outliers
	cleanedPointsX[0] = cleanedPointsX[0]/((float)_noOfNotches-4);
	cleanedPointsY[0] = cleanedPointsY[0]/((float)_noOfNotches-4);

	for(int i = 0; i < _noOfNotches; i++){
		//calculate radius of cleaned point from center
		float deltaX = cleanedPointsX[i+1] - cleanedPointsX[0];
		float deltaY = cleanedPointsY[i+1] - cleanedPointsY[0];
		float mag = sqrt(deltaX*deltaX + deltaY*deltaY);

		if(mag < 0.02){//if the cleaned point was at the center
			//average the previous and next points (cardinal & diagonal) for some sanity
			//note: this will likely bork if this happens to a cardinal or diagonal
			int prevIndex = ((i-1+_noOfNotches) % _noOfNotches) + 1;
			int nextIndex = ((i+1) % _noOfNotches) + 1;

			cleanedPointsX[i+1] = (cleanedPointsX[prevIndex] + cleanedPointsX[nextIndex])/2.0;
			cleanedPointsY[i+1] = (cleanedPointsY[prevIndex] + cleanedPointsY[nextIndex])/2.0;

			notchPointsX[i+1] = (notchPointsX[prevIndex] + notchPointsX[nextIndex])/2.0;
			notchPointsY[i+1] = (notchPointsY[prevIndex] + notchPointsY[nextIndex])/2.0;

			Serial.print("no input was found for notch: ");
			Serial.println(i+1);

			//Mark that notch adjustment should be skipped for this
			notchStatus[i] = _tertiaryNotchInactive;
		}else{
			notchStatus[i] = _notchStatusDefaults[i];
		}
	}

	Serial.println("The cleaned calibration points are:");
	for(int i = 0; i< (_noOfNotches+1); i++){
		Serial.print(cleanedPointsX[i], 4);
		Serial.print(",");
		Serial.println(cleanedPointsY[i], 4);
	}

	Serial.println("The corresponding notch points are:");
	for(int i = 0; i< (_noOfNotches+1); i++){
		Serial.print(notchPointsX[i]);
		Serial.print(",");
		Serial.println(notchPointsY[i]);
	}

	Serial.println("The notch statuses are:");
	for(int i = 0; i< (_noOfNotches); i++){
		Serial.println(notchStatus[i]);
	}
};

//adjustNotch is used to adjust the angles of the notch.
//It is run after calibration points are collected.
//The notch adjustment is limited in order to control
//1. displacement of points (max 12 units out of +/- 100, for now)
//2. stretching of coordinates (max +/- 30%)
void adjustNotch(int currentStepIn, float loopDelta, bool calibratingAStick, float measuredNotchAngles[], float notchAngles[], int notchStatus[], Buttons &btn, HardwareButtons &hardware){
	//set up variables based on current button state
	bool CW = hardware.X;
	bool CCW = hardware.Y;
	bool reset = btn.B;

	//This gets run after all the calibration points are collected
	//So we subtract the number of calibration points and switch over to notch adjust order
	const int notchIndex = _notchAdjOrder[currentStepIn-_noOfCalibrationPoints];

	//display the desired value on the other stick
	float x = 0;
	float y = 0;
	calcStickValues(measuredNotchAngles[notchIndex], &x, &y);
	if(calibratingAStick){
		btn.Cx = (uint8_t) (x + _floatOrigin);
		btn.Cy = (uint8_t) (y + _floatOrigin);
	}else{
		btn.Ax = (uint8_t) (x + _floatOrigin);
		btn.Ay = (uint8_t) (y + _floatOrigin);
	}

	//do nothing if it's not a valid notch to calibrate
	//it'll skip them anyway but just in case
	if(notchStatus[notchIndex] == _tertiaryNotchInactive){
		return;
	}

	//Adjust notch angle according to which button is pressed (do nothing for both buttons)
	if(CW && !CCW){
		notchAngles[notchIndex] -= loopDelta*0.000075;
	}else if(CCW && !CW){
		notchAngles[notchIndex] += loopDelta*0.000075;
	}else if(reset){
		notchAngles[notchIndex] = measuredNotchAngles[notchIndex];
	}else{
		return;
	}

	//Limit the notch adjustment

	//Start out with the limits being 12 units around the circle at the gate
	/*this may be unnecessary in our case, because 12 units is also the 30% stretch limit
	float lowerPosLimit = measuredNotchAngles[notchIndex] - 12/100.f;
	float upperPosLimit = measuredNotchAngles[notchIndex] + 12/100.f;
	if(upperPosLimit < lowerPosLimit){
		upperPosLimit += 2*M_PI;
	}
	*/

	bool isDiagonal = false;

	//Now we need to determine the stretch/compression limit
	//Figure out the previous and next notch angles.
	//For most they're the adjacent notches.
	int prevIndex = (notchIndex-1+_noOfNotches) % _noOfNotches;
	int nextIndex = (notchIndex+1) % _noOfNotches;
	//For diagonals, the cardinals are the index points.
	if((notchIndex - 2) % 4 == 0){
		prevIndex = (notchIndex-2+_noOfNotches) % _noOfNotches;
		nextIndex = (notchIndex+2) % _noOfNotches;
		isDiagonal = true;
	}
	float prevAngle = notchAngles[prevIndex];
	float nextAngle = notchAngles[nextIndex];
	if(nextAngle < prevAngle){
		nextAngle += 2*M_PI;
	}
	float prevMeasAngle = measuredNotchAngles[prevIndex];
	float thisMeasAngle = measuredNotchAngles[notchIndex];
	float nextMeasAngle = measuredNotchAngles[nextIndex];
	if(nextMeasAngle < thisMeasAngle){
		nextMeasAngle += 2*M_PI;
	}

	float lowerCompressLimit = prevAngle + 0.7*(thisMeasAngle-prevMeasAngle);//how far we can squish when reducing the angle
	float lowerStretchLimit  = nextAngle - 1.3*(nextMeasAngle-thisMeasAngle);//how far we can stretch when reducing the angle
	float upperCompressLimit = nextAngle - 0.7*(nextMeasAngle-thisMeasAngle);//how far we can squish when increasing the angle
	float upperStretchLimit  = prevAngle + 1.3*(thisMeasAngle-prevMeasAngle);//how far we can stretch when increasing the angle

	//Now, in order to apply stretch leniency to angles within the deadzone,
	// we need to figure out whether the previous angle or next angle was a cardinal.
	//If the previous one is a cardinal AND the angle is in the deadzone, we make the upperstretchlimit bigger, only if it can't reach 0.3000.
	const float minThreshold  = 0.1500/0.9750;//radians; we don't want to fix things smaller than this
	const float deadzoneLimit = 0.2875/0.9500;//radians; or things larger than this
	const float deadzonePlus  = 0.3000/0.9500;//radians; we want to make sure the adjustment can make it here
	if(prevIndex % 4 == 0 && !isDiagonal && (thisMeasAngle-prevMeasAngle) > minThreshold && (thisMeasAngle-prevMeasAngle) < deadzoneLimit){
		upperStretchLimit = prevAngle + max(1.3*(thisMeasAngle-prevMeasAngle), deadzonePlus);
	}
	//If the next one is a cardinal AND the angle is in the deadzone, we make the lowerstretchlimit smaller.
	if(nextIndex % 4 == 0 && !isDiagonal && (nextMeasAngle-thisMeasAngle) > minThreshold && (nextMeasAngle-thisMeasAngle) < deadzoneLimit){
		lowerStretchLimit = nextAngle - max(1.3*(nextMeasAngle-thisMeasAngle), deadzonePlus);
	}

	float lowerDistortLimit  = max(lowerCompressLimit, lowerStretchLimit);
	float upperDistortLimit  = min(upperCompressLimit, upperStretchLimit);
	if(upperDistortLimit < lowerDistortLimit){
		upperDistortLimit += 2*M_PI;
	}

	//Combine the limits
	float lowerLimit = lowerDistortLimit;//max(lowerStretchLimit, lowerPosLimit);
	float upperLimit = upperDistortLimit;//min(upperStretchLimit, upperPosLimit);
	if(upperLimit < lowerLimit){
		upperLimit += 2*M_PI;
	}

	//Apply the limits
	notchAngles[notchIndex] = max(notchAngles[notchIndex], lowerLimit);
	notchAngles[notchIndex] = min(notchAngles[notchIndex], upperLimit);
};

//displayNotch is used in lieu of adjustNotch when doing basic calibration
void displayNotch(const int currentStepIn, const bool calibratingAStick, const float notchAngles[], Buttons &btn){
	int currentStep = _calOrder[currentStepIn];
	//display the desired value on the other stick
	float x = 0;
	float y = 0;
	if(currentStep%2){
		const int notchIndex = currentStep/2;
		calcStickValues(notchAngles[notchIndex], &x, &y);
	}
	if(calibratingAStick){
		btn.Cx = (uint8_t) (x + _floatOrigin);
		btn.Cy = (uint8_t) (y + _floatOrigin);
	}else{
		btn.Ax = (uint8_t) (x + _floatOrigin);
		btn.Ay = (uint8_t) (y + _floatOrigin);
	}
};

void collectCalPoints(const WhichStick whichStick, const int currentStepIn, float calPointsX[], float calPointsY[], Pins &pin){
	Serial.print("Collecting cal point for step: ");
	Serial.println(currentStepIn);
    const int currentStep = _calOrder[currentStepIn];

	Serial.print("Cal point number: ");
	Serial.println(currentStep);
	float X;
	float Y;

	for(int j = 0; j < MEDIANLEN; j++){
		X = 0;
		Y = 0;
		for(int i = 0; i < 128; i++){
			if(whichStick == ASTICK){
#ifdef USEADCSCALE
				_ADCScale = _ADCScale*0.999 + _ADCScaleFactor/adc->adc1->analogRead(ADC_INTERNAL_SOURCE::VREF_OUT);
#endif
				//otherwise _ADCScale is 1
				X += adc->adc0->analogRead(pin.pinAx)/4096.0*_ADCScale;
				Y += adc->adc0->analogRead(pin.pinAy)/4096.0*_ADCScale;
			}
			else{
				X += adc->adc0->analogRead(pin.pinCx)/4096.0;
				Y += adc->adc0->analogRead(pin.pinCy)/4096.0;
			}
		}
		X = X/128.0;
		Y = Y/128.0;

#ifdef USEMEDIAN
		static float xPosList[MEDIANLEN] = MEDIANARRAY;//for median filtering;
		static float yPosList[MEDIANLEN] = MEDIANARRAY;//for median filtering
		static unsigned int xMedianIndex = 0;
		static unsigned int yMedianIndex = 0;
		runMedian(X, xPosList, xMedianIndex);
		runMedian(Y, yPosList, yMedianIndex);
#endif
	}

	calPointsX[currentStep] = X;
	calPointsY[currentStep] = Y;

	Serial.println("The collected coordinates are: ");
	Serial.println(calPointsX[currentStep],8);
	Serial.println(calPointsY[currentStep],8);
};

/*******************
	linearizeCal
	Generate a fit to linearize the stick response.
	Inputs:
		cleaned points X and Y, (must be 17 points for each of these, the first being the center, the others starting at 3 oclock and going around counterclockwise)
	Outputs:
		linearization fit coefficients for X and Y
*******************/
void linearizeCal(const float inX[], const float inY[], float outX[], float outY[], float fitCoeffsX[], float fitCoeffsY[]){
	Serial.println("beginning linearization");

	//do the curve fit first
	//generate all the notched/not notched specific cstick values we will need

	double fitPointsX[5];
	double fitPointsY[5];

	fitPointsX[0] = inX[8+1];                   //right
	fitPointsX[1] = (inX[6+1] + inX[10+1])/2.0; //right 45 deg
	fitPointsX[2] = inX[0];                     //center
	fitPointsX[3] = (inX[2+1] + inX[14+1])/2.0; //left 45 deg
	fitPointsX[4] = inX[0+1];                   //left

	fitPointsY[0] = inY[12+1];                  //down
	fitPointsY[1] = (inY[10+1] + inY[14+1])/2.0;//down 45 deg
	fitPointsY[2] = inY[0];                     //center
	fitPointsY[3] = (inY[6+1] + inY[2+1])/2.0;  //up 45 deg
	fitPointsY[4] = inY[4+1];                   //up


	//////determine the coefficients needed to linearize the stick
	//create the expected output, what we want our curve to be fit too
	//this is hard coded because it doesn't depend on the notch adjustments
	//                   -100 -74.246        0     74.246         100, centered around 0-255
    //It's not sin(45 deg) because it's a spherical motion, not planar.
	double x_output[5] = {27.5,53.2537879754,127.5,201.7462120246,227.5};
	double y_output[5] = {27.5,53.2537879754,127.5,201.7462120246,227.5};

	Serial.println("The fit input points are (x,y):");
	for(int i = 0; i < 5; i++){
		Serial.print(fitPointsX[i],8);
		Serial.print(",");
		Serial.println(fitPointsY[i],8);
	}

	Serial.println("The corresponding fit output points are (x,y):");
	for(int i = 0; i < 5; i++){
		Serial.print(x_output[i]);
		Serial.print(",");
		Serial.println(y_output[i]);
	}

	//perform the curve fit, order is 3
	double tempCoeffsX[_fitOrder+1];
	double tempCoeffsY[_fitOrder+1];

	fitCurve(_fitOrder, 5, fitPointsX, x_output, _fitOrder+1, tempCoeffsX);
	fitCurve(_fitOrder, 5, fitPointsY, y_output, _fitOrder+1, tempCoeffsY);

	//write these coefficients to the array that was passed in, this is our first output
	for(int i = 0; i < (_fitOrder+1); i++){
		fitCoeffsX[i] = tempCoeffsX[i];
		fitCoeffsY[i] = tempCoeffsY[i];
	}

	//we will now take out the offset, making the range -100 to 100 instead of 28 to 228
	//calculate the offset
	float xZeroError = linearize((float)fitPointsX[2],fitCoeffsX);
	float yZeroError = linearize((float)fitPointsY[2],fitCoeffsY);

	//Adjust the fit's constant coefficient so that the stick zero position is 0
	fitCoeffsX[3] = fitCoeffsX[3] - xZeroError;
	fitCoeffsY[3] = fitCoeffsY[3] - yZeroError;

	Serial.println("The fit coefficients are  are (x,y):");
	for(int i = 0; i < 4; i++){
		Serial.print(fitCoeffsX[i]);
		Serial.print(",");
		Serial.println(fitCoeffsY[i]);
	}

	Serial.println("The linearized points are:");
	for(int i = 0; i <= _noOfNotches; i++){
		outX[i] = linearize(inX[i],fitCoeffsX);
		outY[i] = linearize(inY[i],fitCoeffsY);
		Serial.print(outX[i],8);
		Serial.print(",");
		Serial.println(outY[i],8);
	}
};

void notchCalibrate(const float xIn[], const float yIn[], const float xOut[], const float yOut[], const int regions, float allAffineCoeffs[][6], float regionAngles[]){
	for(int i = 1; i <= regions; i++){
	Serial.print("calibrating region: ");
	Serial.println(i);

	Eigen::MatrixXf pointsIn(3,3);

	Eigen::MatrixXf pointsOut(3,3);

	if(i == (regions)){
	Serial.println("final region");
	pointsIn  << xIn[0],xIn[i],xIn[1],
				 yIn[0],yIn[i],yIn[1],
				 1,1,1;
	pointsOut << xOut[0],xOut[i],xOut[1],
				 yOut[0],yOut[i],yOut[1],
				 1,1,1;
	}
	else{
		pointsIn  << xIn[0],xIn[i],xIn[i+1],
					 yIn[0],yIn[i],yIn[i+1],
					 1,1,1;
		pointsOut << xOut[0],xOut[i],xOut[i+1],
					 yOut[0],yOut[i],yOut[i+1],
					 1,1,1;
	}

	Serial.println("In points:");
	print_mtxf(pointsIn);
	Serial.println("Out points:");
	print_mtxf(pointsOut);

	Eigen::MatrixXf A(3,3);

	A = pointsOut*pointsIn.inverse();
	//A = pointsOut.colPivHouseholderQr().solve(pointsIn);


	Serial.println("The transform matrix is:");
	print_mtxf(A);

	Serial.println("The affine transform coefficients for this region are:");

	for(int j = 0; j <2;j++){
		for(int k = 0; k<3;k++){
			allAffineCoeffs[i-1][j*3+k] = A(j,k);
			Serial.print(allAffineCoeffs[i-1][j*3+k]);
			Serial.print(",");
		}
	}

	Serial.println();
	Serial.println("The angle defining this  regions is:");
	regionAngles[i-1] = atan2f((yIn[i]-yIn[0]),(xIn[i]-xIn[0]));
	//unwrap the angles so that the first has the smallest value
	if(regionAngles[i-1] < regionAngles[0]){
		regionAngles[i-1] += M_PI*2;
	}
	Serial.println(regionAngles[i-1]);
	}
};
void readSticks(int readA, int readC, Buttons &btn, Pins &pin, const HardwareButtons &hardware, const ControlConfig &controls, const FilterGains &normGains, float &dT){
#ifdef USEADCSCALE
	_ADCScale = _ADCScale*0.999 + _ADCScaleFactor/adc->adc1->analogRead(ADC_INTERNAL_SOURCE::VREF_OUT);
#endif
	// otherwise _ADCScale is 1

	//read the L and R sliders

	//set up lockout for mode 5; it's not permissible to have analog trigger
	// inputs available while mode 5 is active
	//when a trigger is in lockout due to the other being mode 5,
	// modes 1, 3, and 4 will have no output on that trigger to warn the user.
	//(the above modes are 1-indexed, user-facing values)
	const bool lockoutL = controls.rConfig == 4;
	const bool lockoutR = controls.lConfig == 4;

	if(hardware.L && hardware.R && btn.A && btn.S) {
		btn.L = (uint8_t) (1);
		btn.R = (uint8_t) (1);
		btn.A = (uint8_t) (1);
		btn.S = (uint8_t) (1);
	} else {
		switch(controls.lConfig) {
			case 0: //Default Trigger state
				if(lockoutL){
					btn.L  = (uint8_t) 0;
					btn.La = (uint8_t) 0;
				} else {
					btn.L  = hardware.L;
					btn.La = adc->adc0->analogRead(pin.pinLa)>>4;
				}
				break;
			case 1: //Digital Only Trigger state
				btn.L  = hardware.L;
				btn.La = (uint8_t) 0;
				break;
			case 2: //Analog Only Trigger state
				if(lockoutL){
					btn.L  = (uint8_t) 0;
					btn.La = (uint8_t) 0;
				} else {
					btn.L  = (uint8_t) 0;
					btn.La = adc->adc0->analogRead(pin.pinLa)>>4;
				}
				break;
			case 3: //Trigger Plug Emulation state
				if(lockoutL){
					btn.L  = (uint8_t) 0;
					btn.La = (uint8_t) 0;
				} else {
					btn.L  = hardware.L;
					btn.La = adc->adc0->analogRead(pin.pinLa)>>4;
					if (btn.La > (((uint8_t) (controls.lTriggerOffset)) + controls.lTrigInitial)) {
						btn.La = (((uint8_t) (controls.lTriggerOffset)) + controls.lTrigInitial);
					}
				}
				break;
			case 4: //Digital => Analog Value state
				btn.L = (uint8_t) 0;
				if(hardware.L) {
					btn.La = min(((uint8_t) (controls.lTriggerOffset)) + controls.lTrigInitial, 255);
				} else {
					btn.La = (uint8_t) 0;
				}
				break;
			case 5: //Digital => Analog Value + Digital state
				btn.L = hardware.L;
				if(hardware.L) {
					btn.La = min(((uint8_t) (controls.lTriggerOffset)) + controls.lTrigInitial, 255);
				} else {
					btn.La = (uint8_t) 0;
				}
				break;
			default:
				if(lockoutL){
					btn.L  = (uint8_t) 0;
					btn.La = (uint8_t) 0;
				} else {
					btn.L  = hardware.L;
					btn.La = adc->adc0->analogRead(pin.pinLa)>>4;
				}
		}

		switch(controls.rConfig) {
			case 0: //Default Trigger state
				if(lockoutR){
					btn.R  = (uint8_t) 0;
					btn.Ra = (uint8_t) 0;
				} else {
					btn.R  = hardware.R;
					btn.Ra = adc->adc0->analogRead(pin.pinRa)>>4;
				}
				break;
			case 1: //Digital Only Trigger state
				btn.R  = hardware.R;
				btn.Ra = (uint8_t) 0;
				break;
			case 2: //Analog Only Trigger state
				if(lockoutR){
					btn.R  = (uint8_t) 0;
					btn.Ra = (uint8_t) 0;
				} else {
					btn.R  = (uint8_t) 0;
					btn.Ra = adc->adc0->analogRead(pin.pinRa)>>4;
				}
				break;
			case 3: //Trigger Plug Emulation state
				if(lockoutR){
					btn.R  = (uint8_t) 0;
					btn.Ra = (uint8_t) 0;
				} else {
					btn.R  = hardware.R;
					btn.Ra = adc->adc0->analogRead(pin.pinRa)>>4;
					if (btn.Ra > (((uint8_t) (controls.rTriggerOffset)) + controls.rTrigInitial)) {
						btn.Ra = (((uint8_t) (controls.rTriggerOffset)) + controls.rTrigInitial);
					}
				}
				break;
			case 4: //Digital => Analog Value state
				btn.R = (uint8_t) 0;
				if(hardware.R) {
					btn.Ra = min(((uint8_t) (controls.rTriggerOffset)) + controls.rTrigInitial, 255);
				} else {
					btn.Ra = (uint8_t) 0;
				}
				break;
			case 5: //Digital => Analog Value + Digital state
				btn.R = hardware.R;
				if(hardware.R) {
					btn.Ra = min(((uint8_t) (controls.rTriggerOffset)) + controls.rTrigInitial, 255);
				} else {
					btn.Ra = (uint8_t) 0;
				}
				break;
			default:
				if(lockoutR){
					btn.R  = (uint8_t) 0;
					btn.Ra = (uint8_t) 0;
				} else {
					btn.R  = hardware.R;
					btn.Ra = adc->adc0->analogRead(pin.pinRa)>>4;
				}
		}
	}

	unsigned int adcCount = 0;
	unsigned int aXSum = 0;
	unsigned int aYSum = 0;
	unsigned int cXSum = 0;
	unsigned int cYSum = 0;

	static unsigned int lastMicros = micros();
	do{
		adcCount++;
		aXSum += adc->adc0->analogRead(pin.pinAx);
		aYSum += adc->adc0->analogRead(pin.pinAy);
		cXSum += adc->adc0->analogRead(pin.pinCx);
		cYSum += adc->adc0->analogRead(pin.pinCy);
	}
	while((micros()-lastMicros) < 1000);


	//Serial.println(adcCount);
	float aStickX = aXSum/(float)adcCount/4096.0*_ADCScale;
	float aStickY = aYSum/(float)adcCount/4096.0*_ADCScale;
	float cStickX = cXSum/(float)adcCount/4096.0*_ADCScale;
	float cStickY = cYSum/(float)adcCount/4096.0*_ADCScale;

	dT = (micros() - lastMicros)/1000.0;
	lastMicros = micros();
	//create the measurement value to be used in the kalman filter
	float xZ;
	float yZ;

	//linearize the analog stick inputs by multiplying by the coefficients found during calibration (3rd order fit)
	xZ = linearize(aStickX,_aFitCoeffsX);
	yZ = linearize(aStickY,_aFitCoeffsY);

	float posCx = linearize(cStickX,_cFitCoeffsX);
	float posCy = linearize(cStickY,_cFitCoeffsY);


	//Run the kalman filter to eliminate snapback
	static float xPosFilt = 0;//output of kalman filter
	static float yPosFilt = 0;//output of kalman filter
	runKalman(xPosFilt, yPosFilt, xZ, yZ, controls, normGains);
	//Run a simple low-pass filter
	static float oldPosAx = 0;
	static float oldPosAy = 0;
	float posAx = normGains.xSmoothing*xPosFilt + (1-normGains.xSmoothing)*oldPosAx;
	float posAy = normGains.ySmoothing*yPosFilt + (1-normGains.ySmoothing)*oldPosAy;
	oldPosAx = posAx;
	oldPosAy = posAy;

	//Run a simple low-pass filter on the C-stick
	static float cXPos = 0;
	static float cYPos = 0;
	float oldCX = cXPos;
	float oldCY = cYPos;
	cXPos = posCx;
	cYPos = posCy;
	float xWeight1 = normGains.cXSmoothing;
	float xWeight2 = 1-xWeight1;
	float yWeight1 = normGains.cYSmoothing;
	float yWeight2 = 1-yWeight1;

	cXPos = xWeight1*cXPos + xWeight2*oldCX;
	cYPos = yWeight1*cYPos + yWeight2*oldCY;

	posCx = cXPos;
	posCy = cYPos;

	//Run a median filter to reduce noise
#ifdef USEMEDIAN
	static float xPosList[MEDIANLEN] = MEDIANARRAY;//for median filtering;
	static float yPosList[MEDIANLEN] = MEDIANARRAY;//for median filtering
	static unsigned int xMedianIndex = 0;
	static unsigned int yMedianIndex = 0;
    runMedian(posAx, xPosList, xMedianIndex);
    runMedian(posAy, yPosList, yMedianIndex);
#endif

	float remappedAx;
	float remappedAy;
	float remappedCx;
	float remappedCy;
	notchRemap(posAx, posAy, &remappedAx, &remappedAy, _aAffineCoeffs, _aBoundaryAngles, _noOfNotches);
	notchRemap(posCx, posCy, &remappedCx, &remappedCy, _cAffineCoeffs, _cBoundaryAngles, _noOfNotches);

	//Clamp values from -125 to +125
	remappedAx = min(125, max(-125, remappedAx));
	remappedAy = min(125, max(-125, remappedAy));
	remappedCx = min(125, max(-125, remappedCx+controls.cXOffset));
	remappedCy = min(125, max(-125, remappedCy+controls.cYOffset));

	float hystVal = 0.3;
	//assign the remapped values to the button struct
	if(readA){
		float diffAx = (remappedAx+_floatOrigin)-btn.Ax;
		if( (diffAx > (1.0 + hystVal)) || (diffAx < -hystVal) ){
			btn.Ax = (uint8_t) (remappedAx+_floatOrigin);
		}
		float diffAy = (remappedAy+_floatOrigin)-btn.Ay;
		if( (diffAy > (1.0 + hystVal)) || (diffAy < -hystVal) ){
			btn.Ay = (uint8_t) (remappedAy+_floatOrigin);
		}
	}
	if(readC){
		float diffCx = (remappedCx+_floatOrigin)-btn.Cx;
		if( (diffCx > (1.0 + hystVal)) || (diffCx < -hystVal) ){
			btn.Cx = (uint8_t) (remappedCx+_floatOrigin);
		}
		float diffCy = (remappedCy+_floatOrigin)-btn.Cy;
		if( (diffCy > (1.0 + hystVal)) || (diffCy < -hystVal) ){
			btn.Cy = (uint8_t) (remappedCy+_floatOrigin);
		}
	}
};

#endif //STICKCAL_H
