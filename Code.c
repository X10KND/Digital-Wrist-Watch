#include <mega32.h>
#include <delay.h>
#include <alcd.h>
#include <stdlib.h>
#include <string.h>

int hour12 = 0;
int hour = 23;
int minute = 59;
int second = 50;
int isPM = 0;
int is24 = 0;

int day = 31;
int month = 12;
int year = 21;

int newSecond = 0;
int newMinute = 0;
int newHour = 12;
int newPM = 1;

int newDay = 1;
int newMonth = 1;
int newYear = 21;

int alarmMinute = 0;
int alarmHour = 12;
int alarmIsPM = 0;
int alarmIsON = 0;
int alarmTrigger = 0;

int stopwatchSecond = 0;
int stopwatchMinute = 0;
int stopwatchHour = 0;

int stopwatchIsON = 0;
int stopwatchTotalTime = 0;

int mode = 0;
int press = 0;
int page = 0;
int blinker = 0;

int steps = 0;
int calorie = 0;
int gap = 0;

char str_print[100];

void convert24to12 () {
    if (hour < 12) {
        isPM = 0;
        if (hour == 0) {
            hour12 = 12;
        }
        else {
            hour12 = hour;
        }
    }
    else {
        isPM = 1;
        if (hour == 12) {
            hour12 = 12;
        }
        else {
            hour12 = hour - 12;
        }
    }
}

void calculate_gap() {

    gap = 20;
    itoa(steps, str_print); 
    gap -= strlen(str_print);
    itoa(calorie, str_print);
    gap -= strlen(str_print);
    
    while(gap > 0) {
        lcd_puts(" ");
        gap--;
    }
}

void show_int(int num) {
    if (num < 10) {
        lcd_puts("0");
    }
    itoa(num, str_print);
    lcd_puts(str_print);
}

void show_blinking_int(int num, int p) {
    if (press == p && blinker == 0) {
        lcd_puts("  ");
    }
    else {
        show_int(num);
    }
}

void show_blinking_text(char* c, int p) {
    if (press == p && blinker == 0) {
        gap = strlen(c); 
        while(gap > 0) {
            lcd_puts(" ");
            gap--;
        }
    }
    else {
        lcd_puts(c);
    }
}

void show_menu() {

    lcd_clear();

    if (page == 1) {
    
        lcd_gotoxy(0,1);

        if (mode == 0) {
            lcd_puts("     STOPWATCH      ");
        }
        else if (mode == 1) {
            lcd_puts("       ALARM        ");
        }
        else if (mode == 2) {
            lcd_puts("CHANGE DATE AND TIME");
        }
        else if (mode == 3) {
            lcd_puts(" CHANGE 12/24H MODE ");
        }
        
        lcd_gotoxy(0,3);
        lcd_puts("EXIT   SELECT   NEXT");
    }

    else if (page == 2) {

        if (mode == 0) {

            lcd_gotoxy(0,1);

            stopwatchSecond = stopwatchTotalTime;

            stopwatchHour = stopwatchSecond / 3600;
            stopwatchSecond = stopwatchSecond % 3600;

            stopwatchMinute = stopwatchSecond / 60;
            stopwatchSecond = stopwatchSecond % 60;

            lcd_puts("      ");
            show_int(stopwatchHour);
            lcd_puts(":");
            show_int(stopwatchMinute);
            lcd_puts(":");
            show_int(stopwatchSecond);

            lcd_gotoxy(0,3);

            if (stopwatchIsON == 0) {
                lcd_puts("RESET   EXIT   START");
            }
            else if (stopwatchIsON == 1) {
                lcd_puts("RESET   EXIT   PAUSE");
            }

        }

        else if (mode == 1){

            lcd_gotoxy(0,1);

            lcd_puts("ALARM ON?: ");

            if (alarmIsON == 1) {
                show_blinking_text("YES", 0);
            }
            else if (alarmIsON == 0) {
                show_blinking_text("NO", 0);
            }

            lcd_gotoxy(0,3);

            lcd_puts("ALARM TIME: ");

            show_blinking_int(alarmHour, 1);
            lcd_puts(":");
            show_blinking_int(alarmMinute, 2);

            lcd_puts(" ");

            if (alarmIsPM == 0) {
                show_blinking_text("AM", 3);
            }
            else if (alarmIsPM == 1) {
                show_blinking_text("PM", 3);
            }
        }

        else if (mode == 2) {

            lcd_gotoxy(0,0);
            lcd_puts("      NEW TIME      ");
            
            lcd_gotoxy(0,1);
            lcd_puts("      ");
            show_blinking_int(newHour, 0);
            lcd_puts(":");
            show_blinking_int(newMinute, 1);
            lcd_puts(":");
            show_blinking_int(newSecond, 2);

            lcd_gotoxy(0,2);
            lcd_puts("      NEW DATE      ");
            
            lcd_gotoxy(0,3);
            lcd_puts("      ");
            show_blinking_int(newDay, 3);
            lcd_puts("/");
            show_blinking_int(newMonth, 4);
            lcd_puts("/");
            show_blinking_int(newYear, 5);
        }
        
        else if (mode == 3) {
            
            lcd_gotoxy(0,1);

            lcd_puts("MODE: ");

            if (is24 == 0) {
                show_blinking_text("12 HOUR", 0);
            }
            else if (is24 == 1) {
                show_blinking_text("24 HOUR", 0);
            }
            
            lcd_gotoxy(0,3);
            lcd_puts("       SAVE   CHANGE");
        }
    }
}

void show_clock() {

    if (page == 0) {
    
        convert24to12();

        lcd_clear();
        
        lcd_gotoxy(0,0);
        show_int(day);
        lcd_puts("/");
        show_int(month);
        lcd_puts("/");
        show_int(year);
        
        if(is24 == 1) {
            lcd_puts("    ");
            show_int(hour);
        }
        else if(is24 == 0) {
            lcd_puts("  ");
            show_int(hour12);
        }
        
        lcd_puts(":");

        show_int(minute);
        lcd_puts(":");

        show_int(second);
        
        if (is24 == 0) {
            if (isPM == 0) {
                lcd_puts("AM");
            }
            else if (isPM == 1) {
                lcd_puts("PM");
            }
        }
    
        ADCSRA |= (1<<ADSC);

        lcd_gotoxy(0,2);
        lcd_putsf("STEP         CALORIE");
        lcd_gotoxy(0,3);
        show_int(steps);
        calculate_gap();
        show_int(calorie);
    }
    
    else {
        show_menu();
    }
}

interrupt [TIM2_COMP] void timer2_comp_isr(void) {

    if(stopwatchIsON == 1) {
        stopwatchTotalTime++;
    }
    
    second++;

    if (second == 60) {
        second = 0;
        minute++;

        if (minute == 60) {
            minute = 0;
            hour++;

            if (hour == 24) {
                hour = 0;
                day++;
                
                switch(month) {
                    case 1:
                    case 3:
                    case 5:
                    case 7:
                    case 8:
                    case 10:
                    case 12:
                        if (day == 32) {
                            day = 1;
                            month++;
                        }
                        break;
                    
                    case 4:
                    case 6:
                    case 9:
                    case 11:
                        if (day == 31) {
                            day = 1;
                            month++;
                        }
                        break;
                    
                    case 2:
                        if (day == 29) {
                            day = 1;
                            month++;
                        }
                        break;
                }
                if (month == 13) {
                    month = 1;
                    year++;
                }
            }
        }
        
        convert24to12();
        
        if (alarmIsON == 1) {
            if (minute == alarmMinute && hour12 == alarmHour && isPM == alarmIsPM) {
                alarmTrigger = 1;
                alarmIsON = 0;
            }
        }
    }
}

interrupt [TIM1_COMPA] void timer1_compa_isr(void) {

    #asm("sei");

    blinker = (blinker + 1) % 2;

    if (alarmTrigger == 1) {
        PORTB.0 = blinker;
        PORTB.1 = blinker;
    }
    else if (alarmTrigger == 0) {
        PORTB.0 = 0;
        PORTB.1 = 0;
    }

    show_clock();

}

interrupt [EXT_INT2] void ext_int2_isr(void) {

    #asm("sei");

    if (alarmTrigger == 1) {
        alarmTrigger = 0;
    }

    else if (alarmTrigger == 0) {

        if (page == 0)    {
            page = 1;
            mode = 0;
            press = 0;
        }
        else if (page == 1) {
            page = 2;
            
            if (mode == 2) {
                newHour = hour; 
                newMinute = minute;
                newSecond = second;
                newPM = isPM;
                newYear = year;
                newMonth = month;
                newDay = day;
            }
        }
        else if (page == 2) {
            if (mode == 0) {
                stopwatchIsON = 0;
                stopwatchTotalTime = 0;
                page = 0;
            }

            else if (mode == 1) {
                press++;
                if (press > 3) {
                    press = 0;
                    page = 0;
                }
            }

            else if (mode == 2) {
                press++;
                if (press > 5) {
                    press = 0;
                    page = 0;

                    second = newSecond;
                    minute = newMinute;
                    hour = newHour;

                    day = newDay;
                    month = newMonth;
                    year = newYear;
                }
            }
            else if (mode == 3) {
                page = 0;
            }
        }
    }

    show_clock();
}

interrupt [EXT_INT1] void ext_int1_isr(void) {

    #asm("sei");

    if (page == 1) {
        page = 0;
        mode = 0;
    }

    else if (page == 2) {
        if (mode == 0) {
            stopwatchIsON = 0;
            stopwatchTotalTime = 0;
        }

        else if (mode == 1) {
            if (press == 0) {
                alarmIsON = (alarmIsON + 1) % 2;
            }
            else if (press == 1) {
                alarmHour--;
                if (alarmHour == 0) {
                    alarmHour = 12;
                }
            }
            else if (press == 2) {
                alarmMinute--;
                if (alarmMinute < 0) {
                    alarmMinute = 59;
                }
            }
            else if (press == 3) {
                alarmIsPM = (alarmIsPM + 1) % 2;
            }
            else {
                press = 0;
            }
        }

        else if (mode == 2) {
            if (press == 0) {
                newHour--;
                if (newHour < 0) {
                    newHour = 23;
                }
            }
            else if (press == 1) {
                newMinute--;
                if (newMinute < 0) {
                    newMinute = 59;
                }
            }
            else if (press == 2) {
                newSecond--;
                if (newSecond < 0) {
                    newSecond = 59;
                }
            }
            else if (press == 3) {
                newDay--;
                if (newDay < 1) {
                    newDay = 31;
                }
            }
            else if (press == 4) {
                newMonth--;
                if (newMonth < 1) {
                    newMonth = 12;
                }
            }
            else if (press == 5) {
                newYear--;
                if (newYear < 0) {
                    newYear = 99;
                }
            }
            else {
                press = 0;
            }
        }
        else if (mode == 3) {
            
        }
    }

    show_clock();
}

interrupt [EXT_INT0] void ext_int0_isr(void) {

    #asm("sei");

    if (page == 1) {
        mode = (mode + 1) % 4;
    }

    else if (page == 2) {
        if (mode == 0) {
            stopwatchIsON = (stopwatchIsON + 1) % 2;
        }

        else if (mode == 1) {
            if (press == 0) {
                alarmIsON = (alarmIsON + 1) % 2;
            }
            else if (press == 1) {
                alarmHour++;
                if (alarmHour == 13) {
                    alarmHour = 1;
                }
            }
            else if (press == 2) {
                alarmMinute = (alarmMinute + 1) % 60;
            }
            else if (press == 3) {
                alarmIsPM = (alarmIsPM + 1) % 2;
            }
            else {
                press = 0;
            }
        }

        else if (mode == 2) {
            if (press == 0) {
                newHour++;
                if (newHour == 24) {
                    newHour = 0;
                }
            }
            else if (press == 1) {
                newMinute = (newMinute + 1) % 60;
            }
            else if (press == 2) {
                newSecond = (newSecond + 1) % 60;
            }
            else if (press == 3) {
                newDay++;
                if (newDay == 32) {
                    newDay = 1;
                }
            }
            else if (press == 4) {
                newMonth++;
                if (newMonth == 13) {
                    newMonth = 1;
                }
            }
            else if (press == 5) {
                newYear++;
                if (newMonth == 100) {
                    newMonth = 0;
                }
            }
            else {
                press = 0;
            }
        }
        else if (mode == 3) {
            is24 = (is24 + 1) % 2;
        }
    }

    show_clock();

}

interrupt [ADC_INT] void adc_isr (void) {
    steps = (ADCW / 1023.0) * 20000;
    calorie = 40 * (steps / 1000);
}

void main(void) {

    #asm("sei");

    GICR = (1 << INT1) | (1 << INT0) | (1 << INT2);
    MCUCR = (1 << ISC11) | (0 << ISC10) | (1 << ISC01) | (0 << ISC00);
    MCUCSR = (0 << ISC2);

    TIMSK = (1 << OCIE2) | (1 << OCIE1A);

    TCCR1A = (0 << WGM11) | (0 << WGM10);
    TCCR1B = (0 << WGM13) | (1 << WGM12) | (1 << CS12) | (0 << CS11) | (0 << CS10);
    OCR1A = 0x7A11;

    ASSR = (1 << AS2);
    TCCR2 = (1 << WGM21) | (0 << WGM20) | (1 << CS22) | (1 << CS21) | (1 << CS20);
    OCR2 = 0x1F;

    DDRB = 0x03;
    PORTB.0 = 0;
    PORTB.1 = 0;

    ADMUX = 0b11000011;
    ADCSRA = (1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

    lcd_init(20);
    
    while(1) {
        if(PINB.3 == 0) {
            PORTB.1 = 1;
        }
    }
}
