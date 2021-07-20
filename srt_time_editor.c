/* Command line program to delay or advance all  the subtitles in the given lines of a file srt, The paramaters are:

                program name_of_your_srt_file minutes seconds miliseconds initial-line final-line

If you want to delay the paramaters must be negative, the contrary goes to advance.  
If you won't use some time paramater just put it as a zero. 
Example: sync legenda.srt 0 -2 -100 ( to delay all the subtitles of the legenda.srt file in two seconds and 100 miliseconds)*/

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

// Functions prototypes
void subtract_measures(int upper_limit, int lower_limit, int *independent_measure, int *dependent_measure);
struct time get_time(char *pointer);
void subtract_time(struct time *time_file, struct time time_arg);
void subtract_measures(int upper_limit, int lower_limit, int *independent_measure, int *dependent_measure);
void isnegative(struct time timefile);
void sync(FILE *arqinp, FILE *arqout, char *strptr, struct time time_arg);
int myatoi(char *s, int *read_count);
void error_reading(int s);


int main(int argc, char *argv[]){
    char strptr[CHAR_SIZE];
    struct time time_arg;
    FILE *arqinp, *arqout;

    arqinp = NULL;
    arqout = NULL;

    // Verifying arguments
    if (argc != 5){
        fprintf(stderr, "Error: Number of arguments invalid: sync namefile.srt minutes seconds miliseconds");
        return EXIT_FAILURE;
    }


    // Opening files
    arqinp = fopen(argv[1], "r");
    arqout = fopen(NAME_FILE_OUT, "w");
    
    // Exiting the function in case something goes wrong
    if (arqinp == NULL){
        fprintf(stderr, "Error: Input file didn't open correctly");
        return EXIT_FAILURE;
    }
    if (arqout == NULL){
        fprintf(stderr, "Error: Output file didn't open correctly");
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

    // It'll check if the time is negative. It'll check just the first time line. 
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
    int count;
    timefile.hours = myatoi(strtok(pointer, ":"), &count);
    error_reading(count);
    timefile.minutes = myatoi(strtok(NULL, ":"), *count);
    error_reading(count);
    timefile.seconds = myatoi(strtok(NULL, ","), &count);
    error_reading(count);
    timefile.miliseconds = myatoi(strtok(NULL, " "), &count);
    error_reading(count);
    return timefile;
}

// Function to verify if the reading was successful. 
void error_reading(int s){
    if (s){
        fprintf(stderr, "Error: The program couldn't read the time correctly, check the syntax of your srt file");
        exit(EXIT_FAILURE);
    }
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


// It'll connect seconds with minutes, minutes with hours, etc. 
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

// IT'll check if some of the time measures is negative. If one of them is, it'll exit the program and print a error.  
void isnegative(struct time timefile){
    if (ISTIMENEGATIVE){
        fprintf(stderr, "Error: Numerical arguments are too small, check the of your first subtitle and pick bigger ones");
        exit(EXIT_FAILURE);
    }
}

// My implementation of atoi. It has a second parameter to count how many characters the function read. 
int myatoi(char *s, int *read_count){
    int count  = 0, minus = *s == '-', res = 0;
    char p;
    if (minus)
        s++;

    p = *s;
    while (isdigit(p)){
        count++;
        res *= 10;
        res += p - 48;
        s++;
        p = *s;
    }
    *read_count = count;
    return minus ? -res : res;
}
