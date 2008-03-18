// Header File
// Created 02/29/2008 03:27:14 PM

#include <tigcclib.h>

#ifndef JSTACK_H
	#define JSTACK_H
	
	float get_float(ESI *ptr);
	
	unsigned int get_int(ESI *ptr);
	
	void check_arg_nums(unsigned int num_args);
	
	ESI get_var_expr(ESI *arg_ptr);
	
	ESI get_expr(ESI *arg_ptr);
	
	void check_can_be_evaluated(ESI expression, ESI var);
#endif





