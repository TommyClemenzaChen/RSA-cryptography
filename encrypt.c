
#include <stdio.h>
#include <gmp.h>

#include "numtheory.h"
#include <errno.h>
#include "randstate.h"
#include <stdlib.h>
#include <unistd.h>
#include "rsa.h"
#include <stdbool.h>
#define OPTIONS "i: o: n: v h"
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char **argv){

        int opt = 0;
      	
        bool print = false;
        FILE * pbfile = fopen("rsa.pub","r");
	FILE * input = stdin; 
	FILE * output = stdout;
	bool defin = true;
	bool defou = true;
			
        while((opt = getopt(argc, argv, OPTIONS)) != -1) {

                switch(opt) {
		
                case('i'):
			defin = false;
			input = fopen(optarg, "r"); 
			if(errno > 0){
				fprintf(stderr, "Usage: ./encrypt-dist [options]\n");
                       		fprintf(stderr, "  ./encrypt-dist encrypts an input file using the specified public key file,\n");
                        	fprintf(stderr, "  writing the result to the specified output file.\n");
                        	fprintf(stderr, "    -i <infile> : Read input from <infile>. Default: standard input.\n");
                        	fprintf(stderr, "    -o <outfile>: Write output to <outfile>. Default: standard output.\n");
                        	fprintf(stderr, "    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n");
                        	fprintf(stderr, "    -v          : Enable verbose output.\n");
                        	fprintf(stderr, "    -h          : Display program synopsis and usage.\n");
				return 1;
			}
                        break;
		case('o'):
			defou = false;
			output = fopen(optarg, "w");
			break;
                case('n'):
			pbfile = fopen(optarg, "r");
                        break;
                case('v'):
                        print = true;
                        break;
                case('h'):
			

			fprintf(stderr, "Usage: ./encrypt-dist [options]\n");
                        fprintf(stderr, "  ./encrypt-dist encrypts an input file using the specified public key file,\n");
                        fprintf(stderr, "  writing the result to the specified output file.\n");
                        fprintf(stderr, "    -i <infile> : Read input from <infile>. Default: standard input.\n");
                        fprintf(stderr, "    -o <outfile>: Write output to <outfile>. Default: standard output.\n");
                        fprintf(stderr, "    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n");
                        fprintf(stderr, "    -v          : Enable verbose output.\n");
                        fprintf(stderr, "    -h          : Display program synopsis and usage.\n");
			return 0;
			break;
		default:
			fprintf(stderr, "Usage: ./encrypt-dist [options]\n");
                        fprintf(stderr, "  ./encrypt-dist encrypts an input file using the specified public key file,\n");
                        fprintf(stderr, "  writing the result to the specified output file.\n");
                        fprintf(stderr, "    -i <infile> : Read input from <infile>. Default: standard input.\n");
                        fprintf(stderr, "    -o <outfile>: Write output to <outfile>. Default: standard output.\n");
                        fprintf(stderr, "    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n");
                        fprintf(stderr, "    -v          : Enable verbose output.\n");
                        fprintf(stderr, "    -h          : Display program synopsis and usage.\n");

			return 1;
			break;
		}
        }
	
	//e = public exponent
	//n = public modular
	//s = username, but in hex
	mpz_t n,e,s,temp;
	mpz_inits(s,e,n,temp,NULL);
	char * user;
	user = malloc(sizeof(char) * (10));

	rsa_read_pub(n,e,s, user, pbfile);
	if(print){
		gmp_printf("%s\n%Zd\n%Zd\n%Zd\n",user,s,n,e);	
	}
	//convert user to mpz and compare to s
	mpz_set_str(temp,user, 62);
	if(!(rsa_verify(temp,s,e,n))){

		fprintf(stderr, "Usage: ./encrypt-dist [options]\n");
                fprintf(stderr, "  ./encrypt-dist encrypts an input file using the specified public key file,\n");
                fprintf(stderr, "  writing the result to the specified output file.\n");
                fprintf(stderr, "    -i <infile> : Read input from <infile>. Default: standard input.\n");
                fprintf(stderr, "    -o <outfile>: Write output to <outfile>. Default: standard output.\n");
                fprintf(stderr, "    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n");
                fprintf(stderr, "    -v          : Enable verbose output.\n");
                fprintf(stderr, "    -h          : Display program synopsis and usage.\n");
	}

	rsa_encrypt_file(input,output,n,e);
	free(user);
	mpz_clears(e,n,s,temp,NULL);
	fclose(pbfile);
	
	if(!(defin)){
		fclose(input);
	}
	if(!(defou)){
		fclose(output);
	}
	return 0;
}

