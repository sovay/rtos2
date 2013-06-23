/**
 * @file   kernel.h
 *
 * @brief kernel data structures used in os.c.
 *
 * CSC 460/560 Real Time Operating Systems - Mantis Cheng
 *
 * @author Scott Craig
 * @author Justin Tanner
 * @author Matt Campbell
 */
#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "os.h"

/** The maximum number of names. Currently the same as the number of tasks. */
#define 	MAXNAME		MAXPROCESS

/** The number of clock cycles in one "tick" or 5 ms */
#define TICK_CYCLES     (F_CPU / 1000 * TICK)

#define LED_DDR DDRB
#define LED_PORT PORTB

/** LEDs for OS_Abort() */
#define LED_RED_MASK    _BV(PB1)	// LED2 on xplained

/** LEDs for OS_Abort() */
#define LED_GREEN_MASK    _BV(PB2)	// LED3 on xplained

/* Typedefs and data structures. */

typedef void (*voidfuncvoid_ptr)(void); /* pointer to void f(void) */

/**
 * @brief This is the set of states that a task can be in at any given time.
 */
typedef enum {
	DEAD = 0, RUNNING, READY, WAITING, SLEEPING
} task_state_t;

/**
 * @brief This is the set of kernel requests, i.e., a request code for each system call.
 */
typedef enum {
	NONE = 0,
	TIMER_EXPIRED,
	TASK_CREATE,
	TASK_TERMINATE,
	TASK_NEXT,
	TASK_GET_ARG,
	TASK_SLEEP,
	EVENT_INIT,
	EVENT_WAIT,
	EVENT_SIGNAL,
	EVENT_BROADCAST,
	EVENT_SIGNAL_AND_NEXT,
	EVENT_BROADCAST_AND_NEXT
} kernel_request_t;

/**
 * @brief The arguments required to create a task.
 */
typedef struct {
	/** The code the new task is to run.*/
	voidfuncvoid_ptr f;
	/** A new task may be created with an argument that it can retrieve later. */
	int arg;
	/** Priority of the new task: RR, PERIODIC, SYSTEM */
	uint8_t level;
	/** If the new task is PERIODIC, this is its name in the PPP array. */
	uint8_t name;
} create_args_t;

typedef struct td_struct task_descriptor_t;
/**
 * @brief All the data needed to describe the task, including its context.
 */
struct td_struct {
	/** The stack used by the task. SP points in here when task is RUNNING. */
	uint8_t stack[WORKSPACE];
	/** A variable to save the hardware SP into when the task is suspended. */
	uint8_t* volatile sp; /* stack pointer into the "workSpace" */
	/** PERIODIC tasks need a name in the PPP array. */
	uint8_t name;
	/** The state of the task in this descriptor. */
	task_state_t state;
	/** The argument passed to Task_Create for this task. */
	int arg;
	/** The priority (type) of this task. */
	uint8_t level;
	/** A link to the next task descriptor in the queue holding this task. */
	task_descriptor_t* next;
	/** A link to the next task descriptor in the queue holding these sleeping tasks */
	task_descriptor_t* sleep_next;
	/** The number of ticks remaining after the previous item in the queue is released. If it is the first item in the queue it is the number of ticks until it is released. */
	unsigned int diff_ticks_remaining;

};

/**
 * @brief Contains pointers to head and tail of a linked list.
 */
typedef struct {
	/** The first item in the queue. NULL if the queue is empty. */
	task_descriptor_t* head;
	/** The last item in the queue. Undefined if the queue is empty. */
	task_descriptor_t* tail;
} queue_t;

#endif

