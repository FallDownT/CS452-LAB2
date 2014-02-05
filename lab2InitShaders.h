#ifndef LAB_2_INIT_SHADERS_H_
#define LAB_2_INIT_SHADERS_H_

// -----------------------
// --- I N C L U D E S ---
// -----------------------
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include <vector>
#include <cstdio>
#include <iostream>

using namespace std;

// -------------------------------------------
// --- D A T A   T Y P E   C R E A T I O N ---
// -------------------------------------------
typedef struct{
	GLenum type;		// May be either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	const char* filename;	// Filename of Shader or path if not in current directory
} ShaderInfo;

// ---------------------------
// --- P R O T O T Y P E S ---
// ---------------------------

// initShaders initializes each shader in input, shaders
void initShaders(ShaderInfo* shaders);
#endif
