# Bitwise Steganography in C - by Kyle Parker

## Description
C program that hides and retrieves binary data inside P3
PPM images using bitwise operations. Encoding stores one payload bit
per pixel by ensuring `green_lsb ^ blue_lsb` equals the payload bit.

## How to Compile
```bash
make
```

## Run
The program provides two modes: encode and decode.

### Encode mode
```bash
./steg encode <input.ppm> <payload.bin> <output.ppm>
```
 <input.ppm>   – P3 PPM file (must have maxval 255) serving as the cover image
 <payload.bin> – Binary file containing the data to hide
 <output.ppm>  – Resulting PPM file with the hidden payload

### Decode mode
```bash
./steg decode <input.ppm> <output.bin>
```
- <input.ppm>   – P3 PPM file containing a hidden payload
- <output.bin>  – File where the extracted payload will be written

## Compile and run
```bash
gcc *c -o steg && ./steg encode in.pmm msg.bin out.pmm
```

## Examples
### Encode a secret message
```bash
./steg encode cover.ppm secret.bin stego.ppm
```

### Decode the hidden message
```bash
./steg decode stego.ppm recovered.bin
```

## Source files overview
 - main.c
    - CLI Parsing
 - ppm.c
    - PPM I/O and ops
- steg.c
    - `void encode(const char *input_ppm, const char *payload, const char *output_ppm);`
    - `void decode(const char *input_ppm, const char *output_file);`
    - `void set_lsb(unsigned char *value, int bit);`
    - `int get_lsb(unsigned char value);`
    - `void write_bit(unsigned char *g, unsigned char *b, int bit);`
    - `int read_bit(unsigned char g, unsigned char b);`
 - utils.c
    - Filesystem and error helpers
