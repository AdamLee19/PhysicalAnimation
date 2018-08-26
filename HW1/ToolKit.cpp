#include "ToolKit.h"
#include <iostream>
#include <cstdio>
using namespace std;

bool load_obj_file( const char *file_name, Vertex *&vp, Face *&face, int &point_count, int &face_count )
{

	FILE *fp = fopen( file_name, "r" );
	char line[ 1024 ];



	int currentV = 0;
	int currentVn = 0;
	int currentf = 0;

	int v_count = 0;
	int vn_count = 0;
	int vt_count = 0;
	int f_count = 0;


	if( !fp )
	{
		std::perror("File opening failed");
        return false;
	}



	while( fgets( line, 1024, fp ) )
	{
		if( line[ 0 ] == 'v' )
		{
			if( line[ 1 ] == ' ' )
				v_count++;
			else if( line[ 1 ] == 't' )
				vt_count++;
			else if( line[ 1 ] == 'n' )
				vn_count++;
		}
		else if ( line[ 0 ] == 'f' )
			f_count++;
	}

	

	if( v_count != vn_count )
	{
		cout << "Vertices are not equal to Vertex normal! " << endl;
		cout << "v_count: " << v_count << "vn_count: " << vn_count << endl;
		return false;
	}

	// cout << "v_count:" << v_count << endl;
	// point_count = v_count;
	// cout << "vt_count:" << vt_count << endl;
	// cout << "vn_count:" << vn_count << endl;
	// cout << "f_count:" << f_count << endl;
	face_count = f_count;

	

	vp = new Vertex [ v_count ];
	face = new Face [ f_count ];

	rewind( fp );

	
	while( fgets( line, 1024, fp ) )
	{
		if( line[ 0 ] == 'v' )
		{
			if( line[ 1 ] == ' ' )
			{
				float x, y, z;
				x = y = z = 0.0f;
				sscanf( line, "v %f %f %f", &x, &y, &z );
				vp[ currentV ].x = x;
				vp[ currentV ].y = y;
				vp[ currentV ].z = z;
				currentV++;
			}
			else if ( line[ 1 ] == 'n' )
			{
				float x, y, z;
				x = y = z = 0.0f;
				sscanf( line, "vn %f %f %f", &x, &y, &z );
				vp[ currentVn ].nx = x;
				vp[ currentVn ].ny = y;
				vp[ currentVn ].nz = z;
				currentVn++;
			}			
		}
		else if( line[ 0 ] == 'f' )
		{
			int v1, v2, v3, vt1, vt2, vt3, vn1, vn2, vn3;
			v1 = v2 = v3 = vt1 = vt2 = vt3 = vn1 = vn2 = vn3 = 0;
			sscanf( line, "f %i/%i/%i %i/%i/%i %i/%i/%i", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3 );
			face[ currentf ].v1 = (unsigned int) ( v1 - 1 );
			face[ currentf ].v2 = (unsigned int) ( v2 - 1 );
			face[ currentf ].v3 = (unsigned int) ( v3 - 1 );
			face[ currentf ].vt1 = (unsigned int) vt1;
			face[ currentf ].vt2 = (unsigned int) vt2;
			face[ currentf ].vt3 = (unsigned int) vt3;
			face[ currentf ].vn1 = (unsigned int) ( vn1 - 1 );
			face[ currentf ].vn2 = (unsigned int) ( vn1 - 1 );
			face[ currentf ].vn3 = (unsigned int) ( vn1 - 1 );
			currentf++;
		}
		
	}
	
	fclose( fp );
	return true;
}




