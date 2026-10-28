#include <cstdio>

// How many days are in the months of a non-leap year
//               J   F   M   A   M   J   J   A   S   O   N   D
int days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool is_triplet(int d_sq, int m_sq, int y_sq) {

    if ( (d_sq + m_sq == y_sq) || \
         (d_sq + y_sq == m_sq) || \
         (m_sq + y_sq == d_sq) ) {
        return true;
    } else {
        return false;
    }
}

int main(void) {

    for (int year = 2001; year < 2101; year++) {
        int year_square = (year % 100) * (year % 100);
        for (int month = 1; month < 13; month++) {
            int month_square = month * month;
            for (int day = 1; day <= days_in_month[month - 1]; day++) {
                if ( is_triplet(day * day, month_square, year_square) ) {
                    printf("Found a Pythagorean triple day: %d, %d, %d\n", day, month, year);
                }
            }
            if ((month == 2) && (year % 4 == 0) && (year != 2100)) {
                if ( is_triplet(29 * 29, month_square, year_square) ) {
                    printf("Found a Pythagorean triple day: %d, %d, %d\n", 29, month, year);
                }
            }
        }
    }
    return 0;
}

/* Output:
Found a Pythagorean triple day: 5, 4, 2003
Found a Pythagorean triple day: 4, 5, 2003
Found a Pythagorean triple day: 5, 3, 2004
Found a Pythagorean triple day: 3, 5, 2004
Found a Pythagorean triple day: 4, 3, 2005
Found a Pythagorean triple day: 3, 4, 2005
Found a Pythagorean triple day: 13, 12, 2005
Found a Pythagorean triple day: 10, 8, 2006
Found a Pythagorean triple day: 8, 10, 2006
Found a Pythagorean triple day: 10, 6, 2008
Found a Pythagorean triple day: 6, 10, 2008
Found a Pythagorean triple day: 15, 12, 2009
Found a Pythagorean triple day: 8, 6, 2010
Found a Pythagorean triple day: 6, 8, 2010
Found a Pythagorean triple day: 13, 5, 2012
Found a Pythagorean triple day: 15, 9, 2012
Found a Pythagorean triple day: 12, 5, 2013
Found a Pythagorean triple day: 5, 12, 2013
Found a Pythagorean triple day: 17, 8, 2015
Found a Pythagorean triple day: 12, 9, 2015
Found a Pythagorean triple day: 9, 12, 2015
Found a Pythagorean triple day: 20, 12, 2016
Found a Pythagorean triple day: 15, 8, 2017
Found a Pythagorean triple day: 16, 12, 2020
Found a Pythagorean triple day: 25, 7, 2024
Found a Pythagorean triple day: 26, 10, 2024
Found a Pythagorean triple day: 24, 7, 2025
Found a Pythagorean triple day: 24, 10, 2026
Found a Pythagorean triple day: 1, 1, 2100
Found a Pythagorean triple day: 2, 2, 2100
Found a Pythagorean triple day: 3, 3, 2100
Found a Pythagorean triple day: 4, 4, 2100
Found a Pythagorean triple day: 5, 5, 2100
Found a Pythagorean triple day: 6, 6, 2100
Found a Pythagorean triple day: 7, 7, 2100
Found a Pythagorean triple day: 8, 8, 2100
Found a Pythagorean triple day: 9, 9, 2100
Found a Pythagorean triple day: 10, 10, 2100
Found a Pythagorean triple day: 11, 11, 2100
Found a Pythagorean triple day: 12, 12, 2100
*/
