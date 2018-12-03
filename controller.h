#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#define TW_BUTTON 1
#define B_BUTTON 2
#define M_BUTTON 3
#define T_BUTTON 4



#define NUM_TASKS 8

void reset_task(uint8_t tsk);
void set_task(uint8_t tsk);
void task_dispatch(void);
void task0(void);
void task1(void);
void task2(void);
void task3(void);
void task4(void);
void task5(void);
void task6(void);
void task7(void);
void Start_Screen(void);
void Main_Screen(void);

#endif
