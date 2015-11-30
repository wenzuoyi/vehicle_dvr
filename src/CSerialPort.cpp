#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include <string>

#include "error_type.h"
#include "CSerialPort.h"

CSerialPort::CSerialPort():m_fd_serialport(0)
{
}

CSerialPort::~CSerialPort()
{
}

int CSerialPort::set_opt(int baudrate, int bits, char event,  int stopbits)
{
    struct termios newtio,oldtio;
    if  ( tcgetattr(m_fd_serialport,&oldtio)  !=  0)
    {
        perror("SetupSerial 1");
        return ERROR_SETTING_SERIAL;
    }

    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch( bits )
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch( event )
    {
    case 'O':                     //奇校验
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':                     //偶校验
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':                    //无校验
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch( baudrate )
    {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }

    if( stopbits == 1 )
    {
        newtio.c_cflag &=  ~CSTOPB;
    }
    else if ( stopbits == 2 )
    {
        newtio.c_cflag |=  CSTOPB;
    }

    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(m_fd_serialport,TCIFLUSH);

    if((tcsetattr(m_fd_serialport,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return ERROR_SETTING_SERIAL;
    }

    return OK;
}

int CSerialPort::open(std::string serial_port) //打开串口操作, "/dev/ttyS0"
{
    m_fd_serialport = ::open( serial_port.c_str(), O_RDWR|O_NOCTTY|O_NDELAY);
    if (-1 == m_fd_serialport)
    {
        perror("Can't Open Serial Port");
        return ERROR_OPEN_SERIAL;
    }

    if(fcntl(m_fd_serialport, F_SETFL, 0) < 0)
    {
        printf("fcntl failed!\n");
        return ERROR_SETTING_SERIAL;
    }

    if(isatty(STDIN_FILENO) == 0)
    {
        printf("standard input is not a terminal device\n");
        return ERROR_SETTING_SERIAL;
    }

    return OK;
}

int CSerialPort::close()
{
    if(m_fd_serialport <= 0)
   {
        printf("invalide handle has been captured\n");
        return ERROR_INVALID_SERIAL_HANDLE;
    }

    return ::close(m_fd_serialport);
}

int CSerialPort::write(std::string str_info)
{
    if(m_fd_serialport <= 0)
   {
        printf("invalide handle has been captured\n");
        return ERROR_INVALID_SERIAL_HANDLE;
    }

    return ::write(m_fd_serialport, str_info.c_str(), str_info.length());
}

int CSerialPort::read(std::string& str)
{
    if(m_fd_serialport <= 0)
   {
        printf("invalide handle has been captured\n");
        return ERROR_INVALID_SERIAL_HANDLE;
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int length = ::read(m_fd_serialport, buffer, sizeof(buffer));
    if(length <= 0)
   {
        printf("read 0 byte from serial!\n");
        return OK;
   }

    str = std::string(buffer, length);
    return OK;
}
