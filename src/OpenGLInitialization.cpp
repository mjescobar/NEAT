#include "OpenGLInitialization.hpp"

#define WIDTH_WINDOW 1024
#define HEIGHT_WINDOW 768




void OpenGLInitialization::cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered)
    {
        fprintf(stderr,"Entro\n"); // The cursor entered the client area of the window
    }
    else
    {
        fprintf(stderr,"Salio\n");
    }
}

void OpenGLInitialization::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        fprintf(stderr,"boton derecho\n");
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {  
        double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		float horizontalMultiplicator = cameraDistance/(double)WIDTH_WINDOW;
		float verticalMultiplicator = cameraDistance/(double)HEIGHT_WINDOW;
		fprintf(stderr,"boton izquierdo, %f \t %f\n", xpos*horizontalMultiplicator, ypos*verticalMultiplicator);
    }
}

void OpenGLInitialization::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	setDistanceCamera (cameraDistance + yoffset*10.f);
}


void OpenGLInitialization::initialization()
{
	
	GLFWwindow * OpenGLInitialization::window = NULL;
	GLFWcursor * OpenGLInitialization::cursor = NULL;
	float OpenGLInitialization::cameraDistance = 1.0f;
	glm::mat4 OpenGLInitialization::Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	glm::mat4 OpenGLInitialization::View       = glm::lookAt(
									glm::vec3(0,0,1), 
									glm::vec3(0,0,0), // and looks at the origin
									glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
							   );

		// Initialise GLFW
		if( !glfwInit() )
		{
			fprintf( stderr, "Failed to initialize GLFW\n" );
			getchar();
			exit(EXIT_FAILURE);
		}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( WIDTH_WINDOW, HEIGHT_WINDOW, "-NEATSpikes-", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	glfwSetCursor(window, cursor);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

}

void OpenGLInitialization::setDistanceCamera(float distance)
{
	cameraDistance = distance;
	View       = glm::lookAt(
							glm::vec3(0,0, distance), 
							glm::vec3(0,0,0), // and looks at the origin
							glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
					   );
}

float OpenGLInitialization::getDistanceCamera()
{
	return 	cameraDistance;
}

