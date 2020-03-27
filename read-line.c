/*
 * CS252: Systems Programming
 * Purdue University
 * Example that shows how to read one line with simple editing
 * using raw terminal.
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h> 
char *strdup(const char *src);
#define MAX_BUFFER_LINE 2048

extern void tty_raw_mode(void);

// Buffer where line is stored
int line_length;
char line_buffer[MAX_BUFFER_LINE];
int history_index = 0;
int h_pointer = 0;

char * history[50];
int h_count=0;

int history_length = sizeof(history)/sizeof(char *);

void read_line_print_usage()
{
  char * usage = "\n"
    " ctrl-?       Print usage\n"
    " Backspace    Deletes last character\n"
    " up arrow     See last command in the history\n";

  write(1, usage, strlen(usage));
}

char *strdup(const char *src) {
    char *dst = malloc(strlen (src) + 1);  // Space for length plus nul
    if (dst == NULL) return NULL;          // No memory
    strcpy(dst, src);                      // Copy the characters
    return dst;                            // Return the new string
}

/* 
 * Input a line with some basic editing.
 */
char * read_line() {

  // Set terminal in raw mode
  tty_raw_mode();

  line_length = 0;
  for(int i = 0 ; i < MAX_BUFFER_LINE;i++){
    line_buffer[i] = 0;
  }
  int lc = 0;
  int rc = 0;
  int c = 0;
  
  // Read one line until enter is typed
  while (1) {

    // Read one character in raw mode.
    char ch;
    read(0, &ch, 1);
    if (ch>=32 && ch < 127) {
      // It is a printable character.  
      if (line_length==MAX_BUFFER_LINE-2) 
      break; 

      // Do echo 
      if(lc == 0 && rc == 0)
      write(1,&ch,1);
      
      // If max number of character reached return

      if(lc > 0 || rc > 0){
        int end = line_length+1;
        for( int i = c ; i >=end ; i--){
          
          line_buffer[i]=line_buffer[i-1];
         
        }
        
      }

      
      line_buffer[line_length]=ch;
      line_length++;
      c++;

      if(lc > 0 || rc > 0){
      for(int i = line_length-1 ; i < c;i++){
        char ch = line_buffer[i];
        write(1,&ch,1);
      }
       for (int i = 0; i < c-line_length; i++) {
          ch = 8;
          write(1,&ch,1);
        }
      }
      

      continue;

    }
    else if (ch==10) {
      // <Enter> was typed. Return line
      
      // Print newline
      history[h_count] = strdup(line_buffer);
      h_count+=1;
      h_pointer = h_count ;
      write(1,&ch,1);
      break;
    }
    else if (ch == 31) { 
      // ctrl-?
      read_line_print_usage();
      line_buffer[0]=0;
      break;
    }
    else if (ch == 127) {
      // <backspace> was typed. Remove previous character read.

      if(line_length > 0){
        ch = 8;
        write(1,&ch,1);
        ch = ' ';
        write(1,&ch,1);
        ch = 8;
        write(1,&ch,1);
        line_length--;
        c--;
       
      
      for(int i = line_length; i <=c;i++){
        line_buffer[i]=line_buffer[i+1];
      }
  

         for(int i = line_length; i < c;i++){
           char ch = line_buffer[i];
           write(1,&ch,1);
         }

             ch = ' ';
            write(1,&ch,1);
            ch = 8;
            write(1,&ch,1);
          
            
           for (int i = 0; i < c-line_length; i++) {
            ch = 8;
            write(1,&ch,1);
          }
      

      }

      continue;
    }
    
    else if (ch==27) {
      // Escape sequence. Read two chars more
      //
      // HINT: Use the program "keyboard-example" to
      // see the ascii code for the different chars typed. 
      //
      char ch1; 
      char ch2;
      read(0, &ch1, 1);
      read(0, &ch2, 1);

      if(ch1 == 91 && ch2== 68 ){
      
        if(line_length > 0){
          ch = 27;
          write(1,&ch,1);
          ch = 91;
          write(1,&ch,1); 
          ch = 68;
          write(1,&ch,1);
          line_length--;
          lc+=1;
        } 
        continue;
      }

      if(ch1 == 91 && ch2== 67 ){
      
        if(line_length >= 0 && line_length <=c-1){
          ch = 27;
          write(1,&ch,1);
          ch = 91;
          write(1,&ch,1);
          ch = 67;
          write(1,&ch,1);
          line_length++;
          rc+=1;
        } 
        continue;
      }

      

      if (ch1==91 && ch2==65) {
        // Up arrow. Print next line in history.
        
        if(h_pointer <= 0){
          continue;
        }
        h_pointer-=1;
        
        // Erase old line
        // Print backspaces
        int i = 0;
        for (i =0; i < c; i++) {
          ch = 8;
          write(1,&ch,1);
        }

        // Print spaces on top
        for (i =0; i < c; i++) {
          ch = ' ';
          write(1,&ch,1);
        }

        // Print backspaces
        for (i =0; i < c; i++) {
          ch = 8;
          write(1,&ch,1);
        }	

        // Copy line from history
        strcpy(line_buffer, history[h_pointer]);
        c = strlen(line_buffer);
        line_length =c;

        // echo line
        write(1, line_buffer, c);
        continue;
      }



    }

  }

  // Add eol and null char at the end of string
  line_buffer[c]=10;
  // line_length++;
  c++;
  line_buffer[c]=0;

  return line_buffer;
}

