#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __USE_POSIX199309
#define __USE_POSIX199309
#endif
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc > 2 || (argc == 2 && strcmp(argv[1], "-NoAnsi")))
    {
        printf(
            "clilog - command line monitor 0.001 gamma\n"
            "\n"
            "Usage: someapp | clilog [-NoAnsi]\n"
            "\n"
            "To switch to line buffering of an app that configure itself to use full stdio buffering when piping, use this trick:\n"
            "script -q -c \"someapp somearg\" | clilog\n"
            "\n"
            "Caution: The script tool will unfortunately also create a junk file named \"typescript\" in the current dir.\n");
        return 1;
    }

    int useansi = argc < 2 || strcmp(argv[1], "-NoAnsi") ? 1 : 0;

    if (isatty(fileno(stdin)))
    {
        printf("Only piped input supported.\n");
        return 1;
    }

    int c = getchar();
    if (c < 0)
    {
        printf("No input.\n");
        return 1;
    }

    setvbuf(stdout, NULL, _IONBF, 0);

    time_t unixtime;
    struct tm *t;

    unixtime = time(NULL);
    t = gmtime(&unixtime);
    printf("%04d-%02d-%02d %02d:%02d:%02d: ",
           t->tm_year + 1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    do
    {
        if (c == '\n')
        {
            unixtime = time(NULL);
            t = gmtime(&unixtime);
            printf("\r%04d-%02d-%02d %02d:%02d:%02d\r\n%04d-%02d-%02d %02d:%02d:%02d: ",
                   t->tm_year + 1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,
                   t->tm_year + 1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
        }
        else if (c == '\r')
        {
            unixtime = time(NULL);
            t = gmtime(&unixtime);
            printf("\r%04d-%02d-%02d %02d:%02d:%02d: ",
                   t->tm_year + 1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
        }
        else if (c < 32 && c != 9)
        {
            if (useansi)
            {
                unixtime = time(NULL);
                t = gmtime(&unixtime);
                printf("\033[s\r%04d-%02d-%02d %02d:%02d:%02d\033[u.",
                       t->tm_year + 1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
            }
            else
            {
                printf(".");
            }
        }
        else
        {
            if (useansi)
            {
                unixtime = time(NULL);
                t = gmtime(&unixtime);
                printf("\033[s\r%04d-%02d-%02d %02d:%02d:%02d\033[u%c",
                       t->tm_year + 1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, c);
            }
            else
            {
                printf("%c", c);
            }
        }
    } while ((c = getchar()) >= 0);

    printf("\r%04d-%02d-%02d %02d:%02d:%02d\n",
           t->tm_year + 1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    return 0;
}
