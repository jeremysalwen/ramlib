// Header File
// Created 02/28/2008 06:45:29 PM

#include <tigcclib.h>
#include "jstack.h"

#ifndef RAMLIB_H
	#define RAMLIB_H
	/*!
	* The number of arguments that this program will take from the expression stack.
	*/
	#define NUM_ARGS 6
	
	float value_at_point(ESI expression, ESI var, float x);
	
	float riemann_sum(ESI expression, ESI var, float lower_bounds, float upper_bounds, unsigned int numsteps,float offset);  

	float trap_rule(ESI expression, ESI var, float lower_bounds, float upper_bounds, unsigned int numsteps);
#endif








