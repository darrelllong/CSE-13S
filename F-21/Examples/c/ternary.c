#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static inline char *strngdup(char *s) { return strcpy(malloc(strlen(s) + 1), s); }

#define TRITS 42 // ceiling(64 Log[2] / Log[3]) + 1

static inline bool palindrome(char *s) {
    char *t = s + strlen(s);
    while (t > s) {
        if (*s++ - *--t) {
            return false;
        }
    }
    return true;
}

static char b[TRITS]; // A buffer to hold all of the trits

char *cvtTernary(int64_t n) {
    char *t = b + TRITS;
    *--t = '\0'; // string terminator
    bool neg = n < 0 ? n = -n : false;
    do {
        int64_t d = n / 3;
        int64_t r = n % 3; // equivalence class of the trit
        if (r > 1) {
            d += 1; // (d + 1) a**n - 1 a**(n - 1) = d a**n + 2 a**(n - 1)
            r -= 3; // -1 ≡ 2 (mod 3)
        }
        *--t = "-01"[r + 1]; // More cleverness!
        n = d;
    } while (n > 0);
    if (neg) { // Negation means we flip the sign of the coefficients
        for (char *p = t; *p != '\0'; p += 1) {
            switch (*p) {
            case '-': *p = '1'; break;
            case '1': *p = '-'; break;
            }
        }
    }
    return strngdup(t);
}

static inline int64_t value(char v) {
    switch (v) {
    case '-': return -1;
    case '0': return  0;
    case '1': return +1;
    default: return 1 << 30; // Yuck! Make it obviously wrong. Suggest a cleaner solution!
    }
}

int64_t cvtInteger(char *s) {
    int64_t sum = 0;
    for (int64_t i = 0; i < strlen(s); i += 1) {
        sum *= 3;
        sum += value(s[i]); // The only difference is the coefficients
    }
    return sum;
}

void TeXMode(char *s) {
    printf("${");
    while (*s) {
    switch (*s++) {
        case '-': printf("\\bar{1}"); break;
        case '0': printf("0"); break;
        case '1': printf("1"); break;
        default: printf("ERR");
        }
    }
    printf("}$\n");
    return;
}

#define OPTIONS "s:e:"

int main(int argc, char **argv) {
    int64_t start = 0, end = 10;

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 's':
            start = strtoq(optarg, NULL, 10);
            break;
        case 'e':
            end = strtoq(optarg, NULL, 10);
            break;
        }
    }
    printf("From %" PRIi64 " to %" PRIi64 "\n", start, end);

    for (int64_t i = start; i < end; i += 1) {
        char *t = cvtTernary(i);
        printf("%" PRIi64 " = %s = %" PRIi64 "%s\t", i, t, cvtInteger(t), palindrome(t) ? " is a palindrome" : "");
        TeXMode(t);
        free(t);
    }
    return 0;
}
