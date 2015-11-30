#include <string.h>

#include <string>
#include  <array>
#include <fstream>

#include "CLog.h"

CLog::CLog(const std::string& log_path)
{
    if(out_log.is_open())
        out_log.close();

    out_log.open(log_path.c_str(), std::ios::binary | std::ios::app);
}

CLog::~CLog()
{
    if(out_log.is_open())
        out_log.close();
}

CLog& CLog::getInstance(const std::string& log_path)
{
    static CLog instance(log_path);
    return instance;
}

std::string CLog::get_cur_timestamp()
{
    time_t t = time(0);
    char tmp[64];
    bzero(tmp, sizeof(tmp));
    strftime( tmp, sizeof(tmp), "%Y/%m/%d %H:%M:%S  ",localtime(&t));
    return std::string(tmp, strlen(tmp));
}

std::string CLog::get_cur_logtype(CLog::LogType log_type)
{
    std::array<std::string, 3> log_type_array = { std::string("[Error] "),  std::string("[Info ] "),  std::string("[Warn ]" )};
    return log_type_array[log_type];
}

int CLog::write(const std::string& input_info, CLog::LogType log_type)
{
    if(out_log.is_open())
    {
        std::string str_input = get_cur_timestamp() + get_cur_logtype(log_type) + input_info + std::string("\n");
        out_log <<str_input;
    }

    return 0;
}

unsigned long CLog::get_file_size()
{
    unsigned long filesize = -1;
  /*  struct stat statbuff = {0};

    if(stat(file_path.c_str(), &statbuff) >= 0)
        filesize = statbuff.st_size;*/
    return filesize;
}
