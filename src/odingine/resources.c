#include "resources.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <physfs.h>

// Needed for the WildcardMatch function
#include "viking_string.h"

#include "utils/hashmap.h"

//********************************************************************
// Resource Loaders & Type management
typedef struct ODIN_ResourceLoader {
    ODIN_UInt32 type_id;
    Resources_Loader handler;
    const char* pattern; 
} ODIN_ResourceLoader;

typedef struct ODIN_ResourceUnloader {
    ODIN_UInt32 type_id;
    Resources_Unloader handler;
} ODIN_ResourceUnloader;

static ODIN_UInt32 nextResourceId = 0;
static ODIN_UInt32 numLoaders = 0;
static ODIN_ResourceLoader* registeredLoaders = 0;
static ODIN_UInt32 numUnloaders = 0;
static ODIN_ResourceUnloader* registeredUnloaders = 0;

// End: Resource Loaders & Type management
//********************************************************************


// Declare any functions that should not be available outside this file
ODIN_Resource* Resources_newResource();
int Resources_freeResource(ODIN_Resource* resource);
ODIN_ResourceLoader* Resources_findLoader(const char* fname);
ODIN_ResourceUnloader* Resources_findUnloader(ODIN_UInt32 type_id);
//int Resources_readFile()

//********************************************************************
// Resource Cache management

static ODIN_UInt64 cacheSize = 0;
static ODIN_UInt64 cacheSizeLimit = 0;
static map_t resourceCache = 0;

// End: Resource Cache management
//********************************************************************



int Resources_init(char* arg0, ODIN_UInt64 mem_limit) {
    // Initialize the PhysicsFS

    if (PHYSFS_init(arg0) == 0) {
        printf("Error loading the PhysicsFS!\n");
        return 1;
    }

    printf("application dir: %s\n", PHYSFS_getBaseDir());

    // Initialize the Resource Cache (HashMap)
    resourceCache = hashmap_new();
    
    cacheSizeLimit = mem_limit;

    return 0;
}

void Resources_uninit() {
    PHYSFS_deinit();
    hashmap_free(resourceCache);
}

ODIN_UInt32 Resources_registerType() {
    return nextResourceId++;
}

ODIN_UInt32 Resources_registerLoader(ODIN_UInt32 type_id, Resources_Loader handler, const char* pattern) {
    // pre-imptively increment the loaders count
    numLoaders++;

    // Add loader to the registeredLoaders
    if (numLoaders == 1) {
        // We are allocating the list for the first time
        registeredLoaders = (ODIN_ResourceLoader*)malloc(sizeof(ODIN_ResourceLoader));
    } else {
        // We need to expand the array to allow for a new ODIN_ResourceLoader
        registeredLoaders = (ODIN_ResourceLoader*)realloc(registeredLoaders, sizeof(ODIN_ResourceLoader) * numLoaders);
    }

    // Get the pointer to the newly created loader
    ODIN_ResourceLoader* loader = &(registeredLoaders[numLoaders - 1]);

    // Populate the data in the Loader
    loader->type_id = type_id;
    loader->handler = handler;
    
    // Copy the pattern and place a pointer in the struct (remember to delete it in the unregister function)
    const char* tmpPattern = (char*)malloc(sizeof(char) * (strlen(pattern) + 1));
    memcpy((char*)tmpPattern, pattern, sizeof(char) * (strlen(pattern) + 1));
    loader->pattern = tmpPattern;

    return 1;
}

ODIN_UInt32 Resources_registerUnloader(ODIN_UInt32 type_id, Resources_Unloader handler) {
    numUnloaders++;

    if (numUnloaders == 1) {
        registeredUnloaders = (ODIN_ResourceUnloader*)malloc(sizeof(ODIN_ResourceUnloader));
    } else {
        registeredUnloaders = (ODIN_ResourceUnloader*)realloc(registeredUnloaders, sizeof(ODIN_ResourceUnloader) * numUnloaders);
    }

    ODIN_ResourceUnloader* unloader = &(registeredUnloaders[numUnloaders - 1]);

    unloader->type_id = type_id;
    unloader->handler = handler;

    return 1;
}

void Resources_freeLoaders() {
    // TODO: We need to loop over all of the ODIN_ResourceLoader's
    //  and free them, including the pattern that was malloc'd

    
}

ODIN_UInt32 Resources_loadResource(const char* fname) {
    // Check if file is already loaded
    ODIN_Resource* res;
    int err = hashmap_get(resourceCache, fname, (void**)(&res));
    if (err == MAP_OK) {
        // ERROR: The file is already loaded into memory
        // TODO: We may want to add the ability to reload
        return 0;
    }
    
    
    ODIN_ResourceLoader* loader = Resources_findLoader(fname);
    if (loader == 0) {
        // There is no loader registered to handle this file extension
        return 0;
    }

    // Read the file's contents
    
    // Check if the file exists
    if (!PHYSFS_exists(fname)) {
        // The file requested doesn't exist
        return 0;
    }

    // Open File for reading.
    PHYSFS_file* file = PHYSFS_openRead(fname);

    // Get the file size in bytes
    PHYSFS_sint64 fSize = PHYSFS_fileLength(file);
    
    // Check if there is space available for at least the buffer size
    if (cacheSizeLimit - cacheSize <= fSize) {
        // We need to clear up some space for the resource to load
        //TODO: Build a freeOne and freeAll function to free up some unused resources
    }

    // Create the buffer to read the file into
    unsigned char* fBuffer = (unsigned char*)malloc(sizeof(unsigned char) * fSize);

    // Read the file into the buffer
    ODIN_Int64 length_read = PHYSFS_read(file, fBuffer, 1, fSize);

    // Compare the size read to the file size
    if (fSize != length_read) {
        // The file was not read completely
        // Free resources before returning 0
        PHYSFS_close(file);
        free(fBuffer);
        return 0;
    }

    // The file was read correctly
    // Create the Resource and send it to the loader
    res = Resources_newResource();
    res->type_id = loader->type_id;
    res->ref_count = 0;

    char* tmpFname = (char*)malloc(sizeof(char) * (strlen(fname) + 1));
    memcpy(tmpFname, fname, sizeof(char) * (strlen(fname) + 1));
    res->fname = tmpFname;


    if (!loader->handler(res, length_read, fBuffer)) {
        // The loader says it failed to load the file
        PHYSFS_close(file);
        free(fBuffer);
        free(res);
        return 0;
    }

    // The resource has been loaded
    // Add it to the hash map
    err = hashmap_put(resourceCache, fname, res);

    if (err != MAP_OK) {
        // There was an error inserting the resource into the cache
        PHYSFS_close(file);
        
        // Unload the resource, there was an error putting it in the hashmap
        Resources_freeResource(res);
        
        free(fBuffer);
        free(res);
        return 0;
    }

    // Free temp memory used
    PHYSFS_close(file);
    free(fBuffer);
    
    // Add the memory used to the cacheSize
    cacheSize += res->total_bytes;

    return 1;
}

ODIN_Resource* Resources_getResource(const char* fname) {
    //TODO: Get the resource from the HashMap if exists
    //  If it isn't loaded yet, load the resource and
    //  return the loaded resource

    ODIN_Resource* res;
    int err = hashmap_get(resourceCache, fname, (void**)(&res));
    
    if (err != MAP_OK) {
        // We need to try and load the resource
        if (!Resources_loadResource(fname)) {
            return 0;
        }

        err = hashmap_get(resourceCache, fname, (void**)(&res));

        if (err != MAP_OK) {
            return 0;
        }
    }

    res->ref_count++;
    return res;
}

/**
    @description 
        Decrements the ref_count of the resource to track if it is still used    
    @return number of remaining references to resource
**/
ODIN_UInt32 Resources_releaseResource(ODIN_Resource* res) {
    res->ref_count--;

    // TEMP: Remove this after the testing is done
    //  resources should only be freed if space is needed,
    //  or we are loading a pack or unloding a pack, or otherwise
    //  directly requested
    if (res->ref_count == 0) {
        Resources_freeResource(res);
    }

    return 0;
}

ODIN_UInt64 Resources_getCacheSize() {
    return cacheSize;
}

int Resources_fileExists(const char* fname) {
    return PHYSFS_exists(fname);
}

int Resources_addSearchPath(const char* path, int append) {
    PHYSFS_addToSearchPath(path, append);
    
    return 1;
}

//******************************************************************************
// Local file function 
//    - not defined in the header
//******************************************************************************
ODIN_Resource* Resources_newResource() {
    ODIN_Resource* res = (ODIN_Resource*)malloc(sizeof(ODIN_Resource));
    return res;
}

int Resources_freeResource(ODIN_Resource* resource) {
    // TODO: Check if the resource has references
    //  Fail if the ref_count is greater than 0
    //  Else: Free the Buffer and then Free the resource

    if (resource->ref_count > 0) {
        return 0;
    }

    // TODO: Call the registered unloader of the data
    // The data in the buffer might contain pointers to other memory (not exactly a good thing)
    // but the unloader should understand the type_id's file format and be able to free it correctly
    ODIN_ResourceUnloader* unloader = Resources_findUnloader(resource->type_id);
    
    if (!unloader) {
        printf("Unable to find unloader for registered type: %d\n", resource->type_id);
        return 0;
    }
    
    ODIN_UInt64 res_size = resource->total_bytes;
    if (!unloader->handler(resource)) {
        // Error, unable to free resource
        return 0;
    }
    
    // Remove from cacheSize counter
    cacheSize -= res_size;
 
    return 1;
}

ODIN_ResourceLoader* Resources_findLoader(const char* fname) {
    // Loop each of the known loaders until the fname matches a pattern
    // Return the first ODIN_ResourceLoader* that matches, else return 0

    ODIN_ResourceLoader* tmpLoader = 0;
    for (int i = 0; i < numLoaders; i++) {
        tmpLoader = &(registeredLoaders[i]);
        if (WildcardMatch(tmpLoader->pattern, fname)) {
            return tmpLoader;
        }
    }

    return NULL;
}

ODIN_ResourceUnloader* Resources_findUnloader(ODIN_UInt32 type_id) {
    printf("finding unloader\n");
    ODIN_ResourceUnloader* tmpUnloader = 0;
    for (int i = 0; i < numUnloaders; i++) {
        tmpUnloader = &(registeredUnloaders[i]);
        if (tmpUnloader->type_id == type_id) {
            return tmpUnloader;
        }
    }

    return NULL;
}