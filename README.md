# twine_FR4133

twine: strong thread or string consisting of two or more strands.

The intent of this code is to create a task manager which is deterministic. Determinism is created by running tasks at a consistent pre-determined periodic rate. The code is designed to run on the [TI MSP430FR4133 LaunchPad Development Kit](https://www.ti.com/store/ti/en/p/product/?p=MSP-EXP430FR4133). A clock on the TI430FR4133 is used to trigger an interrupt at a periodic rate. The interrupt routine increments counters which are used by the task manager to track the length of time in the sleep state and when ready, to trigger tasks to execute. Requsting a task to sleep is performed using taskSleep(...) and provides for a cooperative multi-tasking environment.

There are four tasks, called task_1 through task_4. The task manager uses a bit set in the task status variable to store the state of a task (either executing or sleeping). The taskSleep(...) function can be called which allows other tasks to execute (cooperative multi-tasking). During a taskSleep(...) the task's stack is stored. When a task comes out of sleep the task manager assigns the value of the previously stored stack pointer to the processors SP register and copies the remaining stored stack data onto the processors's stack. The task size for each task is set to a maximum of 8x16 bit words.

Its important to know the size of each task's stack. This is done with the following statement located at the start of each task. This allows the taskManager(…) and taskSleep(…) to push and pop the proper number of stack values into the task_n_stack[] array.

```
task_n_stack_size = ((taskManager_stack_n - (unsigned short) __get_SP_register())>>1) + 1;
```

The taskSleep(count) routine is used to allow one task to sleep and others to execute. Calling taskSleep(count) within a task will set the task to sleep for a period of time determined by count.

When a task comes out of sleep, code located in taskManager(…)  will copy the previously stored stack data from a task and copy that to the system stack, a return will then begin code execution from the previously stored program counter.

```
SP_current = (unsigned short*) __get_SP_register();
for(i=task_n_stack_size - 1;i >= 0;i--)
{
  *--SP_current = (unsigned short) task_n_stack[i];
}
__set_SP_register((unsigned short)SP_current);
return;
````

Here is an examples of task_1(…) and task_2(…) blinking user LED1 and LED2 on the TI430FR4133 LaunchPad development board.

```
short task_1(void)
{
  task_1_stack_size = ((taskManager_stack_1 - (unsigned short) __get_SP_register())>>1) + 1;
  P1OUT |= BIT0; // turn on LED1
  taskSleep(1);
  P1OUT &= ~BIT0; // turn off LED 1
  taskSleep(1);
  return 0;
}
short task_2(void)
{
  task_2_stack_size = ((taskManager_stack_2 - (unsigned short) __get_SP_register())>>1) + 1;
  P4OUT |= BIT0; // turn on LED2
  taskSleep(4);
  P4OUT &= ~BIT0; // turn off LED 2
  taskSleep(1);
  return 0;
}
```

# Is - a simple interpreter

The examples have been expanded to include a simple interpreter called Is.
This simple program will output "Hello", pause for 1 second and repeat. Use Ctrl-C to exit.

Example:
```

# e
 1: out
 2: pause
 3: rerun
 4: .
# r
Hello
Hello
Hello
program terminated
```

# Console Commands

```
h: help
v: version
p: print key words
e: edit program
r: run program
l: list program
--------------------
.: exit program edit
```
