#include "FuzzyLogic.hpp"
#include <algorithm>

FuzzyLogic::FuzzyLogic() {}

FuzzyLogic::~FuzzyLogic() {}

// MEMBERSHIP METHODS FOR FUZZY LOGIC
float FuzzyLogic::calculateMembership(const float value, const float x0, const float x1) {
    return (value / (x1 - x0)) - (x0 / (x1 - x0));
}

float FuzzyLogic::calculateInverseMembership(const float value, const float x0, const float x1) {
    return (-value / (x1 - x0)) + (x1 / (x1 - x0));
}

// Boolean
float FuzzyLogic::getMembership(const float value, const float x0) {
    float membership = 0.;

    if (value > x0)
        membership = 1.;

    return membership;
}

float FuzzyLogic::getInverseMembership(const float value, const float x0) {
    float membership = 1.;

    if (value > x0)
        membership = 0.;

    return membership;
}

// Gradual
float FuzzyLogic::getMembership(const float value, const float x0, const float x1) {
    float membership = 0.;

    if (value > x0 && value < x1)
        membership = calculateMembership(value, x0, x1);
    else if (value >= x1)
        membership = 1.;

    return membership;
}

float FuzzyLogic::getInverseMembership(const float value, const float x0, const float x1) {
    float membership = 1.;

    if (value > x0 && value < x1)
        membership = calculateInverseMembership(value, x0, x1);
    else if (value >= x1)
        membership = 0.;

    return membership;
}

// Trapeze
float FuzzyLogic::getMembership(const float value, const float x0, const float x1, const float x2, const float x3) {
    float membership = 0.;

    if ((value > x0) && (value < x1))
        membership = calculateMembership(value, x0, x1);
    else if ((value >= x1) && (value <= x2))
        membership = 1.;
    else if ((value > x2) && (value < x3))
        membership = calculateInverseMembership(value, x2, x3);

    return membership;
}

float FuzzyLogic::getInverseMembership(const float value, const float x0, const float x1, const float x2, const float x3) {
    float membership = 1.;

    if ((value > x0) && (value < x1))
        membership = calculateInverseMembership(value, x0, x1);
    else if ((value >= x1) && (value <= x2))
        membership = 0.;
    else if ((value > x2) && (value < x3))
        membership = calculateMembership(value, x2, x3);

    return membership;
}

// Triangle
float FuzzyLogic::getMembership(const float value, const float x0, const float x1, const float x2) {
    return FuzzyLogic::getMembership(value, x0, x1, x1, x2);
}

float FuzzyLogic::getInverseMembership(const float value, const float x0, const float x1, const float x2) {
    return FuzzyLogic::getInverseMembership(value, x0, x1, x1, x2);
}

float FuzzyLogic::OR(const float value1, const float value2) {
    return std::max(value1, value2);
}

float FuzzyLogic::AND(const float value1, const float value2) {
    return std::min(value1, value2);
}
