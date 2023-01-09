
#include <stdio.h>

#include <gmp.h>

#include <errno.h>
#include "numtheory.h"
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

        bool print = false, in = true,ou = true;
        FILE * pvfile = fopen("rsa.priv","r");
	FILE * input = stdin;
        FILE * output = stdout;
	
        while((opt = getopt(argc, argv, OPTIONS)) != -1) {

                switch(opt) {

                case('i'):
			in = false;
			input = fopen(optarg, "r");
			if(errno > 0){
				fprintf(stderr,"Usage: ./decrypt-dist [options]\n");
                        	fprintf(stderr,"  ./decrypt-dist decrypts an input file using the specified private key file,\n");
                        	fprintf(stderr,"  writing the result to the specified output file.\n");
                        	fprintf(stderr,"    -i <infile> : Read input from <infile>. Default: standard input.\n");
                        	fprintf(stderr,"    -o <outfile>: Write output to <outfile>. Default: standard output.\n");
                        	fprintf(stderr,"    -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.\n");
                        	fprintf(stderr,"    -v          : Enable verbose output.\n");
                        	fprintf(stderr,"    -h          : Display program synopsis and usage.\n");

				return 1;
			}
                        break;
                case('o'):
			ou = false;
			output = fopen(optarg, "w");
                        break;
                case('n'):
			pvfile = fopen(optarg, "r");
                        break;
                case('v'):
                        print = true;
                        break;
                case('h'):


			fprintf(stderr,"Usage: ./decrypt-dist [options]\n");
			fprintf(stderr,"  ./decrypt-dist decrypts an input file using the specified private key file,\n");
			fprintf(stderr,"  writing the result to the specified output file.\n");
			fprintf(stderr,"    -i <infile> : Read input from <infile>. Default: standard input.\n");
			fprintf(stderr,"    -o <outfile>: Write output to <outfile>. Default: standard output.\n");
			fprintf(stderr,"    -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.\n");
			fprintf(stderr,"    -v          : Enable verbose output.\n");
			fprintf(stderr,"    -h          : Display program synopsis and usage.\n");
                        return 0;
			break;

		default:
			fprintf(stderr,"Usage: ./decrypt-dist [options]\n");
                        fprintf(stderr,"  ./decrypt-dist decrypts an input file using the specified private key file,\n");
                        fprintf(stderr,"  writing the result to the specified output file.\n");
                        fprintf(stderr,"    -i <infile> : Read input from <infile>. Default: standard input.\n");
                        fprintf(stderr,"    -o <outfile>: Write output to <outfile>. Default: standard output.\n");
                        fprintf(stderr,"    -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.\n");
                        fprintf(stderr,"    -v          : Enable verbose output.\n");
                        fprintf(stderr,"    -h          : Display program synopsis and usage.\n");



			


			return 1;
			break;
                }
        }
	mpz_t d,n;
        mpz_inits(d,n,NULL);
        
	rsa_read_priv(n,d,pvfile);

	rsa_decrypt_file(input,output,n,d);

	if(print){
		gmp_printf("%Zd\n%Zd\n",n,d);

	}
	if(!(in)){
		fclose(input);
	}
	if(!(ou)){
		fclose(output);

	}
        mpz_clears(d,n,NULL);
}                                                                   
