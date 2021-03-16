#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf(2, "Invalid number of arguments\n");
        exit();
    }
    int i = 0;
    int i1 = 0, i2 = 0;
    while (argv[1][i] != '\0')
    {
        i1 = (i1 * 10) + (argv[1][i] - (int)'0');
        i++;
    }
    i = 0;
    while (argv[2][i] != '\0')
    {
        i2 = (i2 * 10) + (argv[2][i] - (int)'0');
        i++;
    }
    // printf(1, "i1: %d, i2: %d\n", i1, i2);
    if(i1<0 || i1>100)
    {
        printf(2, "Invalid range\n");
        exit();
    }
    set_priority(i1, i2);
    exit();
}