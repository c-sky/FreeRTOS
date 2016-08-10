What is FreeRTOS
===
FreeRTOS is a class of RTOS that is designed to be small enough to run on a microcontroller - although its use is not limited to microcontroller applications.
FreeRTOS Feature
===
* Free RTOS scheduler - preemptive, cooperative and hybrid configuration options.
* RTOS objects (tasks, queues, semaphores, mutexes and event groups) can be created using either dynamically or statically allocated RAM
* Designed to be small, simple and easy to use
* Support both real time tasks and co-routines

visit http://www.freertos.org/ to know more FreeRTOS
FreeRTOS Open Source License
===
FreeRTOS source code is licensed by a modified GNU General Public License - the modification taking the form of an exception. which permits the source code of application that use FreeRTOS and are distributed as executables to remain closed source, thus permitting the use of FreeRTOS in commercial applications without necessitating that the whole application be open soruced. you can visit the following website to know more. http://www.freertos.org/a00114.html

Getting Started
===
How to build it
---
Run the  run.sh script filw with two parameters (cpu name and soc name)  
For example: ./run.sh ck610 ck5a6  
ck610 is the demo cpu with ck610 core  
ck5a6 is the demo soc with ck610 core cpu  

how to add new cpu in FreeRTOS
---
1. Find out what type of compiler you wil use (take GCC compiler as example)
2. Enter into freertosv8.2.3\FreeRTOS\Source\portable\GCC and Create a folder named with new cpu name. such as ck610
3. Design your cpu porting code in port.c and portmacro.h or other files you use
4. Select one memory management file under freertosv8.2.3\FreeRTOS\Source\portable\MemMang, In general, heap4.c is preferred
5. Add those new files into compile source files path and head files path

how to add new soc in FreeRTOS
---
1. Enter into bsp directory and create a new folder named with your soc name, such as ck5a6
2. Enter into include directory and create a new foloer named with your soc name, such ck5a6
3. put your bsp code files under \bsp\<soc name> and \include\<soc name>
4. Add those new files into compile source files path and head files path

how to configure your system memory\flash address space
---
The default memory/flash address space in LD script file is memory space. you can change it to your system address space



