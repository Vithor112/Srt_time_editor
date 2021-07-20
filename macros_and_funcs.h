#ifndef MACROS
#define MACROS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// STRING MACRO
#define CHAR_SIZE 1000

//  TIME MACROS
#define MILISECONDS_LIMIT 1000
#define SECONDS_LIMIT 60
#define MINUTES_LIMIT 60
#define NEGATIVE_LIMIT 0

//  OUTPUT FILE NAME MACRO
#define NAME_FILE_OUT "Legenda_sinc.srt"

// STRUCT TIME MACROS
#define TIME_STRUC(x) x.hours, x.minutes, x.seconds, x.miliseconds
#define TIME_FORMAT "%02d:%02d:%02d,%03d"
#define ISTIMENEGATIVE timefile.seconds < NEGATIVE_LIMIT || timefile.miliseconds < NEGATIVE_LIMIT || timefile.minutes < NEGATIVE_LIMIT \
                || timefile.hours < NEGATIVE_LIMIT

struct time{
    int hours;
    int minutes;
    int seconds;
    int miliseconds;
};

typedef enum {OFF, ON} flag;

// Functions prototypes
void subtract_measures(int upper_limit, int lower_limit, int *independent_measure, int *dependent_measure);
struct time get_time(char *pointer);
void subtract_time(struct time *time_file, struct time time_arg);
void subtract_measures(int upper_limit, int lower_limit, int *independent_measure, int *dependent_measure);
void isnegative(struct time timefile);
void sync(FILE *arqinp, FILE *arqout, char *strptr, struct time time_arg);
int myatoi(char *s, int *read_count);
void error_reading(int s);
void error_arguments(int s);
void read_and_copy(FILE *arqinp, FILE *arqout, int count_subtitles);
void read_and_modify(FILE *arqinp, FILE *arqout, struct time time_arg);

#endif