#include "text.h"
#include "pq.h"
#include <inttypes.h>
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
        "   -k matches      Set number of top matches to display [default: 1]\n"
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
    uint32_t k = 1;
    bool verbose = false;
    Metric metric = EUCLIDEAN;
    char *dbpath = "lib.db";
    char *noisepath = "noise.txt";

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'd':
            dbpath = optarg;
            break;
        case 'k':
            k = (uint32_t)strtoul(optarg, NULL, 10);
            break;
        case 'n':
            noisepath = optarg;
            break;
        case 'l':
            noiselimit = (uint32_t)strtoul(optarg, NULL, 10);
            break;
        case 'e':
            metric = EUCLIDEAN;
            break;
        case 'm':
            metric = MANHATTAN;
            break;
        case 'c':
            metric = COSINE;
            break;
        case 'v':
            verbose = true;
            break;
        case 'h':
            usage(argv[0]);
            return EXIT_SUCCESS;
        default:
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    // Open up the database of authors and texts.
    FILE *db = fopen(dbpath, "r");
    if (!db) {
        perror(dbpath);
        return EXIT_FAILURE;
    }

    // Open up noise word list.
    FILE *noisefile = fopen(noisepath, "r");
    if (!noisefile) {
        perror(noisepath);
        return EXIT_FAILURE;
    }

    // Create noise word text and load the unidentified text from stdin.
    Text *noise = text_create(NULL, noisefile, NULL);
    Text *sample = text_create(NULL, stdin, noise);

    // Sanity check.
    if (verbose) {
        text_print(noise);
    }

    // Setup our library of texts and respective distances.
    uint32_t texts = 0;
    fscanf(db, "%" SCNu32 "\n", &texts);
    Text **library = (Text **)calloc(texts, sizeof(Text *));
    double *dists = (double *)calloc(texts, sizeof(double));
    PriorityQueue *pq = pq_create(texts);

    // A Text object is created for each database listing. The created Text is
    // added to the library and its distance from the unidentified text is
    // computed. Invalid listings are ignored and are left with a maximal
    // distance.
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
        FILE *text = fopen(file, "r");
        if (!text) {
            perror(file);
            dists[i] = 1.0;
            continue;
        }

        // Add to library and compute distance.
        library[i] = text_create(author, text, noise);
        dists[i] = text_dist(sample, library[i], metric);
        enqueue(pq, anode_create(i, dists[i]));
        fclose(text);

        if (verbose) {
            printf("Processed %s [%lf] (%s)\n", file, dists[i], author);
        }
    }

    const char *metrics[] = {
        [EUCLIDEAN] = "Euclidean distance",
        [MANHATTAN] = "Manhattan distance",
        [COSINE] = "cosine distance"
    };

    printf("Top %" PRIu32 ", metric: %s, noise limit: %" PRIu32 "\n",
        min(k, texts), metrics[metric], noiselimit);

    // Who might be the mystery authors?
    for (uint32_t i = 0; i < min(k, texts); i += 1) {
        AuthorNode *n = NULL;
        dequeue(pq, &n);
        printf(" %" PRIu32 ") %s [%16.15lf]\n",
            i + 1, text_author(library[n->index]), n->dist);
        anode_delete(&n);
    }

    // Cleanup.
    for (uint64_t i = 0; i < texts; i += 1) {
        text_delete(&library[i]);
    }
    free(library);
    free(dists);
    fclose(db);
    fclose(noisefile);
    pq_delete(&pq);
    text_delete(&sample);
    text_delete(&noise);
    return EXIT_SUCCESS;
}
