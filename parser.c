#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 circle: add a circle to the edge matrix - 
	    takes 3 arguments (cx, cy, r)
	 hermite: add a hermite curve to the edge matrix -
	    takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
	 bezier: add a bezier curve to the edge matrix -
	    takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 translate: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 xrotate: create an x-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 yrotate: create an y-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 zrotate: create an z-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * pm,
                  screen s) {

  FILE *f;
  char line[256];
  char** args;
  color c;
  
  c.red = MAX_COLOR;
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);  

    if (strcmp(line, "quit")){
      exit(0);
    }
    else if (strcmp(line, "line")){
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      args = parse_line(line, ' ');
      add_edge(pm, args[0], args[1], args[2], args[3], args[4], args[5]);
    }
    else if (strcmp(line, "circle")){

    }
    else if (strcmp(line, "hermite")){

    }
    else if (strcmp(line, "bezier")){

    }
    else if (strcmp(line, "ident")){

    }
    else if (strcmp(line, "scale")){

    }
    else if (strcmp(line, "translate")){

    }
    else if (strcmp(line, "xrotate")){

    }
    else if (strcmp(line, "yrotate")){

    }
    else if (strcmp(line, "zrotate")){
    
    }
    else if (strcmp(line, "apply")){
      natrix_mult(transform, pm);
    }
    else if (strcmp(line, "display")){
      draw_lines(pm, s, c);
      display(s);
    }
    else if (strcmp(line, "save")){
      draw_lines(pm, s, c);
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      save_extension(s, line);
    }  
  }
}

  
/*------------------------------ count_tokens ------------------------------*/

/*
  Counts the number of tokens in a string
  Params:
  char *line - input string
  char dlimit - delimiter
  Returns:
  number of tokens
*/

int count_tokens( char *line, char dlimit ){
  char *token = line;
  int n = 0;
  while( token ){
    token = strchr(token, dlimit);
    if( token ){
      token++;
      while (*token == dlimit)
	token++;
    }
    n++;
  }
  return n;
}

/*------------------------------ parse_line ------------------------------*/

/*
  Separates the tokens in a string and returns an array of tokens
  Params:
  char *line - input string
  char dlimit - delimiter
  Returns:
  array of tokens
*/

char** parse_line( char *line, char dlimit ){
  int size = count_tokens(line,dlimit);
  int i = 0;
  char** tokens = (char**)malloc(sizeof(char*)*size+1);
  char sdlimit[2] = {dlimit, '\0'};
  char* tmp;
	
  while (i < size){
    tmp = strsep(&line, sdlimit);
    if (*tmp){
      tmp = trim(tmp);
      tokens[i] = tmp;
      i++;
    }
  }
  tokens[size] = NULL;
  return tokens;
}

/*------------------------------ trim ------------------------------*/

/*
  trims leading and trailing whitespace
  Params:
  char *line - input string
  Returns:
  pointer to the beginning of the trimmed string
*/

char* trim( char *line ){
  int i = 0;
  int len;
  while( *(line+i) == ' '){
    i++;
  }
  line = line+i;
  len = strlen(line);
  i = len-1;
  while( line[i] == ' ' || line[i] == '\n' ){
    i--;
  }
  if (i < len - 1 &&
      (line[i+1] == ' ' ||
       line[i+1] == '\n'))
    line[i+1] = '\0';
  return line;
}

