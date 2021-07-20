/* Command line program to delay or advance all  the subtitles in the given lines of a file srt, The paramaters are:

                program name_of_your_srt_file minutes seconds miliseconds initial-line final-line

If you want to delay the paramaters must be negative, the contrary goes to advance.  
If you won't use some time paramater just put it as a zero. 
Example: sync legenda.srt 0 -2 -100 ( to delay all the subtitles of the legenda.srt file in two seconds and 100 miliseconds)*/

#include "macros_and_funcs.h"

int main(int argc, char *argv[]){
    flag all_subtitles = ON;
    int first_subtitle, last_subtitle, count;
    struct time time_arg;
    FILE *arqinp, *arqout;

    arqinp = NULL;
    arqout = NULL;

    // Verifying arguments
    if (argc != 5 && argc != 7){
        fprintf(stderr, "Error: Number of arguments invalid: sync namefile.srt minutes seconds miliseconds or\nsync namefile.srt minutes seconds miliseconds first_subtitle last_subtitle");
        return EXIT_FAILURE;
    }


    // Opening files
    arqinp = fopen(argv[1], "r");
    arqout = fopen(NAME_FILE_OUT, "w");
    
    // Exiting the function in case something goes wrong
    if (arqinp == NULL){
        fprintf(stderr, "Error: Input file couldn't be opened correctly");
        return EXIT_FAILURE;
    }
    if (arqout == NULL){
        fprintf(stderr, "Error: Output file couldn't be opened correctly");
        return EXIT_FAILURE;
    }

    // Passing args from the command line to the struct
    time_arg.minutes = myatoi(argv[2], &count);
    error_arguments(count);
    time_arg.seconds = myatoi(argv[3], &count);
    error_arguments(count);
    time_arg.miliseconds = myatoi(argv[4], &count);
    error_arguments(count);

    if (argc == 7){
        all_subtitles = OFF;
        first_subtitle = myatoi(argv[5], &count);
        error_arguments(count);
        last_subtitle = myatoi(argv[6], &count);
        error_arguments(count);
    }

    int count_subtitles = 1;
    // Copying the file to another one and changing the time if it's in the subtitles chosen. 
    while (!feof(arqinp)){
        if (all_subtitles == ON || (count_subtitles >= first_subtitle && count_subtitles <= last_subtitle))
            read_and_modify(arqinp, arqout, time_arg);
        else
            read_and_copy(arqinp, arqout, count_subtitles); 
        count_subtitles++;
    }


    fclose(arqout);
    fclose(arqinp);

    return EXIT_SUCCESS; 
}

void read_and_copy(FILE *arqinp, FILE *arqout, int count_subtitles){
    char strptr[CHAR_SIZE];
    char character_break;
    fgets(strptr, CHAR_SIZE, arqinp);
    while (*strptr != '\n'){ // Loop to copy the subtitle's content
        fprintf(arqout, strptr);
        fgets(strptr, CHAR_SIZE, arqinp);
        }
    putc('\n', arqout);

    while ((character_break = getc(arqinp)) == '\n'); // Loop to consume the break line chars
    if (character_break != EOF){ 
        putc(character_break, arqout);
    if (count_subtitles < 10){ // If the subtitles is in the first ten, we have to consume the break line char too.
        character_break = getc(arqinp);
        putc(character_break, arqout);
        }
    }
}

// Read and modify one line
void read_and_modify(FILE *arqinp, FILE *arqout, struct time time_arg){
    char character_break;
    char strptr[CHAR_SIZE];
    // Copying the file to another one and changing the time. 
    fprintf(arqout, fgets(strptr, CHAR_SIZE, arqinp)); // Copying the number. ( first line )
    sync(arqinp, arqout, strptr, time_arg); // Copying the time of the subtitle and changing it.
    fgets(strptr, CHAR_SIZE, arqinp);
    while (*strptr != '\n'){ // Loop to copy the subtitle's content
        fprintf(arqout, strptr);
        fgets(strptr, CHAR_SIZE, arqinp);
    }
    putc('\n', arqout);
    while ((character_break = getc(arqinp)) == '\n'); // Loop to consume the new line characters
    if (character_break != EOF)
        putc(character_break, arqout);
    return;
}


// It will write the time ( after it has been fixed) in the new file.  
void sync(FILE *arqinp, FILE *arqout, char *strptr, struct time time_arg){
    static flag aux = ON;
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
    if (aux == ON){
        isnegative(timefile_left);
        aux = OFF;
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
    timefile.minutes = myatoi(strtok(NULL, ":"), &count);
    error_reading(count);
    timefile.seconds = myatoi(strtok(NULL, ","), &count);
    error_reading(count);
    timefile.miliseconds = myatoi(strtok(NULL, " "), &count);
    error_reading(count);
    return timefile;
}

// Function to verify if  reading the file was successful. 
void error_reading(int s){
    if (!s){
        fprintf(stderr, "Error: The program couldn't read the time correctly, check the syntax of your srt file");
        exit(EXIT_FAILURE);
    }
}

// Function to verify if  reading the arguments was successful. 
void error_arguments(int s){
    if (!s){ // If s equals 0 then the atoi couldn't read any digit. 
        fprintf(stderr, "Error: The program couldn't read the arguments correctly, remeber to put just numerical value in the time arguments!");
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
    flag minus = OFF;
    if (*s == '-')
        minus = ON;
    int count  = 0, res = 0;
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
    return minus == ON ? -res : res;
}
