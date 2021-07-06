/* Command line program to delay or advance all  the subtitles of a file srt, The paramaters are:

                program name_of_your_srt_file minutes seconds miliseconds 

If you want to delay the paramaters must be negative, the contrary goes to advance.  
If you don't want to change in one time paramaters just put it as zero
Example: sync legenda.srt 0 -2 0 ( to delay all the subtitles of the legenda.srt file in two seconds)

Compiling just type: gcc srt_time_editor.c -o sync
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

// Functions prototypes
void subtract_measures(int upper_limit, int lower_limit, int *independent_measure, int *dependent_measure);
struct time get_time(char *pointer);
void subtract_time(struct time *time_file, struct time time_arg);
void subtract_measures(int upper_limit, int lower_limit, int *independent_measure, int *dependent_measure);
void isnegative(struct time timefile);
void sync(FILE *arqinp, FILE *arqout, char *strptr, struct time time_arg);


int main(int argc, char *argv[]){
    char strptr[CHAR_SIZE];
    struct time time_arg;
    FILE *arqinp, *arqout;

    arqinp = NULL;
    arqout = NULL;

    // Verifying arguments
    if (argc != 5){
        fprintf(stderr, "Number of arguments invalid: sync namefile.srt minutes seconds miliseconds");
        return EXIT_FAILURE;
    }


    // Opening files
    arqinp = fopen(argv[1], "r");
    arqout = fopen(NAME_FILE_OUT, "w");
    
    // Exiting the function in case something goes wrong
    if (arqinp == NULL){
        fprintf(stderr, "Input file didn't open correctly");
        return EXIT_FAILURE;
    }
    if (arqout == NULL){
        fprintf(stderr, "Output file didn't open correctly");
        return EXIT_FAILURE;
    }

    // Passing args from the command line to the struct
    time_arg.minutes = atoi(argv[2]);
    time_arg.seconds = atoi(argv[3]);
    time_arg.miliseconds = atoi(argv[4]);

    char character_break, aux = 0;
    // Copying the file to another one and changing the time. 
    while (!feof(arqinp)){
        if (aux)
            putc(character_break, arqout);
        else 
            aux++;
        fprintf(arqout, fgets(strptr, CHAR_SIZE, arqinp)); // Copying the number. ( first line )
        sync(arqinp, arqout, strptr, time_arg); // Copying the time of the subtitle and changing it.
        fgets(strptr, CHAR_SIZE, arqinp);
        while (*strptr != '\n'){ // Loop to copy the subtitle's content
            fprintf(arqout, strptr);
            fgets(strptr, CHAR_SIZE, arqinp);
        }
        fprintf(arqout, strptr);
        while ((character_break = getc(arqinp)) == '\n'); // Loop to consume the new line characters
    }


    fclose(arqout);
    fclose(arqinp);

    return EXIT_SUCCESS; 
}


// It will write the time ( after it has been fixed) in the new file.  
void sync(FILE *arqinp, FILE *arqout, char *strptr, struct time time_arg){
    static int aux = 1;
    struct time timefile_left, timefile_right;

    fgets(strptr, CHAR_SIZE, arqinp);

    // Putting string time in a struct ( Tokenzing it)
    timefile_left = get_time(strptr);
    strtok(NULL, " ");
    timefile_right = get_time(NULL);

    // Changing the time
    subtract_time(&timefile_left, time_arg);
    subtract_time(&timefile_right, time_arg);

    if (aux){
        isnegative(timefile_left);
        aux--;
    }

    // Putting it in a new file
    fprintf(arqout, TIME_FORMAT" --> "TIME_FORMAT"\n", TIME_STRUC(timefile_left), TIME_STRUC(timefile_right));
    return;
}

// It will get the time from the srt file and put it in a struct 
struct time get_time(char *pointer){
    struct time timefile;
    timefile.hours = atoi(strtok(pointer, ":"));
    timefile.minutes = atoi(strtok(NULL, ":"));
    timefile.seconds = atoi(strtok(NULL, ","));
    timefile.miliseconds = atoi(strtok(NULL, " "));
    return timefile;
}

// It will sum the miliseconds, seconds and minutes field of the second struct into the first struct. 
void subtract_time(struct time *time_file, struct time time_arg){
    time_file->miliseconds += time_arg.miliseconds;
    subtract_measures(MILISECONDS_LIMIT, NEGATIVE_LIMIT, &(time_file->seconds), &(time_file->miliseconds));
    time_file->seconds += time_arg.seconds;
    subtract_measures(SECONDS_LIMIT, NEGATIVE_LIMIT, &(time_file->minutes), &(time_file->seconds));
    time_file->minutes += time_arg.minutes;
    subtract_measures(MINUTES_LIMIT, NEGATIVE_LIMIT, &(time_file->hours), &(time_file->minutes));
    return;
    }



void subtract_measures(int upper_limit, int lower_limit, int *independent_measure, int *dependent_measure){
    if (*dependent_measure >= upper_limit){
        *dependent_measure -= abs(upper_limit);
        (*independent_measure)++;
    }
    if (*dependent_measure < lower_limit){
        *dependent_measure += abs(upper_limit);
        (*independent_measure)--;
    }
    return;
}


void isnegative(struct time timefile){
    if (ISTIMENEGATIVE){
        fprintf(stderr, "Numeric arguments are too small, time is getting negative, pick bigger ones");
        exit(EXIT_FAILURE);
    }
}
