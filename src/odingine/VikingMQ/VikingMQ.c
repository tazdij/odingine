#include "VikingMQ.h"

VMQ_Message* VMQ_StartMessage() {

	/* Allocate the message in application space memory */
	VMQ_Message *message = (VMQ_Message*)malloc(sizeof(VMQ_Message));

	/* Initialize count to zero */
	(*message).count = 0;

	return message;
}

void VMQ_PushMessage(VMQ_MessageQueue *queue, VMQ_Message* message, const char* name) {

}

int VMQ_PushInt(VMQ_Message* message, int intval) {

	/* Increment the total count of arguments in message */
	(*message).count++;

	/* Expand the available memory for the new value, or allocate if first value */
	if ((*message).count == 1) {
		(*message).values = (VMQ_Value*)malloc(sizeof(VMQ_Value));
	} else {
		(*message).values = realloc((*message).values, sizeof(VMQ_Value) * (*message).count);
	}

	/* Set the value of the new argument */
	(*message).values[(*message).count - 1].intval = intval;

	return (*message).count - 1;
}

int VMQ_PushUInt(VMQ_Message* message, unsigned int uintval) {

	/* Increment the total count of arguments in message */
	(*message).count++;

	/* Expand the available memory for the new value, or allocate if first value */
	if ((*message).count == 1) {
		(*message).values = (VMQ_Value*)malloc(sizeof(VMQ_Value));
	} else {
		(*message).values = realloc((*message).values, sizeof(VMQ_Value) * (*message).count);
	}

	/* Set the value of the new argument */
	(*message).values[(*message).count - 1].uintval = uintval;

	return (*message).count - 1;
}

int VMQ_PushFloat(VMQ_Message* message, float floatval) {
	
	/* Increment the total count of arguments in message */
	(*message).count++;

	/* Expand the available memory for the new value, or allocate if first value */
	if ((*message).count == 1) {
		(*message).values = (VMQ_Value*)malloc(sizeof(VMQ_Value));
	}
	else {
		(*message).values = realloc((*message).values, sizeof(VMQ_Value) * (*message).count);
	}

	/* Set the value of the new argument */
	(*message).values[(*message).count - 1].floatval = floatval;

	return (*message).count - 1;
}

int VMQ_PushDouble(VMQ_Message* message, double doubleval) {
	/* Increment the total count of arguments in message */
	(*message).count++;

	/* Expand the available memory for the new value, or allocate if first value */
	if ((*message).count == 1) {
		(*message).values = (VMQ_Value*)malloc(sizeof(VMQ_Value));
	}
	else {
		(*message).values = realloc((*message).values, sizeof(VMQ_Value) * (*message).count);
	}

	/* Set the value of the new argument */
	(*message).values[(*message).count - 1].doubleval = doubleval;

	return (*message).count - 1;
}

int VMQ_PushString(VMQ_Message* message, const char* stringval) {
	/* Increment the total count of arguments in message */
	(*message).count++;

	/* Expand the available memory for the new value, or allocate if first value */
	if ((*message).count == 1) {
		(*message).values = (VMQ_Value*)malloc(sizeof(VMQ_Value));
	}
	else {
		(*message).values = realloc((*message).values, sizeof(VMQ_Value) * (*message).count);
	}

	/* Allocate the memory needed for the string value in VMQ_Value */

	/* Get a local pointer to the new value in message */
	VMQ_Value *val = &( (*message).values[(*message).count - 1] );

	/* Add the extra space for string in new val */
	(*val).stringval = (char*)malloc((sizeof(char) * strlen(stringval)) + 1);
	strncpy((*val).stringval, stringval, (sizeof(char) * strlen(stringval)) + 1);


	//(*var).stringval = (char*)malloc((sizeof(char) * strlen(string)) + 1);
	//strncpy((*var).stringval, string, (sizeof(char) * strlen(string)) + 1);

	return (*message).count - 1;
}


int VMQ_GetInt(VMQ_Message* message, int index) {

	// Stack Example
	// |  0 |  1 |  2 |
	// | -3 | -2 | -1 |

	int idx = index;
	if (index < 0) {
		idx = (*message).count + index;
	}

	return (*message).values[idx].intval;
}

unsigned int VMQ_GetUInt(VMQ_Message* message, int index) {
	int idx = index;
	if (index < 0) {
		idx = (*message).count + index;
	}

	return (*message).values[idx].uintval;
}

float VMQ_GetFloat(VMQ_Message* message, int index) {
	int idx = index;
	if (index < 0) {
		idx = (*message).count + index;
	}

	return (*message).values[idx].floatval;
}

double VMQ_GetDouble(VMQ_Message* message, int index) {
	int idx = index;
	if (index < 0) {
		idx = (*message).count + index;
	}

	return (*message).values[idx].doubleval;
}

char* VMQ_GetString(VMQ_Message* message, int index) {

	int idx = index;
	if (index < 0) {
		idx = (*message).count + index;
	}

	return (*message).values[idx].stringval;
}

VMQ_MessageQueue* VMQ_NewQueue() {
	//Malloc a new empty queue
	VMQ_MessageQueue *queue = (VMQ_MessageQueue*)malloc(sizeof(VMQ_MessageQueue));
	(*queue).count = 0;
	(*queue).messages = NULL;
	(*queue).numstacks = 0;
	(*queue).stacks = NULL;
	return queue;
}

void VMQ_FreeQueue(VMQ_MessageQueue *queue) {
	printf("VMQ_FreeQueue: TODO. Free the memory used by the queue\n");
}

VMQ_ReceiverStack* VMQ_NewQueueStack(VMQ_MessageQueue *queue, char *label) {
	// Preimptive add the count to the stack counter for this queue
	(*queue).numstacks++;

	// Make room and place the stack in the queue
	if ((*queue).numstacks == 1) {
		// First entry - malloc memory
		(*queue).stacks = (VMQ_ReceiverStack*)malloc(sizeof(VMQ_ReceiverStack));
	} else {
		// Additional entry - realloc memory
		(*queue).stacks = realloc((*queue).stacks, sizeof(VMQ_ReceiverStack) * (*queue).numstacks);
	}

	// Malloc the new stack object & set defaults into queue
	VMQ_ReceiverStack *stack = &( (*queue).stacks[(*queue).numstacks - 1] );
	(*stack).count = 0;
	(*stack).receivers = NULL;

	// Copy the label passed in, into the stacks memory
	(*stack).label = (char*)malloc((sizeof(char) * strlen(label)) + 1);
	strncpy((*stack).label, label, (sizeof(char) * strlen(label)) + 1);


	return stack;
}

VMQ_ReceiverStack* VMQ_GetQueueStack(VMQ_MessageQueue *queue, char *label) {
	printf("VMQ_GetQueueStack: TODO. Get the stack from a queue by  it's label\n");
	return NULL;
}

void VMQ_Subscribe(VMQ_MessageQueue *queue, const char *label, VMQ_Receiver cb) {
	VMQ_SubscribeCtx(queue, label, cb, NULL);
}

void VMQ_SubscribeCtx(VMQ_MessageQueue *queue, const char *label, VMQ_Receiver cb, void *context) {
	// Check that the Queue is valid
	if (!queue) return;

	// Check if the label is already being monitored in the queue
	VMQ_ReceiverStack *stack = VMQ_GetQueueStack(queue, label);

	// Create stack if not exists
	if (!stack) {
		stack = VMQ_NewQueueStack(queue, label);
	}

	// Add the receiver to the stack
	(*stack).count++;

	if ((*stack).count == 1) {
		(*stack).receivers = (VMQ_ReceiverNode*)malloc(sizeof(VMQ_ReceiverNode));
	} else {
		(*stack).receivers = realloc((*stack).receivers, sizeof(VMQ_ReceiverNode) * (*stack).count);
	}

	// Create the Boxed Receiver (VMQ_ReceiverNode) at the memory location allocated
	VMQ_ReceiverNode *receiver = &( (*stack).receivers[(*stack).count - 1] );
	(*receiver).callback = cb;
	(*receiver).context = context;

}

/*
	Called from in a sub loop inside of the main loop normally.
	This function will actually take the next message in the
	Queue and call all Subscribers, sending them the message
	for processing. It will only process one message, to process
	more a loop is created:
		while (VMQ_IsMessage()) { VMQ_ProcessMessage(); }
*/
int VMQ_ProcessMessage(VMQ_MessageQueue *queue) {
	// Returns 1 if a message was processed and 0 if there was none
	// to process

	if (!VMQ_IsMessage(queue)) {
		return 0;
	}



	return 1;
}

/*
    Called normally in the main loop, and checks if there are
    any messages to be processed. This function does NOT Process
    any messages, however only checks if there are any to process.
*/
int VMQ_IsMessage(VMQ_MessageQueue *queue) {
	return 0;
}
