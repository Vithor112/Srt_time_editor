#include "macros_and_funcs.h"

// read and copy one block
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

// Read and modify one block
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
