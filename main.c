#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main( int argc, char **argv ) {

  screen s;
  color c;
  struct matrix *edges;
  struct matrix *transform;

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);

  add_circle(edges, 250, 250, 100, 0.01);

  c.blue = MAX_COLOR;
  clear_screen(s);
  //draw_lines(edges, s, c);
  //display(s);

  print_matrix(make_hermite());
  printf("\n");
  print_matrix(make_bezier());
  /*
  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );
  */
  free_matrix( transform );
  free_matrix( edges );
}  
