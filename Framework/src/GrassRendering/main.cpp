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

	if(argc > 3)
	{
		window.load_mesh(argv[3], GRASS);
	}

	if(argc > 4)
	{
		window.load_mesh(argv[4], PARTICLE_PATTERN);
		window.load_grass();
	}
	
	glutMainLoop();
}