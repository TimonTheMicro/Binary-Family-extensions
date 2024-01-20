/* sqrt example */
#include <stdio.h>      /* printf */
#include <math.h>       /* sqrt */

#include "rotm2quat.h"

inline double SIGN(double x) { 
	return (x >= 0.0f) ? +1.0f : -1.0f; 
}

inline double NORM(double a, double b, double c, double d) { 
	return sqrt(a * a + b * b + c * c + d * d); 
}

// quaternion = [w, x, y, z]'
float mRot2Quat(int what,
	float r11, float r12, float r13,
	float r21, float r22, float r23,
	float r31, float r32, float r33
) {
	double q0 = (r11 + r22 + r33 + 1.0f) / 4.0f;
	double q1 = (r11 - r22 - r33 + 1.0f) / 4.0f;
	double q2 = (-r11 + r22 - r33 + 1.0f) / 4.0f;
	double q3 = (-r11 - r22 + r33 + 1.0f) / 4.0f;
	if (q0 < 0.0f) {
		q0 = 0.0f;
	}
	if (q1 < 0.0f) {
		q1 = 0.0f;
	}
	if (q2 < 0.0f) {
		q2 = 0.0f;
	}
	if (q3 < 0.0f) {
		q3 = 0.0f;
	}
	q0 = sqrt(q0);
	q1 = sqrt(q1);
	q2 = sqrt(q2);
	q3 = sqrt(q3);
	if (q0 >= q1 && q0 >= q2 && q0 >= q3) {
		q0 *= +1.0f;
		q1 *= SIGN(r32 - r23);
		q2 *= SIGN(r13 - r31);
		q3 *= SIGN(r21 - r12);
	}
	else if (q1 >= q0 && q1 >= q2 && q1 >= q3) {
		q0 *= SIGN(r32 - r23);
		q1 *= +1.0f;
		q2 *= SIGN(r21 + r12);
		q3 *= SIGN(r13 + r31);
	}
	else if (q2 >= q0 && q2 >= q1 && q2 >= q3) {
		q0 *= SIGN(r13 - r31);
		q1 *= SIGN(r21 + r12);
		q2 *= +1.0f;
		q3 *= SIGN(r32 + r23);
	}
	else if (q3 >= q0 && q3 >= q1 && q3 >= q2) {
		q0 *= SIGN(r21 - r12);
		q1 *= SIGN(r31 + r13);
		q2 *= SIGN(r32 + r23);
		q3 *= +1.0f;
	}
	else {
		return -1;
	}
	double r = NORM(q0, q1, q2, q3);
	q0 /= r;
	q1 /= r;
	q2 /= r;
	q3 /= r;
	
	if (what == 0)
		return q0;
		else if (what == 1)
			return q1;
			else if (what == 2)
				return q2;
					else if (what == 3)
						return q3;
	return 0;
}
