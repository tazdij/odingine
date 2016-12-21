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

#include "viking_math.h"

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
static const GLfloat vertices[108] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
};

static const GLfloat colors[108] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
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
    
    // Create the Texture
    
    // Read the Bitmap
    unsigned char header[54];
    unsigned int dataPos = 0;
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int imageSize = 0;
    unsigned char* data = 0;
    
	// Get the header from the passed in data
    memcpy(header, rawdata, sizeof(unsigned char) * 54);

	if (header[0] != 'B' || header[1] != 'M') {
		return 0;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	
	if (imageSize == 0)
		imageSize = width * height * 3;

	if (dataPos == 0)
		dataPos = 54;

	// Allocate the Memory needed for the image
	data = (unsigned char*)malloc(sizeof(unsigned char) * imageSize);
	
	memcpy(data, rawdata, sizeof(unsigned char) * size);

	return 1;
}

int TextureResource_Unloader(ODIN_Resource* res) {
    
    return 0;
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

int FragmentShaderResource_Loader(ODIN_Resource* resource, ODIN_Int64 size, unsigned char* rawdata) {

	unsigned char* tmpSource = (unsigned char*)malloc(sizeof(unsigned char) * (size + 1));
	memcpy((void*)tmpSource, rawdata, sizeof(unsigned char) * (size + 1));
	tmpSource[size] = 0;

	resource->buffer = (void*)Shaders_newShaderSource(ODIN_SHADERTYPE_FRAGMENT, tmpSource);
	resource->total_bytes = sizeof(ODIN_ShaderSource) + (sizeof(unsigned char) * (size + 1));
	resource->buffer_size = sizeof(ODIN_ShaderSource);

	free(tmpSource);

	return 1;
}

int ShaderSourceResource_Unloader(ODIN_Resource* resource) {
	
	resource->buffer_size = 0;
	resource->total_bytes = 0;
	Shaders_freeShaderSource((ODIN_ShaderSource*)resource->buffer);
	resource->buffer = 0;

	return 1;
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
	Resources_addSearchPath("./assets", 1);
	ODIN_UInt64 RESTYPE_TEXTURE = Resources_registerType();
	ODIN_UInt64 RESTYPE_TEXT = Resources_registerType();
	ODIN_UInt64 RESTYPE_SHADERSRC = Resources_registerType();
	
	Resources_registerLoader(RESTYPE_TEXTURE, &BitmapResource_Loader, "*.bmp");
    //Resources_registerLoader(RESTYPE_TEXTURE, &TGAResource_Loader, "*.tga");
    //Resources_registerLoader(RESTYPE_TEXTURE, &DDSResource_Loader, "*.dds");
    Resources_registerUnloader(RESTYPE_TEXTURE, &TextureResource_Unloader);
    
	Resources_registerLoader(RESTYPE_TEXT, &TextResource_Loader, "*.txt");
	Resources_registerUnloader(RESTYPE_TEXT, &TextResource_Unloader);

	Resources_registerLoader(RESTYPE_SHADERSRC, &VertexShaderResource_Loader, "*.vs");
	Resources_registerLoader(RESTYPE_SHADERSRC, &FragmentShaderResource_Loader, "*.fs");
	Resources_registerUnloader(RESTYPE_SHADERSRC, &ShaderSourceResource_Unloader);
    
    if (!init())
        return 1;
    
    if (!loadMedia())
        return 1;
    

	printf("CacheSize: %llu\n", Resources_getCacheSize());

	if (!Resources_loadResource("hello.txt")) {
		printf("Unable to load file 'hello.txt', error happend.\n");
	}

	ODIN_Shader* shaderTest = 0;
	ODIN_Resource* vertShaderRes = Resources_getResource("test.vs");
	if (!vertShaderRes) {
		printf("Unable to load file 'test.vs', error happend.\n");
	} else {
		ODIN_ShaderSource* vertShader = (ODIN_ShaderSource*)vertShaderRes->buffer;
		printf("Vertex Shader Source: \n%s\n", vertShader->shader_source);


		ODIN_Resource* fragShaderRes = Resources_getResource("test.fs");
		if (!fragShaderRes) {
			printf("Unable to load file 'test.fs', error happend.\n");
		} else {
			ODIN_ShaderSource* fragShader = (ODIN_ShaderSource*)fragShaderRes->buffer;
			printf("Fragment Shader Source: \n%s\n", fragShader->shader_source);

			// Create the new Shader Program
			shaderTest = Shaders_newShader("test", vertShader, fragShader);

			Resources_releaseResource(fragShaderRes);
		}

		Resources_releaseResource(vertShaderRes);
	}
    
    printf("Shader Created: %s @ %d\n", shaderTest->name, shaderTest->program_id);

	

	if (!Resources_loadResource("helloworld.bmp")) {
		printf("Unable to load file 'helloworld.bmp', error happend.\n");
	}

	ODIN_Resource* helloText = Resources_getResource("hello.txt");
	if (!helloText) {
		printf("There was an error retreiving the resource 'hello.txt'\n");
	} else {
		printf("Retreived the resource %s\n", helloText->fname);
		char* text = (char*)helloText->buffer;
		printf("The text is: %s\n", text);

		printf("CacheSize: %llu\n", Resources_getCacheSize());

		Resources_releaseResource(helloText);
		helloText = 0;

		printf("CacheSize: %llu\n", Resources_getCacheSize());
	}
	
	// END TEST

	// TEST: init VMQ_Values
	// VMQ_Message *message = VMQ_StartMessage();
	// VMQ_PushInt(message, 20);
	// VMQ_PushUInt(message, 130009);
	// VMQ_PushString(message, "This is a TEST Message");

	// printf("Message Created, and values pushed!\n");
	// printf("Argument Count: %d, Value 1: %d, Value 2: %d\n", (*message).count, VMQ_GetInt(message, -3), VMQ_GetUInt(message, -2));
	// printf("The Last argument is: %s\n", VMQ_GetString(message, -1));
    
    // TEST: Matrices
	vec3_t cam_pos = vec3_create(NULL);
	cam_pos[0] = 4.0f;
	cam_pos[1] = 3.0f;
	cam_pos[2] = 3.0f;

    mat4_t projection = mat4_perspective(45.0, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.5f, 100.0f, NULL);
    mat4_t view = mat4_lookAt(
                              cam_pos,
                              (float[]){0.0f, 0.0f, 0.0f},
                              (float[]){0.0f, 1.0f, 0.0f},
                              NULL);
    mat4_t model = mat4_identity(NULL);

    
    mat4_t mvp = mat4_create(NULL);
    
    // Calculate the MVP
    mat4_multiply(projection, view, mvp);
    mat4_multiply(model, mvp, mvp);
    
    GLuint MvpID = glGetUniformLocation(shaderTest->program_id, "MVP");
    
    
    // END Matrices Init


	

	// TEMP: Render test setup
	printf("Starting gl triangle setup.\n");
	
	glGenVertexArrays(1, &VaoID);
	glBindVertexArray(VaoID);

	glGenBuffers(1, &VboID);
	glBindBuffer(GL_ARRAY_BUFFER, VboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 108, vertices, GL_STATIC_DRAW);
    
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 108, colors, GL_STATIC_DRAW);
    

    bool leftKey = 0;
    bool rightKey = 0;
    bool upKey = 0;
    bool downKey = 0;
    bool outKey = 0;
    bool inKey = 0;


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
				} else if (e.key.keysym.sym == SDLK_d) {
                    rightKey = 1;
				} else if (e.key.keysym.sym == SDLK_a) {
                    leftKey = 1;
                } else if (e.key.keysym.sym == SDLK_w) {
                    upKey = 1;
                } else if (e.key.keysym.sym == SDLK_s) {
                    downKey = 1;
                } else if (e.key.keysym.sym == SDLK_q) {
                    outKey = 1;
                } else if (e.key.keysym.sym == SDLK_e) {
                    inKey = 1;
                }
            } else if (e.type == SDL_KEYUP) {
                if (e.key.keysym.sym == SDLK_d) {
                    rightKey = 0;
                } else if (e.key.keysym.sym == SDLK_a) {
                    leftKey = 0;
                } else if (e.key.keysym.sym == SDLK_w) {
                    upKey = 0;
                } else if (e.key.keysym.sym == SDLK_s) {
                    downKey = 0;
                } else if (e.key.keysym.sym == SDLK_q) {
                    outKey = 0;
                } else if (e.key.keysym.sym == SDLK_e) {
                    inKey = 0;
                }
            }
		}
        
        if (leftKey | rightKey | upKey | downKey | inKey | outKey) {
            
            cam_pos[0] += rightKey * 0.1f;
            cam_pos[0] -= leftKey * 0.1f;
            
            cam_pos[1] += upKey * 0.1f;
            cam_pos[1] -= downKey * 0.1f;
            
            cam_pos[2] -= inKey * 0.1f;
            cam_pos[2] += outKey * 0.1f;
            
            mat4_lookAt(
                        cam_pos,
                        (float[]) {0.0f, 0.0f, 0.0f},
                        (float[]) {0.0f, 1.0f, 0.0f},
                        view);
            
            // Calculate the MVP
            mat4_multiply(projection, view, mvp);
            mat4_multiply(model, mvp, mvp);
        }

		while (VMQ_ProcessMessage(queue)) {
			// This will loop until the queue is empty
		}

		/* Check for Messages in VikingMQ */


		// OpenGL Rendering loop
		Window_clearColor(100.0/255.0, 149.0/255.0, 237.0/255.0, 1.0);
        
        glUseProgram(shaderTest->program_id);
        glUniformMatrix4fv(MvpID, 1, GL_FALSE, mvp);

		// Render studd here?
		glEnableVertexAttribArray(0);
        
		glBindBuffer(GL_ARRAY_BUFFER, VboID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
                              1,                   // attribute. No particular reason for 1, but must match the layout in the shader.
                              3,                   // size (num elements)
                              GL_FLOAT,            // element type
                              GL_FALSE,            // normalized?
                              0,                   // stride
                              (void*)0             // array buffer offset
                              );
        
		glDrawArrays(GL_TRIANGLES, 0, 12*3);
		glDisableVertexAttribArray(0);


		Window_swapBuffer(window);

	}
	
    Shaders_freeShader(shaderTest);
    
    free(projection);
    free(view);
    free(model);
	free(mvp);

	close();

	return 0;
}