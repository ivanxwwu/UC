//
// Created by root on 17-10-18.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>
char const * const morse_table[]=
        {"._",//A
         "_...",//B
         "_._.",//C
         "_..",//D
         ".",//E
         ".._.",//F
         "__.",//G
         "....",//H
         "..",//I
         ".___",//J
         "_._",//K
         "._..",//L
         "__",//M
         "_.",//N
         "___",//O
         ".__.",//P
         "__._",//Q
         "._.",//R
         "...",//S
         "_",//T
         ".._",//U
         "..._",//V
         ".__",//W
         "_.._",//X
         "_.__",//Y
         "__..",//Z
         "_____",//0
         ".____",//1
         "..___",//2
         "...__",//3
         "...._",//4
         ".....",//5
         "_....",//6
         "__...",//7
         "___..",//8
         "____."//9
        };

void encodeMorse(char *d,const char *s);
void decodeMorse(char *d,const char *s);

int main(void)
{
    char str1[1000],str2[1000];
    encodeMorse(str1,"HappyNewYear2014");
    puts(str1);
    decodeMorse(str2,str1);
    puts(str2);

    return 0;
}

void encodeMorse(char *d,const char *s)
{
    *d='\0';
    while(*s)
    {
        if(isalpha(*s))
        {
            strcat(d,morse_table[toupper(*s)-'A']);
        }
        if(isdigit(*s))
        {
            strcat(d,morse_table[*s-'0'+('Z'-'A')+1]);
        }
        strcat(d," ");
        s++;
    }
}
void decodeMorse(char *d,const char *s)
{
    char temp[50];
    int i;
    while(*s)
    {
        sscanf(s,"%s",temp);
        s+=strlen(temp);
        while(*s==' ')s++;
        for(i=0;i<36;++i)
        {
            if(!strcmp(temp,morse_table[i]))
            {
                *d++=(i<26?('A'+i):('0'+i-26));
            }
        }
    }
    *d='\0';
}