#include <stdio.h>
#include <gmp.h>

#include <stdbool.h>
#include <stdint.h>
#include "numtheory.h"
#include "randstate.h"
#include <stdlib.h>
#include <unistd.h>
#include "rsa.h"
#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>

#define OPTIONS "b: i: n: d: s: v h"

int main(int argc, char **argv){
	int opt = 0;
	//change seed!!!
	uint64_t seed = time(NULL);
	uint64_t bits = 1024, iters = 50;
	bool print = false;
	FILE * pbfile = fopen("rsa.pub", "w");
	FILE * pvfile = fopen("rsa.priv","w");
	char * user = getenv("USER");

	while((opt = getopt(argc, argv, OPTIONS)) != -1) {
		
		switch(opt) {
		case('b'):
			bits = strtoull(optarg,NULL,10);
			if(bits < 50 || bits > 4096){
				return 1;
			}
			break;
		case('i'):
			
			iters = strtoull(optarg, NULL, 10);
			if(iters < 1 || iters > 500){
				return 1;
			}
			break;
		case('n'):
			pbfile = fopen(optarg, "w");;
			break;
		case('d'):
			pvfile = fopen(optarg, "w");
			break;
		case('s'):
			seed = strtoull(optarg,NULL,10);
			break;
		case('v'):
			print = true;
			break;
		case('h'):
			fprintf(stderr,"Usage: ./keygen-dist [options]\n");
			fprintf(stderr,"  ./keygen-dist generates a public / private key pair, placing the keys into the public and private\n");  
			fprintf(stderr,"  key files as specified below. The keys have a modulus (n) whose length is specified in\n");
			fprintf(stderr,"  the program options.\n");
			fprintf(stderr,"    -s <seed>   : Use <seed> as the random number seed. Default: time()\n");
			fprintf(stderr,"    -b <bits>   : Public modulus n must have at least <bits> bits. Default: 1024\n");
			fprintf(stderr,"    -i <iters>  : Run <iters> Miller-Rabin iterations for primality testing. Default: 50\n");
			fprintf(stderr,"    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n");
			fprintf(stderr,"    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv\n");
			fprintf(stderr,"    -v          : Enable verbose output.\n");
			fprintf(stderr,"    -h          : Display program synopsis and usage\n");
			return 0;
			break;
		default:

			fprintf(stderr,"Usage: ./keygen-dist [options]\n");
                        fprintf(stderr,"  ./keygen-dist generates a public / private key pair, placing the keys into the public and private\n");
                        fprintf(stderr,"  key files as specified below. The keys have a modulus (n) whose length is specified in\n");
                        fprintf(stderr,"  the program options.\n");
                        fprintf(stderr,"    -s <seed>   : Use <seed> as the random number seed. Default: time()\n");
                        fprintf(stderr,"    -b <bits>   : Public modulus n must have at least <bits> bits. Default: 1024\n");
                        fprintf(stderr,"    -i <iters>  : Run <iters> Miller-Rabin iterations for primality testing. Default: 50\n");
                        fprintf(stderr,"    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n");
                        fprintf(stderr,"    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv\n");
                        fprintf(stderr,"    -v          : Enable verbose output.\n");
                        fprintf(stderr,"    -h          : Display program synopsis and usage\n");
			return 1;
			break;
		}

	}
	
	randstate_init(seed);
	//p: prime num1
	//q: prime num2
	//n: pq
	//e: public exponent
	//d: private key
	//s: signature
	mpz_t p,q,n,e,d,s,temp;
	mpz_set_ui(s,1);
	mpz_inits(p,q,n,e,d,s, temp,NULL);
	
	rsa_make_pub(p,q,n,e,bits, iters);
	rsa_make_priv(d,e,p,q);

	mpz_set_str(temp,user, 62);
	rsa_sign(s,temp,d,n);

	rsa_write_pub(n,e,s,user, pbfile);
	rsa_write_priv(n,d,pvfile);
	int fileNum = fileno(pvfile);
	fchmod(fileNum, 0600);

	if(print){
		gmp_printf("%s\n%Zd\n%Zd\n%Zd\n%Zd\n%Zd\n%Zd\n",user,s,p,q,n,e,d);

	}
	fclose(pvfile);
	fclose(pbfile);
	mpz_clears(p,q,n,e,d,s,temp,NULL);
	randstate_clear();
}

