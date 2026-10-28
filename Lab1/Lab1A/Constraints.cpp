#include "Constraints.h"
#include "Schedule.h"
#include <string>
#include <sstream>

const unsigned short LESSON_BREAK = 95;


// Проверка преподавателя

std::string TeacherConstraint::check(
    const BaseLesson& lesson,
    const Schedule& schedule
) const
{
    for (const auto& current : schedule.getLessons()) {
        if (current->getDate() != lesson.getDate()) {
            continue;
        }

        if (current->getProfessor() != lesson.getProfessor()) {
            continue;
        }

        if (current->getInterval() & lesson.getInterval()) {
            std::ostringstream message;
            message << "Преподаватель " << lesson.getProfessor()
                << " занят до " << current->getInterval().getEnd();
            return message.str();
        }
    }
    return "";
}


// Проверка группы

std::string GroupConstraint::check(
    const BaseLesson& lesson,
    const Schedule& schedule
) const
{
    for (const auto& current : schedule.getLessons()) {
        if (current->getDate() != lesson.getDate()) {
            continue;
        }

        if (current->getGroup() != lesson.getGroup()) {
            continue;
        }

        if (current->getInterval() & lesson.getInterval()) {
            return "Группа " + lesson.getGroup() +
                " уже занята";
        }
    }

    return "";
}


// Проверка аудитории

std::string RoomConstraint::check(
    const BaseLesson& lesson,
    const Schedule& schedule
) const
{
    if (lesson.isOnline()) {
        return "";
    }

    const IRLesson* newLesson =
        dynamic_cast<const IRLesson*>(&lesson);

    if (newLesson == nullptr) {
        return "Не удалось определить аудиторию занятия";
    }

    for (const auto& current : schedule.getLessons()) {
        if (current->getDate() != lesson.getDate()) {
            continue;
        }

        if (current->isOnline()) {
            continue;
        }

        const IRLesson* ir =
            dynamic_cast<const IRLesson*>(current.get());

        if (ir == nullptr) {
            return "Не удалось определить аудиторию в расписании";
        }

        if (ir->getRoom() != newLesson->getRoom()) {
            continue;
        }

        if (current->getInterval() & lesson.getInterval()) {
            return "Аудитория " +
                std::to_string(newLesson->getRoom()) +
                " уже занята";
        }
    }

    return "";
}


// Проверка перехода между форматами

std::string FormatConstraint::check(
    const BaseLesson& lesson,
    const Schedule& schedule
) const
{
    for (const auto& current : schedule.getLessons()) {
        if (current->getDate() != lesson.getDate()) {
            continue;
        }

        if (current->isOnline() == lesson.isOnline()) {
            continue;
        }

        if (current->getGroup() != lesson.getGroup()) {
            continue;
        }

        const TimeInterval& a = current->getInterval();
        const TimeInterval& b = lesson.getInterval();

        unsigned int aStart = a.getStart().toMinutes();
        unsigned int aEnd = a.getEnd().toMinutes();

        unsigned int bStart = b.getStart().toMinutes();
        unsigned int bEnd = b.getEnd().toMinutes();

        if (aEnd <= bStart) {
            if (bStart - aEnd < LESSON_BREAK) {
                return "Между очным и дистанционным занятиями требуется "
                    "свободное окно не менее " + std::to_string(LESSON_BREAK) + " мин.";
            }
        }
        else if (bEnd <= aStart) {
            if (aStart - bEnd < LESSON_BREAK) {
                return "Между очным и дистанционным занятиями требуется "
                    "свободное окно не менее " + std::to_string(LESSON_BREAK) + " мин.";
            }
        }
    }

    return "";
}


// Допустимое время начала занятия

std::string StartHoursConstraint::check(
    const BaseLesson& lesson,
    const Schedule&
) const {
    Time start = lesson.getInterval().getStart();
    if (start < Time(8, 0) || start > Time(22, 0)) {
        return "Занятие должно начинаться с 08:00 до 22:00 включительно";
    }
    return "";
}
