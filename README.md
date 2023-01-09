# README

#### flaws in my code
* In my make public key function, make_prime sometimes generates 0 so I had a loop that kept calling it until it isn't 0

#### README.md
* Talks about the code

#### numtheory.c
* Contains math functions that are used in rsa functions to help encrypt/decrypt/generate keys

#### numtheory.h
* Header file for numtheory.c 
* Allows other functions to access numtheory functions.

#### randstate.c
* functions that generate/intiialize and clear randstate variables.

#### randstate.h
* Allows other functions to be able access the randstate functions

#### rsa.h
* Allows other files and functions to be able to access the rsa functions

#### rsa.c
* Contains functions that create private/public keys and functions that read/write them from and into other files.
* Also has function that can verify and sign a file.

#### DESIGN.pdf
* Talks about each function and what they do
* talks about other functions

#### keygen.c
* Generates keys
* options:
* b : input minimum bits needed for the public mod n(Default = 1024)
* i: inputs number of iteration(default:50)
* n pbfile: specifies public key file(default:rsa.pub)
* d pvfile: specidies the private key file(defaul: rsa.priv)
* s specifies the rand seed for random state initialization(default: the seconds since the unix epoch, give by time(NULL)
* v: enables verbose output
* h: displays program synopsis and usage


#### encrypt.c
* encrypts keys
* options:
* i: specifies the input file to decrypt(default: stdin)
* o: specifies the output file to decrypt(default: stdout)
* -n: specifies file containing the private key( default: rsa.priv)
* -v: enables verbose output
* -h: displays program synopsis and usage


#### decrypt.c
* decrypts keys
* -i: specifies the input file to decrypt(default: stdin)
* o: specifies the output file to decrypt(default: stdout)
* -n: specifies file containing the private key( default: rsa.priv)
* -v: enables verbose output
* -h: displays program synopsis and usage
* Very cool :))))))

#### WRITEUP.c
* Discusses how i tested my code


#### Sources
* Cse13s discord server
* Classmates from cse13s
* https://gmplib.org/manual/Integer-Functions
* Alot of gmp syntax stuff
* man (insert function name)

