void main(void){
	
	/*
	* IR readings and PING readings are stored in two arrays called "irArray" and "sonarArray" respectivly.
	*	length of "irArray" and "sonarArray" is 180.
	*/
	
	obj_init(irArray, sonarArray, 180);
	obj_run();

	for(i = 0; i < get_n_objects();i++){
		sprintf(string, "%d\t%0.2f\t%d\t%d\n", i, get_obj_dist(i), get_obj_location(i), get_obj_width(i));
		uart_sendStr(string);
	}
		
    while(1);
	
	/* SAMPLE OUTPUT
		index	dist.	loc.	width
		0       185.85  50      1
		1       290.77  60      5
		2       11.42   101     22
		3       12.53   128     6
		4       22.62   178     2

	*/
	
}
