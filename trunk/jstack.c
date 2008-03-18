#include "jstack.h"

/*!
*Checks if the current number of args on the stack is greater or less than num_args,
*	and throws the corresponding error if so.
*/
void check_arg_nums(unsigned int num_args) {
	unsigned int arg_cnt=ArgCount();
	if(ArgCount()>num_args) {
		ER_throw(ER_TOO_MANY_ARGS);
	} else if(arg_cnt<num_args) {
		ER_throw(ER_TOO_FEW_ARGS);
	} 
}

/*!
*	Returns the expression, and makes arg_ptr point to the next expression on the stack.
*		If the expression originally pointed to is not a variable, it will throw an error.
*/
ESI get_var_expr(ESI *arg_ptr) {
	ESI var=*arg_ptr;
	*arg_ptr=next_expression_index(*arg_ptr);
	if(!is_variable(var)) {
		ER_throw(ER_EXPECTED_VAR);
	}
	return var;
}
/*!
* Simply returns another pointer to arg_ptr, and then makes arg_ptr point to the next expression
*/
ESI get_expr(ESI *arg_ptr) {
	ESI expression=*arg_ptr;
	*arg_ptr=next_expression_index(*arg_ptr);
	return expression;
}
/*!
*	Converts any expression pointed to by ptr to a float, makes ptr point to the next expression,
*  and returns the float.  Will throw ER_ARGUMENT if ptr cannot be simplified to a float.  Will
*  add a bcd of the float to the top of the stack.
*/
float get_float(ESI *ptr) {
	if(!can_be_approxed(*ptr,FALSE)) {
		ER_throw(ER_ARGUMENT);
	}
	NG_approxESI(*ptr);
	float result=estack_to_float(top_estack);
	*ptr=next_expression_index(*ptr);
	return result;
}
/*!
*	Converts any expression pointed to by ptr to a positive int, makes ptr point to the next
*  expression, and returns the int.  Will throw ER_ARGUMENT if ptr cannot be simplified to
*  an int, however, it will convert if it is negative or decimal, it will simply round or take
*  the absolute value.  Will add a bcd of the float to the top of the stack.
*/
unsigned int get_int(ESI *ptr) {
	short result;
	if(estack_to_short(*ptr,&result)==1) {
		*ptr=next_expression_index(*ptr);
	} else {
		result=get_float(ptr);
	}
	return abs(result);
}

/*!
* Throws an error if the expression contains any variables other than var.
*/

void check_can_be_evaluated(ESI expression, ESI var) {
	if(has_different_variable(expression,var,FALSE)) {
		ER_throw(ER_ARGUMENT);
	}
}

