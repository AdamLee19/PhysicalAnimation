#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#ifndef _TOOL_KIT_H_
#define _TOOL_KIT_H_

struct Vertex
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;	
};

struct Face
{
	unsigned int v1;
	unsigned int v2;
	unsigned int v3;
	unsigned int vt1;
	unsigned int vt2;
	unsigned int vt3;
	unsigned int vn1;
	unsigned int vn2;
	unsigned int vn3;
};

bool load_obj_file( const char *file_name, Vertex *&point, Face *&face, int &point_count, int &face_count );


#endif