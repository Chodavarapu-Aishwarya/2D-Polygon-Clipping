///////////////////////////////////////////////////////////////////////////////////////////////////////////////       
//
// Name      :  Chodavarapu Aishwarya
// Roll No.  :  2003112
// Branch    :  CSE
// 
// Q2) Given a polygon with N vertices, and a rectangular window.
//     Clip the polygon against clipping window using Sutherland-Hodgeman polygon clipping algorithm.
//
//
// Interaction:
//
// Rectangle primitve on the left is to draw a rectangle.
// Line primitive on the left is to draw the edges of a polygon.
// Done primitive on the left is to join first and last vertices of the polygon and get the clipped polygon.
// 
// Left click on a box on the left to select a primitive.
// Then left click on the drawing area: once for point, twice for line or rectangle.
// Right click for menu options.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

#include <cstdlib>
#include <vector>
#include <iostream>
#include <list>
#include <GL/glew.h>
#include <GL/freeglut.h> 

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define NUMBERPRIMITIVES 3

// Globals.
static int width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int tempX, tempY; // Co-ordinates of clicked point.
static int isGrid = 1; // Is there grid?


int count = 0;
int polygon_size = 0;
int x_start, y_start;
int x_prev, y_prev;
std::list<float>polygon_x;
std::list<float>polygon_y;
std::list<float>polygon_xs;
std::list<float>polygon_ys;
float RectLeft_x;
float RectLeft_y;
float RectRight_x;
float RectRight_y;

void clip(int polygon_size, float p1, float p2, float p3, float p4)
{
	std::list<float>my_polygon_x;
	std::list<float>my_polygon_y;
	float x_start = polygon_xs.front();
	float y_start = polygon_ys.front();
	while(((polygon_xs.size()==0) == NULL) && ((polygon_ys.size()==0) == NULL))
	{
		float q1x, q1y, q2x, q2y;
		if(polygon_xs.size()==1)
		{
			q1x = polygon_xs.front();
			q1y = polygon_ys.front();
			polygon_xs.pop_front();
			polygon_ys.pop_front();
			q2x = x_start;
			q2y = y_start;
		}
        else
        {
        	q1x = polygon_xs.front();
			q1y = polygon_ys.front();
			polygon_xs.pop_front();
			polygon_ys.pop_front();
			q2x = polygon_xs.front();
			q2y = polygon_ys.front();
        }

        float pos1 = ((p3-p1)*(q1y-p2)) - ((p4-p2)*(q1x-p1));  // Position of first pont w.r.t. clipper line
        float pos2 = ((p3-p1)*(q2y-p2)) - ((p4-p2)*(q2x-p1));  // Position of second pint w.r.t. clipper line
        
        // Case1 : If both points are inside
        if(pos1 < 0 && pos2 < 0)
        {
        	// Only second point is added
        	my_polygon_x.push_back(q2x);
        	my_polygon_y.push_back(q2y);
        }
        // Case2 : If first point is outside and second point is inside
        else if(pos1 >=0 && pos2 < 0)
        {
        	// Ponint of intersection with edge and the second point are added
        	float num1 = ((p1*p4 - p2*p3)*(q1x-q2x)) - ((p1-p3)*(q1x*q2y - q1y*q2x));
        	float den = ((p1-p3)*(q1y-q2y)) - ((p2-p4)*(q1x-q2x));
        	float val1 = num1/den;
        	my_polygon_x.push_back(val1);
        	float num2 = ((p1*p4 - p2*p3)*(q1y-q2y)) - ((p2-p4)*(q1x*q2y - q1y*q2x));
        	float val2 = num2/den;
        	my_polygon_y.push_back(val2);
        	my_polygon_x.push_back(q2x);
        	my_polygon_y.push_back(q2y);
        }
        // Case3 : If first point is inside and second point is outside
        else if(pos1 < 0 && pos2 >= 0)
        {
        	// Only point of intersection with edge is added
        	float num1 = ((p1*p4 - p2*p3)*(q1x-q2x)) - ((p1-p3)*(q1x*q2y - q1y*q2x));
        	float den = ((p1-p3)*(q1y-q2y)) - ((p2-p4)*(q1x-q2x));
        	float val1 = num1/den;
        	my_polygon_x.push_back(val1);
        	float num2 = ((p1*p4 - p2*p3)*(q1y-q2y)) - ((p2-p4)*(q1x*q2y - q1y*q2x));
        	float val2 = num2/den;
        	my_polygon_y.push_back(val2);
        }
        // Case4 : If both points are outside
        else
        {
        	// No points are added
        }
    }
    swap(polygon_xs, my_polygon_x);
    swap(polygon_ys, my_polygon_y);
}

void SutherlandHodgman(int polygon_size)
{
	clip(polygon_size, RectLeft_x, RectLeft_y, RectLeft_x, RectRight_y);
	clip(polygon_size, RectLeft_x, RectRight_y, RectRight_x, RectRight_y);
	clip(polygon_size, RectRight_x, RectRight_y, RectRight_x, RectLeft_y);
	clip(polygon_size, RectRight_x, RectLeft_y, RectLeft_x, RectLeft_y);
}


// Point class.
class Point
{
public:
	Point(float xVal, float yVal, float sizeVal)
	{
		x = xVal; y = yVal; size = sizeVal;
	}
	Point() {};
	void drawPoint(void); // Function to draw a point.
private:
	float x, y; // x and y co-ordinates of point.
	float size; // Size of point.
};

//float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex3f(x, y, 0.0);
	glEnd();
}

// Vector of points.
std::vector<Point> points;

// Iterator to traverse a Point array.
std::vector<Point>::iterator pointsIterator;

// Function to draw all points in the points array.
void drawPoints(void)
{
	// Loop through the points array drawing each point.
	for (auto point : points) { point.drawPoint(); }
}

// Line class.
class Line
{
public:
	Line(float x1Val, float y1Val, float x2Val, float y2Val)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
	}
	Line() {};
	void drawLine();
private:
    float x1, y1, x2, y2; // x and y co-ordinates of endpoints.
};
float arr1[100];
float arr2[100];

// Function to draw a line.
void Line::drawLine()
{
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(x1, y1, 0.0);
	glVertex3f(x2, y2, 0.0);
	glEnd();
}

// Vector of lines.
std::vector<Line> lines;

// Function to draw all lines in the lines array.
void drawLines(void)
{
	// Loop through the lines array drawing each line.
	for (auto line : lines) { line.drawLine(); }
}

// Rectangle class.
class Rect
{
public:
	Rect(float x1Val, float y1Val, float x2Val, float y2Val)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
	}
	Rect() {};
	void drawRectangle();
private:
    float x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices.
};

// Function to draw a rectangle.
void Rect::drawRectangle()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0, 0.0, 1.0);
	glRectf(x1, y1, x2, y2);
}

// Vector of rectangles.
std::vector<Rect> rectangles;

// Function to draw all rectangles in the rectangles array.
void drawRectangles(void)
{
	// Loop through the rectangles array drawing each rectangle.
	for (auto rectangle : rectangles) { rectangle.drawRectangle(); }
}
std::string text1 = "Step1 : Select rectangle in the left menu and draw it on the drawing area.";
std::string text2 = "Step2 : Select line in the left menu and mark vertices of the polygon on the drawing area to connect them by lines.";

void drawText(const char *text, int length, int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glColor3f(1, 0, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x,y);
	for(int i=0; i<length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

// Function to draw point selection box in left selection area.
void drawPointSelectionBox(void)
{
	if (primitive == POINT) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.9*height, 0.1*width, height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.9*height, 0.1*width, height);

	// Draw point icon.
	std::string string1 = "Done";
	drawText(string1.data(), string1.size(), 15, 560);

	glEnd();
}

// Function to draw line selection box in left selection area.
void drawLineSelectionBox(void)
{
	if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

	// Draw line icon.
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.025*width, 0.825*height, 0.0);
	glVertex3f(0.075*width, 0.875*height, 0.0);
	glEnd();
}

// Function to draw rectangle selection box in left selection area.
void drawRectangleSelectionBox(void)
{
	if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

	// Draw rectangle icon.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.025*width, 0.735*height, 0.075*width, 0.765*height);
	glEnd();
}

// Function to draw unused part of left selection area.
void drawInactiveArea(void)
{
	glColor3f(0.6, 0.6, 0.6);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);
}

// Function to draw temporary point.
void drawTempPoint(void)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glVertex3f(tempX, tempY, 0.0);
	glEnd();
}

// Function to draw a grid.
void drawGrid(void)
{
	int i;

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x5555);
	glColor3f(0.75, 0.75, 0.75);

	glBegin(GL_LINES);
	for (i = 2; i <= 9; i++)
	{
		glVertex3f(i*0.1*width, 0.0, 0.0);
		glVertex3f(i*0.1*width, height, 0.0);
	}
	for (i = 1; i <= 9; i++)
	{
		glVertex3f(0.1*width, i*0.1*height, 0.0);
		glVertex3f(width, i*0.1*height, 0.0);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

int n;
int flag=0;
void my_func(void)
{
	std::cout<<"Entered my_func\n";
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	std::cout<<n<<std::endl;
	for(int i=0; i<n; i++)
	{
		glVertex3f(arr1[i], arr2[i], 0.0);
	}
	glEnd();
	std::cout<<"Ended my_func\n";
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	drawPointSelectionBox();
	drawLineSelectionBox();
	drawRectangleSelectionBox();
	drawInactiveArea();

	drawPoints();
	drawLines();
	drawRectangles();
    
    if(primitive == POINT)
    {
    	lines.push_back(Line(x_prev, y_prev, x_start, y_start));
    	if (flag==0)
    	{
    		SutherlandHodgman(polygon_size);
    		for(auto it = polygon_xs.begin(); it !=polygon_xs.end(); ++it){
    			std::cout<< ' ' << *it;}
    		std::cout<<"\n";
    		for(auto it = polygon_ys.begin(); it !=polygon_ys.end(); ++it){
    			std::cout<< ' ' << *it;}
    		std::cout<<"\n";
    		n = polygon_xs.size();
    		int i=0;
    		while(!polygon_xs.empty())
    		{
    			arr1[i++] = polygon_xs.front();
    			polygon_xs.pop_front();
   		    }
   		    int j=0;
   		    while(!polygon_ys.empty())
   		    {
   		    	arr2[j++] = polygon_ys.front();
   		    	polygon_ys.pop_front();
   		    }
   		    flag = 1;
    	}
    	my_func();
    }

	if (((primitive == LINE) || (primitive == RECTANGLE)) &&
		(pointCount == 1)) drawTempPoint();
	if (isGrid) drawGrid();
    drawText(text1.data(), text1.size(), 100, 550);
	drawText(text2.data(), text2.size(), 100, 520);
	std::string text3 = "Step3 : Select Done in the left menu which joins first and last vertices of the polygon and gives the clipped polygon.";
	drawText(text3.data(), text3.size(), 100, 490);
	glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
	if (y < (1 - NUMBERPRIMITIVES*0.1)*height) primitive = INACTIVE;
	else if (y < (1 - 2 * 0.1)*height) primitive = RECTANGLE;
	else if (y < (1 - 1 * 0.1)*height) primitive = LINE;
	else primitive = POINT;
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		y = height - y; // Correct from mouse to OpenGL co-ordinates.
					
		if (x < 0 || x > width || y < 0 || y > height); // Click outside canvas - do nothing.

		// Click in left selection area.
		else if (x < 0.1*width)
		{
			pickPrimitive(y);
			pointCount = 0;
		}

		// Click in canvas.
		else
		{
			if (primitive == POINT)
			{
				points.push_back(Point(x, y, pointSize));
			}
			else if (primitive == LINE)
			{
				if (count == 0)
				{
					if (pointCount == 0)
				    {
				    	polygon_x.push_back(x);
				    	polygon_y.push_back(y);
				    	tempX = x; tempY = y;
				    	x_start = x; y_start = y;
				    	polygon_xs.push_back(x);
				    	polygon_ys.push_back(y);
				    	polygon_size++;
					    pointCount++;
				    }
				    else
				    {
				    	polygon_x.push_back(x);
				    	polygon_y.push_back(y);
				    	x_prev = x; y_prev = y;
				    	lines.push_back(Line(tempX, tempY, x, y));
				    	count+=2;
				    	polygon_xs.push_back(x);
				    	polygon_ys.push_back(y);
				    	polygon_size++;
					    pointCount = 0;
				    }
				}
				else
				{
					polygon_x.push_back(x);
				    polygon_y.push_back(y);
				    lines.push_back(Line(x_prev, y_prev, x, y));
				    polygon_xs.push_back(x);
				    polygon_ys.push_back(y);
				    polygon_size++;
				    count+=1;
				    x_prev = x; y_prev = y;
				}   
			}
			else if (primitive == RECTANGLE)
			{
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else
				{
					rectangles.push_back(Rect(tempX, tempY, x, y));
					RectLeft_x = std::min(tempX, x);
					RectLeft_y = std::min(tempY, y);
					RectRight_x = std::max(tempX, x);
					RectRight_y = std::max(tempY, y);
					std::cout<<"Rectangle : \n";
					std::cout<<"("<<RectLeft_x<<", "<<RectLeft_y<<")"<<std::endl;
					std::cout<<"("<<RectRight_x<<", "<<RectRight_y<<")"<<std::endl;
					pointCount = 0;
				}
			}
		}
	}
	glutPostRedisplay();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set viewing box dimensions equal to window dimensions.
	glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

	// Pass the size of the OpenGL window to globals.
	width = w;
	height = h;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
	points.clear();
	lines.clear();
	rectangles.clear();
	primitive = INACTIVE;
	pointCount = 0;
}

// The right button menu callback function.
void rightMenu(int id)
{
	if (id == 1)
	{
		clearAll();
		glutPostRedisplay();
	}
	if (id == 2) exit(0);
}

// The sub-menu callback function.
void grid_menu(int id)
{
	if (id == 3) isGrid = 1;
	if (id == 4) isGrid = 0;
	glutPostRedisplay();
}

// Function to create menu.
void makeMenu(void)
{
	int sub_menu;
	sub_menu = glutCreateMenu(grid_menu);
	glutAddMenuEntry("On", 3);
	glutAddMenuEntry("Off", 4);

	glutCreateMenu(rightMenu);
	glutAddSubMenu("Grid", sub_menu);
	glutAddMenuEntry("Clear", 1);
	glutAddMenuEntry("Quit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	makeMenu(); // Create menu.
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Rectangle primitve on the left is to draw a rectangle." << std::endl;
    std::cout << "Line primitive on the left is to draw the edges of a polygon." << std::endl;
    std::cout << "Done primitive on the left is to join first and last vertices of the polygon and get the clipped polygon." << std::endl;
	std::cout << "Left click on a box on the left to select a primitive." << std::endl
		<< "Then left click on the drawing area: once for point, twice for line or rectangle." << std::endl
		<< "Right click for menu options." << std::endl;
}

// Main routine.
int main(int argc, char **argv)
{
	printInteraction();
	glutInit(&argc, argv);

	//glutInitContextVersion(4, 3);
	//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("canvas.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouseControl);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
