#ifndef CLOG_H
#define CLOG_H

class CLog
{
  public:
    enum LogType
    {
        Error = 0,
        Info,
        Warn
    };

    public:
        CLog(const std::string&);
        virtual ~CLog();

        int write(const std::string&, CLog::LogType log_type);

    public:
        static CLog& getInstance(const std::string&);

    private:
        std::string  get_cur_timestamp();
        std::string  get_cur_logtype(CLog::LogType);

        unsigned long get_file_size();

    private:
        std::ofstream out_log;
        std::string file_path;
};

#endif // CLOG_H
