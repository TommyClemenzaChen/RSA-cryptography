
#include <stdio.h>
#include <gmp.h>


#include <stdbool.h>
#include <stdint.h>
#include "randstate.h"


void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n){
	//o = output, a = base, d = exponent, n = modulus
	//o=1	
	//Creating "p", setting it to the value of a
	mpz_t p, tempD;
	mpz_inits(p,tempD,NULL);
	mpz_set(p,a);
	mpz_set(tempD,d);
	mpz_set_ui(o,1);
	//if d > 0
	while(mpz_cmp_ui(tempD,0) > 0){
		//if d is odd
		
		if(mpz_odd_p(tempD) > 0){
			
		//	printf("IF STATMENT");
			//v = (v x p) % n
			mpz_mul(o,o,p);
			mpz_mod(o,o,n);
			
				
			//Since division in mpz rounds up, we need to subtract one to every odd value
			mpz_sub_ui(tempD,tempD,1);
		}
		//p = (pxp) % n
		mpz_mul(p,p,p);
		mpz_mod(p,p,n);
		//d =d/2
		
			
		mpz_cdiv_q_ui(tempD,tempD,2);
		
	}
	
	
	mpz_clears(p,tempD,NULL);

	
}


void gcd(mpz_t d, mpz_t a, mpz_t b){
	mpz_t tempA, tempB;
	mpz_inits(tempA,tempB, NULL);

	mpz_set(tempA,a);
	mpz_set(tempB,b);

	while(mpz_cmp_ui(tempB,0) != 0){
		//d = b
		mpz_set(d,tempB);

		//b = a%b
		mpz_mod(tempB,tempA,tempB);
		
		//a = d
		mpz_set(tempA,d);		
		
	}
	mpz_clears(tempA,tempB,NULL);
}

bool is_prime(mpz_t n, uint64_t iters){
	//If n is even or 1, instantly return False
	//Otherwise use equaation n-1 = (2^s) * r
	//Loop, Each iteration: (r = n before loop), r = r/2, s+=1
	if(mpz_cmp_ui(n,2) == 0 || mpz_cmp_ui(n,3)==0){
        	return true;
	}
	
	if(mpz_even_p(n) > 0 || mpz_cmp_ui(n,1)<=0){
		return false;
	}
	//intialize
	//r and s are something
	//r,s,y,rand, nTemp, TWO, j,nTemp4,NULL
	mpz_t r, s, y, rand, nTemp, TWO, j, nTemp4, tempY;
	mpz_inits(r,s,y,rand,nTemp, TWO, j, nTemp4, tempY, NULL);

	//r = n
	//r = r-1
	mpz_set(r,n);
	mpz_sub_ui(r,r,1);

	//nTemp = n-1
	//nTemp4 = n-4
	//TWO = 2
	mpz_set(nTemp,r);
	mpz_set(nTemp4,r);
	mpz_sub_ui(nTemp4,nTemp4,2);
	mpz_set_ui(TWO,2);

	//stops when r is odd
	while(mpz_even_p(r) > 0){
		mpz_cdiv_q_ui(r,r,2);
		mpz_add_ui(s,s,1);
	
	}	
	//gmp_printf(" (s,r): (%Zd,%Zd)", s,r);
	//s= s-1
	mpz_sub_ui(s,s,1);
	
	mpz_set_ui(y,1);
	//main loop
	for(uint64_t i = 1; i <= iters; i++) {
		//get random number from  0 to n-4
		//nTemp4 = n-3 and since the bounds is 0 to n-1, it would be 0 to n-4
		mpz_urandomm(rand, state,nTemp4);
		mpz_add_ui(rand,rand,2);



		
		pow_mod(y,rand, r, n);
		//y != 1 and y != n-1
		//gmp_printf(" %Zd  %Zd  %Zd  %Zd //", y,rand,r,n);
		if(mpz_cmp_ui(y,1) != 0 && mpz_cmp(y,nTemp) != 0){

			//j = 1
			mpz_set_ui(j,1);
			
			//j <= s-1, y != n-1
			while(mpz_cmp(j,s) <= 0 && mpz_cmp(y,nTemp)!= 0){
				// y = pow_mod(y,2,n)
				// Reset to y to 1 while tempY will be Y
				mpz_set(tempY,y);
				mpz_set_ui(y,1);
				pow_mod(y,tempY,TWO,n);
					
				//gmp_printf(" %Zd  %Zd  %Zd  %Zd     ", y,tempY,TWO,n);
				//if y == 0 return false			
				if(mpz_cmp_ui(y,1) == 0){
					
					mpz_clears(r,s,y,rand, nTemp, TWO, j,nTemp4,tempY, NULL);
					return false;
				}
				mpz_add_ui(j,j,1);
			}
			if(mpz_cmp(y,nTemp) != 0){
				
				
				mpz_clears(r,s,y,rand, nTemp, TWO, j,nTemp4,tempY,NULL);
				return false;
			}
		}	
	
	}
	mpz_clears(r,s,y,rand, nTemp, TWO, j,nTemp4,tempY,NULL);
	return true;
	

}

void mod_inverse(mpz_t o, mpz_t a, mpz_t n){
		
	mpz_t r, r1, t, t1,q,tempR,tempQ,tempQ2, tempT;
	mpz_inits(r,r1,t,t1,q,tempR,tempQ,tempQ2, tempT, NULL);
	//(r,r1) = (n,a)
	mpz_set(r,n);
	mpz_set(r1,a);

	//  t,t1 = 0,1
	mpz_set_ui(t,0);
	mpz_set_ui(t1,1);

	while(mpz_cmp_ui(r1,0) != 0){

		
		//q = r/r1
		mpz_fdiv_q(q,r,r1);
		//temp vars
		mpz_set(tempR,r);
		mpz_set(tempQ,q);
		mpz_set(tempQ2,q);
		mpz_set(tempT,t);
		
		//r = r1
		mpz_set(r,r1);
		//r - q x r1
		mpz_mul(tempQ,tempQ,r1);
		mpz_sub(r1,tempR, tempQ);

		
		//t = t1
		mpz_set(t,t1);
		//r- q x t1
		mpz_mul(tempQ2,tempQ2,t1);
		mpz_sub(t1, tempT, tempQ2);
	}
	if(mpz_cmp_ui(r,1) > 0) {
		//NO INVERSE
		mpz_set_ui(o,0);
		mpz_clears(r,r1,t,t1,q,tempR,tempQ,tempQ2,tempT,NULL);
		return;
	}

	if(mpz_cmp_ui(t,0) < 0){
		mpz_add(t,t,n);
	}
	
	mpz_set(o,t);
	
	mpz_clears(r, r1, t, t1,q,tempR,tempQ, tempT,tempQ2, NULL);
	

}



void make_prime(mpz_t p, uint64_t bits, uint64_t iters){
	mpz_t temp;
	size_t size;
	mpz_inits(temp,NULL);

	for(int i = 0; i < 1000; i++){
		//generate random number
		mpz_urandomb(temp, state, bits);

		//Check the size of that number
		size = mpz_sizeinbase(temp, 2);
		uint64_t tempSize = size;	
	       	if(is_prime(temp,iters) && tempSize >= bits){
			mpz_set(p,temp);

			mpz_clears(temp,NULL);
			return;

		}

	}
	mpz_clears(temp,NULL);
	return;

}



