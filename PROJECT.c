#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARS 200
#define MAX_LEN 32
#define FILENAME "cars.csv"

typedef struct {
    char make[MAX_LEN], model[MAX_LEN], buyDate[11], sellDate[11];
    int mileage;
    double buyPrice, sellPrice;
} Car;

static void trim(char *s) {
    char *p = s, *q = s + strlen(s);
    while (*p && isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p) + 1);
    while (q > s && isspace((unsigned char)q[-1])) *--q = '\0';
}

static int leap(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

static int daysInMonth(int year, int month) {
    static const int days[] = {31, 28, 31, 30, 31, 30,
                               31, 31, 30, 31, 30, 31};

    if (month == 2 && leap(year)) return 29;
    return days[month - 1];
}

static int validDate(const char *s) {
    int y, m, d;
    if (!s || strlen(s) != 10 || s[4] != '-' || s[7] != '-') return 0;
    if (sscanf(s, "%d-%d-%d", &y, &m, &d) != 3) return 0;
    return y >= 1900 && m >= 1 && m <= 12 && d >= 1 && d <= daysInMonth(y, m);
}

static long toDays(const char *s) {
    int year, month, day, i;
    long total = 0;

    if (!validDate(s)) return -1;
    sscanf(s, "%d-%d-%d", &year, &month, &day);
    for (i = 1900; i < year; i++) {
        total += 365 + leap(i);
    }
    for (i = 1; i < month; i++) {
        total += daysInMonth(year, i);
    }
    return total + day - 1;
}

static int parseInt(const char *s, int min, int max, int *out) {
    char *end;
    long value;

    if (!s || *s == '\0') return 0;
    errno = 0;
    value = strtol(s, &end, 10);
    if (errno || end == s || *end != '\0' || value < min || value > max) {
        return 0;
    }
    *out = (int)value;
    return 1;
}

static int parseDouble(const char *s, double min, double max, double *out) {
    char *end;
    double value;

    if (!s || *s == '\0') return 0;
    errno = 0;
    value = strtod(s, &end);
    if (errno || end == s || *end != '\0' || value < min || value > max) {
        return 0;
    }
    *out = value;
    return 1;
}

static int readText(const char *prompt, char *buf, int n) {
    int ch;

    printf("%s", prompt);
    if (!fgets(buf, n, stdin)) return 0;
    if (buf[strlen(buf) - 1] != '\n') {
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
        printf("Error: Input is too long.\n");
        return 0;
    }
    buf[strcspn(buf, "\r\n")] = '\0';
    trim(buf);
    if (!buf[0]) {
        printf("Error: Empty input is not allowed.\n");
        return 0;
    }
    return 1;
}

static int readIntValue(const char *prompt, int min, int max) {
    char buf[64];
    int v;
    while (1) {
        if (!readText(prompt, buf, sizeof(buf))) continue;
        if (parseInt(buf, min, max, &v)) return v;
        printf("Error: Enter a number between %d and %d.\n", min, max);
    }
}

static double readDoubleValue(const char *prompt, double min, double max) {
    char buf[64];
    double v;
    while (1) {
        if (!readText(prompt, buf, sizeof(buf))) continue;
        if (parseDouble(buf, min, max, &v)) return v;
        printf("Error: Enter a valid number between %.2f and %.2f.\n", min, max);
    }
}

static void readDate(const char *prompt, char *buf) {
    char input[64];

    while (1) {
        if (!readText(prompt, input, sizeof(input))) continue;
        if (validDate(input)) {
            strcpy(buf, input);
            return;
        }
        printf("Error: Use YYYY-MM-DD with a valid date.\n");
    }
}

static int cmpProfit(const void *a, const void *b) {
    const Car *x = (const Car *)a, *y = (const Car *)b;
    double diff = (x->sellPrice - x->buyPrice) - (y->sellPrice - y->buyPrice);
    return (diff > 0) ? -1 : (diff < 0) ? 1 : 0;
}

static int cmpTime(const void *a, const void *b) {
    const Car *x = (const Car *)a, *y = (const Car *)b;
    long d1 = toDays(x->sellDate) - toDays(x->buyDate);
    long d2 = toDays(y->sellDate) - toDays(y->buyDate);
    return (d1 > d2) ? -1 : (d1 < d2) ? 1 : 0;
}

static void printHeader(void) {
    puts("Make       Model        Mileage  Purchase Date  Purchase Price  Selling Date  Selling Price  Profit");
}

static void printCar(const Car *c) {
    printf("%-10s %-12s %-8d %-14s %-15.2f %-14s %-14.2f %.2f\n",
           c->make, c->model, c->mileage, c->buyDate, c->buyPrice, c->sellDate, c->sellPrice, c->sellPrice - c->buyPrice);
}

static double totalProfit(const Car *cars, int count) {
    double t = 0.0; int i;
    for (i = 0; i < count; i++) t += cars[i].sellPrice - cars[i].buyPrice;
    return t;
}

static int loadCars(Car *cars) {
    FILE *f = fopen(FILENAME, "r");
    char line[256]; int count = 0;
    if (!f) return 0;
    while (fgets(line, sizeof(line), f)) {
        Car c;
        if (sscanf(line, "%31[^,],%31[^,],%d,%10[^,],%lf,%10[^,],%lf",
                   c.make, c.model, &c.mileage, c.buyDate, &c.buyPrice, c.sellDate, &c.sellPrice) == 7 &&
            validDate(c.buyDate) && validDate(c.sellDate)) {
            cars[count++] = c;
            if (count >= MAX_CARS) break;
        }
    }
    fclose(f);
    return count;
}

static void saveCar(const Car *c) {
    FILE *f = fopen(FILENAME, "a");
    if (!f) { puts("Error: Could not open file for writing."); return; }
    fprintf(f, "%s,%s,%d,%s,%.2f,%s,%.2f\n", c->make, c->model, c->mileage, c->buyDate, c->buyPrice, c->sellDate, c->sellPrice);
    fclose(f);
}

static void printReport(Car *cars, int count, int mode) {
    if (!count) { puts("No cars loaded."); return; }
    qsort(cars, count, sizeof(Car), mode ? cmpTime : cmpProfit);
    printHeader();
    for (int i = 0; i < count; i++) printCar(&cars[i]);
    printf("Total profit made: %.2f\n", totalProfit(cars, count));
}

static void addCar(Car *cars, int *count) {
    Car c;
    if (*count >= MAX_CARS) { puts("Error: database full."); return; }
    if (!readText("Make: ", c.make, MAX_LEN)) return;
    if (!readText("Model: ", c.model, MAX_LEN)) return;
    c.mileage = readIntValue("Mileage: ", 0, 1000000);
    readDate("Purchase date (YYYY-MM-DD): ", c.buyDate);
    c.buyPrice = readDoubleValue("Purchase price: ", 0.0, 1000000000.0);
    readDate("Selling date (YYYY-MM-DD): ", c.sellDate);
    c.sellPrice = readDoubleValue("Selling price: ", 0.0, 1000000000.0);
    if (toDays(c.sellDate) < toDays(c.buyDate)) { puts("Error: selling date must be after purchase date."); return; }
    cars[(*count)++] = c;
    saveCar(&c);
    puts("Car added successfully.");
}

int main(void) {
    Car cars[MAX_CARS];
    int count = 0, choice;
    char line[64];
    count = loadCars(cars);
    puts("Welcome to the Car Sales Manager.");
    puts("The program stores data in cars.csv.");
    while (1) {
        printf("\n1. Add a car\n2. Print report by profit\n3. Print report by sale time\n4. Exit\nChoose an option: ");
        if (!fgets(line, sizeof(line), stdin)) { puts("Input ended. Exiting."); return 0; }
        line[strcspn(line, "\r\n")] = '\0';
        trim(line);
        if (!line[0]) { puts("Error: empty choice."); continue; }
        if (!parseInt(line, 1, 4, &choice)) {
            puts("Error: choose 1-4.");
            continue;
        }
        switch (choice) {
            case 1: addCar(cars, &count); break;
            case 2: printReport(cars, count, 0); break;
            case 3: printReport(cars, count, 1); break;
            case 4: puts("Goodbye."); return 0;
            default: puts("Error: choose 1-4."); break;
        }
    }
}
