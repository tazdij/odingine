/**
    Resources - responsible for all resource loading and memory (resource) management.

    Description:
        Resource management is sometimes a tricky task, loading assets at the right time
        and keeping them in memory for as long as they are needed. For the first pass
        on this Resource Manager, we are going to implement a Naive reference counting
        system, and have a unique ID and indexing system. Allowing an Integer to be used
        to identify resources once loaded.
**/


/**
    -- Dynamic loaders --

    During the initialization process, the application will need to register types which
    will create a Unique Idenetifier for the type. Using this unique id the application
    needs to register search patterns, which are used during the resource loading process.
    After search patterns are registered the application will need to regiter the function
    responsible for loading the requested file into memory.

    The Resource_Loader - is responsible for taking the buffer data (raw bytes) read from
    the file and converting it into a usable object by the engine. This type used by the
    engine is normally Audio, 2D/3D Mesh, Texture, Config <XML> file, Script bytecode or
    Script source code. As the engine is developed more types will be added to enable new
    features.
**/

/**
    -- Resource Management --

    All loaded resources will be placed in a simple HashMap that is keyed on the fileName.
    Resources are stored in the ODIN_Resource struct with a pointer to the loaded data as
    a Byte Array. This is the result of calling the correct Resource_Loader, which takes
    the Raw file Bytes and creates

**/
#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "viking_types.h"


typedef struct ODIN_Resource {
    ODIN_UInt32 type_id;
    ODIN_UInt64 buffer_size;
	ODIN_UInt64 total_bytes; // The loader is responsible for determining this size
	ODIN_UInt32 ref_count;
	void* buffer;    
    const char* fname; // the location of the original file
} ODIN_Resource;


typedef int (*Resources_Loader)(ODIN_Resource* resource, ODIN_Int64 size, unsigned char* rawdata);


/*
	Resources_Unloader
	The function is responsible for correctly freeing the resource->buffer
	and is NOT Responsible for freeing the resource itself, this will be
	done after the Unloader returns 1 to the caller.
*/
typedef int (*Resources_Unloader)(ODIN_Resource* resource);

int Resources_init(char* arg0, ODIN_UInt64 mem_limit);
void Resources_uninit();

ODIN_UInt32 Resources_registerType();
ODIN_UInt32 Resources_registerLoader(ODIN_UInt32 type_id, Resources_Loader handler, const char* pattern);
ODIN_UInt32 Resources_registerUnloader(ODIN_UInt32 type_id, Resources_Unloader handler);


ODIN_UInt32 Resources_loadResource(const char* fname);
ODIN_Resource* Resources_getResource(const char* fname);

// This will open an XML file and load all of the resources
// listed in the group.
//ODIN_UInt32 Resources_loadResourceGroup(const char* fname);


ODIN_UInt32 Resources_releaseResource(ODIN_Resource* res);

ODIN_UInt64 Resources_getCacheSize();
int Resources_fileExists(const char* fname);
int Resources_addSearchPath(const char* path, int append);

//int Resources_load(const char* path);
//int Resources_get(const char* path);

#endif