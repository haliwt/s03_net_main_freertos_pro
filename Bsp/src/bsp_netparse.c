#include "bsp.h"

#if 0

char time_str[60];


char week_day[4], month[4];


int8_t day, hour, minute, second, year;

void net_parse_beijint_time(void)
{

  char *newline_pos = strstr(time_str, "\n");

  if (newline_pos) { 
    // 将字符串分割为两部分 
    *newline_pos = '\0'; 
    newline_pos += 2; 
    // 跳过 "\r\n" 
    //sscanf();//parse_time(newline_pos); 
    sscanf(newline_pos,"%13s %3s %3s %d %d:%d:%d %d", week_day, month, &day, &hour, &minute, &second, &year);
    } 
    else 
    {



    }

 // sscanf(time_str,"%29s %3s %3s %d %d:%d:%d %d", week_day, month, &day, &hour, &minute, &second, &year);
 

}
#endif 
