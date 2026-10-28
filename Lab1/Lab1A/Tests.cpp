#include "Tests.h"
#include "Schedule.h"
#include <sstream>


void require(bool condition, const std::string& name) {
    if (!condition) {
        throw std::runtime_error("Тест не пройден: " + name);
    }
    std::cout << "[OK] " << name << "\n";
}


class StartTimeConstraint : public Constraint {
public:
    std::string check(const BaseLesson& lesson, const Schedule&) const override {
        if (lesson.getInterval().getStart() < Time(8, 0)) {
            return "Занятие должно начинаться не раньше 08:00";
        }
        return "";
    }
};


std::unique_ptr<BaseLesson> makeLesson(int start, int length, int teacher,
    int group, int room, bool online = false, int day = 1) {
    TimeInterval interval(Time(static_cast<unsigned short>(start / 60),
        static_cast<unsigned short>(start % 60)), Duration(static_cast<unsigned short>(length)));
    if (online) {
        return std::make_unique<OnlineLesson>("ООП", std::to_string(teacher),
            std::to_string(group), Date(2026, 9, static_cast<unsigned short>(day)), interval, LessonType::Lab);
    }
    return std::make_unique<IRLesson>("ООП", std::to_string(teacher),
        std::to_string(group), Date(2026, 9, static_cast<unsigned short>(day)), interval,
        LessonType::Lab, static_cast<unsigned short>(room));
}

void runTests() {
    bool error = false;
    require(Date(2024, 2, 29) == Date(2024, 2, 29), "равные даты");
    require(Date(2024, 2, 29) != Date(2024, 3, 1), "разные даты");
    for (int test = 0; test < 8; test++) {
        error = false;
        try {
            if (test == 0) {
                Date invalid(0, 1, 1);
            }
            if (test == 1) {
                Date invalid(2026, 13, 1);
            }
            if (test == 2) {
                Date invalid(2026, 4, 32);
            }
            if (test == 3) {
                Time invalid(24, 1);
            }
            if (test == 4) {
                Time invalid(1, 60);
            }
            if (test == 5) {
                Duration invalid(0);
            }
            if (test == 6) {
                Duration invalid(1441);
            }
            if (test == 7) {
                TimeInterval invalid(Time(23, 59), Duration(2));
            }
        }
        catch (const std::invalid_argument&) {
            error = true;
        }
        require(error, "некорректное значение " + std::to_string(test));
    }
    require(Duration(45) + Duration(50) == Duration(95), "сумма длительностей");
    require(Duration(1) < Duration(2) && Duration(2) > Duration(1) &&
        Duration(1) != Duration(2), "сравнения длительностей");
    require(Time(9, 59) < Time(10, 0) && Time(10, 0) > Time(9, 59) &&
        Time(9, 59) != Time(10, 0), "сравнения времени");
    require(Time(Time(9, 50), 20) == Time(10, 10), "переход через час");
    TimeInterval a(Time(9, 0), Duration(60));
    require(a == TimeInterval(Time(9, 0), Duration(60)), "равенство интервалов");
    require(a != TimeInterval(Time(9, 0), Duration(61)), "неравенство интервалов");
    require(a & TimeInterval(Time(9, 59), Duration(1)), "пересечение на одну минуту");
    require(!(a & TimeInterval(Time(10, 0), Duration(1))), "касание границ разрешено");
    require(a & TimeInterval(Time(9, 15), Duration(10)), "вложенный интервал");
    require(TimeInterval(Time(23, 0), Duration(60)).getEnd() == Time(24, 0),
        "окончание ровно в полночь");
    std::ostringstream out;
    out << Date(2026, 9, 1) << " " << Time(9, 5) << " " << Duration(95) << " " << a;
    require(out.str() == "01.09.2026 09:05 95 мин. 09:00 - 10:00", "русский вывод значений");
    for (int conflict = 0; conflict < 3; conflict++) {
        Schedule schedule;
        schedule.addLesson(makeLesson(600, 60, 1, 1, 232));
        int teacher = 2;
        int group = 2;
        int room = 216;
        if (conflict == 0) {
            teacher = 1;
        }
        if (conflict == 1) {
            group = 1;
        }
        if (conflict == 2) {
            room = 232;
        }
        AddResult result = schedule.addLesson(makeLesson(630, 30, teacher, group, room));
        require(result.success && result.moved && result.finalTime == Time(11, 0),
            "конфликт и ближайший перенос " + std::to_string(conflict));
        require(!result.reasons.empty(), "причина конфликта возвращена");
    }
    {
        Schedule schedule;
        schedule.addLesson(makeLesson(600, 60, 1, 1, 232));
        AddResult result = schedule.addLesson(makeLesson(600, 30, 1, 2, 216));
        require(result.finalTime == Time(9, 30), "ранний вариант ближе позднего");
        require(schedule.addLesson(makeLesson(600, 60, 1, 1, 232, false, 2)).moved == false,
            "разные даты не конфликтуют");
    }
    for (int online = 0; online <= 1; online++) {
        Schedule schedule;

        schedule.addLesson(
            makeLesson(600, 60, 1, 1, 232, online != 0)
        );

        AddResult result = schedule.addLesson(
            makeLesson(700, 30, 2, 1, 216, online == 0)
        );

        require(result.success && result.finalTime == Time(12, 35),
            "окно 95 минут для группы");
    }
    {
        Schedule schedule;
        schedule.addLesson(makeLesson(600, 60, 1, 1, 232));
        require(!schedule.addLesson(makeLesson(755, 30, 1, 1, 216, true)).moved,
            "ровно 95 минут достаточно");
        require(!schedule.addLesson(makeLesson(600, 60, 3, 3, 216, true)).moved,
            "независимые преподаватель и группа");
    }
    {
        Schedule schedule;
        schedule.addLesson(makeLesson(480, 960, 1, 1, 232));
        AddResult result = schedule.addLesson(makeLesson(600, 60, 2, 2, 232));
        require(!result.success && schedule.getLessons().size() == 1,
            "всё допустимое время занято, отклонённое занятие не сохранено");
    }
    {
        Schedule schedule;
        schedule.addConstraint(std::make_unique<StartTimeConstraint>());
        require(schedule.addLesson(makeLesson(400, 60, 1, 1, 232)).finalTime == Time(8, 0),
            "новое правило без изменения поиска");
        error = false;
        try {
            schedule.addLesson(nullptr);
        }
        catch (const std::invalid_argument&) {
            error = true;
        }
        require(error, "пустой указатель на занятие");
        error = false;
        try {
            schedule.addConstraint(nullptr);
        }
        catch (const std::invalid_argument&) {
            error = true;
        }
        require(error, "пустой указатель на правило");
    }
    for (int start : {480, 1320, 460, 1321}) {
        Schedule schedule;
        AddResult result = schedule.addLesson(makeLesson(start, 30, 1, 1, 232));
        int expected = start;
        if (start < 480) {
            expected = 480;
        }
        if (start > 1320) {
            expected = 1320;
        }
        require(result.success && result.finalTime == Time(static_cast<unsigned short>(expected / 60),
            static_cast<unsigned short>(expected % 60)),
            "границы начала 08:00 и 22:00");
    }
    std::cout << "Тесты задания 1 пройдены.\n";
}
