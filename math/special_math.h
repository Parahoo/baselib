#ifndef _SPECIAL_MATH_
#define _SPECIAL_MATH_

int db45toint(double db);

int JudgeLeapYear(int Year);

int GetConfucianDay(int Year, int Month, int Day);

void DateUpdateByDays(int &Year, int &Month, int &Day, int Days);

void DateUpdateBySomeTime(int &Year, int &Month, int &Day, int &DayOfWeek, int Years, int Months, int Days);
#endif