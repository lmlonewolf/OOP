#include "Schedule.h"
#include "Tests.h"
#include "Console.h"
#include <iostream>

void showResult(const AddResult& result) {
    if (!result.reasons.empty()) {
        std::cout << "Невозможно добавить занятие в " << result.requestedTime << ".\nПричины:\n";
        for (const std::string& reason : result.reasons) {
            std::cout << "- " << reason << "\n";
        }
    }
    if (!result.success) {
        std::cout << "В указанную дату допустимого времени нет.\n";
    }
    else if (result.moved) {
        std::cout << "Рекомендуемое время начала: " << result.finalTime
            << ". Изменение принято.\n";
    }
    else {
        std::cout << "Занятие успешно добавлено.\n";
    }
}

void runExample() {
    Schedule schedule;
    Date date(2026, 9, 1);
    unsigned short room = 232;
    showResult(schedule.addLesson(std::make_unique<IRLesson>(
        "БД", "Новосёлова О.В.", "ИДБ-25-02", date,
        TimeInterval(Time(12, 20), Duration(95)), LessonType::Lection, room)));
    showResult(schedule.addLesson(std::make_unique<OnlineLesson>(
        "ООП", "Скобенко Т.Д.", "ИДБ-25-02", date,
        TimeInterval(Time(13, 0), Duration(95)), LessonType::Lab)));
    schedule.print();
}

void inputLesson(Schedule& schedule) {
    std::string subject = readText("Дисциплина: ");
    std::string professor = readText("Преподаватель: ");
    std::string group = readText("Группа: ");
    unsigned short year = static_cast<unsigned short>(readNumber("Год: ", 1, 2077));
    unsigned short month = static_cast<unsigned short>(readNumber("Месяц: ", 1, 12));
    unsigned short day = static_cast<unsigned short>(readNumber("День: ", 1, 31));
    unsigned short hour = static_cast<unsigned short>(readNumber("Час начала: ", 0, 23));
    unsigned short minute = static_cast<unsigned short>(readNumber("Минута начала: ", 0, 59));
    unsigned short length = static_cast<unsigned short>(readNumber("Продолжительность в минутах: ", 1, 1440));
    int type = readNumber("Вид (1 - лекция, 2 - практика, 3 - лабораторная): ", 1, 3);
    int format = readNumber("Формат (1 - очный, 2 - дистанционный): ", 1, 2);

    Date date(year, month, day);
    TimeInterval interval(Time(hour, minute), Duration(length));
    LessonType lessonType = static_cast<LessonType>(type - 1);
    std::unique_ptr<BaseLesson> lesson;
    if (format == 1) {
        unsigned short room = static_cast<unsigned short>(readNumber("Аудитория: ", 1, 1000));
        lesson = std::make_unique<IRLesson>(subject, professor, group,
            date, interval, lessonType, room);
    }
    else {
        lesson = std::make_unique<OnlineLesson>(subject, professor, group,
            date, interval, lessonType);
    }
    showResult(schedule.addLesson(std::move(lesson)));
}

void manualMode(Schedule& schedule) {
    while (true) {
        std::cout << "\nРАСПИСАНИЕ\n1. Добавить занятие\n2. Показать расписание\n0. Назад\n";
        int choice = readNumber("Выбор: ", 0, 2);
        if (choice == 0) {
            return;
        }
        try {
            if (choice == 1) {
                inputLesson(schedule);
            }
            else {
                schedule.print();
            }
        }
        catch (const std::exception& error) {
            if (std::cin.eof()) {
                return;
            }
            std::cout << "Ошибка: " << error.what() << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    std::system("chcp 65001 > nul");
    try {
        if (argc > 1 && std::string(argv[1]) == "--test") {
            runTests();
            return 0;
        }
        if (argc > 1 && std::string(argv[1]) == "--demo") {
            runExample();
            return 0;
        }
        Schedule schedule;
        while (true) {
            std::cout << "\n1. Запустить тесты\n2. Ручное использование\n3. Готовый пример\n0. Выход\n";
            int choice = readNumber("Выбор режима: ", 0, 3);
            if (choice == 0) {
                return 0;
            }
            if (choice == 1) {
                runTests();
            }
            else if (choice == 2) {
                manualMode(schedule);
            }
            else {
                runExample();
            }
        }
    }
    catch (const std::exception& error) {
        std::cout << "Ошибка: " << error.what() << "\n";
        return 1;
    }
    return 0;
}
