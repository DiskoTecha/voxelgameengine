#include <glad/glad.h> // Include this first, contains required OpenGL headers
#include <GLFW/glfw3.h>
#include <iostream>
//#include "Compute.h"
//#include "Shader.h"
#include "stb_image.h"
#include "Chunk.h"
#include "ShaderCombined.h"

#define IMAGE_WIDTH 600
#define IMAGE_HEIGHT 600

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void printWorkGroupSizes()
{
	int workGroupSizes[3] = { 0 };
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workGroupSizes[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workGroupSizes[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workGroupSizes[2]);

	int workGroupCounts[3] = { 0 };
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workGroupCounts[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workGroupCounts[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workGroupCounts[2]);

	int workGroupInvocations;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workGroupInvocations);

	std::cout << "work group sizes: " << workGroupSizes[0] << " " << workGroupSizes[1] << " " << workGroupSizes[2] << std::endl;
	std::cout << "work group counts: " << workGroupCounts[0] << " " << workGroupCounts[1] << " " << workGroupCounts[2] << std::endl;
	std::cout << "work group invocations: " << workGroupInvocations << std::endl;
}

GLFWwindow* initialize()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(IMAGE_WIDTH, IMAGE_HEIGHT, "Combined Shader Test", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFWwindow" << std::endl;
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	return window;
}


int main(int argc, char* argv[])
{
	GLFWwindow* window = initialize();
	if (window == NULL)
	{
		return -1;
	}

	printWorkGroupSizes();

	ShaderCombined shaderProgram("shader.vert", "shader.frag");

	// Define the width and height of the texture (same size as screen for now)
	unsigned int textureWidth = IMAGE_WIDTH;
	unsigned int textureHeight = IMAGE_HEIGHT;

	// Create compute shader and compile
	ShaderCombined compute("shader.comp", nullptr, COMPUTE, textureWidth, textureHeight);

	// Triangle vertices
	float vertices[] =
	{
		 1.0f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f, // Top right
		 1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f, // Bottom right
		-1.0f, -1.0f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // Bottom left
		-1.0f,  1.0f, 0.0f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f  // Top left
	};

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	// Generate Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Generate Element Buffer Object
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// Create a vertex array object which will store the vertex buffer and vertexattribpointers into the object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// Bind it to start storing those calls
	glBindVertexArray(VAO);

	// Bind the VertexBufferObject to the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy vertex data into buffer's memory on the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind the Element Buffer Object to the GL_ELEMENT_ARRAY target
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Copy index data into the element buffer's memory on the GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	// Enable the vertex attribute, with location = 0 for the vec3 attribute we're passing
	glEnableVertexAttribArray(0);

	// Vertex attrib pointer for vertex color data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	// Enable the vertex attribute with location = 1 for the vec3 color attribute we're passing
	glEnableVertexAttribArray(1);

	// Vertex attrib pointer for texture coordinates we're passing
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// Enable tex coords vertex attrib array with location = 2 for the vec2 tex coords attribute we're passing
	glEnableVertexAttribArray(2);

	// Create texture that we will write to in the shader and display on the screen
	GLuint tex_output;
	glGenTextures(1, &tex_output);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_output);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureWidth, textureHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	// Create chunk and get chunk data
	Chunk chunk1(40, Vector3(-20.0f, -20.0f, 1.0f), 1.0f);
	chunk1.setRegion(10, 15, 10, 12, 10, 18, Color4(0.0f, 1.0f, 0.0f, 1.0f));
	chunk1.set(7, 12, 20, Color4(1.0f, 0.7f, 1.0f, 1.0f));
	chunk1.set(22, 20, 3, Color4(0.3f, 0.2f, 0.1f, 1.0f));
	chunk1.set(20, 22, 1, Color4(1.0f, 1.0f, 1.0f, 1.0f));
	chunk1.set(12, 0, 20, Color4(1.0f, 0.7f, 1.0f, 1.0f));
	chunk1.set(9, 20, 3, Color4(0.3f, 0.2f, 0.1f, 1.0f));
	chunk1.set(25, 28, 1, Color4(1.0f, 1.0f, 1.0f, 1.0f));
	float* chunkData = chunk1.getArrayPointer();
	size_t chunkDataSize = chunk1.getArrayPointerByteSize();

	// Create texture
	GLuint tex_input;
	glGenTextures(1, &tex_input);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_BUFFER, tex_input);

	// Create buffer
	GLuint tex_buffer;
	glGenBuffers(1, &tex_buffer);
	glBindBuffer(GL_TEXTURE_BUFFER, tex_buffer);

	// Create mutable buffer storage
	glBufferData(GL_TEXTURE_BUFFER, chunkDataSize, chunkData, GL_STATIC_DRAW);

	// Link texture to buffer to create texture buffer
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, tex_buffer);

	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	double prevTime = 0.0;
	double currentTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;

	// Disable vsync
	glfwSwapInterval(0);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		//fps counter
		currentTime = glfwGetTime();
		timeDiff = currentTime - prevTime;
		counter++;

		// Update every 30th of a second
		if (timeDiff >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string title = FPS + " fps :: " + ms + " ms";
			// Update title with fps and ms per frame
			glfwSetWindowTitle(window, title.c_str());

			prevTime = currentTime;
			counter = 0;
		}

		// Process Input
		processInput(window);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_BUFFER, tex_input);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, tex_buffer);
		glUniform1i(tex_input, 0);

		compute.dispatchCompute();

		// Rendering commands
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the shader program for all following render calls
		shaderProgram.use();
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_output);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}