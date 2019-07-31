#include "special_math.h"

static int data[2][13] = { {0,31,28,31,30,31,30,31,31,30,31,30,31},
                        {0,31,29,31,30,31,30,31,31,30,31,30,31} };

int db45toint(double db)
{
  if(db >= 0)
    return (int)(db + 0.5);
  else
    return (int)(db - 0.5);
}

int JudgeLeapYear(int Year)
{
    if ((Year % 4 == 0 && Year % 100 != 0) || Year % 400 == 0)
        return 1;
    else
        return 0;
}

int GetConfucianDay(int Year, int Month, int Day)
{
    int k = JudgeLeapYear(Year);

    for (int i = 1; i < Month; i++)
        Day += data[k][i];
    return Day;
}

void DateUpdateByDays(int &Year, int &Month, int &Day, int Days)
{
    int Y = Year, M = Month, D = Day;
    int ConfucianDay = GetConfucianDay(Y, M, D);
    M = 1;
    D = ConfucianDay + Days;
    int IsLeapYear = JudgeLeapYear(Y);
    while ((IsLeapYear && D > 366) || (!IsLeapYear && D > 365))
    {
        Y++;
        D -= IsLeapYear ? 366 : 365;
        IsLeapYear = JudgeLeapYear(Y);
    }
    while (D > data[IsLeapYear][M])
    {
        D -= data[IsLeapYear][M];
        M++; 
    }
    Year = Y;
    Month = M;
    Day = D;
}

void DateUpdateBySomeTime(int &Year, int &Month, int &Day, int &DayOfWeek, int Years, int Months, int Days)
{
    int Y = Year, M = Month, D = Day, DofW = DayOfWeek;
    DateUpdateByDays(Y, M, D, Days);
    /*M += Months;
    Y += (M - 1) / 12;
    M = (M - 1) % 12 + 1;
    Y += Years;*/
    if (Years != 0)
    {
        int mark;
        for (int i = 0; i < Years; i++)
        {
            if (M <= 2)
                mark = JudgeLeapYear(Y);
            else
                mark = JudgeLeapYear(Y + 1);
            DofW += mark ? 366 : 365;
            Y++;
        }
    }
    else
    {
        if (Months != 0)
        {
            for (int i = 0; i < Months; i++)
            {
                int mark = JudgeLeapYear(Y);
                DofW += data[mark][M];
                M++;
                if (M > 12)
                {
                    M -= 12;
                    Y++;
                }
            }
        }
        else
        {
            DofW += Days;
        }
    }
    DofW = (DofW - 1) % 7 + 1;
    Year = Y;
    Month = M;
    Day = D;
    DayOfWeek = DofW;
}