#include "Schedule.h"
#include "Constraints.h"

#include <iostream>
#include <stdexcept>
#include <utility>


Schedule::Schedule() {
    addConstraint(std::make_unique<StartHoursConstraint>());
    addConstraint(std::make_unique<TeacherConstraint>());
    addConstraint(std::make_unique<GroupConstraint>());
    addConstraint(std::make_unique<RoomConstraint>());
    addConstraint(std::make_unique<FormatConstraint>());
}

const std::vector<std::unique_ptr<BaseLesson>>&
Schedule::getLessons() const {
    return lessons;
}

void Schedule::addConstraint(std::unique_ptr<Constraint> constraint) {
    if (!constraint) {
        throw std::invalid_argument("Ограничение не задано");
    }

    constraints.push_back(std::move(constraint));
}

std::vector<std::string> Schedule::checkConstraints(
    const BaseLesson& lesson
) const
{
    std::vector<std::string> reasons;

    for (const auto& constraint : constraints) {
        std::string result = constraint->check(lesson, *this);

        if (!result.empty()) {
            reasons.push_back(result);
        }
    }

    return reasons;
}

AddResult Schedule::addLesson(std::unique_ptr<BaseLesson> lesson) {
    if (!lesson) {
        throw std::invalid_argument("Занятие не задано");
    }

    Time original = lesson->getInterval().getStart();

    std::vector<std::string> reasons = checkConstraints(*lesson);

    if (reasons.empty()) {
        lessons.push_back(std::move(lesson));

        return { true, false, original, original, {} };
    }

    unsigned int startMinutes =
        original.getHour() * 60 + original.getMinute();

    unsigned int length =
        lesson->getInterval().getLength().getMinutes();


    for (int distance = 1; distance <= 1440; distance++) {
        for (int direction = 1; direction >= -1; direction -= 2) {
            int candidate = static_cast<int>(startMinutes) + direction * distance;
            if (candidate < 0 || candidate + static_cast<int>(length) > 1440) {
                continue;
            }
            Time newTime(static_cast<unsigned short>(candidate / 60),
                static_cast<unsigned short>(candidate % 60));
            TimeInterval newInterval(newTime,
                Duration(static_cast<unsigned short>(length)));
            lesson->setInterval(newInterval);
            if (checkConstraints(*lesson).empty()) {
                lessons.push_back(std::move(lesson));
                return { true, true, original, newTime, reasons };
            }
        }
    }

    return { false, false, original, original, reasons };
}

void Schedule::print() const {
    if (lessons.empty()) {
        std::cout << "Расписание пусто.\n";
        return;
    }

    for (const auto& lesson : lessons) {
        std::cout << "\nДисциплина: "
            << lesson->getSubject() << "\n";

        std::cout << "Преподаватель: "
            << lesson->getProfessor() << "\n";

        std::cout << "Группа: "
            << lesson->getGroup() << "\n";

        std::cout << "Дата: "
            << lesson->getDate() << "\n";

        std::cout << "Время: "
            << lesson->getInterval() << "\n";

        std::cout << "Тип: ";

        switch (lesson->getLessonType()) {
        case LessonType::Lection:
            std::cout << "Лекция\n";
            break;

        case LessonType::Seminar:
            std::cout << "Практика\n";
            break;

        case LessonType::Lab:
            std::cout << "Лабораторная\n";
            break;
        }

        if (lesson->isOnline()) {
            std::cout << "Формат: дистанционный\n";
        }
        else {
            std::cout << "Формат: очный\n";

            const IRLesson* ir =
                dynamic_cast<const IRLesson*>(lesson.get());

            if (ir != nullptr) {
                std::cout << "Аудитория: "
                << ir->getRoom() << "\n";
            }
        }

        std::cout << "-------------------------\n";
    }
}
