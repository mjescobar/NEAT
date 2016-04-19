#include "BasicNeuron.hpp"
/*
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace NEATSpikes;

int main()
{
	srand( time( 0 ) ); //  Para que cada vez que se use el método random tenga una piscina de números randoms diferentes.
	GlobalInformation * information = new GlobalInformation();
	cout << "Basic Neuron Test initialized" << endl;
	// Lo primero que se prueba es la carga correcta de los parámetros de usuario.
	BasicNeuron * BN = new BasicNeuron(information, "./BasicNeuronUserDefinitions");
	cout << "First Neuron created by constructor method:" << endl;
	BN->printState(); // Se imprime el estado buscando ver que están correctamente inicializadas todas las variables importantes.

	cout << "Now 100 mutations " << endl;
	// Se prueban 5 mutaciones.
	for (int i = 0; i < 100; ++i)
	{
		BN->mutate();
		BN->printState();	
	}

	cout << "Testing the last one with a random input voltage" << endl;

	double random = rand()/(double)RAND_MAX;

	BN->sumIncomingConnectionsOutputs(random);
	cout << "input voltage: " << random << "\t Neuron output: " << BN->eval() << endl;


	cout << "Saving data" << endl;
	BN->saveState("./save/");
	BN->saveUserDefinitions("./save/");
	BN->saveId("./save/");


	cout << "Loading UserDef" << endl;
	BasicNeuron * BNPrototype = new BasicNeuron(information, "./save/userDefinitions");
	cout << "Loading Id" << endl;
	BNPrototype->loadId("./save/BN_id");	


	cout << "Loading data" << endl;
	BasicNeuron * BN2 = dynamic_cast< BasicNeuron * > ( BNPrototype->duplicate());
	BN2->load("./save/BN1");

	cout << "Basic neuron loaded" << endl;
	BN2->printState();

	BN2->mutate();
	BN2->mutate();
	BN2->mutate();
	BN2->mutate();

	cout << "BN " << endl;
	BN->printState();

	cout << "BN2" << endl;
	BN2->printState();

	cout << " Diference between BN2 and BN: "<< BN2->getDistance(BN) << endl;



	return EXIT_SUCCESS;
}*/


// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
// GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glut.h>
using namespace glm;

#include "shader.hpp"
#define TRIANGULOS 36
#include "OpenGLInitialization.hpp"

#include <iostream>

using namespace NEATSpikes;
using namespace std;

int triangulos = BASIC_NEURON_TRIANGLES_AMOUNT;
GLfloat g_vertex_buffer_data[ BASIC_NEURON_TRIANGLES_AMOUNT * 3 * 3 ];

int numeroNeuronas = 35;
float espacioEntreMedio = 1.0f;
float diametro = 2.0f;

glm::mat4 Model;
glm::mat4 MVP;


void ComputarCaramaSegunCantidadNeuronas (int cantidadNeuronas)
{
	// Dado que el angulo de perspectiva es de 45 grados, la distancia debe ser exactamente igual
	// Al ancho deseado.
	float distancia = espacioEntreMedio * (cantidadNeuronas + 1.0f) + diametro * cantidadNeuronas;
	cout << "distancia: "<< distancia << endl;
	OpenGLInitialization::setDistanceCamera (distancia);
	
}

void dibujarNeuronas (GLuint & MatrixID, int cantidadNeuronas)
{
	
	float distancia = espacioEntreMedio * (cantidadNeuronas + 1.0f) + diametro * cantidadNeuronas;
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, BasicNeuron::vertexbuffer );
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);


	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	for (int i = 0; i < cantidadNeuronas; ++i)
	{
		float posicion = -distancia/2.0 + espacioEntreMedio * (i+1) + diametro * i;
		Model      = glm::translate(glm::mat4(1.f), glm::vec3(posicion, 0.f, 0.f));
		MVP        = OpenGLInitialization::Projection * OpenGLInitialization::View * Model; // Remember, matrix multiplication is the other way around	
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES , 0, triangulos * 3 ); // 3 indices starting at 0 -> 1 triangle
	}
	glDisableVertexAttribArray( 0 );

}


int main( void )
{
	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	ComputarCaramaSegunCantidadNeuronas (numeroNeuronas);
	
	do{

		// Model matrix : an identity matrix (model will be at the origin)
		Model = glm::translate(glm::mat4(1.f), glm::vec3(0.5f, 0.f, 0.f));
		// Our ModelViewProjection : multiplication of our 3 matrices
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram( programID );

		
		dibujarNeuronas ( MatrixID, numeroNeuronas );


		void mouse(int button, int state, int x, int y);
		// Swap buffers
		glfwSwapBuffers( OpenGLInitialization::window );
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(OpenGLInitialization::window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(OpenGLInitialization::window) == 0 );

	// Cleanup VBO
	glDeleteBuffers(1, &BasicNeuron::vertexbuffer);
	glDeleteProgram(programID);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}



