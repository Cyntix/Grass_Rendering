//=============================================================================
//                                                                            
//   Exercise code for the lecture
//                                                                            
//=============================================================================

#include "GrassRendering.h"



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	
	GrassRendering window("Solar Viewer", 800, 800);
	
	if(argc > 1)
	{
		window.load_mesh(argv[1], SKY);
	}
	
	if(argc > 2)
	{
		window.load_mesh(argv[2], TERRAIN);
	}
	
	glutMainLoop();
}
