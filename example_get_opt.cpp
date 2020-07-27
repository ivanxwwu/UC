//
// Created by root on 2020/4/11.
//

#include <getopt.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int opt;
    char *string = "a::b:c:d";
    int option_index = 0;
    while ((opt=getopt(argc, argv, string))!= -1)
    {
        printf("opt = %c\t\t", opt);
        printf("optarg=%s\t\t", optarg);
        printf("optind=%d\t\t", optind);
        printf("argv[optind]=%s\n", argv[optind]);
    }
    static struct option long_options[] =
            {
                    {"reqarg", required_argument,NULL, 'r'},
                    {"optarg", optional_argument,NULL, 'o'},
                    {"noarg",  no_argument,         NULL,'n'},
                    {NULL,     0,                      NULL, 0},
            };
    while((opt =getopt_long_only(argc,argv,string,long_options,&option_index))!= -1)
    {
        printf("opt = %c\t\t", opt);
        printf("optarg = %s\t\t",optarg);
        printf("optind = %d\t\t",optind);
        printf("argv[optind] =%s\t\t", argv[optind]);
        printf("option_index = %d\n",option_index);
    }
}