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
// TODO: добавить операцию сопр€жени€

// +
#define C_plus_C(Dest_Re, Dest_Im, Op1_Re, Op1_Im, Op2_Re, Op2_Im)\
	Dest_Re = Op1_Re + Op2_Re;\
	Dest_Im = Op1_Im + Op2_Im

// ƒа - хрень, но не охота - пиши руками (лучше не придумал, блин)
#define P_plus_P(Dest_R, Dest_A, Op1_R, Op1_A, Op2_R, Op2_A)\
	Dest_R = sqrt((Op1_R*sin(Op1_A) + Op2_R*sin(Op2_A)) * (Op1_R*sin(Op1_A) + Op2_R*sin(Op2_A))\
	+ (Op1_R*cos(Op1_A) + Op2_R*cos(Op2_A)) * (Op1_R*cos(Op1_A) + Op2_R*cos(Op2_A)));\
	Dest_A = atan2((Op1_R*sin(Op1_A) + Op2_R*sin(Op2_A)), (Op1_R*cos(Op1_A) + Op2_R*cos(Op2_A)))

#define H_plus_H(Dest_R, Dest_S, Dest_C, Op1_R, Op1_S, Op1_C, Op2_R, Op2_S, Op2_C)\
	Dest_R = sqrt((Op1_R*Op1_S + Op2_R*Op2_S) * (Op1_R*Op1_S + Op2_R*Op2_S)\
	+ (Op1_R*Op1_C + Op2_R*Op2_C) * (Op1_R*Op1_C + Op2_R*Op2_C));\
	Dest_S = (Op1_R*Op1_S + Op2_R*Op2_S) / Dest_R;\
	Dest_C = (Op1_R*Op1_C + Op2_R*Op2_C) / Dest_R

// *
#define C_prod_C(Dest_Re, Dest_Im, Op1_Re, Op1_Im, Op2_Re, Op2_Im)\
	Dest_Re = Op1_Re * Op2_Re - Op1_Im * Op2_Im;\
	Dest_Im = Op1_Im * Op2_Re + Op1_Re * Op2_Im

#define P_prod_P(PDestR, PDestA, P1R, P1A, P2R, P2A)\
	PDestR = P1R * P2R;\
	PDestA = P1A + P2A

#define H_prod_H(Dest_R, Dest_S, Dest_C, Op1_R, Op1_S, Op1_C, Op2_R, Op2_S, Op2_C)\
	Dest_R = Op1_R * Op2_R;\
	Dest_S = Op1_C * Op2_S + Op1_S * Op2_C;\
	Dest_C = Op1_C * Op2_C - Op1_S * Op2_S

// /
#define C_div_C(Dest_Re, Dest_Im, Op1_Re, Op1_Im, Op2_Re, Op2_Im)\
	Dest_Re = (Op1_Re * Op2_Re + Op1_Im * Op2_Im)/(Op2_Re * Op2_Re + Op2_Im * Op2_Im);\
	Dest_Im = (Op1_Im * Op2_Re - Op1_Re * Op2_Im)/(Op2_Re * Op2_Re + Op2_Im * Op2_Im)

#define P_div_P(PDestR, PDestA, P1R, P1A, P2R, P2A)\
	PDestR = P1R / P2R;\
	PDestA = P1A - P2A

#define H_div_H(Dest_R, Dest_S, Dest_C, Op1_R, Op1_S, Op1_C, Op2_R, Op2_S, Op2_C)\
	Dest_R = Op1_R * Op2_R;\
	Dest_S = Op1_C * Op2_S - Op1_S * Op2_C;\
	Dest_C = Op1_C * Op2_C + Op1_S * Op2_S