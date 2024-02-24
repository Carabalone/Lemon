#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "lemon.h"

float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
	0, 3, 1,
	3, 2, 1
};

class MyApp : public App {
public:
	void displayCallback(GLFWwindow* window, double elapsed);
	void initCallback(GLFWwindow* window);
private:
	lemon::Shader shader;
	unsigned int VBO, VAO;
	unsigned int EBO;

	lemon::Logger& logger = lemon::Logger::getInstance();
	void configureVAO();
};

void MyApp::initCallback(GLFWwindow* window) {
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	configureVAO();

	shader = lemon::Shader();
	shader.init();
	shader.addShader("vertex.glsl", GL_VERTEX_SHADER);
	shader.addShader("fragment.glsl", GL_FRAGMENT_SHADER);

	shader.linkAndCreate();
}

void MyApp::displayCallback(GLFWwindow* window, double elapsed) {

	shader.bind();
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	}
	shader.unbind();
}

void MyApp::configureVAO() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindVertexArray(0);
}


int main()
{
	lemon::Engine& lemon = lemon::Engine::getInstance();
	lemon.setApp(new MyApp());
	lemon.setOpenGLVersion(4, 6);
	lemon.setWindow(800, 600, "Lemon instance", 0, 1);
	lemon.init();
	lemon.run();
	exit(EXIT_SUCCESS);
}