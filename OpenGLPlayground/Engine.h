#pragma once
#include "App.h"
#include <stdlib.h>
#include <iostream>

/////////////////////////////////////////////
//										   //
// Heaviliy inspired from Carlos Martinho  //
//                                         //
/////////////////////////////////////////////

namespace lemon {

	class Engine
	{
	public:
		int windowWidth, windowHeight;

		void setApp(App* app);
		App* getApp();

		int glMajor, glMinor;

		int fullscreen, VSync;

		const char* windowTitle;

		void init();
		void run();

		static Engine& getInstance();
		Engine(Engine const&) = delete;
		void operator=(Engine const&) = delete;
		void setOpenGLVersion(int major, int minor);
		void setWindow(int width, int height, const char* title, int fullscreen, int VSync);

	private:
		App* app;
		GLFWwindow* window;
		Engine();
		double lastTime, currentTime;

		void setupWindow();
		void setupGLFW();
		void setupGLEW();
		void setupOpenGL();
		void setupCallbacks();
	};

}
