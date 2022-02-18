#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define OPTIONS "hvi:o:"

// Used to keep track of compressed/decompressed file sizes
uint64_t bytes_read;
uint64_t bytes_written;

static uint8_t buf[BLOCK] = { 0 };
static uint64_t decoded_symbols = 0;
static uint64_t counter = 0;
// Traverses the tree and buffers each symbol based on the bits
// that are read in from the infile. Once the buffer is full or
// the number of decoded symbols equals the file size, it is
// written to outfile.
//
// infile: contains the bits that represent the codes
// outfile: where to print out the buffered symbols
// current_node: keeps track of the current node
// root_node: keeps track of root node so we can return to it
// file_size: how many bytes need to be printed out
void traverse(int infile, int outfile, Node *current_node, Node *root_node, uint64_t file_size) {
    uint8_t bit = 0;
    if (current_node && root_node) {
        while (decoded_symbols < file_size) {
            // Read the next bit, traverse left or right depending on value
            read_bit(infile, &bit);
            if (bit == 0) {
                current_node = current_node->left;
            } else {
                current_node = current_node->right;
            }

            // If a leaf node is reached, buffer symbol and return to root node
            if (!current_node->left && !current_node->right) {
                buf[counter] = current_node->symbol;
                decoded_symbols += 1;
                counter += 1;
                current_node = root_node;
            }

            if (counter == BLOCK) {
                // Write the full buffer to outfile
                write_bytes(outfile, buf, BLOCK);
                counter = 0;
            } else if (decoded_symbols == file_size) {
                // Write the decoded message to outfile
                write_bytes(outfile, buf, counter);
            }
        }
        return;
    }
}

// Prints out the usage information and then exits the program
void print_help() {
    fprintf(stderr, "SYNOPSIS\n"
                    "  A Huffman decoder.\n"
                    "  Decompresses a file using the Huffman coding algorithm.\n\n"
                    "USAGE\n"
                    "  ./decode [-h] [-v] [-i infile] [-o outfile]\n\n"
                    "OPTIONS\n"
                    "  -h             Program usage and help.\n"
                    "  -v             Print compression statistics.\n"
                    "  -i infile      Input file to decompress.\n"
                    "  -o outfile     Output of decompressed data.\n");
    exit(0);
}

// Prints out the compressed and decompressed file sizes and the
// percentage of space saved.
//
// compressed_size: size (bytes) of the compressed input file
// decompressed_size: size (bytes) of the decompressed output file
void print_stats(uint64_t compressed_size, uint64_t decompressed_size) {
    float space_saving = 100 * (1 - ((double) compressed_size / (double) decompressed_size));
    fprintf(stderr,
        "Compressed file size: %lu\n"
        "Decompressed file size: %lu\n"
        "Space saving: %.2f%s\n",
        compressed_size, decompressed_size, space_saving, "%");
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
        case 'i': infile = open(optarg, O_RDONLY); break;
        case 'o': outfile = open(optarg, O_WRONLY | O_TRUNC | O_CREAT); break;
        case 'v': stats = true; break;
        case '?': help = true; break;
        default: help = true; break;
        }
    }

    // Print out the usage information to stderr
    if (help) {
        print_help();
    }

    // Read in the header from infile
    Header header;
    Header *header_ptr = &header;
    uint8_t *bytes_ptr = (uint8_t *) header_ptr;
    read_bytes(infile, bytes_ptr, sizeof(Header));

    // If the magic number is invalid, end the program
    if (header.magic != MAGIC) {
        fprintf(stderr, "Invalid magic number.\n");
        exit(0);
    }

    // Copy the outfile permissions from the header
    fchmod(outfile, header.permissions);

    // Read the dumped tree into an array
    uint8_t dumped_tree[header.tree_size];
    read_bytes(infile, &(*dumped_tree), header.tree_size);

    // Reconstruct the Huffman tree starting from root node
    Node *root = rebuild_tree(header.tree_size, dumped_tree);

    // Traverse the tree using codes from infile to decompress the file
    traverse(infile, outfile, root, root, header.file_size);

    // Delete the tree
    delete_tree(&root);

    // Print out file size statistics to stderr
    if (stats) {
        print_stats(bytes_read, bytes_written);
    }

    // Close the infile and outfile
    close(infile);
    close(outfile);
}
