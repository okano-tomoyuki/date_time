/**
 * @file date_time.hpp
 * @author okano tomoyuki (tomoyuki.okano@tsuneishi.com)
 * @brief 日時情報の管理を行う @ref Utility::DateTime クラスの定義ヘッダー
 * @version 0.1
 * @date 2024-01-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _UTILITY_DATE_TIME_HPP_
#define _UTILITY_DATE_TIME_HPP_

#include <chrono>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <ctime>
#include <stdexcept>

/**
 * @class DateTime
 * @brief 日時を管理するクラス
 * @note 
 * 
 * @snippet test/utility/date_time/test_date_time.cpp DateTime
 */
class DateTime final
{
using system_clock = std::chrono::system_clock;

public:
    enum Unit { HOUR, MIN, SEC, MSEC, USEC };   

    DateTime(const system_clock::time_point& tp = system_clock::now())
    {
        using std::chrono::duration_cast;
        using std::chrono::seconds;
        using std::chrono::microseconds;
        
        auto tt  = system_clock::to_time_t(tp);
        auto tm_ptr = std::gmtime(&tt);
        int usec_epoch  = duration_cast<microseconds>(tp.time_since_epoch()).count();
        int sec_epoch   = 1000000 * duration_cast<seconds>(tp.time_since_epoch()).count();
        int usec = usec_epoch - sec_epoch;
        year_ = tm_ptr->tm_year + 1900; 
        mon_  = tm_ptr->tm_mon + 1; 
        day_  = tm_ptr->tm_mday;
        hour_ = tm_ptr->tm_hour; 
        min_  = tm_ptr->tm_min; 
        sec_  = tm_ptr->tm_sec;
        msec_ = usec / 1000; 
        usec_ = usec % 1000;
    }


    DateTime(const char* str, const char* fmt)
    {
        *this = from_str(str, fmt);
    }

    DateTime(const int& year, const int& mon, const int& day, const int& hour, const int& min, const int& sec, const int& msec, const int& usec)
     :  year_(year), mon_(mon), day_(day), 
        hour_(hour), min_(min), sec_(sec), msec_(msec), usec_(usec)
    {}

    DateTime(const DateTime& other)
     :  year_(other.year_), mon_(other.mon_), day_(other.day_), 
        hour_(other.hour_), min_(other.min_), sec_(other.sec_), msec_(other.msec_), usec_(other.usec_)
    {}

    void operator=(const DateTime& other)
    {
        year_ = other.year_; mon_  = other.mon_; day_ = other.day_; 
        hour_ = other.hour_; min_  = other.min_; sec_ = other.sec_; msec_ = other.msec_; usec_ = other.usec_;
    }

    bool operator==(const DateTime& other) const 
    { 
        return (to_time_point() == other.to_time_point()); 
    }

    bool operator!=(const DateTime& other) const 
    { 
        return (to_time_point() != other.to_time_point()); 
    }

    bool operator<(const DateTime& other) const 
    { 
        return (to_time_point() <  other.to_time_point()); 
    }

    bool operator>(const DateTime& other) const 
    { 
        return (to_time_point() >  other.to_time_point()); 
    }

    /**
     * @fn year
     * @brief 年 取得用Getterメソッド
     * 
     * @return int インスタンスが示す 年 情報を返却する。
     * @note 1970 ~ 2030
     */
    int year() const 
    { 
        return year_; 
    }

    /**
     * @fn month
     * @brief 月 取得用Getterメソッド
     * 
     * @return int インスタンスが示す 月 情報を返却する。
     * @note 1 ~ 12
     */
    int month() const 
    { 
        return  mon_; 
    }

    /**
     * @fn day
     * @brief 日 取得用Getterメソッド
     * 
     * @return int インスタンスが示す 日 情報を返却する。
     * @note 1 ~ その月の最大日数
     */
    int day() const 
    { 
        return  day_; 
    }

    /**
     * @fn hour
     * @brief 時 取得用Getterメソッド
     * 
     * @return int インスタンスが示す 時 情報を返却する。
     * @note 0 ~ 23
     */
    int hour() const 
    { 
        return hour_; 
    }

    /**
     * @fn minute
     * @brief 分 取得用Getterメソッド
     * 
     * @return int インスタンスが示す 分 情報を返却する。
     * @note 0 ~ 59 
     */  
    int minute() const 
    { 
        return  min_; 
    }

    /**
     * @fn second
     * @brief 秒 取得用Getterメソッド
     * 
     * @return int インスタンスが示す 秒 情報を返却する。
     * @note 0 ~ 59
     */
    int second() const 
    { 
        return  sec_; 
    }

    /**
     * @fn millisecond
     * @brief ミリ秒 取得用Getterメソッド
     * 
     * @return int インスタンスが示す 秒 情報を返却する。
     * @note 0 ~ 999
     */
    int millisecond() const 
    { 
        return msec_; 
    }

    /**
     * @fn millisecond
     * @brief マイクロ秒 取得用Getterメソッド
     * 
     * @return int インスタンスが示す マイクロ秒 情報を返却する。
     * @note 0 ~ 999
     */
    int microsecond() const 
    { 
        return usec_; 
    }

    /**
     * @fn to_str
     * @brief フォーマット指定した文字列化メソッド
     * 
     * @return int インスタンスが示す マイクロ秒 情報を返却する。
     * @note 出力時のフォーマット指定方法は以下のフォーマットに従う。
     * @n yyyy   西暦4桁                      1970  ~   2030   
     * @n mm     0埋めされた月                  01  ~     12
     * @n dd     0埋めされた日                  01  ~     31
     * @n hh     0埋めされた時                  00  ~     23
     * @n nn     0埋めされた分                  00  ~     59
     * @n ss     0埋めされた秒                  00  ~     59
     * @n zzz    0埋めされたミリ秒             000  ~    999
     * @n zzzzzz 0埋めされたミリ+マイクロ秒 000000  ~ 999999
     */
    std::string to_str(const char *format) const
    {
        std::string result(format);
        dump(result,   "yyyy", std::to_string(year_));
        dump(result,     "mm", zero_fill(std::to_string(mon_), 2));
        dump(result,     "dd", zero_fill(std::to_string(day_), 2));
        dump(result,     "hh", zero_fill(std::to_string(hour_), 2));
        dump(result,     "nn", zero_fill(std::to_string(min_), 2));
        dump(result,     "ss", zero_fill(std::to_string(sec_), 2));
        dump(result, "zzzzzz", zero_fill(std::to_string(1000*msec_ + usec_), 6));
        dump(result,    "zzz", zero_fill(std::to_string(msec_), 3));
        return std::string(result);
    }

    /**
     * @fn from_str
     * @brief フォーマット指定し文字列からDateTimeインスタンスを生成するメソッド(Factory Method)
     * 
     * @return int インスタンスが示す マイクロ秒 情報を返却する。
     * @note 出力時のフォーマット指定方法は以下のフォーマットに従う。
     * @n yyyy   西暦4桁                      1970  ~   2030   
     * @n mm     0埋めされた月                  01  ~     12
     * @n dd     0埋めされた日                  01  ~     31
     * @n hh     0埋めされた時                  00  ~     23
     * @n nn     0埋めされた分                  00  ~     59
     * @n ss     0埋めされた秒                  00  ~     59
     * @n zzz    0埋めされたミリ秒             000  ~    999
     * @n zzzzzz 0埋めされたミリ+マイクロ秒 000000  ~ 999999
     * @n 
     * @n インスタンスメソッドでないことに注意
     */
    static DateTime from_str(const char* date_time_str, const char* format)
    {
        std::string dt_str(date_time_str);
        std::string fmt_str(format);
        int year, mon, day, hour, min, sec, msec, usec;
        year = 1970; mon = day = 1; day = hour = min = sec = msec = usec = 0;
        int days_of_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        try 
        {
            parse(dt_str, fmt_str,   "yyyy", year, 1970,    2300);
            if(year % 4 == 0 && !(year % 100 == 0 && year % 400 != 0))
                days_of_month[1]++;
            
            parse(dt_str, fmt_str,     "mm",  mon,    1,      12);
            parse(dt_str, fmt_str,     "dd",  day,    1, days_of_month[mon-1]);
            parse(dt_str, fmt_str,     "hh", hour,    0,      23);
            parse(dt_str, fmt_str,     "nn",  min,    0,      59);
            parse(dt_str, fmt_str,     "ss",  sec,    0,      59);
            parse(dt_str, fmt_str, "zzzzzz", usec,    0,  999999);
            msec = usec/1000; usec=usec%1000;
            parse(dt_str, fmt_str,    "zzz", msec,    0,    999);
        } 
        catch(const std::runtime_error& e) 
        {
            std::stringstream ss;
            ss << __func__      << " failed. " << date_time_str << " couldn't parse to "  << format << "." << e.what();
            throw std::runtime_error(ss.str());
        }

        return DateTime(year, mon, day, hour, min, sec, msec, usec);
    }

    /**
     * @fn to_time_point
     * @brief time_point型への変換関数
     * 
     * @return system_clock::time_point 
     */
    system_clock::time_point to_time_point() const
    {
        using std::chrono::milliseconds;
        using std::chrono::microseconds;

        std::tm tm_var{0};
        tm_var.tm_year  = year_  - 1900;
        tm_var.tm_mon   = mon_ - 1;
        tm_var.tm_mday  = day_;
        tm_var.tm_hour  = hour_;
        tm_var.tm_min   = min_;
        tm_var.tm_sec   = sec_;
        tm_var.tm_isdst = -1;
        std::time_t time_seed;
        std::time(&time_seed);  
        auto tt = std::mktime(&tm_var) + std::mktime(std::localtime(&time_seed)) - std::mktime(std::gmtime(&time_seed));
        auto tp = system_clock::from_time_t(tt) + milliseconds(msec_) + microseconds(usec_);

        return tp;
    }

    /**
     * @fn add
     * @brief 日時加算処理
     * 
     * @param increment 加算量
     * @param unit      加算単位 { HOUR, MIN, SEC, MSEC, USEC }
     */
    void add(const int& increment, const enum Unit& unit = DateTime::MSEC)
    {
        if(unit==HOUR)
            *this = DateTime(to_time_point() + std::chrono::hours(increment));
        else if(unit==MIN)
            *this = DateTime(to_time_point() + std::chrono::minutes(increment));
        else if(unit==SEC)
            *this = DateTime(to_time_point() + std::chrono::seconds(increment));
        else if(unit==MSEC)
            *this = DateTime(to_time_point() + std::chrono::milliseconds(increment));
        else // (unit==USEC)
            *this = DateTime(to_time_point() + std::chrono::microseconds(increment));
    }

    /**
     * @fn add
     * @brief 日時加算処理
     * 
     * @param other     加算対象の日時
     * @param increment 加算量
     * @param unit      加算単位 { HOUR, MIN, SEC, MSEC, USEC }
     * @return DateTime 加算対象の日時に加算量を加えた日時
     */
    static DateTime add(const DateTime& other, const int& increment, const enum Unit& unit = DateTime::MSEC)
    {
        auto result = other;
        result.add(increment, unit);
        return result;
    }


    static int diff(const DateTime& lhs, const DateTime& rhs, const enum Unit& unit = DateTime::MSEC)
    {
        using std::chrono::duration_cast;
        if(unit==HOUR)
            return duration_cast<std::chrono::hours>(lhs.to_time_point() - rhs.to_time_point()).count();
        else if (unit==MIN)
            return duration_cast<std::chrono::minutes>(lhs.to_time_point() - rhs.to_time_point()).count();
        else if (unit==SEC)
            return duration_cast<std::chrono::seconds>(lhs.to_time_point() - rhs.to_time_point()).count();
        else if (unit==MSEC)
            return duration_cast<std::chrono::milliseconds>(lhs.to_time_point() - rhs.to_time_point()).count();
        else // unit==USEC
            return duration_cast<std::chrono::microseconds>(lhs.to_time_point() - rhs.to_time_point()).count();
    }

private:
    int year_;
    int mon_;
    int day_;
    int hour_;
    int min_;
    int sec_;
    int msec_;
    int usec_;

    std::string zero_fill(const std::string& str, const std::size_t& digit) const
    {
        std::string result(str);
        int remain = digit > str.size() ? digit - str.size() : 0;
        while(remain--)
            result = '0' + result;
        return result;
    }

    void dump(std::string& str, const char* fmt, const std::string& rep) const
    {
        std::string::size_type pos = 0;
        int fmt_size = std::string(fmt).size();
        while(true)
        {
            pos = str.find(fmt, pos);
            if(pos == std::string::npos)
                break;
            else
            {
                auto l = str.substr(0, pos);
                auto r = str.substr(pos+fmt_size);
                str = l + rep + r;
                pos = pos + rep.size();
            }
        }
    }

    static void parse(std::string& dt_str, std::string& fmt_str, const char* fmt, int& val, const int& low_range, const int& upper_range)
    {
        std::string::size_type pos = 0;
        int fmt_size = std::string(fmt).size();
        while(true)
        {
            pos = fmt_str.find(fmt, pos);
            if(pos == std::string::npos)
                break;
            else
            {
                int tmp;
                try
                {
                    tmp = std::stol(dt_str.substr(pos, fmt_size));
                }
                catch(const std::invalid_argument& err)
                {
                    std::stringstream ss;
                    ss << " convert from " << dt_str.substr(pos, fmt_size) << " to " << fmt << " failed";
                    throw std::runtime_error(ss.str());
                }
                fmt_str.replace(pos, fmt_size, fmt_size, '*');
                dt_str.replace(pos, fmt_size, fmt_size, '*');
                
                if(tmp<low_range || tmp>upper_range)
                {
                    std::stringstream ss;
                    ss << tmp  << " is out of range for " << fmt 
                       << " (" << low_range << " - " << upper_range << ")";
                    throw std::runtime_error(ss.str());
                }

                val = tmp;
                pos = pos + fmt_size;
            }
        }
    }

};

#endif