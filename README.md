# Srt_time_editor
> Command line program (made in C) to delay or advance all the subtitles of a srt file. 


   A srt file is a text file that contains the subtitles of a video. Its syntax is made of blocks like this:

*A number ( that represents the number of the block )

The time that the text must appear and dissapear on screen **(hours:minutes:seconds,milliseconds -> hours:minutes:seconds,milliseconds)**

The text (Can be separated by breakline characters)*


### My program 

   It's a command line program that changes the time of a number of blocks (or all of them) at once.
It's useful because saves you time when you need to advance or delay a part or all the subtitles. 

### The parameters

    sync name_file.srt minutes seconds milliseconds first_block last_block

   If you want to delay, the time parameters must be negative. If you won't use one of the time parameters puts it as a zero.
   If you want to change the time of all the blocks, you can omit the two last parameters.



### Compile

   For compile, you need to have CMake installed:  
   
   `  
   
      mkrdir -p build        
      cd build  
      cmake ..  
      cmake --build .
   `   
   
   Then you'll have an executable called **srt_time_editor** in the build file.

