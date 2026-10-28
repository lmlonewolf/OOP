#include "DateTime.h"
#include <stdexcept>
#include <iomanip>


// Дата

Date::Date(unsigned short y, unsigned short m, unsigned short d) :
    year(y), month(m), day(d)
{
    if (year == 0) {
        throw std::invalid_argument("Год должен быть больше нуля");
    }
    if (month < 1 || month > 12) {
        throw std::invalid_argument("Месяц должен быть от 1 до 12");
    }
    if (day < 1 || day > 31) {
        throw std::invalid_argument("День должен быть от 1 до 31");
    }
}

bool Date::operator==(const Date& other) const {
    return year == other.year &&
        month == other.month &&
        day == other.day;
}
bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

unsigned short Date::getYear() const {
    return year;
}
unsigned short Date::getMonth() const {
    return month;
}
unsigned short Date::getDay() const {
    return day;
}


// Продолжительность

Duration::Duration(unsigned short m) : minutes(m) {
    if (minutes == 0 || minutes > 1440) {
        throw std::invalid_argument("Продолжительность должна быть от 1 до 1440 минут");
    }
}

Duration Duration::operator+(const Duration& other) const {
    unsigned int sum = minutes + other.minutes;
    if (sum > 1440) {
        throw std::invalid_argument("Сумма продолжительностей больше суток");
    }
    return Duration(static_cast<unsigned short>(sum));
}

bool Duration::operator==(const Duration& other) const {
    return minutes == other.minutes;
}
bool Duration::operator!=(const Duration& other) const {
    return !(*this == other);
}
bool Duration::operator<(const Duration& other) const {
    return minutes < other.minutes;
}
bool Duration::operator>(const Duration& other) const {
    return minutes > other.minutes;
}

unsigned short Duration::getMinutes() const {
    return minutes;
}


// Время

Time::Time(unsigned short h, unsigned short m) :
    hour(h), minute(m)
{
    if (hour > 24 || (hour == 24 && minute != 0)) {
        throw std::invalid_argument("Допустимо время от 00:00 до 24:00");
    }

    if (minute > 59) {
        throw std::invalid_argument("Минута должна быть от 0 до 59");
    }
}

Time::Time(const Time& other, unsigned short additionalMinutes) :
    Time(
        static_cast<unsigned short>((other.hour * 60 + other.minute + additionalMinutes) / 60),
        static_cast<unsigned short>((other.hour * 60 + other.minute + additionalMinutes) % 60)
    ) {
}

bool Time::operator==(const Time& other) const {
    return hour == other.hour && minute == other.minute;
}

bool Time::operator!=(const Time& other) const {
    return !(*this == other);
}

bool Time::operator<(const Time& other) const {
    return hour < other.hour ||
        (hour == other.hour && minute < other.minute);
}

bool Time::operator>(const Time& other) const {
    return hour > other.hour ||
        (hour == other.hour && minute > other.minute);
}

unsigned short Time::getHour() const {
    return hour;
}
unsigned short Time::getMinute() const {
    return minute;
}
unsigned int Time::toMinutes() const {
    return hour * 60 + minute;
}


// Временной интервал

TimeInterval::TimeInterval(Time startTime, Duration len) :
    start(startTime),
    length(len),
    end(startTime, len.getMinutes()) {
}

bool TimeInterval::operator==(const TimeInterval& other) const {
    return start == other.start && length == other.length;
}

bool TimeInterval::operator!=(const TimeInterval& other) const {
    return !(*this == other);
}

bool TimeInterval::operator&(const TimeInterval& other) const {
    return start < other.end && other.start < end;
}

Time TimeInterval::getStart() const {
    return start;
}

Time TimeInterval::getEnd() const {
    return end;
}

Duration TimeInterval::getLength() const {
    return length;
}


// Вывод

std::ostream& operator<<(std::ostream& out, const Date& date) {
    out << std::setfill('0')
        << std::setw(2) << date.getDay() << "."
        << std::setw(2) << date.getMonth() << "."
        << date.getYear()
        << std::setfill(' ');

    return out;
}

std::ostream& operator<<(std::ostream& out, const Time& time) {
    out << std::setfill('0')
        << std::setw(2) << time.getHour() << ":"
        << std::setw(2) << time.getMinute()
        << std::setfill(' ');

    return out;
}

std::ostream& operator<<(std::ostream& out, const Duration& duration) {
    out << duration.getMinutes() << " мин.";
    return out;
}

std::ostream& operator<<(std::ostream& out, const TimeInterval& interval) {
    out << interval.getStart() << " - " << interval.getEnd();
    return out;
}
