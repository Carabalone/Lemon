#include "Engine.h"

namespace lemon {

	Engine::Engine(void) {
		app = 0;
		window = 0;
		windowWidth = 640, windowHeight = 480;
		glMajor = 4, glMinor = 3;
		fullscreen = 0, VSync = 0;
		windowTitle = "Window";
	}
	
	void Engine::setApp(App* app) {
		this->app = app;
	}

	App* Engine::getApp() {
		return app;
	}

	void Engine::setOpenGLVersion(int major, int minor) {
		glMajor = major;
		glMinor = minor;
	}

	void Engine::setWindow(int width, int height, const char* title, int fullscreen, int VSync) {
		this->fullscreen = fullscreen;
		this->VSync = VSync;
		this->windowWidth = width;
		this->windowHeight = height;
		this->windowTitle = title;
	}
	/////////////////////// callbacks ///////////////////

	static void windowCloseCallback(GLFWwindow* window) {
		Engine::getInstance().getApp()->windowCloseCallback(window);
	}

	static void windowSizeCallback(GLFWwindow* window, int width, int height) {
		Engine::getInstance().getApp()->windowSizeCallback(window, width, height);
	}

	static void glfwErrorCallback(int error, const char* description) {
		std::cerr << "GLFW Error: " << description << std::endl;
	}

	static void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
		Engine::getInstance().getApp()->cursorCallback(window, xpos, ypos);
	}

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action,
		int mods) {
		Engine::getInstance().getApp()->keyCallback(window, key, scancode, action,
			mods);
	}

	static void mouseButtonCallback(GLFWwindow* window, int button, int action,
		int mods) {
		Engine::getInstance().getApp()->mouseButtonCallback(window, button, action,
			mods);
	}

	static void scrollCallback(GLFWwindow* window, double xoffset,
		double yoffset) {
		Engine::getInstance().getApp()->scrollCallback(window, xoffset, yoffset);
	}

	static void joystickCallback(int jid, int event) {
		Engine::getInstance().getApp()->joystickCallback(jid, event);
	}

	/////////////////// init functions //////////////////

	void Engine::setupWindow() {
		GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : 0;

		window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, monitor, 0);

		if (!window) {
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(window);
		glfwSwapInterval(VSync);
	}

	void Engine::setupCallbacks() {
		glfwSetCursorPosCallback(window, cursorCallback);
		glfwSetKeyCallback(window, keyCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetScrollCallback(window, scrollCallback);
		glfwSetJoystickCallback(joystickCallback);
		glfwSetWindowCloseCallback(window, windowCloseCallback);
		glfwSetWindowSizeCallback(window, windowSizeCallback);
	}

	void Engine::setupGLFW() {
		if (!glfwInit()) {
			std::cerr << "Failed to init glfw" << std::endl;
			exit(EXIT_FAILURE);
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		setupWindow();
		setupCallbacks();
	}

	void Engine::setupGLEW() {
		glewExperimental = GL_TRUE;

		GLenum result = glewInit();
		if (result != GLEW_OK) {
			std::cerr << "Error in glew init" << glewGetString(result);
			exit(EXIT_FAILURE);
		}

		glGetError();
	}

	void Engine::setupOpenGL() {
		glClearColor(0.2f, 0.1f, 0.8f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		glDepthRange(0.0, 1.0);
		glClearDepth(1.0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glViewport(0, 0, windowWidth, windowHeight);
	}

	void Engine::init() {
		setupGLFW();
		setupGLEW();
		setupOpenGL();
		app->initCallback(window);
	}

	void Engine::run() {
		currentTime = glfwGetTime();
		double elapsed = currentTime - lastTime;
		lastTime = currentTime;
		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			app->displayCallback(window, elapsed);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	Engine& Engine::getInstance(void) {
		static Engine instance;
		return instance;
	}

}
