#include "Date.hpp"

Date :: Date():m_year(0), m_month(0), m_day(0), m_hour(0), m_minute(0){}

Date :: Date(int t_year, int t_month, int t_day, int t_hour, int t_minute):
             m_year(t_year), m_month(t_month), m_day(t_day), m_hour(t_hour), m_minute(t_minute){}

Date :: Date(const std::string &dateString){
    m_year = Date::stringToDate(dateString).getYear();
    m_month = Date::stringToDate(dateString).getMonth();
    m_day = Date::stringToDate(dateString).getDay();
    m_hour = Date::stringToDate(dateString).getHour();
    m_minute = Date::stringToDate(dateString).getMinute();
}

int Date :: getYear(void) const{
    return m_year;
}

void Date :: setYear(const int t_year){
    m_year = t_year;
}

int Date :: getMonth(void) const{
    return m_month;
}

void Date :: setMonth(const int t_month){
    m_month = t_month;
}

int Date :: getDay(void) const{
    return m_day;
}

void Date :: setDay(const int t_day){
    m_day = t_day;
}

int Date :: getHour(void) const{
    return m_hour;
}

void Date :: setHour(const int t_hour){
    m_hour = t_hour;
}

int Date :: getMinute(void) const{
    return m_minute;
}

void Date :: setMinute(const int t_minute){
    m_minute = t_minute;
}

bool Date :: isValid(const Date &t_date){
    if(t_date.getYear() < 1000 || t_date.getYear() > 9999)
        return false;
    if(t_date.getMonth() < 1 || t_date.getMonth() > 12)
        return false;
    if(t_date.getHour() < 0 || t_date.getHour() > 23)
        return false;
    if(t_date.getMinute() < 0 || t_date.getMinute() > 59)
        return false;
    if(t_date.getMonth() == 1 || t_date.getMonth() == 3 || t_date.getMonth() == 5 || t_date.getMonth() == 7 ||
       t_date.getMonth() == 8 ||t_date.getMonth() == 10 || t_date.getMonth() == 12){
        if(t_date.getDay() < 1 || t_date.getDay() > 31)
            return false;
    }
    if(t_date.getMonth() == 4 || t_date.getMonth() == 6 || t_date.getMonth() == 9 || t_date.getMonth() == 11){
        if(t_date.getDay() < 1 || t_date.getDay() > 30)
            return false;
    }
    if(t_date.getMonth() == 2){
        if((t_date.getYear() % 4 == 0 && t_date.getYear() % 100 != 0) ||
            t_date.getYear() % 400 == 0){
            if(t_date.getDay() < 1 || t_date.getDay() > 29)
                return false;
        }
        else if(t_date.getDay() < 1 || t_date.getDay() > 28)
            return false;
    }
    
    
    return true;
}

Date Date :: stringToDate(const std::string &t_dateString){
    Date result;

    if(t_dateString.length() != 16)
        return result;
    if(t_dateString[4] != '-' || t_dateString[7] != '-' || t_dateString[10] != '/' || t_dateString[13] != ':')
        return result;
    for(int i = 0; i < 16; i++){
        if(i == 4 || i == 7 || i == 10 || i == 13){}
        else if(!(t_dateString[i] >= '0' && t_dateString[i] <= '9')){
            return result;
        }
    }

    result.setYear((t_dateString[0] - '0') * 1000 + (t_dateString[1] - '0') * 100 + (t_dateString[2] - '0') * 10 + (t_dateString[3] - '0'));
    result.setMonth((t_dateString[5] - '0') * 10 + (t_dateString[6] - '0'));
    result.setDay((t_dateString[8] - '0') * 10 + (t_dateString[9] - '0'));
    result.setHour((t_dateString[11] - '0') * 10 + (t_dateString[12] - '0'));
    result.setMinute((t_dateString[14] - '0') * 10 + (t_dateString[15] - '0'));
    return result;
}

std::string Date :: dateToString(const Date &t_date){
    std::string str;
    if(!Date::isValid(t_date)){
        return "0000-00-00/00:00";
    }
    str.resize(16);
    str[0] = (t_date.getYear() / 1000) + '0';
    str[1] = (t_date.getYear() % 1000 / 100) + '0';
    str[2] = (t_date.getYear() % 100 / 10) + '0';
    str[3] = (t_date.getYear() % 10) + '0';
    str[4] = '-';

    str[5] = (t_date.getMonth() / 10) + '0';
    str[6] = (t_date.getMonth() % 10) + '0';
    str[7] = '-';

    str[8] = (t_date.getDay() / 10) + '0';
    str[9] = (t_date.getDay() % 10) + '0';
    str[10] = '/';

    str[11] = (t_date.getHour() / 10) + '0';
    str[12] = (t_date.getHour() % 10) + '0';
    str[13] = ':' ;

    str[14] = (t_date.getMinute() / 10) + '0';
    str[15] = (t_date.getMinute() % 10) + '0';

    return str;
}


Date & Date :: operator=(const Date &t_date){
    m_year = t_date.getYear();
    m_month = t_date.getMonth();
    m_day = t_date.getDay();
    m_hour = t_date.getHour();
    m_minute = t_date.getMinute();
    return *this;
}

bool Date :: operator==(const Date &t_date) const{
    if(m_year == t_date.getYear() && m_month == t_date.getMonth() && m_day == t_date.getDay() && m_hour == t_date.getHour() && m_minute == t_date.getMinute())
        return true;
    return false;
}

bool Date :: operator>(const Date &t_date) const{
    if(m_year > t_date.getYear())
        return true;
    else if(m_year == t_date.getYear() && m_month > t_date.getMonth())
        return true;
    else if(m_year == t_date.getYear() && m_month == t_date.getMonth() && m_day > t_date.getDay())
        return true;
    else if(m_year == t_date.getYear() && m_month == t_date.getMonth() && m_day == t_date.getDay() && m_hour > t_date.getHour())
        return true;
    else if(m_year == t_date.getYear() && m_month == t_date.getMonth() && m_day == t_date.getDay() && m_hour == t_date.getHour() && m_minute > t_date.getMinute())
        return true;
    return false;
}

bool Date :: operator<(const Date &t_date) const{
    if(m_year < t_date.getYear())
        return true;
    else if(m_year == t_date.getYear() && m_month < t_date.getMonth())
        return true;
    else if(m_year == t_date.getYear() && m_month == t_date.getMonth() && m_day < t_date.getDay())
        return true;
    else if(m_year == t_date.getYear() && m_month == t_date.getMonth() && m_day == t_date.getDay() && m_hour < t_date.getHour())
        return true;
    else if(m_year == t_date.getYear() && m_month == t_date.getMonth() && m_day == t_date.getDay() && m_hour == t_date.getHour() && m_minute < t_date.getMinute())
        return true;
    return false;
}

bool Date :: operator>=(const Date &t_date) const{
    if(*this > t_date || *this == t_date)
        return true;
    return false;
}

bool Date :: operator<=(const Date &t_date) const{
    if(*this < t_date || *this == t_date)
        return true;
    return false;
}