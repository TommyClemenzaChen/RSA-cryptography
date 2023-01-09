
#include <stdio.h>
#include <gmp.h>


#include <stdbool.h>
#include <stdint.h>
#include "numtheory.h"
#include "randstate.h"
#include <stdlib.h>



void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters){

	//allocate bits for each prime num
	//only bits of greater than or equal to 11 will satisfy log_2(n) >= nbits. Tested on calc?
	
	
	mpz_t tot, LCMval, GCDval,tempGCD,qtemp,ptemp;
	mpz_inits(tot, LCMval, GCDval,tempGCD, qtemp, ptemp, NULL);
	
	uint64_t pbits = random() % ((3 * nbits)/4) + nbits/4;

	uint64_t qbits = nbits - pbits;

	//satisfies I think log_2(p) >= nbits
	while(mpz_cmp_ui(p,0)==0){
		make_prime(p,pbits,iters);
	}
	while(mpz_cmp_ui(q,0)==0){
		make_prime(q,qbits,iters);
	}
	//q-1,p-1
	//n = pq
	mpz_mul(n,p,q);
	mpz_sub_ui(ptemp,p,1);
	mpz_sub_ui(qtemp,q,1);
	
	//tot = (p-1)(q-1)	
	mpz_mul(tot, ptemp,qtemp);
	//GCD(p-1,q-1)
	gcd(GCDval, ptemp,qtemp);
	//lCM = tot/GCDval
	
	mpz_fdiv_q(LCMval, tot, GCDval);
	//Stops when the gcd(e,LCMval) is equal to 1
	while(mpz_cmp_ui(tempGCD,1) != 0){
		mpz_urandomb(e, state, nbits);
		if(mpz_cmp_ui(e, 2) <= 0){
			continue;
		}	
		gcd(tempGCD, e, LCMval);
	}
	mpz_clears(tot, LCMval,GCDval, tempGCD ,qtemp, ptemp,NULL);
	

}
//%Zx converts the thing to hex
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile){
	
	gmp_fprintf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n,e,s,username);

}
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile){

	
	gmp_fscanf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n,e,s,username);


}
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q){
	mpz_t LCMval, qtemp, ptemp,GCDval,tot;
	mpz_inits(LCMval, qtemp,ptemp,GCDval,tot, NULL);

	mpz_sub_ui(qtemp, q,1);
	mpz_sub_ui(ptemp, p, 1);

        //tot = (p-1)(q-1)      
        mpz_mul(tot, ptemp,qtemp);
        //GCD(p-1,q-1)
        gcd(GCDval, ptemp,qtemp);
	//lCM = tot/GCDval
        mpz_fdiv_q(LCMval, tot, GCDval);
	

	mod_inverse(d,e,LCMval);
	mpz_clears(LCMval, qtemp,ptemp, GCDval,tot,NULL);

}
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile){
	
	gmp_fprintf(pvfile, "%Zx\n%Zx\n", n,d);

}


void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile){

	gmp_fscanf(pvfile, "%Zx\n%Zx\n", n,d);


}

//encrypt: c = (m^e)%n
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n){
	//MPZ_T rop ALWAYS NEEDS TO BE ONE
	pow_mod(c, m , e, n);
}



// Encrypts an entire file given an RSA public modulus and exponent.
// All mpz_t arguments are expected to be initialized.
// All FILE * arguments are expected to be properly opened.
//
// infile: the input file to encrypt.
// outfile: the output file to write the encrypted input to.
// n: the public modulus.
// e: the public exponent.
//
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e){
		uint64_t blockSize, temp;
		uint8_t* arr;
		mpz_t imported,encrypted;
		mpz_inits(imported,encrypted, NULL);
		//sizeinbase is basically log
		
		mpz_set_ui(encrypted,1);	
		blockSize = (mpz_sizeinbase(n,2)-1)/8;
		arr = malloc(sizeof(uint8_t) * blockSize);
		arr[0] = 0xFF;
		//
		//man feof instad of this
		do{
			
			temp = fread(&arr[1],1, blockSize-1, infile);
			//import to convert to mpz_t
			//(mpz_t rop, size_t count, int order, size_t size, int endian, size_t nails, const void *op)
			//
			
			mpz_import(imported, temp+1, 1, 1, 1,0,arr);
			//encrypt
			
			rsa_encrypt(encrypted, imported, e,n);
			//fprintf convert to hex
			gmp_fprintf(outfile, "%Zx\n", encrypted);
		}while(feof(infile) == 0);





	
		mpz_clears(imported, encrypted, NULL);
		free(arr);


}

//
// Decrypts some ciphertext given an RSA private key and public modulus.
// All mpz_t arguments are expected to be initialized.
//
// m: will store the decrypted message.
// c: the ciphertext to decrypt.
// d: the private key.
// n: the public modulus.
//
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n){
	pow_mod(m, c,d,n);

}

//
// Decrypts an entire file given an RSA public modulus and private key.
// All mpz_t arguments are expected to be initialized.
// All FILE * arguments are expected to be properly opened.
//
// infile: the input file to decrypt.
// outfile: the output file to write the decrypted input to.
// n: the public modulus.
// d: the private key.
//
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d){
		uint64_t blockSize;
                uint64_t blockIndex = 1;
		
		uint8_t * exportedArray;
		
		mpz_t decrypted,scanned;
                mpz_inits(decrypted,scanned, NULL);

                //sizeinbase is basically log
		mpz_set_ui(decrypted,1);
                blockSize = (mpz_sizeinbase(n,2)-1)/8;
		

		
		exportedArray = malloc(sizeof(uint8_t) * blockSize);
                //
                do{
			//fscanf
			gmp_fscanf(infile,"%Zx\n",scanned);
			//decrypt
			rsa_decrypt(decrypted, scanned, d,n);
			//export
			//(void *rop, size_t *countp, int order, size_t size, int endian, size_t nails, const mpz_t op)
			mpz_export(exportedArray, &blockIndex, 1,1,1,0, decrypted);

			fwrite(exportedArray+1,1, blockIndex-1, outfile);
                }while(feof(infile) == 0);


                mpz_clears(decrypted, scanned, NULL);

}

//
// Signs some message given an RSA private key and public modulus.
// All mpz_t arguments are expected to be initialized.
//
// s: will store the signed message (the signature).
// m: the message to sign.
// d: the private key.
// n: the public modulus.
//
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n){
	pow_mod(s,m,d,n);

}

//
// Verifies some signature given an RSA public exponent and modulus.
// Requires the expected message for verification.
// All mpz_t arguments are expected to be initialized.
//
// m: the expected message.
// s: the signature to verify.
// e: the public exponent.
// n: the public modulus.
// returns: true if signature is verified, false otherwise.
//
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n){
	mpz_t t;
	mpz_inits(t,NULL);

	pow_mod(t,s,e,n);
	//gmp_printf("%Zd,%Zd",t,m);
	if(mpz_cmp(t,m) == 0){
			
		mpz_clears(t,NULL);
		return true;
	}
	

	mpz_clears(t,NULL);
	return false;
}
