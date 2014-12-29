#include <stdlib.h>
#include <iostream>
#include <gl\glew.h>
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>
#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

#include "Shader.h"
#include "Sea.h"

void Draw(const Shader &shader)
{
	//Sea vertices
	const float scale = 100;
	const int size = 40;

	float *pVertices = new float[size * size * 18];

	float interval = scale / size;

	//Vertices
	for(int i = 0; i < size; ++i)
	{
		for(int j = 0; j < size; ++j)
		{
			int it = (i*size + j) * 18;

			//First triangle
			pVertices[it]	  = (-scale / 2.0f) + i*interval;
			pVertices[it + 1] = 0.0f;
			pVertices[it + 2] = (-scale / 2.0f) + j*interval;

			pVertices[it + 3] = (-scale / 2.0f) + i*interval;
			pVertices[it + 4] = 0.0f;
			pVertices[it + 5] = (-scale / 2.0f) + (j+1)*interval;

			pVertices[it + 6] = (-scale / 2.0f) + (i+1)*interval;
			pVertices[it + 7] = 0.0f;
			pVertices[it + 8] = (-scale / 2.0f) + j*interval;

			//Second triangle
			pVertices[it + 9]  = (-scale / 2.0f) + (i+1)*interval;
			pVertices[it + 10] = 0.0f;
			pVertices[it + 11] = (-scale / 2.0f) + j*interval;

			pVertices[it + 12] = (-scale / 2.0f) + i*interval;
			pVertices[it + 13] = 0.0f;
			pVertices[it + 14] = (-scale / 2.0f) + (j+1)*interval;

			pVertices[it + 15] = (-scale / 2.0f) + (i+1)*interval;
			pVertices[it + 16] = 0.0f;
			pVertices[it + 17] = (-scale / 2.0f) + (j+1)*interval;
		}
	}

	//Matrix process
	glm::mat4 projection = glm::perspective(70.0f, 4.0f / 3.0f, 1.0f, 100.0f);
	glm::mat4 modelview  = glm::mat4(1.0f);

	modelview = glm::rotate(modelview, 20.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelview = glm::translate(modelview, 0.0f, -3.0f, -10.0f);

	glm::mat4 mvp = projection * modelview;

	//Rendering
	glUseProgram(shader.GetProgramID());

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, pVertices);
	glEnableVertexAttribArray(0);

	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramID(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLES, 0, 6 * size * size);

	glDisableVertexAttribArray(0);

	glUseProgram(0);

	//Free memory
	delete pVertices;
}

int main(int argc, char** argv)
{
	//Random numbers
	srand(time(NULL));

	//Set OpenGL context
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 8;
	settings.majorVersion = 3;
	settings.minorVersion = 3;

	//Create window
	sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	//Display current OpenGL context
	settings = window.getSettings();
	std::cout << "=== OpenGL Context ===" << std::endl;
	std::cout << "Depth bits : " << settings.depthBits << std::endl;
	std::cout << "Stencil bits : " << settings.stencilBits << std::endl;
	std::cout << "Antialiasing level : " << settings.antialiasingLevel << std::endl;
	std::cout << "Version : " << settings.majorVersion << "." << settings.minorVersion << std::endl;
	std::cout << "======================" << std::endl;

	//Init Glew
	if(glewInit() != GLEW_OK)
	{
		return EXIT_FAILURE;
	}

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Sea sea(100, 20);
	if(!sea.Init())
	{
		return EXIT_FAILURE;
	}

	//Timer
	sf::Clock timer;

	//Main loop
	bool running = true;
	while(running)
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			//Window closed
			if(event.type == sf::Event::Closed)
			{
				running = false;
			}
			//Window resized
			else if(event.type == sf::Event::Resized)
			{
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		//Clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Matrix process
		glm::mat4 projection = glm::perspective(70.0f, 4.0f / 3.0f, 1.0f, 100.0f);
		glm::mat4 modelview = glm::mat4(1.0f);

		modelview = glm::rotate(modelview, 20.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		modelview = glm::rotate(modelview, 30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		modelview = glm::translate(modelview, 0.0f, -5.0f, -50.0f);

		glm::mat4 mvp = projection * modelview;

		//Draw sea
		sf::Time elapsed = timer.getElapsedTime();
		sea.Draw(mvp, elapsed.asSeconds());

		//Display
		window.display();
	}

	return EXIT_SUCCESS;
}