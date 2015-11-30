#ifndef CSERIALPORT_H
#define CSERIALPORT_H


class CSerialPort
{
    public:
        CSerialPort();
        virtual ~CSerialPort();

        int set_opt(int nSpeed, int nBits, char nEvent, int nStop);

        int open(std::string);
        int close();

        int write(std::string);//写入串口
        int read(std::string&);//从串口读取

private:
        int m_fd_serialport;
};

#endif // CSERIALPORT_H
