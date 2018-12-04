
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "controller.h"
#include "lcd.h"
#include "i2c.h"

uint32_t data = 0;
uint8_t datflg = 0;

uint8_t task_bits = 0;
uint8_t count = 45, precision = 2;
volatile char tick_flag = 0;
unsigned int task_timers[NUM_TASKS]={0,0,0,0,0,0,0,0};
static const PROGMEM char bit_mask[]={1,2,4,8,16,32,64,128};

int main(void){
    
    PORTC |= (1<<PC3);
    PORTB |= (1<<PB3)|(1<<PB4)|(1<<PB5);
    DDRB |= (1<<PB2);
    PORTB |= (1<<PB2);

    //---------------------Buzzer timer
    TCCR1A |= 1<<WGM11 | 1<<COM1B1 | 1<<COM1B0;
    TCCR1B |= 1<<WGM13 | 1<<WGM12 | 1<<CS11;
    ICR1 = 100;
    OCR1B = ICR1 - count;
    //--------------------------------

   //------------------task timer
    TCCR0B = 0x00;
    TCNT0 = 0x00;
    TCCR0A = 0x02;
    OCR0A = 0xB4; 
    TCCR0B = 0x05;
    TIMSK0 = 0x02; //timer 0 interrupt sources
    //----------------------------

    EICRA |= (1<<ISC11);
    EIMSK |= (1<<INT1);

    Initialize_LCD();
    _delay_ms(500);
    Start_Screen();
    _delay_ms(3000);
    clear_Screen();
    Main_Screen();
    //addPatterns();
    i2cInitSlave();
    sei();

    set_task(0);
    set_task(1); 
    //set_task(3); 
    //set_task(4);    
    //set_task(5); 
	//set_task(6);
    //set_task(7);



  while(1){
    if (tick_flag){
	tick_flag = 0;
	task_dispatch();
	}
  }
  return 0;
} 
//
//  a task gets dispatched on every tick_flag tick (10ms)
//
void task_dispatch(void){
// scan the task bits for an active task and execute it 

  uint8_t task;
// take care of the task timers. if the value ==0 skip it
//   else decrement it. If it decrements to zero, activate the task associated with it

  task=0;
  while (task < NUM_TASKS ){
		if (task_timers[task]){
		task_timers[task]--;            // dec the timer
		if (task_timers[task] == 0 ){
		set_task(task); // if ==0 activate the task bit
			}
	}
	task++;
	}

  task = 0; // start at the most significant task 
  while (task <= NUM_TASKS ){
	if ((task_bits & pgm_read_byte(&bit_mask[task]))){
            break; // if activate task found..
        }
	task++;         // else try the next one 
  }
  switch(task){       // if task bit is active..execute the task
	case 0:
	task0();
	break;
    case 1:
	task1();
	break;
    case 2:
	task2();
	break;
    case 3:
	task3();
	break;
    case 4:
	task4();
	break;
    case 5:
	task5();
	break;
    case 6:
	task6();
	break;
    case 7:
	task7();
	break;
    default:
	break;                  // no task was active!! 
    }                       
}

// enable a task for execution
void set_task(uint8_t tsk){
  task_bits |= pgm_read_byte(&bit_mask[tsk]);       // sets a task bit 
}
// disable a task from executing
void reset_task(uint8_t tsk){
  task_bits &= (~pgm_read_byte(&bit_mask[tsk]));  // resets a task bit 
}

void task0(void){
	float freq = ((float)data) / 1000;
    float fc = freq * 0.892;   // Magic Number
    float i_ua = fc * 0.0464459 - 0.0922608;
	if(datflg == 1){
		Send_A_Double_With_Tail(7, 1, freq, 5, 3, "Hz");
		Send_A_Double_With_Tail(14, 2, fc, 5, 1, "fc");
		Send_A_Double_With_Tail(10, 3, i_ua, 5, 3, "uA");
		//Send_An_Integer(7, 1, count, 2);
		datflg = 0;
	}
	task_timers[0] = 20;  
	reset_task(0);
}

void task1(void){
	OCR1B = ICR1 - count;
	task_timers[1] = 10; 
	reset_task(1);
}

void task2(void){
	task_timers[2] = 90;
    reset_task(2);
}

void task3(void){
    task_timers[3] = 5;
    reset_task(3);
}

void task4(void){
    task_timers[4] = 3;
	reset_task(4);
}

void task5(void){
    task_timers[5] = 20;
	reset_task(5);
}

void task6(void){
	task_timers[6] = 50;
	reset_task(6);
}

void task7(void){
    task_timers[7] = 30;
    reset_task(7);
}
//call this routine to initialize all periphera
//TIMER0 initialize - prescale:1024
// WGM: CTC
// desired value: 10mSec
// actual value: 10.048mSec (-0.5%)

ISR(TIMER0_COMPA_vect){
 //TIMER0 has overflowed
    tick_flag = 1;
}


void Start_Screen(void){
    Send_A_String_With_Location(3, 1, "EE449 Lux Meter");
    Send_A_String_With_Location(4, 2, "Sukhman Marok");
    Send_A_String_With_Location(2, 3, "John Contovasilis");
}

void Main_Screen(void){
	Send_A_String_With_Location(1, 1, "Freq:");
	Send_A_String_With_Location(1, 2, "Illuminance:");
	Send_A_String_With_Location(1, 3, "I Diode:");
}

ISR(TWI_vect){ 
    static uint8_t index = 0;
    static uint32_t buffer[BUFFER_SIZE] = {0,0,0,0};

    switch(i2cGetStatus()){
        case 0x60:
            TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
            break;

        case 0x80:

            if(index >= BUFFER_SIZE){
                TWCR = (1<<TWINT)|(1<<TWEN);
            }
            else{
                buffer[index] = TWDR;
                index++;
                TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
            }
            break;
        
        case 0xA0:
            TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
            index = 0;
            data = (buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | (buffer[0]);
            datflg = 1;
            break;
    }   
    TWCR |= (1<<TWIE);
}


ISR (INT1_vect)
{ 
    if(bit_is_clear(PIND,2))
    {
	if(count > 0){
        count = count - (precision + 1);
	}
    }
    else if(bit_is_set(PIND,2))
    {
	if(count < ICR1 - 10){
        count = count + precision + 1;
	}
    }
    
}
