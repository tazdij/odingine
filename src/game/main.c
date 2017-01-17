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
#include "textures.h"

#include "mesh.h"

#include "viking_math.h"

#include "mpc.h"

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

static const GLuint indices[12] = {
	0, 1, 2, // Front Tri
	2, 3, 0, // Right Tri
	0, 3, 4, // Back Tri
	4, 1, 0  // Left Tri
};

static const GLfloat vertices[15] = {
	 0.0f, 1.0f, 0.0f, // 0 - Top Center
	-1.0f,-1.0f,-1.0f, // 1 - Bottom Left Front
	 1.0f,-1.0f,-1.0f, // 2 - Bottom Right Front
	 1.0f,-1.0f, 1.0f, // 3 - Bottom Right Back
	-1.0f,-1.0f, 1.0f  // 4 - Bottom Left Back
};

static const GLfloat colors[15] = {
	1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.5f, 0.5f
};


static const GLfloat uvs[10] = {
	0.5f, 0.5f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};


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
	data = (unsigned char*)malloc(sizeof(unsigned char) * size);
	memcpy((void*)data, rawdata, sizeof(unsigned char) * size);
    
    resource->buffer = (void*)Textures_createTexture(width, height, GL_RGB, size, rawdata);
    resource->buffer_size = sizeof(ODIN_Texture);
    
    //TODO: Get the complete size of the resource in memory
    
    free(data);
    
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

int ObjResource_numberOfNodes(mpc_ast_t* t) {
	if (t->children_num == 0) { return 1; }
	if (t->children_num >= 1) {
		int total = 1;
		for (int i = 0; i < t->children_num; i++) {
			total = total + ObjResource_numberOfNodes(t->children[i]);
		}
		return total;
	}

	return 0;
}

typedef struct {
	float* verts;
	int num_verts;

	float* uvs;
	int num_uvs;

	float* normals;
	int num_normals;

	/*
	OJB Format
	f 1/1/10 2/1/30 3/2/1

	f: the prefix for the Polygon or Shape creation
	1/1/10: this is the indexes for the first point in the Polygon
	Vertex / UV / Normal
	*/
	int* indices; //the structure of this is 3 sets of 3 = one polygon
	int num_indices;
} ObjResource_t;

ObjResource_t* ObjResource_loadAst(mpc_ast_t* t) {
	return NULL;
}

int ObjResource_Loader(ODIN_Resource* resource, ODIN_Int64 size, unsigned char* rawdata) {
	printf("Loading .obj file: %s\n", resource->fname);

	unsigned char* tmpSource = (unsigned char*)malloc(sizeof(unsigned char) * (size + 1));
	memcpy((void*)tmpSource, rawdata, sizeof(unsigned char) * (size + 1));
	tmpSource[size] = 0;

	float* verts;
	float* uvs;
	float* normals;

	int file_pos = 0;

	mpc_result_t r;
	mpc_parser_t* Number = mpc_new("number");
	mpc_parser_t* Symbol = mpc_new("symbol");
	mpc_parser_t* Index = mpc_new("index");
	mpc_parser_t* V = mpc_new("v");
	mpc_parser_t* Vt = mpc_new("vt");
	mpc_parser_t* Vn = mpc_new("vn");
	mpc_parser_t* G = mpc_new("g");
	mpc_parser_t* Usemtl = mpc_new("usemtl");
	mpc_parser_t* Mtllib = mpc_new("mtllib");
	mpc_parser_t* S = mpc_new("s");
	mpc_parser_t* F = mpc_new("f");
	mpc_parser_t* Cmd = mpc_new("cmd");
	mpc_parser_t* Obj = mpc_new("obj");

	mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT,
		"number               : /[\\-0-9\\.]/                         ; \
		 symbol               : /[a-zA-Z\\._\\-]+/					  ; \
		 index                : <number> '/' <number> '/' <number>    ; \
		 v                    : \"v\" <number>+      ; \
		 vt                   : \"vt\" <number>+              ; \
		 vn                   : \"vn\" <number>+     ; \
		 g                    : \"g\" <symbol>                        ; \
		 usemtl               : \"usemtl\" <symbol>                   ; \
		 mtllib               : \"mtllib\" <symbol>                   ; \
		 s                    : \"s\" <number>                        ; \
		 f                    : \"f\" <index>+         ; \
		 cmd                  : <usemtl> | <mtllib> | <vn> | <vt> | <v> | <g> | <s> | <f> ; \
		 obj                  : /^/ <cmd>* /$/ ;"
		, Number, Symbol, Index, V, Vt, Vn, G, Usemtl, Mtllib, S, F, Cmd, Obj, NULL);

	if (err != NULL) {
		mpc_err_print(err);
		mpc_err_delete(err);
		//exit(1);
	}

	if (mpc_parse(resource->fname, tmpSource, Obj, &r)) {
		mpc_ast_print(r.output);
		mpc_ast_t* ast = (mpc_ast_t*)r.output;

		//int total_nodes = ObjResource_numberOfNodes(ast);
		//printf("Total Nodes: %d\n", total_nodes);

		printf("num children: %d", ast->children_num);

		mpc_ast_delete(r.output);
	} else {
		mpc_err_print(r.error);
		mpc_err_delete(r.error);
	}


	mpc_cleanup(13, Number, Symbol, Index, V, Vt, Vn, G, Usemtl, Mtllib, S, F, Cmd, Obj);

	//resource->buffer = (void*)Shaders_newShaderSource(ODIN_SHADERTYPE_FRAGMENT, tmpSource);
	//resource->total_bytes = sizeof(ODIN_ShaderSource) + (sizeof(unsigned char) * (size + 1));
	//resource->buffer_size = sizeof(ODIN_ShaderSource);

	free(tmpSource);

	
}

int MeshResource_Unloader(ODIN_Resource* resource) {

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
	ODIN_UInt64 RESTYPE_MESH = Resources_registerType();
	
	// Texture Loaders
	Resources_registerLoader(RESTYPE_TEXTURE, &BitmapResource_Loader, "*.bmp");
    //Resources_registerLoader(RESTYPE_TEXTURE, &TGAResource_Loader, "*.tga");
    //Resources_registerLoader(RESTYPE_TEXTURE, &DDSResource_Loader, "*.dds");
    Resources_registerUnloader(RESTYPE_TEXTURE, &TextureResource_Unloader);
    
	// Text Loaders
	Resources_registerLoader(RESTYPE_TEXT, &TextResource_Loader, "*.txt");
	Resources_registerUnloader(RESTYPE_TEXT, &TextResource_Unloader);

	// Shader Loaders
	Resources_registerLoader(RESTYPE_SHADERSRC, &VertexShaderResource_Loader, "*.vs");
	Resources_registerLoader(RESTYPE_SHADERSRC, &FragmentShaderResource_Loader, "*.fs");
	Resources_registerUnloader(RESTYPE_SHADERSRC, &ShaderSourceResource_Unloader);

	// Mesh Loaders
	Resources_registerLoader(RESTYPE_MESH, &ObjResource_Loader, "*.obj");
	Resources_registerUnloader(RESTYPE_MESH, &MeshResource_Unloader);
    
    if (!init())
        return 1;
    
    if (!loadMedia())
        return 1;
    

	printf("CacheSize: %llu\n", Resources_getCacheSize());

	if (!Resources_loadResource("hello.txt")) {
		printf("Unable to load file 'hello.txt', error happend.\n");
	}

	Resources_loadResource("cube.obj");

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
	mat4_t model2 = mat4_identity(NULL);

	vec3_t model2_pos = vec3_create(NULL);
	model2_pos[0] = 2.0f;
	model2_pos[1] = 0.0f;
	model2_pos[2] = 2.0f;

	mat4_translate(model2, model2_pos, NULL);

    
	mat4_t vp = mat4_create(NULL);
    mat4_t mvp = mat4_create(NULL);

    
    // Calculate the MVP
    mat4_multiply(projection, view, vp);
    mat4_multiply(vp, model, mvp);
    
    //GLuint MvpID = glGetUniformLocation(shaderTest->program_id, "MVP");
    
    
    // END Matrices Init


	

	// TEMP: Render test setup
	printf("Starting gl triangle setup.\n");
	
	
	// TEMP: Test the Mesh Object
    ODIN_Mesh* mesh =  Mesh_newMesh();
    Mesh_loadBufferData(mesh, 0, GL_ARRAY_BUFFER, GL_FLOAT, sizeof(GLfloat), 3, &vertices[0], 15);
    Mesh_loadBufferData(mesh, 1, GL_ARRAY_BUFFER, GL_FLOAT, sizeof(GLfloat), 3, &colors[0], 15);
    Mesh_loadBufferData(mesh, 2, GL_ARRAY_BUFFER, GL_FLOAT, sizeof(GLfloat), 2, &uvs[0], 10);

    // Load the index buffer now
    Mesh_loadIBOData(mesh, &indices[0], 12);
    
    

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
            mat4_multiply(projection, view, vp);
        }


		/* Check for Messages in VikingMQ */
		//while (VMQ_ProcessMessage(queue)) {
			// This will loop until the queue is empty
		//}

		


		// OpenGL Rendering loop
		Window_clearColor(100.0/255.0, 149.0/255.0, 237.0/255.0, 1.0);
        
        
		Shaders_setShader(shaderTest);

		mat4_multiply(vp, model, mvp);
        Shaders_setUniformMat4(shaderTest, "MVP", mvp);
        
        // Bind mesh vao, This also binds the ibo if present
        Mesh_bindVao(mesh);
        
		// Draw the Elements from the 
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        
		// Unbind mesh ibo and vao
        Mesh_unbindVao();



		
		//vec3_str(model2_pos, buf);
		//printf("model2_pos: %s\n", buf);

		//mat4_str(model2, buf);
		//printf("model2: %s\n", buf);

		mat4_multiply(vp, model2, mvp);
		//mat4_str(mvp, buf);
		//printf("mvp: %s\n", buf);
		Shaders_setUniformMat4(shaderTest, "MVP", mvp);

		// Bind mesh vao, This also binds the ibo if present
		Mesh_bindVao(mesh);

		// Draw the Elements from the 
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		// Unbind mesh ibo and vao
		Mesh_unbindVao();



		// Unset the Shader Program Used
		Shaders_unsetShader();


		// Swap the buffer
		Window_swapBuffer(window);
	}
	
    Shaders_freeShader(shaderTest);
    
    free(projection);
    free(view);
    free(model);
	free(model2);
	free(model2_pos);
	free(mvp);

	close();

	return 0;
}