#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define NUM_SIDES      10
#define NUM_TRIALS 100000

void usage(void)
{
    printf("usage: vtm_dice <num dice> <difficulty>\n");
    printf("usage: <num dice> must be greater than 0\n");
    printf("usage: <difficulty> must be between 2 and 10 inclusive\n");
    exit(1);
}

/*
 * Calculates probabilities of rolling each number of successes
 * plus probability for botching
 */
void prob_success(int ndice, int diff)
{
    int i, j, roll, succ = 0;
    int *num_succ;

    // we need indexes for 0 through ndice successes,
    // plus an extra for tracking botches
    num_succ = malloc((ndice+2) * sizeof(int)); // last index is for botch
    if (num_succ == NULL) {
        printf("num_succ allocation failed\n");
        return;
    }
    memset(num_succ, 0, (ndice+2) * sizeof(int)); // zero it out

    for (i=0; i<NUM_TRIALS; i++) {
        int s = 0;
        for (j=0; j<ndice; j++) {
            roll = (rand() % NUM_SIDES) + 1;
            if (roll >= diff) {
                s++;
            } else if (roll == 1) {
                s--;
            }
        }

        // now fill in array
        if (s < 0) { // botch
            num_succ[ndice+1]++;
        } else {
            num_succ[s]++;
        }
    }
    
    // print results
    for (i=0; i<ndice+1; i++) {
        printf("%2d successes: %f\n", i,
               (float)num_succ[i] / (float)NUM_TRIALS);
    }
    printf("botch:        %f\n",
           (float)num_succ[ndice+1] / (float)NUM_TRIALS);
    
    free(num_succ);
}

/*
 * Average number of successes rolled
 */
void avg_success(int ndice, int diff)
{
    int i, j, roll, succ = 0;

    // count total successes, subtracting rolls of 1, then divide by
    // total number of trials to get average number of successes
    for (i=0; i<NUM_TRIALS; i++) {
        for (j=0; j<ndice; j++) {
            roll = (rand() % NUM_SIDES) + 1;
            if (roll >= diff) {
                succ++;
            } else if (roll == 1) {
                succ--;
            }
        }
    }
    
    printf("Avg num of successes: %f\n", (float)succ / (float)NUM_TRIALS);
}

int main(int argc, char *argv[])
{
    int i, j, ndice, diff;
    time_t t;

    if (argc != 3)
        usage();

    // grab arguments
    sscanf(argv[1], "%d", &ndice);
    sscanf(argv[2], "%d", &diff);
    printf("Num of dice: %3d\n", ndice);
    printf("Difficulty:  %3d\n\n", diff);

    // sanity checks
    if (ndice < 1)
        usage();

    if (diff < 2 || diff > 10)
        usage();

    // seed rng
    srand((unsigned) time(&t));
    
    // let 'er rip
    prob_success(ndice, diff);
    printf("\n");
    avg_success(ndice, diff);

    return 0;
}
