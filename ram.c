/*! \mainpage Riemann Sums Program Documentation
*
*	This is a program which computes the Riemann sums and trapezoid rule sums of an expression over
*  a given interval with a given number of steps.  It is used for the estimation of the area under
*  a curve, i.e. an integral.  For information on Riemann sums, see
*  http://en.wikipedia.org/wiki/Riemann_sum .  The entry point is _main() in ram.c when run on
*  a graphing calculator.  From the user's point of view, the program is a function, taking arguments
*  and returning a result.  From the code's point of view, it must pop off the arguments from an
*  "expression stack", in order to access the algebraic expressions it is passed.  It then pushes 
*  the result back onto the stack.  This is useful as an executable on the calculator, but not as
*  useful when called from other C or assembly language programs.
*
* For reentrant library functions which do Riemann sums and trapeziod rule calculations,
*  see  riemann_sum() or trap_rule() in ram.c.
*
* Various functions dealing with popping or examining elements on the expression stack are located in
*  jstack.c.
*
* \section Arguments
*  The function call on the graphing calculator is of the form 
*  <B> ramlib(expression, var, lower_bounds, upper_bounds, number_of_steps, function_index) </B>
*
* 	Note: The arguments are taken from the expression stack on the calculator. For the user, this
*   simply means that they can be passed in the usual manner i.e. "ramlib(a,b,c..)".  However, in
*   order to use the results as part of a further expression (i.e. "2*ramlib(a,b,c...)"), it might
*   require the HW3 Patch, available from Kevin Kofler at
*   http://www.tigen.org/kevin.kofler/ti89prog.htm
*	\subsection expression
*		The function to be integrated
* \subsection var
*  The variable of integration.
* \subsection lower_bounds lower bounds
*  The lower boundary of the interval.
* \subsection upper_bounds upper bounds
*  The upper boundary of the interval.
* \subsection  number_of_steps number of steps
*  The number of sections to divide the region into; the number of sampling points.  Increasing
*  	this will increase the accuracy, but will make the program take longer to execute.
* \subsection function_index function index
*  This determines which computation will be performed according to the following table: \n
*  		0 = left riemann sum \n
*  		1 = middle riemann sum \n
*  		2 = right riemann sum \n
*  		3 = trapezoid rule
*/

// C Source File
// Created 02/28/2008 06:45:06 PM

#include "ram.h"
/*!
*	The entry point of the program.  Takes arguments as follow from the expression stack: an
*  expression to be integrated, the variable of integration, the lower bounds (float), the upper
*  bounds (float), number of steps, and the function index, which instructs the _main() to call
* the corresponding function. \n 0=lram \n 1=mram \n 2=rram \n 3=trapezoid rule
*/
void _main() {
	ESI arg_ptr=top_estack;
	check_arg_nums(NUM_ARGS);
	ESI expression=get_expr(&arg_ptr);
	ESI var=get_var_expr(&arg_ptr);
	float lower_bounds=get_float(&arg_ptr);
	float upper_bounds=get_float(&arg_ptr);	
	unsigned int numsteps=get_int(&arg_ptr);
	unsigned int functionindex=get_int(&arg_ptr);
	check_can_be_evaluated(expression,var);
	float f;
	switch(functionindex) {
		case 0:
		case 1:
		case 2:
			f=riemann_sum(expression,var,lower_bounds,upper_bounds,numsteps,((float)functionindex)/2.0f);
			break;
		case 3:
		default:
			f=trap_rule(expression, var, lower_bounds, upper_bounds, numsteps);
	}
	top_estack=arg_ptr;
	push_Float(f);
}

/*!
*	Returns the float value of expression, when var is set to the value x.
*/
float value_at_point(ESI expression, ESI var, float x) {
		ESI tmp=top_estack;
		push_Float(x);
		push_substitute_no_simplify(expression,var,top_estack);
		NG_approxESI(top_estack);
    float result= estack_number_to_Float(top_estack);
    top_estack=tmp;
    return result;
}
/*!
* Returns the Riemann sum of the argument "expression".  The "var" argument is the variable which
*  changes; it is analagus to the variable of integration. The bounds are determind by lower_bounds
*  and upper_bounds, and the number of steps is determined by numsteps. 
* Assuming left Riemann sums are the default operation, offset * dx (with var as x), will be added
*  to the location of every sampling point.  This effectively shifts over the location on which the
*  sum is being taken.  Thus, for example, one could do a right riemann sum by setting offset to 1.
*/
float riemann_sum(ESI expression, ESI var, float lower_bounds, float upper_bounds,
			unsigned int numsteps,float offset) {
	float step=(upper_bounds-lower_bounds)/numsteps;
	lower_bounds+=step*offset;
	float result=0;
	for(unsigned int i=0; i<numsteps; i++) {
		result+=value_at_point(expression,var,lower_bounds+i*step);
	}
	return result*step;
}
/*!
*	This function is very similar to riemann_sum, except that it computes the trapezoid rule instead,
*  and *  has no "offset" argument, because there are no variations on the trapeziod rule.
*/
float trap_rule(ESI expression, ESI var, float lower_bounds, float upper_bounds,
		 unsigned int numsteps) {
	float step=(upper_bounds-lower_bounds)/numsteps;
	float result=0;
	for(unsigned int i=1; i<numsteps; i++) {
		result+=value_at_point(expression,var,lower_bounds+i*step);
	}
	result*=2;
	result+=value_at_point(expression,var,lower_bounds);
	result+=value_at_point(expression,var,upper_bounds);
	return result*step/2;
}


