#pragma once

class FuzzyLogic {
public:
    FuzzyLogic();
    ~FuzzyLogic();
    // MEMBERSHIP METHODS FOR FUZZY LOGIC
    // Boolean
    static float getMembership(const float value, const float x0);
    static float getInverseMembership(const float value, const float x0);
    // Gradual
    static float getMembership(const float value, const float x0, const float x1);
    static float getInverseMembership(const float value, const float x0, const float x1);
    // Triangle
    static float getMembership(const float value, const float x0, const float x1, const float x2);
    static float getInverseMembership(const float value, const float x0, const float x1, const float x2);
    // Trapeze
    static float getMembership(const float value, const float x0, const float x1, const float x2, const float x3);
    static float getInverseMembership(const float value, const float x0, const float x1, const float x2, const float x3);

    static float OR(const float value1, const float value2);
    static float AND(const float value1, const float value2);

private:
    static float calculateMembership(const float value, const float x0, const float x1);
    static float calculateInverseMembership(const float value, const float x0, const float x1);
};