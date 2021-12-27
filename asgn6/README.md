Assignment 6:
Description:
This program encrypts and decrypts files. It includes three main files: keygen.c, encrypt.c and decrypt.c. Keygen genereates public and private key pairs which are used to encrypt and decrypt the files. encrypt.c reads plaintext from an input file and outputs ciphertext to an output file. decrypt.c then uses the private key to decrypt the file, reading ciphertext from the input file and writing plaintext to the output file.

Directions:
To execute the program navigate to the directory and make the program. Then type ./keygen followed by the command line options. Specify the input and output files by typing -n [pbfile] and -d [pvfile] and then -v if you want verbose printing and -h if you want the help message. You can also type -b [bits] to specify the minimum number of bits for the public modulus n and -i [iters] to specify the number of iterations that the Miller Rabin primality test should run to generate prime numbers.

Once you have generated your public and private key, you can then run encrypt. Type ./ encrypt followed by -i [infile] and -o [outfile] (they are stdin and stdout by default). You can also add -v for verbose printing and -h if you want the help message. Running this will generate the ciphertext in the outfile. ./decrypt followed by -i [infile] and -o [outfile] will decrypt the ciphertext. Be sure to set the outfile of encrypt to the infile of decrypt. You can also specify command line options -v for verbose printing and -h for the help message.
