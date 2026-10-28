#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Lessons.h"
#include "Constraints.h"


struct AddResult {
    bool success;
    bool moved;

    Time requestedTime;
    Time finalTime;

    std::vector<std::string> reasons;
};


class Schedule {
private:
    std::vector<std::unique_ptr<BaseLesson>> lessons;
    std::vector<std::unique_ptr<Constraint>> constraints;

    std::vector<std::string> checkConstraints(
        const BaseLesson& lesson
    ) const;

public:
    Schedule();

    const std::vector<std::unique_ptr<BaseLesson>>&
        getLessons() const;

    void addConstraint(std::unique_ptr<Constraint> constraint);

    AddResult addLesson(std::unique_ptr<BaseLesson> lesson);

    void print() const;
};
