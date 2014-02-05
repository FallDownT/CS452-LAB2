// -----------------------
// --- I N C L U D E S ---
// -----------------------
#include "lab2InitShaders.h"


// ---------------------------
// --- P R O T O T Y P E S ---
// ---------------------------
const GLchar* inputShader(const char* filename);
GLuint createShader(GLenum type, const GLchar* shadeSource);
GLuint createProgram(const vector<GLuint> shadeList);


// -----------------------------------------------
// --- F U N C T I O N   D E F I N I T I O N S ---
// -----------------------------------------------

// ----------------------------------------------------------------------------
// - initShaders takes in a pointer to a ShaderInfo struct.  It uses this
// -   pointer to create a vector of these shaders.  With this vector, the
// -   function creates and uses a program created from these shaders.
// ----------------------------------------------------------------------------
void initShaders(ShaderInfo* shaders)
{
	// Declare data members
	ShaderInfo* shade = shaders;
	vector<GLuint> Shadelist;
	GLuint program;

	// Iterate through shaders and add all shaders to ShadeList
	while(shade->type != GL_NONE)
	{
		Shadelist.push_back(createShader(shade->type,inputShader(shade->filename)));
		++shade;
	}

	// Creates a program which links all shaders with your OpenGL program
	program = createProgram(Shadelist);

	// Installs program incorporating shaders into OpenGL program
	glUseProgram(program);
}

// ----------------------------------------------------------------------------
// - inputShader reads in and returns the shader file denoted by the inputted
//    filename.
// ----------------------------------------------------------------------------
const GLchar* inputShader(const char* filename)
{
	// Open the file
	FILE* fshade = fopen(filename, "rb");

	// If file is open, print error and return
	if(!fshade)
	{
		fprintf(stderr,"unable to open file '%s'\n",filename);
		return NULL;
	}

	// Get the length of the file in ASCII characters
	fseek(fshade, 0, SEEK_END);
	long filesize=ftell(fshade);

	// Reset position to beginning of file
	fseek(fshade, 0, SEEK_SET);


	// Read in the file 
	GLchar* shadingSource= new GLchar[filesize+1];//
	fread(shadingSource, 1, filesize, fshade);


	// If file is empty, print error and return
	if(ftell(fshade) == 0)
	{
		fprintf(stderr, "File '%s' is empty.\n",filename);
		return NULL;
	}

	// Close the file
	fclose(fshade);

	// Add null terminating character to end of file
	shadingSource[filesize] = 0;

	// Return the file's text
	return const_cast<const GLchar*>(shadingSource);
}

// ----------------------------------------------------------------------------
// - createShader takes the shader (presumably read in from inputShader) and
// -   compiles the shader.  If the shader fails to compile, a log is printed
// -   to stderr.  Regardless of whether the compile was successful, the shader
// -   GLuint is returned.
// ----------------------------------------------------------------------------
GLuint createShader(GLenum type, const GLchar* shadeSource)
{
	// Create a shader based on type GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	GLuint shader = glCreateShader(type);
 	glShaderSource(shader, 1, &shadeSource, NULL);

	// Compile the shader object
	glCompileShader(shader);

	// Get the status of the compilation
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	// If the shader did not compile, generate and print an error log
	if(!compileStatus)
	{
		// Get the length of the shadeSource file
		GLint logSize;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		// Put debug info into infoLog
		GLchar* infoLog = new GLchar[logSize+1];
		glGetShaderInfoLog(shader,logSize,&logSize,infoLog);

		// Puts which type of shader having the error in shadeInfo
		const char *shadeInfo = NULL;
		switch(type)
		{
			case GL_VERTEX_SHADER: shadeInfo = "vertex"; break;
			case GL_GEOMETRY_SHADER_EXT: shadeInfo = "geometric"; break;
			case GL_FRAGMENT_SHADER: shadeInfo = "fragment"; break;
		}

		// Prints error containing info on the erroneous shader
		fprintf(stderr,"\nCompile failure in %u shader: %s\n Error message:\n%s\n",type,shadeInfo,infoLog);

		// Deallocate dynamically allocated data members
		delete[] infoLog;
	}
	return shader;
}

// ----------------------------------------------------------------------------
// - createProgram takes in a vector of shaders.  A program is created, shaders
// -   are attached, and attributes are bound to locations.  The program is
// -   linked to your actual code (lab2.cpp) and is returned.  If the link fails,
// -   a log is written to stderr.
// ----------------------------------------------------------------------------
GLuint createProgram(const vector<GLuint> shadeList)
{
	// Create a program object
	GLuint program = glCreateProgram();

	// Attach all shaders to newly created program object
	for(GLuint i=0;i<shadeList.size();i++){glAttachShader(program,shadeList[i]);}

	// Bind position attribute to 0; color attribute to 1
	glBindAttribLocation(program, 0, "in_position");
	glBindAttribLocation(program, 1, "in_color");

	// Link program oject to OpenGL program
	glLinkProgram(program);

	// Get the status of the program linking
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	// If link fails, generate and print an error log
	if(!linkStatus)
	{
		// Get the linker status log
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);


		// Put debug info into infoLog
		GLchar *infoLog = new GLchar[logSize+1];
		glGetProgramInfoLog(program,logSize,&logSize,infoLog);


		// Prints error containing info on the erroneous linking
		fprintf(stderr,"\nShader linking failed: %s\n",infoLog);

		// Deallocate dynamically allocated data members
		delete[] infoLog;
		for(GLuint i=0;i<shadeList.size();i++){glDeleteShader(shadeList[i]);}
	}
	return program;
}
