#include "ht.h"
#include "bf.h"
#include "parser.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <regex.h>
#include <ctype.h>
#include <string.h>

#define OPTIONS "ht:f:s"
#define HT_SIZE 65536
#define BF_SIZE 1048576
#define WORD    "(-*[_A-Za-z0-9]'*)+"

// Prints out the usage information for the program.
// Ends the program after printing out the message.
void print_usage(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "  A word filtering program for the GPRSC\n"
                    "  Filers out and reports bad words parsed from stdin.\n\n"
                    "USAGE\n"
                    "   ./banhammer [-hs] [-t size] [-f size]\n\n"
                    "OPTIONS\n"
                    "   -h           Program usage and help.\n"
                    "   -s           Print program statistics.\n"
                    "   -t size      Specify hash table size (default: 2^16).\n"
                    "   -f size      Specify bloom filter size (default: 2^20).\n");
    exit(0);
}

// Prints out a message if the user commits thoughtcrime and wrongthink.
void print_mixspeak() {
    printf("Dear beloved citizen of the GPRSC,\n\n"
           "We have some good news, and we have some bad news.\n"
           "The good news is that there is bad news. The bad news is that you will\n"
           "be sent to joycamp and subjected to a week-long destitute existence.\n"
           "This is the penalty for using degenerate words, as well as using\n"
           "oldspeak in place of newspeak. We hope you can correct your behavior.\n\n"
           "Your transgressions, followed by the words you must think on:\n\n");
}

// Prints out a message if the user only commits thoughtcrime
void print_thoughtcrime() {
    printf("Dear beloved citizen of the GPRSC,\n\n"
           "You have been caught using degenerate words that may cause\n"
           "distress among the moral and upstanding citizens of the GPSRC.\n"
           "As such, you will be sent to joycamp. It is there where you will\n"
           "sit and reflect on the consequences of your choice in language.\n\n"
           "Your transgressions:\n\n");
}

// Prints out a message if the user only commits wrongthink
void print_wrongthink() {
    printf("Dear beloved citizen of the GPRSC,\n\n"
           "We recognize your efforts in conforming to the language standards\n"
           "of the GPRSC. Alas, you have been caught uttering questionable words\n"
           "and thinking unpleasant thoughts. You must correct your wrongspeak\n"
           "and badthink at once. Failure to do so will result in your deliverance\n"
           "to joycamp.\n\n"
           "Words that you must think on:\n\n");
}

// Print out statistics of the program
void print_stats(
    double bst_size, double bst_height, double traversed, double ht_load, double bf_load) {
    printf("Average BST size: %.6f\n"
           "Average BST height: %.6f\n"
           "Average branches traversed: %.6f\n"
           "Hash table load: %.6f%s\n"
           "Bloom filter load: %.6f%s\n",
        bst_size, bst_height, traversed, ht_load, "%", bf_load, "%");
}

int main(int argc, char **argv) {
    int opt = 0;
    bool help = false;
    bool stats = false;
    bool thoughtcrime = false;
    bool wrongthink = false;
    uint64_t hash_size = HT_SIZE;
    uint64_t bloom_size = BF_SIZE;
    FILE *badspeak_file = fopen("badspeak.txt", "r");
    FILE *newspeak_file = fopen("newspeak.txt", "r");
    char *word = NULL;

    // Credit to Prof. Long in assignment 7 pdf
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return 1;
    }

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': hash_size = atoi(optarg); break;
        case 'f': bloom_size = atoi(optarg); break;
        case 's': stats = true; break;
        case 'h': help = true; break;
        default: help = true; break;
        }
    }

    // If help option is enabled, print the usage guide
    if (help) {
        print_usage();
    }

    // Initialize Hash Table and Bloom Filter
    HashTable *ht = ht_create(hash_size);
    BloomFilter *bf = bf_create(bloom_size);

    // Initialize Binary Search Trees for badspeak and oldspeak words
    Node *badspeak = bst_create();
    Node *oldspeak = bst_create();

    // Read in a list of badspeak words, insert into ht and bf
    char bad_buffer[1024];
    while (fscanf(badspeak_file, "%s\n", bad_buffer) != EOF) {
        ht_insert(ht, bad_buffer, NULL);
        bf_insert(bf, bad_buffer);
    }

    // Read in a list of oldspeak and newspeak pairs, insert only
    // oldspeak into bf, insert both into ht.
    char old_buffer[1024];
    char new_buffer[1024];
    while (fscanf(newspeak_file, "%s %s\n", old_buffer, new_buffer) != EOF) {
        ht_insert(ht, old_buffer, new_buffer);
        bf_insert(bf, old_buffer);
    }

    // Read in words from stdin and check if they are in the bf and ht
    while ((word = next_word(stdin, &re)) != NULL) {
        // Lowercase any uppercase words
        for (uint32_t i = 0; i < strlen(word); i += 1) {
            word[i] = tolower(word[i]);
        }
        // Remove ending apostrophe from plural words ending with s'
        if (strcmp(&word[strlen(word) - 1], "'") == 0) {
            word[strlen(word) - 1] = '\0';
        }
        // If word is not in bf, nothing needs to be done
        if (bf_probe(bf, word)) {
            Node *node = ht_lookup(ht, word);
            // If word is not in ht, there was a false positive
            if (node != NULL) {
                // If word has no newspeak translation, thoughtcrime was committed
                if (node->newspeak == NULL) {
                    badspeak = bst_insert(badspeak, word, NULL);
                    thoughtcrime = true;
                } else { // If word has newspeak translation, wrongthink was committed
                    oldspeak = bst_insert(oldspeak, word, node->newspeak);
                    wrongthink = true;
                }
            }
        }
    }

    // If stats option is enabled, print stats but nothing else
    if (stats) {
        double bst_size = ht_avg_bst_size(ht);
        double bst_height = ht_avg_bst_height(ht);
        double traversed = (double) branches / (double) lookups;
        double ht_load = 100 * ((double) ht_count(ht) / (double) ht_size(ht));
        double bf_load = 100 * ((double) bf_count(bf) / (double) bf_size(bf));
        print_stats(bst_size, bst_height, traversed, ht_load, bf_load);
    } else {
        // If both thoughtcrime and wrongthink was committed, print a message
        // along with the words they used wrongly
        if (thoughtcrime && wrongthink) {
            print_mixspeak();
            bst_print(badspeak);
            bst_print(oldspeak);
            // If only thoughtcrime was committed, print a message along with
            // the words they used wrongly
        } else if (thoughtcrime) {
            print_thoughtcrime();
            bst_print(badspeak);
            // If only wrongthink was committed, print a message along with
            // the words they used wrongly
        } else if (wrongthink) {
            print_wrongthink();
            bst_print(oldspeak);
        }
    }

    // Close the files
    fclose(badspeak_file);
    fclose(newspeak_file);

    // Free memory used for parsing module
    clear_words();

    // Free memory used for regex
    regfree(&re);

    // Free memory used for Hash Table, Bloom Filter, and BSTs
    ht_delete(&ht);
    bf_delete(&bf);
    bst_delete(&badspeak);
    bst_delete(&oldspeak);
}
