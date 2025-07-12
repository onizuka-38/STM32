/*
 * arm_melody.h
 *
 *  Created on: Oct 30, 2023
 *      Author: JeongWhan Lee
 */

#ifndef INC_ARM_MELODY_H_
#define INC_ARM_MELODY_H_


/* ---------------- */
#define DO		261.63
#define RE		293.66
#define MI		329.63
#define FA		349.23
#define SOL		392.00
#define LA		440.00
#define TI		493.88
#define DDO		523.25

#define C4		261.63
#define CS4		277.18
#define D4		293.66
#define DS4		311.13
#define E4		329.63
#define F4		349.23
#define FS4		369.99
#define G4		392.00
#define GS4		415.30
#define A4		440.00
#define AS4		466.16
#define B4		493.88

#define C5		523.25

#define R		1	// Rest...
#define EOS		-1	// End of Score...

#define FREQUENCY	1000000	// 1MHz

#define N2		60*1000/96*2
#define N4		60*1000/96
#define N2N4	(N2+N4)
#define N8		60*1000/96/2
#define N4N8	(N4+N8)
#define R4		60*1000/96

/**================================================== **/


int song[] = {DO, RE, MI,  DO, MI,  DO, MI,  RE, MI,  FA, FA, MI,  RE, FA, R,
			  MI, FA, SOL, MI, SOL, MI, SOL, FA, SOL, LA, LA, SOL, FA, LA, R, EOS};
int time[] = {N4N8, N8, N4N8, N8, N4, N4, N2, N4N8, N8, N8, N8, N8, N8, N2N4, R4,
			  N4N8, N8, N4N8, N8, N4, N4, N2, N4N8, N8, N8, N8, N8, N8, N2N4, R4, R4};
// 학교종이 땡떙떙
int song2[] = { SOL, SOL, LA, LA, SOL, SOL, MI, SOL, SOL, MI, MI, RE, R,
		        SOL, SOL, LA, LA, SOL, SOL, MI,	SOL, MI,  RE, MI, DO, R, EOS};
int time2[] = { N4,  N4,  N4, N4, N4,  N4,  N2,	N4,  N4,  N4, N4, N2, R4,
				N4,  N4,  N4, N4, N4,  N4,  N2,	N4,  N4,  N4, N4, N2, R4, R4};


#endif /* INC_ARM_MELODY_H_ */
