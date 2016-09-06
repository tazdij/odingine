/*
	VikingMQ - Message Queue library
	Copyright 2015 - MidgetViking Games
	Developed by: Don Duvall
	License: GPLv3
	Date: 2015-04-19

	Description:
		VikingMQ is a simple MessageQueue library used for the odingine and libraries. Allowing for
		all systems and subsystems of a game and game engine to communicate without sharing memory
		or possibly even the same thread. (Hopefully allowing for 'OdiNgine' to be able to be a fully
		threaded game engine)

		The top level view of this library should start with the ability to send
		a message with any amount of arguments. Followed by the ability to 
		listen to all messages or limited amounts by filtering by Name, Type or System.

		The structure of this sytem is:
			Message:
				A message contains a private Argument Stack of any amount of arguments
				of type VMQ_Value. The subscriber and publisher must place the arguments on the stack
				in order by type, and read them by type in order.
				Argument Count




	Updates:
		2016-01-01: Change Doubly-Linked List structure, into a contiguos array format. (Making read speeds much faster)

*/

#ifndef __VIKINGMQ_H_
#define __VIKINGMQ_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define VMQ_TYPE_INT	= 0;
#define VMQ_TYPE_UINT	= 1;
#define VMQ_TYPE_FLOAT	= 2;
#define VMQ_TYPE_DOUBLE	= 3;
#define VMQ_TYPE_STRING	= 4;

/*
	VMQ_Value is used to store a variant like structure of data, while it is
	not typeless and is not able to type cast; it does allow for a single
	value type to be assigned in the stack.
*/
typedef struct VMQ_Value_t VMQ_Value;
struct VMQ_Value_t {
	int type;
	union {
		int intval;
		unsigned int uintval;
		float floatval;
		double doubleval;
		char *stringval;
		/* Possibly add NestedValues (Similar to Lua's) */
		/* VMQ_StackEntry *stackval */
	};
};

/*
	UPDATED: This is possibly not needed
	VMQ_StackEntry contains the value and the pointers to next and prev entries
	in the stack 
*/
/*typedef struct VMQ_StackEntry_t {
	VMQ_Value val;
	struct VMQ_StackEntry *prev;
	struct VMQ_StackEntry *next;
} VMQ_StackEntry;*/

/*
	VMQ_Message contains the value array including count, a name for debugging 
	(possibly filtering). The name is used for the Pub/Sub pattern
*/
typedef struct VMQ_Message_t {
	int count;
	VMQ_Value *values;
	char *name; // For debug purposes
} VMQ_Message;

/*
TypeDef of a Handler Function (Message Receiver)
*/
typedef void (*VMQ_Receiver)(VMQ_Message* message, void *context);

/*
	VMQ_ReceiverNode is a boxed receiver function. This means that
	it is possible to store the pointer of an object that should
	be passed to the receiver every time it is called.

	Example:
		// Below we only want to play the laser sound if the laser canon
		// is the currently selected weapon
		void PlayShootSound(VMQ_Message* message, void* player) {
			...
		}
		VMQ_SubscribeCtx(eventQueue, "shoot", &PlayShootSound, (void*)player);

	Now every time the 'shoot' message is published, the PlayShootSound
	function will be called, and the pointer to player will be passed to
	the function. This will allow the function to use this as a sort of
	context. Especially is cases where the same function may be registered
	to the same message however need to affect different entities or actors.
*/
typedef struct VMQ_ReceiverNode_t {
	VMQ_Receiver callback;
	void *context;
} VMQ_ReceiverNode;


/* 
	VMQ_ReceiverStack is a stack of boxed receiver pointers and the label
	which they are all listening for.

	To add a receiver to a stack you need to call either the VMQ_Subscribe
	or VMQ_SubscribeCtx function. It will Box the Receiver, and add it to
	appropriate stack object.
*/
typedef struct VMQ_ReceiverStack_t {
	unsigned int count;
	VMQ_ReceiverNode *receivers;
	char *label;
} VMQ_ReceiverStack;

/*
	VMQ_MessageQueue is the structure that holds all of the messages in a queue
	as they are processed, and holds all of the 'Listeners' which should be notified
	when a message is processed
*/
typedef struct VMQ_MessageQueue_t {
	unsigned int count; /* Contains the size of the Queue array */
	VMQ_Message *messages;

	unsigned int numstacks;
	VMQ_ReceiverStack *stacks;


} VMQ_MessageQueue;

// Declare Message Receiver handling functions

// Declare all of the Messaging functions
VMQ_Message* VMQ_StartMessage();
void VMQ_PushMessage(VMQ_MessageQueue *queue, VMQ_Message *message, const char *name);
int VMQ_PushInt(VMQ_Message *message, int intval);
int VMQ_PushUInt(VMQ_Message *message, unsigned int uintval);
int VMQ_PushFloat(VMQ_Message *message, float floatval);
int VMQ_PushDouble(VMQ_Message *message, double doubleval);
int VMQ_PushString(VMQ_Message *message, const char *stringval);
void VMQ_FreeMessage(VMQ_Message *message);

int VMQ_GetInt(VMQ_Message *message, int index);
unsigned int VMQ_GetUInt(VMQ_Message *message, int index);
float VMQ_GetFloat(VMQ_Message *message, int index);
double VMQ_GetDouble(VMQ_Message *message, int index);
char* VMQ_GetString(VMQ_Message *message, int index);

VMQ_MessageQueue* VMQ_NewQueue();
void VMQ_FreeQueue(VMQ_MessageQueue *queue);

VMQ_ReceiverStack* VMQ_NewQueueStack(VMQ_MessageQueue *queue, char *label);
VMQ_ReceiverStack* VMQ_GetQueueStack(VMQ_MessageQueue *queue, char *label);

void VMQ_Subscribe(VMQ_MessageQueue *queue, const char *label, VMQ_Receiver cb);
void VMQ_SubscribeCtx(VMQ_MessageQueue *queue, const char *label, VMQ_Receiver cb, void *context);

// Declare Queue functions
int VMQ_ProcessMessage(VMQ_MessageQueue *queue);
int VMQ_IsMessage(VMQ_MessageQueue *queue);


#endif
