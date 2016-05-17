// ============================================================================
//
// This file contains the actions, conditions and expressions your object uses
// 
// ============================================================================

#include "common.h"
#include "floats.hpp"
// no lobyte and hibyte

// ============================================================================
//
// CONDITIONS
// 
// ============================================================================


// ============================================================================
//
// ACTIONS
// 
// ============================================================================

ACTION(
	/* ID */			0,
	/* Name */			("%o: Set integer value %0 size %1 at %2"),
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,("Set Integer value"),PARAM_NUMBER,("Set size of the value (1: BYTE, 2: WORD, 4: DWORD)"),PARAM_NUMBER,("at address"))
){	
	unsigned int value = Param(TYPE_INT);
	unsigned char size = Param(TYPE_INT);
	DWORD src = Param(TYPE_INT);

	memcpy((void*)src, (BYTE*)&value, min(size,4));
}

ACTION(
	/* ID */			1,
	/* Name */			("%o: Set String %0 Null Terminated flag: %1 at %2"),
	/* Flags */			0,
	/* Params */		(3,PARAM_STRING,("Set String"),PARAM_NUMBER,("Null Terminated (0: No, 1: Yes)"),PARAM_NUMBER,("At address"))
){	
	
	char* text = (TCHAR*)CNC_GetStringParameter(rdPtr);
	bool NullTerminated = Param(TYPE_INT);
	DWORD src = Param(TYPE_INT);

	memcpy((char*)src, text, strlen(text));
	if (NullTerminated) *(char*)(src+strlen(text)) = NULL;
}

// ============================================================================
//
// EXPRESSIONS
// 
// ============================================================================

int NumberOfSetBits(int i)
{
     // Java: use >>> instead of >>
     // C or C++: use uint32_t
     i = i - ((i >> 1) & 0x55555555);
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

int NumberOfBits(int input)
{
	int output = 0;
	for (int i=0; i<sizeof(input)*CHAR_BIT; i++)
	{
		if ((input >> i) & 1)
			output = i+1;
	}
	return output;
}

EXPRESSION(
	/* ID */			0,
	/* Name */			"SHR(",
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,"Integer value",EXPPARAM_NUMBER,"Shift by")
) {
	int p1 = ExParam(TYPE_INT);
	int p2 = ExParam(TYPE_INT);
	
	return p1 >> p2;
}

EXPRESSION(
	/* ID */			1,
	/* Name */			"SHL(",
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER, "Integer value", EXPPARAM_NUMBER, "Shift by")
	) {
	int p1 = ExParam(TYPE_INT);
	int p2 = ExParam(TYPE_INT);

	return p1 << p2;
}

EXPRESSION(
	/* ID */			2,
	/* Name */			"sign(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	int input = ExParam(TYPE_INT);
	if ( input >= 0 )
		return 1;
	else
		return -1;
}

EXPRESSION(
	/* ID */			3,
	/* Name */			"char(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	char input = ExParam(TYPE_INT);
	return input;
}

EXPRESSION(
	/* ID */			4,
	/* Name */			"uchar(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	unsigned char input = ExParam(TYPE_INT);
	return input;
}

EXPRESSION(
	/* ID */			5,
	/* Name */			"short(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	short input = ExParam(TYPE_INT);
	return input;
}

EXPRESSION(
	/* ID */			6,
	/* Name */			"ushort(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	unsigned short input = ExParam(TYPE_INT);
	return input;
}

EXPRESSION(
	/* ID */			7,
	/* Name */			"BinToHFlt(",
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	short input = ExParam(TYPE_INT);
	float output;

	FP32 float32;
	FP16 float16;
	float16.u = input;
	float32 = half_to_float(float16);
	output = float32.f;

	ReturnFloat(output);
}

EXPRESSION(
	/* ID */			8,
	/* Name */			"BinToFlt(",
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	int p1temp = ExParam(TYPE_INT);
	float output = *reinterpret_cast<float*>(&p1temp);

	ReturnFloat(output);
}

EXPRESSION(
	/* ID */			9,
	/* Name */			"GetFlag(",
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER, "Integer value",EXPPARAM_NUMBER, "Flag (0 based)")
	) {
	int p1 = ExParam(TYPE_INT);
	int p2 = ExParam(TYPE_INT);
	int output = (p1 >> p2) & 1;

	return output;
}

EXPRESSION(
	/* ID */			10,
	/* Name */			"FlagON(",
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER, "Integer value",EXPPARAM_NUMBER, "Flag (0 based)")
	) {
	int p1 = ExParam(TYPE_INT);
	int p2 = ExParam(TYPE_INT);
	int output = p1 |= 1 << p2;

	return output;
}

EXPRESSION(
	/* ID */			11,
	/* Name */			"FlagOFF(",
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER, "Integer value",EXPPARAM_NUMBER, "Flag (0 based)")
	) {
	int p1 = ExParam(TYPE_INT);
	int p2 = ExParam(TYPE_INT);
	int output = p1 &= ~(1 << p2);

	return output;
}

EXPRESSION(
	/* ID */			12,
	/* Name */			"Toggle(",
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER, "Integer value",EXPPARAM_NUMBER, "Flag (0 based)")
	) {
	int p1 = ExParam(TYPE_INT);
	int p2 = ExParam(TYPE_INT);
	int output = p1 ^= 1 << p2;

	return output;
}

EXPRESSION(
	/* ID */			13,
	/* Name */			"CountON(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	int p1 = ExParam(TYPE_INT);
	unsigned char output = 0;
	for (int i=0; i<sizeof(p1)*CHAR_BIT; i++)
	{
		if ((p1 >> i) & 1)
			output++;
	}
	return output;
}

EXPRESSION(
	/* ID */			14,
	/* Name */			"HFltToBin(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Floating-Point value")
){	
	int p1temp = ExParam(TYPE_FLOAT);
	float value = *(float*)&p1temp;

	FP32 float32;
	float32.f = value;
	FP16 float16;
	float16 = float_to_half_full(float32);
	return (WORD)float16.u;
}

EXPRESSION(
	/* ID */			15,
	/* Name */			"FltToBin(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Floating-Point value")
){	
	int p1temp = ExParam(TYPE_FLOAT);
	return p1temp;
}

EXPRESSION(
	/* ID */			16,
	/* Name */			"Pattern(",
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER, "Integer value",EXPPARAM_NUMBER, "Pattern size")
	) {
	int p1 = ExParam(TYPE_INT);
	int p2 = ExParam(TYPE_INT);

	int bitsCount = NumberOfBits(p1);
	int output = 0;

	if (bitsCount)
	{
		output = ( p1 << (p2-bitsCount) );
		for ( int i=0; i<p2/bitsCount; i++ )
			output |= p1 << (p2 - bitsCount*(i+1));
		if ( p2%bitsCount )
			output |= p1 >> bitsCount - p2%bitsCount;
	}

	return output;
}

EXPRESSION(
	/* ID */			17,
	/* Name */			"CountAll(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	int p1 = ExParam(TYPE_INT);

	return NumberOfBits(p1);
}

EXPRESSION(
	/* ID */			18,
	/* Name */			"LOBYTE(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	long input = ExParam(TYPE_INT);
	return LOBYTE(input);
}

EXPRESSION(
	/* ID */			19,
	/* Name */			"HIBYTE(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	short input = ExParam(TYPE_INT);
	return HIBYTE(input);
}

EXPRESSION(
	/* ID */			20,
	/* Name */			"LOWORD(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	short input = ExParam(TYPE_INT);
	return LOWORD(input);
}

EXPRESSION(
	/* ID */			21,
	/* Name */			"HIWORD(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	long input = ExParam(TYPE_INT);
	return HIWORD(input);
}
