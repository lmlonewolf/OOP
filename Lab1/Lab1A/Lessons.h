#pragma once

#include <string>
#include <memory>
#include "DateTime.h"

enum class LessonType {
    Lection,
    Seminar,
    Lab
};


class BaseLesson {
protected:
    std::string subj;
    std::string prof;
    std::string group;
    Date date;
    std::unique_ptr<TimeInterval> timeInterval;
    LessonType type;

public:
    BaseLesson(
        const std::string& subject,
        const std::string& professor,
        const std::string& groupName,
        const Date& lessonDate,
        const TimeInterval& interval,
        LessonType lessonType
    );

    virtual ~BaseLesson() = default;

    virtual bool isOnline() const = 0;

    const std::string& getSubject() const;
    const std::string& getProfessor() const;
    const std::string& getGroup() const;
    const Date& getDate() const;
    const TimeInterval& getInterval() const;
    LessonType getLessonType() const;

private:
    friend class Schedule;

    void setInterval(const TimeInterval& interval);
};


class IRLesson : public BaseLesson {
    unsigned short room;

public:
    IRLesson(
        const std::string& subject,
        const std::string& professor,
        const std::string& groupName,
        const Date& lessonDate,
        const TimeInterval& interval,
        LessonType lessonType,
        unsigned short roomNumber
    );

    bool isOnline() const override;

    unsigned short getRoom() const;
};


class OnlineLesson : public BaseLesson {
public:
    OnlineLesson(
        const std::string& subject,
        const std::string& professor,
        const std::string& groupName,
        const Date& lessonDate,
        const TimeInterval& interval,
        LessonType lessonType
    );

    bool isOnline() const override;
};
