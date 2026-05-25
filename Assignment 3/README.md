# Assignment 3: Password Generation, PRNG, and Entropy Analysis - by Kyle Parker

## Description
An assignment.

## How to compile & run:
```bash
make makefile
```

## Changes Made
1. Fix include statements. User-written headers should always come after the system heads, except for very limited cases.
2. prng.c
    1. Add return satement for lcg compution with comment
3. alphabet.c
    1. Make code more readable by breaking down funcs
    2. Create a union of alphabets as per specs and validate graphical chars.
4. pw_generator.c
    1. Update random_index_in_range
    2. Finish generate_password
5. information_content.c
    1. Implement given `information_content(int,size_t)` function
    2. Add `displayInfoContentForPasswords(char**,size_t,char*)`
6. fileio.c

## General
- Fix include statements.
    
## Part 1: Implement a PRNG
- Use prng_next() to recieve the next random number based on a seed.
- To set the seed, please use prng_seed(uint) and pass in an unisnged int.
- State of random number generation (RNG) is handled internally when prng_next() is used.

## Part 2: Password Alphabet Construction
- Use simple loop to combine custom alphabet and other chars per flags.

## Part 3: Password Generation
- Use rejection sampling to implment `random_index_in_range`.
- Rejection sampling ensures there is no index out of bound (and is safe guarded through the % operator in return statement.)
- Implement `generate_password` with a simple for loop.
- Add `generate_n_passwords` to handle multiple passwords in house.
- Add random index to the list of random numbers.

## Part 4: Information Content
- Implemented simple equation
- Add function to display all passwords in a convient wrapper.
    - Call `displayInfoContentForPasswords` by passing in the passwords, num of passwords, and alphabet in use.
    
## Part 5: File I/O
- Use `perror` over `fprintf(stderror, ...)` due to error numbers set internally are automatically read and printed.
- Check for complete and partial r/w failures


## Part 9
### What the program does:
Sample codes showing the insecurity of implementing PRNGs at a novel understanding.

### How to compile
```bash
make
```

### How to clean
```bash
make clean
```

### How to run
```bash
./pwgen
```

### Example cli usage
Gen 5 pw, 16 char long all alphabets
```bash
./pwgen 16 5 -luds
```

Gen 5 pw, 14 char long uppercase alphabet, plus `123`
```bash
./pwgen 14 4 -u 789
```

### Arguments
```bash
pwgen length quantity [-luds] [alphabet]

l = lowercase alpha abcdefghijklmnopqrstuvwxyz
u = uppercase alpha ABCDEFGHIJKLMNOPQRSTUVWXYZ
d = digits 0123456789
s = symbols ~!@#$%^&*()_+-={}[]|:;<>,.?/

length: Length of each password (positive integer)
quantity: Number of passwords to generate (positive integer)
alphabet: Optional custom characters to include in the alphabet
```

### LCG & limitations
Only useful for high-school demostrations of how number generators work. Key generation with LCGs result in insecure practices. All output is know prior to running, assuming the input is known.

### Simple PRNG inappropriate
Does not provide an unpredictable enviorment for generating random numbers. Can minipulate the algorithm to produce a series of known output, thus being able to break passwords generated.

# Security Discussion Req

- Why does the LCG appear random at first?
    - It may appear random because the values are chaning upto the highest value of an unsigned int (18,446,744,073,709,551,615 on 64-bit). But evenutally it will wrap around and start generating the same or similar-enough numbers.
- Why is it still predictable?
    - It is predictable because the formula is consistant for the same seed. There will never be an input i that produces o1 or o2, there is a 1:1 mapping, always.
- Why is predictability dangerous in password generation?
    - To reproduce a password generation, we simply need to know (1) the seed used, (2) # of pws generated, (3) algorithm/processes used.
- What would be a better choice?
    - arc4random on unix systems is a good option for moderate security, but frameworks specifically built for cryptography will provide the absolute highest security and randomness. Hardware also plays into how secure the password generation is.
