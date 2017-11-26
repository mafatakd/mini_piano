#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef
#include <ugpio/ugpio.h>
#endif

#define freqC 261
#define freqD 293
#define freqE 329
#define freqF 349
#define freqG 392
#define freqA 440
#define freqB 493




void playFreq(float freq, float timeToPlay);