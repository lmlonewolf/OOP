#pragma once

#include <iostream>

class Date {
    const unsigned short year;
    const unsigned short month;
    const unsigned short day;

public:
    Date(unsigned short y, unsigned short m, unsigned short d);

    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;

    unsigned short getYear() const;
    unsigned short getMonth() const;
    unsigned short getDay() const;
};


class Duration {
    const unsigned short minutes;

public:
    Duration(unsigned short m);

    Duration operator+(const Duration& other) const;

    bool operator==(const Duration& other) const;
    bool operator!=(const Duration& other) const;
    bool operator<(const Duration& other) const;
    bool operator>(const Duration& other) const;

    unsigned short getMinutes() const;
};


class Time {
    const unsigned short hour;
    const unsigned short minute;

public:
    Time(unsigned short h, unsigned short m);
    Time(const Time& other, unsigned short additionalMinutes);

    bool operator==(const Time& other) const;
    bool operator!=(const Time& other) const;
    bool operator<(const Time& other) const;
    bool operator>(const Time& other) const;

    unsigned short getHour() const;
    unsigned short getMinute() const;
    unsigned int toMinutes() const;
};


class TimeInterval {
    const Time start;
    const Duration length;
    const Time end;

public:
    TimeInterval(Time startTime, Duration len);

    bool operator==(const TimeInterval& other) const;
    bool operator!=(const TimeInterval& other) const;
    bool operator&(const TimeInterval& other) const;

    Time getStart() const;
    Time getEnd() const;
    Duration getLength() const;
};


std::ostream& operator<<(std::ostream& out, const Date& date);
std::ostream& operator<<(std::ostream& out, const Time& time);
std::ostream& operator<<(std::ostream& out, const Duration& duration);
std::ostream& operator<<(std::ostream& out, const TimeInterval& interval);
