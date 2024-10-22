#include "metric.h"
#include "pq.h"
#include "text.h"

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINE_SIZE 1024
#define OPTIONS   "d:k:n:l:emcvh"

static inline uint32_t min(uint32_t x, uint32_t y) {
    return x < y ? x : y;
}

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Identifies the author of some anonymous text passed through stdin.\n"
        "   Basically computes k-nearest neighbors, where k = 1 by default.\n"
        "   The number of noise words that are removed is tunable.\n"
        "   Uses either Euclidean, Manhattan, or cosine distance as metric.\n"
        "\n"
        "USAGE\n"
        "   %s [OPTIONS]\n"
        "\n"
        "OPTIONS\n"
        "   -d database     Database of authors and texts [default: lib.db]\n"
        "   -k matches      Set number of top matches to display [default: 5]\n"
        "   -n noisefile    Set file of words to ignore [default: noise.txt]\n"
        "   -l limit        Set noise word limit [default: 100]\n"
        "   -e              Set distance metric as Euclidean distance [default]\n"
        "   -m              Set distance metric as Manhattan distance\n"
        "   -c              Set distance metric as cosine distance\n"
        "   -v              Enable verbose printing of program run\n"
        "   -h              Display program help and usage\n",
        exec);
}

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t k = 5;
    bool verbose = false;
    Metric metric = EUCLIDEAN;
    char *dbpath = "lib.db";
    char *noisepath = "noise.txt";

    // Will be set in make noise and sample text
    Text *noise, *sample;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'd': dbpath = optarg; break;
        case 'k': k = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'n': noisepath = optarg; break;
        case 'l': noiselimit = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'e': metric = EUCLIDEAN; break;
        case 'm': metric = MANHATTAN; break;
        case 'c': metric = COSINE; break;
        case 'v': verbose = true; break;
        case 'h': usage(argv[0]); return EXIT_SUCCESS;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }

    // Open up the database of authors and texts.
    FILE *db = fopen(dbpath, "r");
    if (!db) {
        perror(dbpath);
        return EXIT_FAILURE;
    }

    // Make the noise and sample text
    {
        // Open up noise word list.
        FILE *noisefile = fopen(noisepath, "r");
        if (!noisefile) {
            perror(noisepath);
            return EXIT_FAILURE;
        }

        // Create noise word text and load the unidentified text from stdin.
        noise = text_create(noisefile, NULL);
        sample = text_create(stdin, noise);

        // Close the noise file
        fclose(noisefile);
    }

    // Get the number of texts and make a pq that can hold them all
    uint32_t texts = 0;
    fscanf(db, "%" SCNu32 "\n", &texts);
    PriorityQueue *pq = pq_create(texts);

    // A Text object is created for each database listing. The created Text is
    // compared with the unidentified text and the distance is
    // computed. Invalid listings are ignored.
    char author[LINE_SIZE];
    char file[LINE_SIZE];
    for (uint32_t i = 0; i < texts; i += 1) {
        // Scan in the author and path to text.
        fgets(author, LINE_SIZE, db);
        fgets(file, LINE_SIZE, db);

        // Knock off the newlines left by fgets().
        author[strlen(author) - 1] = '\0';
        file[strlen(file) - 1] = '\0';

        // Set distance to max if the file couldn't be opened.
        FILE *textfile = fopen(file, "r");
        if (!textfile) {
            perror(file);
            continue;
        }

        // Read text and compute distance
        Text *text = text_create(textfile, noise);
        double dist = text_dist(sample, text, metric);
        enqueue(pq, strdup(author), dist);

        // Clean up
        text_delete(&text);
        fclose(textfile);

        if (verbose) {
            printf("Processed %s [%lf] (%s)\n", file, dist, author);
        }
    }

    printf("Top %" PRIu32 ", metric: %s, noise limit: %" PRIu32 "\n", min(k, texts),
        metric_names[metric], noiselimit);

    // Who might be the mystery authors?
    uint32_t print_count = min(k, pq_size(pq));
    for (uint32_t i = 0; i < print_count; i += 1) {
        char *author;
        double dist;
        dequeue(pq, &author, &dist);
        printf("%" PRIu32 ") %s [%16.15lf]\n", i + 1, author, dist);
        free(author);
    }

    // Cleanup.
    fclose(db);
    pq_delete(&pq);
    text_delete(&sample);
    text_delete(&noise);
    return EXIT_SUCCESS;
}