/* A simple terminal program for testing the serial library by Karl Stenerud
 */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "serial.h"
#include <dos.h>

char* errors[] =
{
    "Successful",
    "Unknown error",
    "Port not open",
    "Port already open",
    "No UART found on that comport",
    "Invalid comport",
    "Invalid BPS",
    "Invalid data bits",
    "Invalid parity",
    "Invalid stop bits",
    "Invalid handshaking",
    "Invalid fifo threshold",
    "Passed in a NULL pointer",
    "",
    "",
    ""
};

char* handshaking[] =
{
    "none",
    "xon/xoff",
    "rts/cts",
    "dtr/dsr",
    "",
    ""
};


int main(int argc, char* argv[])
{
    int len;
    char buff[100];
    char ch;
    int rc;
    int com = COM_1;

    if(argc < 2)
    {
        printf("Usage: term <com number>\n");
        return 0;
    }

    switch(atoi(argv[1]))
    {
        case 1:
            com = COM_1;
            break;
        case 2:
            com = COM_2;
            break;
        case 3:
            com = COM_3;
            break;
        case 4:
            com = COM_4;
            break;
        default:
            printf("%s: invalid com port number\n", argv[1]);
            return 0;
    }

    if((rc=serial_open(com, 19200L, 8, 'n', 1, SER_HANDSHAKING_XONXOFF)) != SER_SUCCESS)
    {
        printf("Can't open port! (%s)\n", errors[-rc]);
        return 0;
    }

printf("Opened COM%d. Base 0x%03x, IRQ %d, %ld %d%c%d, Handshaking %s\n", com+1,
serial_get_base(com), serial_get_irq(com), serial_get_bps(com), serial_get_data(com),
(char)serial_get_parity(com), serial_get_stop(com), handshaking[serial_get_handshaking(com)]);

    for(;;)
    {
        if(kbhit())
        {
            if((ch=(char)getch()) == 0x1b)
                break;
            if((rc=serial_write(com, &ch, 1)) < 0)
                printf("Error writing (%s)\n", errors[-rc]);
        }
        if((len=serial_read(com, buff, 100)) > 0)
            write(1, buff, len);
        else if(len < 0)
            printf("Error reading (%s)\n", errors[-len]);
    }

    if((rc=serial_close(com)) != SER_SUCCESS)
        printf("Can't close serial port! (%s)\n", errors[-rc]);

    return 0;
}
