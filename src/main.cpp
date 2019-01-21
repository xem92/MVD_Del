//
//  main.cpp
//
//  Copyright © 2018 Alun Evans. All rights reserved.
//
#include "includes.h"
#include "extern.h"
#include "Game.h"

//initialise GAME point for use in this file
Game* GAME = nullptr;
//initialise global ECS. By including extern.h in any cpp file (NOT .h file!) we can access this variable
EntityComponentStore ECS;

bool glCheckError() {
    GLenum errCode;
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << errCode << std::endl;
        return false;
    }
    return true;
}

void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW ERROR: code " << error << "; msg: " << description << std::endl;
}

// a call-back function
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    //TODO: update game viewport size
	GAME->update_viewports(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //quit
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
	GAME->key_callback(key, scancode, action, mods);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	GAME->mouse_button_callback(button, action, mods);
}

int main(void)
{
    // register the error call-back function before doing anything else
    glfwSetErrorCallback(glfw_error_callback);
    
    //create window pointer
    GLFWwindow* window;
    
    // Initialize the library
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(800, 600, "Hello OpenGL!", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    // Make the window's context current
    glfwMakeContextCurrent(window);
    
    //initialise GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    
    //get info about OpenGL version
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: "<< renderer << "; version: " << version << std::endl;
    
    //input callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //variables storing mouse position
    double mouse_x, mouse_y;
    
	//set initial position before loop
	glfwGetCursorPos(window, &mouse_x, &mouse_y);

	//create game singleton and initialise it
	GAME = new Game();
	GAME->init();
	GAME->update_viewports(800, 600);
	//stores difference in time between each frame
	float dt = 0.0f;
	double curr_time = 0.0, prev_time = glfwGetTime();
	int frame_counter_frames = 0;
	float frame_counter_time = 0;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
		//update time
		curr_time = glfwGetTime();
		dt = (float)(curr_time - prev_time);
		prev_time = curr_time;
        
		frame_counter_frames++;
		frame_counter_time += dt;
		if (frame_counter_time >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			//printf("%f ms/frame\n", 1000.0 / float(frame_counter_frames));
			frame_counter_frames = 0;
			frame_counter_time = 0.0;
		}

        // Poll events update mouse position
        glfwPollEvents();
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
		GAME->updateMousePosition((int)mouse_x, (int)mouse_y);

		//update game
		GAME->update(dt);
		glfwSwapBuffers(window);

    }

	//free game memory - not necessary but good practice!
	delete GAME;

    //terminate glfw and exit
    glfwTerminate();
    return 0;
}


