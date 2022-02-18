#include "io.h"
#include "code.h"
#include "defines.h"

#include <unistd.h>
#include <stdbool.h>

static int count = 0;
// Keeps track of total bytes read and written
uint64_t bytes_read = 0;
uint64_t bytes_written = 0;

// Used in write_bytes, write_code, and flush_codes
static uint8_t write_buf[BLOCK] = { 0 };
static uint32_t write_index = 0;

// Used in read_bytes and read_bit
static uint8_t read_buf[BLOCK] = { 0 };
static uint32_t read_index = 0;
static uint32_t read_end = -1;

// Credit to Eugene
// Reads a specified number of bytes from infile and
// buffers the contents into a given buffer. Keeps
// track of total bytes read in bytes_read. Return
// the number of bytes read in this function call.
//
// infile: read from this file
// buf: buffer into this buffer
// nbytes: bytes to be read
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    count = 0;
    int bytes = 0;
    // If there are no bytes to read initially, return 0
    if (nbytes <= 0) {
        return bytes;
    }
    // Loops calls to read() until all bytes are read
    do {
        count = read(infile, buf + bytes, nbytes - bytes);
        bytes += count;
        bytes_read += count;
    } while (count > 0);
    return bytes;
}

// Credit to Eugene
// Writes a specified number of bytes to outfile from
// a given buffer. Keeps track of total bytes written
// in bytes_written. Return the number of bytes written
// in this function call.
//
// outfile: write to this file
// buf: write out from this buffer
// nbytes: bytes to be written
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    count = 0;
    int bytes = 0;
    // If there are no bytes to write initially, return 0
    if (nbytes <= 0) {
        return bytes;
    }
    // Loop calls to write() until all bytes are written
    do {
        count = write(outfile, buf + bytes, nbytes - bytes);
        bytes += count;
        bytes_written += count;
    } while (count > 0);
    return bytes;
}

// Credit to Eugene
// Read bits one at a time from a full buffer. Only provide
// the value of one bit each time this function is called.
// Return true if there are still bits to read.
//
// infile: read from this input file
// bit: store the value of the read bit here
bool read_bit(int infile, uint8_t *bit) {
    int bytes = 0;
    if (read_index == 0) {
        bytes = read_bytes(infile, read_buf, BLOCK);
        // Set end to be last valid bit
        if (bytes < BLOCK) {
            read_end = bytes * 8 + 1;
        }
    }

    if ((read_buf[read_index / 8] >> read_index % 8) & 0x1) {
        *bit = 1;
    } else {
        *bit = 0;
    }
    read_index++;

    // Reset read_bit_top if you are done reading
    if (read_index == BLOCK * 8) {
        read_index = 0;
    }
    return read_index != read_end;
}

// Credit to Prof. Long from Code Comments Repo
// Iterate through the entire given code and buffer each
// bit into a buffer. Write out the buffer once it is
// full.
//
// outfile: write to this output file
// c: get the bits from this code
void write_code(int outfile, Code *c) {
    for (uint32_t i = 0; i < code_size(c); i += 1) {
        // Get the bit from the code
        bool bit = code_get_bit(c, i);
        if (bit) {
            // Set the bit at this index
            write_buf[write_index / 8] |= (0x1 << (write_index % 8));
        } else {
            // Clear the bit at this index
            write_buf[write_index / 8] &= ~(0x1 << (write_index % 8));
        }
        write_index++;
        // Write the buffer once it is full
        if (write_index / 8 == BLOCK) {
            write_bytes(outfile, write_buf, BLOCK);
            write_index = 0;
        }
    }
}

// In the case that write_code() does not fill the buffer
// and thus, does not write out the buffer, this function
// will flush out the remaining codes in the buffer.
//
// outfile: write to this output file
void flush_codes(int outfile) {
    // If there are any bits remaining in the buffer
    if (write_index > 0) {
        // Keep track of the bytes needed for remaining bits
        int bytes_needed = write_index / 8;
        if (write_index % 8 != 0) {
            do {
                // Clear out the extra bits in a byte if necessary
                write_buf[write_index / 8] &= ~(0x1 << (write_index % 8));
                write_index += 1;
            } while (write_index % 8 != 0);
            bytes_needed += 1;
        }
        // Write out only the amount of bytes necessary
        write_bytes(outfile, write_buf, bytes_needed);
    }
}
