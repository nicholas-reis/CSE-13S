#include "defines.h"
#include "node.h"
#include "huffman.h"
#include "header.h"
#include "io.h"

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#define OPTIONS "hvi:o:"

// Used for file permissions
static struct stat st;
// Used to keep track of uncompressed/compressed file sizes
uint64_t bytes_read;
uint64_t bytes_written;

// Prints out the usage information and then exits the program
void print_help() {
    fprintf(stderr, "SYNOPSIS\n"
                    "  A Huffman encoder.\n"
                    "  Compresses a file using the Huffman coding algorithm.\n\n"
                    "USAGE\n"
                    "  ./encode [-h] [-v] [-i infile] [-o outfile]\n\n"
                    "OPTIONS\n"
                    "  -h             Program usage and help.\n"
                    "  -v             Print compression statistics.\n"
                    "  -i infile      Input file to compress.\n"
                    "  -o outfile     Output of compressed data.\n");
    exit(0);
}

// Prints out the uncompressed and compressed file sizes and the
// percentage of space saved.
//
// uncompressed_size: size (bytes) of the uncompressed file input
// compressed_size: size (bytes) of the compressed file output
void print_stats(uint64_t uncompressed_size, uint64_t compressed_size) {
    float space_saving = 100 * (1 - ((double) compressed_size / (double) uncompressed_size));
    fprintf(stderr,
        "Uncompressed file size: %lu\n"
        "Compressed file size; %lu\n"
        "Space saving: %.2f%s\n",
        uncompressed_size, compressed_size, space_saving, "%");
}

int main(int argc, char **argv) {
    int opt = 0;
    FILE *input = stdin;
    FILE *output = stdout;
    int infile = fileno(input);
    int outfile = fileno(output);
    bool help = false;
    bool stats = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help = true; break;
        case 'i':
            infile = open(optarg, O_RDONLY);
            fstat(infile, &st);
            break;
        case 'o':
            outfile = open(optarg, O_WRONLY | O_TRUNC | O_CREAT);
            fchmod(outfile, st.st_mode);
            break;
        case 'v': stats = true; break;
        case '?': help = true; break;
        default: help = true; break;
        }
    }

    // Print out the usage information to stderr
    if (help) {
        print_help();
    }

    // Create a histogram of size ALPHABET (256)
    uint64_t histogram[ALPHABET] = { 0 };
    uint8_t buf[BLOCK] = { 0 };
    // Read from infile and increment symbol values in the histogram
    while (read_bytes(infile, buf, BLOCK) > 0) {
        for (uint32_t i = 0; i < BLOCK; i += 1) {
            histogram[buf[i]] += 1;
        }
    }

    // Minimum two elements in the histogram
    histogram[0] += 1;
    histogram[255] += 1;

    // Construct the Huffman Tree
    Node *root = build_tree(histogram);

    // Construct a code table by traversing the tree
    Code table[ALPHABET];
    build_codes(root, table);

    // Keep track of the number of unique symbols
    uint16_t unique_symbols = 0;
    for (uint32_t i = 0; i < ALPHABET; i += 1) {
        if ((histogram[i]) > 0) {
            unique_symbols += 1;
        }
    }

    // Set all the fields of header
    Header header = { .magic = MAGIC,
        .permissions = st.st_mode,
        .tree_size = 3 * unique_symbols - 1,
        .file_size = st.st_size };
    Header *header_ptr = &header;
    uint8_t *bytes_ptr = (uint8_t *) header_ptr;

    // Write the header and dump the tree to outfile
    write_bytes(outfile, bytes_ptr, sizeof(Header));
    dump_tree(outfile, root);

    // Start from beginning of infile
    lseek(infile, 0, SEEK_SET);
    int bytes;
    // Write all the codes of each input byte to outfile
    do {
        bytes = read_bytes(infile, buf, BLOCK);
        for (int i = 0; i < bytes; i += 1) {
            write_code(outfile, &table[buf[i]]);
        }
    } while (bytes > 0);

    // Flush out the rest of the codes
    flush_codes(outfile);

    // Delete the tree
    delete_tree(&root);

    // Print out file size statistics to stderr
    if (stats) {
        print_stats(bytes_read / 2, bytes_written);
    }

    // Close the infile and outfile
    close(infile);
    close(outfile);
}
