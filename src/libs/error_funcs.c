#include "macros_and_funcs.h"

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

// IT'll check if some of the time measures is negative. If one of them is, it'll exit the program and print a error.  
void isnegative(struct time timefile){
    if (ISTIMENEGATIVE){
        fprintf(stderr, "Error: Numerical arguments are too small, check the of your first subtitle and pick bigger ones");
        exit(EXIT_FAILURE);
    }
}