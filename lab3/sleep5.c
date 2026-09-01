#include <stdio.h>
#include <unistd.h>

int main (void)
{
    fprintf(stdout, "indo dormir...\n");
    fflush(stdout);
    sleep(5);
    fprintf(stdout, "Acordei!\n");
    return 0;
}
