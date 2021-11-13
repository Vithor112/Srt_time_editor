#include "macros_and_funcs.h"

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