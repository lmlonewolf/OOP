#include "Lessons.h"
#include <stdexcept>


// Базовый класс занятия

BaseLesson::BaseLesson(
    const std::string& subject,
    const std::string& professor,
    const std::string& groupName,
    const Date& lessonDate,
    const TimeInterval& interval,
    LessonType lessonType
) :
    subj(subject),
    prof(professor),
    group(groupName),
    date(lessonDate),
    timeInterval(std::make_unique<TimeInterval>(interval)),
    type(lessonType) {
    if (subj.empty() || prof.empty() || group.empty()) {
        throw std::invalid_argument("Название дисциплины, преподаватель и группа обязательны");
    }
    if (type != LessonType::Lection && type != LessonType::Seminar && type != LessonType::Lab) {
        throw std::invalid_argument("Неизвестный тип занятия");
    }
}

const std::string& BaseLesson::getSubject() const {
    return subj;
}
const std::string& BaseLesson::getProfessor() const {
    return prof;
}
const std::string& BaseLesson::getGroup() const {
    return group;
}
const Date& BaseLesson::getDate() const {
    return date;
}
const TimeInterval& BaseLesson::getInterval() const {
    return *timeInterval;
}
LessonType BaseLesson::getLessonType() const {
    return type;
}

void BaseLesson::setInterval(const TimeInterval& interval) {
    timeInterval = std::make_unique<TimeInterval>(interval);
}


// Очка

IRLesson::IRLesson(
    const std::string& subject,
    const std::string& professor,
    const std::string& groupName,
    const Date& lessonDate,
    const TimeInterval& interval,
    LessonType lessonType,
    unsigned short roomNumber
) :
    BaseLesson(subject, professor, groupName, lessonDate, interval, lessonType),
    room(roomNumber) {
    if (room == 0) {
        throw std::invalid_argument("Номер аудитории должен быть больше нуля");
    }
}

bool IRLesson::isOnline() const {
    return false;
}

unsigned short IRLesson::getRoom() const {
    return room;
}


// Дистант

OnlineLesson::OnlineLesson(
    const std::string& subject,
    const std::string& professor,
    const std::string& groupName,
    const Date& lessonDate,
    const TimeInterval& interval,
    LessonType lessonType
) :
    BaseLesson(subject, professor, groupName, lessonDate, interval, lessonType) {
}

bool OnlineLesson::isOnline() const {
    return true;
}
