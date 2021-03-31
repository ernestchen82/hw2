#include "mbed.h"
#include "uLCD_4DGL.h"

using namespace std::chrono;

InterruptIn button1(D12);
InterruptIn button2(D13);
InterruptIn button3(D11);
AnalogOut Aout(D7);
AnalogIn Ain(A0);
EventQueue wavequeue(32 * EVENTS_EVENT_SIZE);
EventQueue choosequeue(32 * EVENTS_EVENT_SIZE);
EventQueue lcdqueue(32 * EVENTS_EVENT_SIZE);

Thread t1;
Thread t2;
Thread t3;

Timeout t;

uLCD_4DGL uLCD(D1,D0,D2);

int sample = 200;
float ADCdata[200];

int output = 0;

void triangular()
{
    float i;

    while(1)
    {
        if (output == 1)
        {
            for(i = 0; i <= 1; i += 0.003008f)
            {   
                Aout.write(i);
                wait_us(10);
            }     
            for(i = 1; i >= 0; i -= 4 * 0.003008f)
            {
                Aout.write(i);
                wait_us(10);
            }  
        }

        if (output == 2)
        {
            for(i = 0; i <= 1; i += 0.001504f)
            {   
                Aout.write(i);
                wait_us(10);
            }     
            for(i = 1; i >= 0; i -= 4 * 0.001504f)
            {
                Aout.write(i);
                wait_us(10);
            }  
        }
       
    }
    
            
    
}

/*void ulcd(const char* f)
{
    uLCD.text_width(2);
    uLCD.text_height(2);
    uLCD.locate(0,0);
    uLCD.color(WHITE);
    uLCD.textbackground_color(GREEN);
    uLCD.printf(f);
}*/
void ulcd1()
{
    uLCD.text_width(2);
    uLCD.text_height(2);
    uLCD.locate(0,0);
    uLCD.color(WHITE);
    uLCD.textbackground_color(GREEN);
    uLCD.printf("frequency 100hz\n");
    uLCD.textbackground_color(WHITE);
    uLCD.printf("frequency  50hz");
}

void ulcd2()
{
    uLCD.text_width(2);
    uLCD.text_height(2);
    uLCD.locate(0,0);
    uLCD.color(WHITE);
    uLCD.textbackground_color(WHITE);
    uLCD.printf("frequency 100hz\n");
    uLCD.textbackground_color(GREEN);
    uLCD.printf("frequency  50hz");
}



void wave1()
{
    output = 1;
    for (int i = 0; i < sample; i++)
    {
        
        ADCdata[i] = Ain*3.3f;
        ThisThread::sleep_for(4ms);
        
    }
    for (int i = 0; i < sample; i++)
    {
        printf("%f\r\n", ADCdata[i]);
        ThisThread::sleep_for(10ms);
    }
    
    
    
}

void wave2()
{
    output = 2;
    for (int i = 0; i < sample; i++)
    {
        
        ADCdata[i] = Ain*3.3f;
        ThisThread::sleep_for(4ms);
        
    }

    for (int i = 0; i < sample; i++)
    {
        printf("%f\r\n", ADCdata[i]);
        ThisThread::sleep_for(10ms);
    }
}

void output1()
{
    //ulcd("frequency 100hz");
    ulcd1();
    button3.fall(choosequeue.event(wave1));
}

void output2()
{
    //ulcd("frequency  50hz");
    ulcd2();
    button3.fall(choosequeue.event(wave2));
}



int main()
{
  
    t3.start(callback(&lcdqueue, &EventQueue::dispatch_forever));
    t2.start(callback(&choosequeue, &EventQueue::dispatch_forever));
    t1.start(triangular);
    

    button1.rise(lcdqueue.event(output1));
    button2.rise(lcdqueue.event(output2));
}



 


