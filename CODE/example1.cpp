// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "Angel.h"



//const int NumPoints = 5000;
//Start of Example 2
using namespace std;

int NumTimesToSubdivide = 1;
const int NumTriangles = 2187;  // 3^7 triangles generated
const int NumVertices = 3 * NumTriangles;
int level = 0;


vec2 points[NumVertices];
vec3 colors[NumVertices];
int Index = 0;


// Function to read text file.
char* readTextFile(char* aTextFile)
{
	FILE* filePointer = fopen(aTextFile, "rb");
	char* content = NULL;
	long numVal = 0;

	fseek(filePointer, 0L, SEEK_END);
	numVal = ftell(filePointer);
	fseek(filePointer, 0L, SEEK_SET);
	content = (char*)malloc((numVal + 1) * sizeof(char));
	fread(content, 1, numVal, filePointer);
	content[numVal] = '\0';
	fclose(filePointer);
	return content;
}

void triangle(const vec2& a, const vec2& b, const vec2& c, const int color)
{
	static vec3  base_colors[] = {
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0),
		vec3(0.0, 0.0, 1.0),
		vec3(0.0, 0.0, 0.0)
	};

	points[Index] = a; colors[Index] = base_colors[color]; Index++;
	points[Index] = b; colors[Index] = base_colors[color]; Index++;
	points[Index] = c; colors[Index] = base_colors[color]; Index++;
}

void divide_triangle(const vec2& a, const vec2& b, const vec2& c, int count, int color)
{
	level = count;
	if (count > 0) {
		vec2 v0 = (a + b) / 2.0;
		vec2 v1 = (a + c) / 2.0;
		vec2 v2 = (b + c) / 2.0;
		divide_triangle(a, v0, v1, count - 1, level);
		divide_triangle(c, v1, v2, count - 1, level);
		divide_triangle(b, v2, v0, count - 1, level);
	}
	else {
		triangle(a, b, c, color);    // draw triangle at end of recursion
	}

}

//----------------------------------------------------------------------------

void init( void )
{
    
	//vec2 points[NumPoints];

 //   // Specifiy the vertices for a triangle
 //   vec2 vertices[3] = {
 //       vec2( -1.0, -1.0 ), vec2( 0.0, 1.0 ), vec2( 1.0, -1.0 )
 //   };

    // Select an arbitrary initial point inside of the triangle
    //points[0] = vec2( 0.25, 0.50 );

    //// compute and store N-1 new points
    //for ( int i = 1; i < NumPoints; ++i ) {
    //    int j = rand() % 3;   // pick a vertex at random

    //    // Compute the point halfway between the selected vertex
    //    //   and the previous point
    //    points[i] = ( points[i - 1] + vertices[j] ) / 2.0;
    //}

    // Create a vertex array object
    //GLuint vao;
    //glGenVertexArrays( 1, &vao );
    //glBindVertexArray( vao );

    //// Create and initialize a buffer object
    //GLuint buffer;
    //glGenBuffers( 1, &buffer );
    //glBindBuffer( GL_ARRAY_BUFFER, buffer );
    //glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

	//Example2
	vec2 vertices[3] = {
		vec2(-1.0, -1.0), vec2(0.0, 1.0), vec2(1.0, -1.0)
	};

	// Subdivide the original triangle
	divide_triangle(vertices[0], vertices[1], vertices[2],
		NumTimesToSubdivide, 3);

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	//GLuint buffer;
	//glGenBuffers(1, &buffer);
	//glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	// Create and initialize a buffer object
	
	
	
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// First, we create an empty buffer of the size we need by passing
	//   a NULL pointer for the data values
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);

	// Next, we load the real data in parts.  We need to specify the
	//   correct byte offset for placing the color data after the point
	//   data in the buffer.  Conveniently, the byte offset we need is
	//   the same as the size (in bytes) of the points array, which is
	//   returned from "sizeof(points)".
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);



    //// Load shaders and use the resulting shader program

	// Create shader program executable.
	char* vertexShader = readTextFile("C:\\Users\\poitras_michael.LRC\\Documents\\chapt2_sierpinski_example_program_msvs\\CODE\\vshader24.glsl");
	static unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, (const char**)&vertexShader, NULL);
	glCompileShader(vertexShaderId);

	
	char* fragmentShader = readTextFile("C:\\Users\\poitras_michael.LRC\\Documents\\chapt2_sierpinski_example_program_msvs\\CODE\\fshader24.glsl");;
	static unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char**)&fragmentShader, NULL);
	glCompileShader(fragmentShaderId);

	static unsigned int programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);


     //Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(programId, "vPosition");
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

	GLuint vColor = glGetAttribLocation(programId, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

//void
//display( void )
//{
//    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
//    glDrawArrays( GL_POINTS, 0, NumPoints );    // draw the points
//    glFlush();
//}

//Example 2 display
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glFlush();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

void specialKeyInput(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		if (NumTimesToSubdivide <= 7) {
			NumTimesToSubdivide += 1;
		}
	}

		if (key == GLUT_KEY_DOWN) {
			if (NumTimesToSubdivide >= 1) {
				NumTimesToSubdivide -= 1;
			}
		}
		init();
	display();
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    
	glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 512, 512 );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
     glutInitContextVersion( 3, 2 );
     glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow( "LSD Sierpinski Gasket" );

    glewInit();
    
    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	glutSpecialFunc(specialKeyInput);

    glutMainLoop();
    return 0;
}
