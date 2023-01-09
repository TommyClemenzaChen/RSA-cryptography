#include <stdio.h>
#include <gmp.h>

#include <stdint.h>
#include <stdlib.h>
#include "randstate.h"


gmp_randstate_t state;
void randstate_init(uint64_t seed){
	
	gmp_randinit_mt(state);
	srandom(seed);	
	gmp_randseed_ui(state, seed);

}

void randstate_clear(void){
	gmp_randclear(state);

}
