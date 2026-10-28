#pragma once

#include <string>

class BaseLesson;
class Schedule;

class Constraint {
public:
    virtual ~Constraint() = default;

    virtual std::string check(
        const BaseLesson& lesson,
        const Schedule& schedule
    ) const = 0;
};


class TeacherConstraint : public Constraint {
public:
    std::string check(
        const BaseLesson& lesson,
        const Schedule& schedule
    ) const override;
};


class GroupConstraint : public Constraint {
public:
    std::string check(
        const BaseLesson& lesson,
        const Schedule& schedule
    ) const override;
};


class RoomConstraint : public Constraint {
public:
    std::string check(
        const BaseLesson& lesson,
        const Schedule& schedule
    ) const override;
};


class FormatConstraint : public Constraint {
public:
    std::string check(
        const BaseLesson& lesson,
        const Schedule& schedule
    ) const override;
};


class StartHoursConstraint : public Constraint {
public:
    std::string check(
        const BaseLesson& lesson,
        const Schedule& schedule
    ) const override;
};
