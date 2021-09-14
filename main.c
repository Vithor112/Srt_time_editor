/* Command line program to delay or advance all  the subtitles in the given subtitles blocks of a file srt, The paramaters are:

                program name_of_your_srt_file minutes seconds miliseconds initial-line final-line 
    
    if you want to modify the time of all the subtitles 
    Or if you want to choose specific block you can then:

                program name_of_your_srt_file minutes seconds miliseconds number-first-block number-last-block

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
        fprintf(stderr, "Error: Number of arguments invalid: sync namefile.srt minutes seconds miliseconds or\n\tsync namefile.srt minutes seconds miliseconds first_subtitle last_subtitle");
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
