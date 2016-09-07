// Windows Specific include (TOCO: Cleanup later)
//#include "targetver.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
	#include <tchar.h>
	#include <windows.h>
#endif

#include <stdbool.h>

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

#include "VikingMQ/VikingMQ.h"

#include <SDL.h>
#include <GL/glew.h>

#include "window.h"
#include "resources.h"
#include "shaders.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Function Definitions
int init();
int loadMedia();
void close();


// Create this files global variables
// Window pointer
static ODIN_Window* window = 0;


// Test OpenGL rendering
static const GLfloat vertices[9] = {
	-0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

GLuint VaoID = 0;
GLuint VboID = 0;
// End OpenGL rendering Test

// Function Declorations
int init() {
	int success = 1;

	// Try init
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = 0;
	}

	// Create the window
	window = Window_new("OdiNgine, base application", SCREEN_WIDTH, SCREEN_HEIGHT);
	return success;
}

int loadMedia() {
	int success = 1;

	return success;
}

void close() {
	Resources_uninit();

	// Destroy window
	Window_delete(window);

	// Quit SDL subsystems
	SDL_Quit();

}


void KeypressListener(VMQ_Message *message, void* ctx) {
	printf("Message Received\n");
}

// TEST: Simple text Resource_Loader
int BitmapResource_Loader(ODIN_Resource* resource, ODIN_Int64 size, unsigned char* rawdata) {
	// TODO: Load the resource from the rawdata
	printf("BitmapResource_Loader called: resource file = %s\n", resource->fname);

	printf("TODO: BitmapResource_Loader not implemented yet.\n");
	
	
	return 1;
}

int VertexShaderResource_Loader(ODIN_Resource* resource, ODIN_Int64 size, unsigned char* rawdata) {
	// A Shader is a source file that is loaded at runtime (which loads the actual shader parts)
	// and sent to the GPU to be compiled. Odingine has a single
	// struct that is used to manage and cache it's 
	// different information needed during runtime, to avoid round-trips
	// with the GPU

	unsigned char* tmpSource = (unsigned char*)malloc(sizeof(unsigned char) * (size + 1));
	memcpy((void*)tmpSource, rawdata, sizeof(unsigned char) * (size + 1));
	tmpSource[size] = 0;

	resource->buffer = (void*)Shaders_newShaderSource(ODIN_SHADERTYPE_VERTEX, tmpSource);

	resource->total_bytes = sizeof(ODIN_ShaderSource) + (sizeof(unsigned char) * (size + 1));
	resource->buffer_size = sizeof(ODIN_ShaderSource);

	free(tmpSource);

	return 1;
}

int VertexShaderResource_Unloader(ODIN_Resource* resource) {

	return 0;
}

int TextResource_Loader(ODIN_Resource* resource, ODIN_Int64 size, unsigned char* rawdata) {
	// TODO: Load the resource from the rawdata
	// Since the text file does not contain the null terminator, we need to add it after
	// copying the rawdata into our resource buffer
	printf("TextResource_Loader called: resource file = %s\n", resource->fname);
	
	resource->buffer_size = sizeof(unsigned char) * size;
	resource->total_bytes = sizeof(unsigned char) * size;

	resource->buffer = (void*)malloc(sizeof(unsigned char) * (size + 1));
	memcpy((void*)resource->buffer, rawdata, sizeof(unsigned char) * (size + 1));
	char* text = (char*)resource->buffer;
	text[size] = 0; // add the null terminator

	return 1;
}

int TextResource_Unloader(ODIN_Resource* resource) {
	resource->buffer_size = 0;
	resource->total_bytes = 0;
	free(resource->buffer);
	resource->buffer = 0;

	return 1;
}


int main(int argc, char* argv[]) {
	printf("Odinheim, you are becoming greater!\n");

	// Initialize the Resources (Resource Manager)
	// TEMP: only allow Resources to take up 200MB
	// ADD: argv[0]
	if (Resources_init(argv[0], 1024 * 1024 * 200) != 0) {
		printf("Error loading Resource Manager\n");
		return 1;
	}

	// TEST:
	Resources_addSearchPath("./Data", 1);
	ODIN_UInt64 RESTYPE_BITMAP = Resources_registerType();
	ODIN_UInt64 RESTYPE_TEXT = Resources_registerType();
	ODIN_UInt64 RESTYPE_SHADERSRC = Resources_registerType();
	Resources_registerLoader(RESTYPE_BITMAP, &BitmapResource_Loader, "*.bmp");
	Resources_registerLoader(RESTYPE_TEXT, &TextResource_Loader, "*.txt");
	Resources_registerLoader(RESTYPE_SHADERSRC, &VertexShaderResource_Loader, "*.vs");
	Resources_registerUnloader(RESTYPE_TEXT, &TextResource_Unloader);
	Resources_registerUnloader(RESTYPE_SHADERSRC, &VertexShaderResource_Unloader);
    
	printf("CacheSize: %llu\n", Resources_getCacheSize());

	if (!Resources_loadResource("hello.txt")) {
		printf("Unable to load file 'hello.txt', error happend.\n");
	}

	ODIN_Resource* vertShaderRes = Resources_getResource("test.vs");
	if (!vertShaderRes) {
		printf("Unable to load file 'test.vs', error happend.\n");
	} else {
		ODIN_ShaderSource* shader = (ODIN_ShaderSource*)vertShaderRes->buffer;
		printf("Shader Source: \n%s\n", shader->shader_source);
	}

	if (!Resources_loadResource("helloworld.bmp")) {
		printf("Unable to load file 'helloworld.bmp', error happend.\n");
	}

	ODIN_Resource* helloText = Resources_getResource("hello.txt");
	if (!helloText) {
		printf("There was an error retreiving the resource 'hello.tst'\n");
	} else {
		printf("Retreived the resource %s\n", helloText->fname);
		char* text = (char*)helloText->buffer;
		printf("The text is: %s\n", text);  
	}
    
    printf("CacheSize: %llu\n", Resources_getCacheSize());

	Resources_releaseResource(helloText);
	helloText = 0;

	printf("CacheSize: %llu\n", Resources_getCacheSize());
	
	// END TEST

	// TEST: init VMQ_Values
	// VMQ_Message *message = VMQ_StartMessage();
	// VMQ_PushInt(message, 20);
	// VMQ_PushUInt(message, 130009);
	// VMQ_PushString(message, "This is a TEST Message");

	// printf("Message Created, and values pushed!\n");
	// printf("Argument Count: %d, Value 1: %d, Value 2: %d\n", (*message).count, VMQ_GetInt(message, -3), VMQ_GetUInt(message, -2));
	// printf("The Last argument is: %s\n", VMQ_GetString(message, -1));


	if (!init())
		return 1;

	if (!loadMedia())
		return 1;

	// TEMP: Render test setup
	printf("Starting gl triangle setup.\n");
	
	glGenVertexArrays(1, &VaoID);
	glBindVertexArray(VaoID);

	glGenBuffers(1, &VboID);
	glBindBuffer(GL_ARRAY_BUFFER, VboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, vertices, GL_STATIC_DRAW);


	// Create a VikingMQ
	VMQ_MessageQueue *queue = VMQ_NewQueue();

	// Subscribe to the keypress messages
	VMQ_Subscribe(queue, "keypress", (VMQ_Receiver)&KeypressListener);

	int quit = 0;
	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			// User quites
			if (e.type == SDL_QUIT) {
				quit = 1;
			} else if (e.type == SDL_KEYDOWN) {
				
				// Create a VMQ_Message and push to queue
				
				
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					quit = 1;
				}
			}
		}

		while (VMQ_ProcessMessage(queue)) {
			// This will loop until the queue is empty
		}

		/* Check for Messages in VikingMQ */


		// OpenGL Rendering loop
		Window_clearColor(100.0/255.0, 149.0/255.0, 237.0/255.0, 1.0);

		// Render studd here?
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VboID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);


		Window_swapBuffer(window);

	}
	


	close();

	return 0;
}