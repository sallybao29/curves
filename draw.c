#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"


/*======== void add_circle() ==========
  Inputs:   struct matrix * points
	double cx
	double cy
	double y
	double step  
  Returns: 


  03/16/12 19:53:52
  jdyrlandweaver
  ====================*/
void add_circle( struct matrix * points, 
								 double cx, double cy, 
								 double r, double step ) {

  double x0, y0, x, y, t;

  x0 = r + cx;
  y0 = cy;
  for (t = step; t < 1.0000001; t+= step){
    x = r * cos(2 * M_PI * t) + cx;
		y = r * sin(2 * M_PI * t) + cy;
		add_edge(points, x0, y0, 0, x, y, 0);
    x0 = x;
    y0 = y;
  }
}


/*======== void add_curve() ==========
	Inputs:   struct matrix *points
	double x0
	double y0
	double x1
	double y1
	double x2
	double y2
	double x3
	double y3
	double step
	int type  
	Returns: 

	Adds the curve bounded by the 4 points passsed as parameters
	of type specified in type (see matrix.h for curve type constants)
	to the matrix points

	03/16/12 15:24:25
	jdyrlandweaver
	====================*/
void add_curve( struct matrix *points, 
								double x0, double y0, 
								double x1, double y1, 
								double x2, double y2, 
								double x3, double y3, 
								double step, int type ) {

	struct matrix* coefs_x, *coefs_y;
	double h0, v0, h, v, t;

	if (type == HERMITE_MODE){
		coefs_x = generate_curve_coefs(x0, x1, x2, x3, HERMITE_MODE);
		coefs_y = generate_curve_coefs(y0, y1, y2, y3, HERMITE_MODE);
	}
	else if (type == BEZIER_MODE){
		coefs_x = generate_curve_coefs(x0, x1, x2, x3, BEZIER_MODE);
		coefs_y = generate_curve_coefs(y0, y1, y2, y3, BEZIER_MODE);
	}
	else {
		printf("Not a valid type. Must be either 0 or 1\n");
		return;
	}

	h0 = coefs_x -> m[3][0];
	v0 = coefs_y -> m[3][0];
	for (t = step; t < 1.0000001; t+= step){
		h = t * (t * (coefs_x -> m[0][0] * t +
									coefs_x -> m[1][0]) +
						 coefs_x -> m[2][0]) + coefs_x -> m[3][0];
		v = t * (t * (coefs_y -> m[0][0] * t +
									coefs_y -> m[1][0]) +
						 coefs_y -> m[2][0]) + coefs_y -> m[3][0]; 
	
		add_edge(points, h0, v0, 0, h, v, 0);
		h0 = h;
		v0 = v;
	}
}

/*======== void add_point() ==========
	Inputs:   struct matrix * points
	int x
	int y
	int z 
	Returns: 
	adds point (x, y, z) to points and increment points.lastcol
	if points is full, should call grow on points
	====================*/
void add_point( struct matrix * points, double x, double y, double z) {
  
	if ( points->lastcol == points->cols )
		grow_matrix( points, points->lastcol + 100 );

	points->m[0][points->lastcol] = x;
	points->m[1][points->lastcol] = y;
	points->m[2][points->lastcol] = z;
	points->m[3][points->lastcol] = 1;

	points->lastcol++;
}

/*======== void add_edge() ==========
	Inputs:   struct matrix * points
	int x0, int y0, int z0, int x1, int y1, int z1
	Returns: 
	add the line connecting (x0, y0, z0) to (x1, y1, z1) to points
	should use add_point
	====================*/
void add_edge( struct matrix * points, 
							 double x0, double y0, double z0, 
							 double x1, double y1, double z1) {
	add_point( points, x0, y0, z0 );
	add_point( points, x1, y1, z1 );
}

/*======== void draw_lines() ==========
	Inputs:   struct matrix * points
	screen s
	color c 
	Returns: 
	Go through points 2 at a time and call draw_line to add that line
	to the screen
	====================*/
void draw_lines( struct matrix * points, screen s, color c) {

	int i;
 
	if ( points->lastcol < 2 ) {
    
		printf("Need at least 2 points to draw a line!\n");
		return;
	}

	for ( i = 0; i < points->lastcol - 1; i+=2 ) {

		draw_line( points->m[0][i], points->m[1][i], 
							 points->m[0][i+1], points->m[1][i+1], s, c);
	} 	       
}

void draw_line(int x0, int y0, int x1, int y1, screen s, color c) {
 
	int x, y, d, dx, dy;

	x = x0;
	y = y0;
  
	//swap points so we're always draing left to right
	if ( x0 > x1 ) {
		x = x1;
		y = y1;
		x1 = x0;
		y1 = y0;
	}

	//need to know dx and dy for this version
	dx = (x1 - x) * 2;
	dy = (y1 - y) * 2;

	//positive slope: Octants 1, 2 (5 and 6)
	if ( dy > 0 ) {

		//slope < 1: Octant 1 (5)
		if ( dx > dy ) {
			d = dy - ( dx / 2 );
  
			while ( x <= x1 ) {
				plot(s, c, x, y);

				if ( d < 0 ) {
					x = x + 1;
					d = d + dy;
				}
				else {
					x = x + 1;
					y = y + 1;
					d = d + dy - dx;
				}
			}
		}

		//slope > 1: Octant 2 (6)
		else {
			d = ( dy / 2 ) - dx;
			while ( y <= y1 ) {

				plot(s, c, x, y );
				if ( d > 0 ) {
					y = y + 1;
					d = d - dx;
				}
				else {
					y = y + 1;
					x = x + 1;
					d = d + dy - dx;
				}
			}
		}
	}

	//negative slope: Octants 7, 8 (3 and 4)
	else { 

		//slope > -1: Octant 8 (4)
		if ( dx > abs(dy) ) {

			d = dy + ( dx / 2 );
  
			while ( x <= x1 ) {

				plot(s, c, x, y);

				if ( d > 0 ) {
					x = x + 1;
					d = d + dy;
				}
				else {
					x = x + 1;
					y = y - 1;
					d = d + dy + dx;
				}
			}
		}

		//slope < -1: Octant 7 (3)
		else {

			d =  (dy / 2) + dx;

			while ( y >= y1 ) {
	
				plot(s, c, x, y );
				if ( d < 0 ) {
					y = y - 1;
					d = d + dx;
				}
				else {
					y = y - 1;
					x = x + 1;
					d = d + dy + dx;
				}
			}
		}
	}
}

