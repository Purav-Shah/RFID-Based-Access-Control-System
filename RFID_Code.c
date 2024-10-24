// RFID Based Attendance System Using 8051 Microcontroller
//library description
#include<reg51.h>
#include<string.h>
#include<stdio.h>
#define LCDPORT P1

sbit rs=P1^0;
sbit rw=P1^1;
sbit en=P1^2;
sbit Motor1=P2^4;
sbit Motor2=P2^3;
sbit Speaker=P2^6;
char i,rx_data[50];
char rfid[13],ch=0;
unsigned char result[1];

 void delay(int itime) // used to create delay in real time
{
    int i,j;
    for(i=0;i<itime;i++)
    for(j=0;j<1275;j++); //1275 to match the freq of mc
}

void daten() //initialise the led pins
{
    rs=1;
    rw=0;
    en=1;
    delay(5);
    en=0;
}

void lcddata(unsigned char ch) //sends info to the led
{
    LCDPORT=ch & 0xf0;
    daten();
    LCDPORT=(ch<<4) & 0xf0;
    daten();
}

void cmden(void) // to enable command transmission
{
    rs=0;
    en=1;
    delay(5);
    en=0;
}

void lcdcmd(unsigned char ch) // to define the placement of cursor on the screen or to clear it
{
    LCDPORT=ch & 0xf0;
    cmden();
    LCDPORT=(ch<<4) & 0xf0;
    cmden();
}

void lcdstring(char *str) // loads the string on the lcd
{
    while(*str)
    {
        lcddata(*str);
        str++;
    }
}

void lcd_init(void) // initialises the led screen
{
    lcdcmd(0x02);
    lcdcmd(0x28);
    lcdcmd(0x0e);
    lcdcmd(0x01);
}

void uart_init() // initialises UART communication
{
 TMOD=0x20;
 SCON=0x50;
 TH1=0xfd;
 TR1=1;
}

char rxdata() //reads character to UART
{
  while(!RI);
    ch=SBUF;    
    RI=0;
    return ch;
}

void main()
{
    Speaker=1;
    uart_init();
    lcd_init();
    lcdstring("RFID Based Access");
    lcdcmd(0xc0); // sends cursor to next line
    lcdstring("Control System"); 
    delay(500);
  	lcd_init();
    lcdstring("Group 5");
    lcdcmd(0xc0);
    lcdstring("C006,C016,C033");
    delay(400);
    while(1)
    {
        lcdcmd(1);
        lcdstring("Scan Your Card:");
        lcdcmd(0xc0);
        i=0;
        for(i=0;i<12;i++)
        rfid[i]=rxdata();
        rfid[i]='\0';
        lcdcmd(1); // resets the lcd or 0x01 in hex
				lcdstring("Rfid No. is:");
        lcdcmd(0xc0);
        for(i=0;i<12;i++)
        lcddata(rfid[i]);
        delay(100);
        if(strncmp(rfid,"10035AD856C1",12)==0)
        {
            lcdcmd(1);             
            lcdstring(" Authorised");
            delay(200);
            lcdcmd(1);
            lcdstring(" Manya ");
						delay(200);
            Motor1=1;
            Motor2=0;
            delay(300);
            Motor1=0;
            Motor2=0;
            delay(200);
            Motor1=0;
            Motor2=1;
        }
        
        else if(strncmp(rfid,"1600ADC369A1",12)==0)
            {
            lcdcmd(1);
            lcdstring(" Authorised");
            delay(200);
            lcdcmd(1);
            lcdstring(" Mansi ");
						delay(200);
            Motor1=1;
            Motor2=0;
            delay(300);
            Motor1=0;
            Motor2=0;
            delay(200);
            Motor1=0;
            Motor2=1;
            delay(300);
            Motor1=0;
            Motor2=0;
						}
        else if(strncmp(rfid,"1600ABCD147A",12)==0)
            {
            lcdcmd(1);
            lcdstring(" Authorised");
            delay(200);
            lcdcmd(1);
            lcdstring(" Purav ");
						delay(200); 
            Motor1=1;
            Motor2=0;
            delay(300);
            Motor1=0;
            Motor2=0;
            delay(200);
            Motor1=0;
            Motor2=1;
            delay(300);
            Motor1=0;
            Motor2=0;
            }
        else 
        {
           lcdcmd(1);
           lcdstring("UNAUTHORISED");
					 lcdcmd(0xc0);
           lcdstring("Try Another Card");
           Speaker=0;
           delay(300);
           Speaker=1;
        }
  }
}