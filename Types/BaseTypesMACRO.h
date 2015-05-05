#pragma once
#include <math.h>

// Conversion
#define C2P(C_Re, C_Im, P_R, P_A)\
	P_A = atan2(C_Im, C_Re);\
	P_R = sqrt(C_Re * C_Re + C_Im * C_Im)

#define P2C(P_R, P_A, C_Re, C_Im)\
	C_Re = P_R * cos(P_A);\
	C_Im = P_R * sin(P_A)

#define C2H(C_Re, C_Im, H_R, H_sin, H_cos)\
	H_R = sqrt(C_Re * C_Re + C_Im * C_Im);\
	H_sin = C_Im / H_R;\
	H_cos = C_Re / H_R

#define H2C(H_R, H_sin, H_cos, C_Re, C_Im)\
	C_Re = H_R * H_cos;\
	C_Im = H_R * H_sin

#define H2P(H_R, H_sin, H_cos, P_R, P_A)\
	P_R = H_R;\
	P_A = atan2(H_sin, H_cos)

#define P2H(P_R, P_A, H_R, H_sin, H_cos)\
	H_R = P_R;\
	H_sin = sin(P_A);\
	H_cos = cos(P_A)

// Operations
#define P_prod_P(PDestR, PDestA, P1R, P1A, P2R, P2A)\
	PDestR = P1R * P2R;\
	PDestA = P1A + P2A
