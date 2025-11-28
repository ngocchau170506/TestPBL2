#pragma once
#include <cstddef>

class Utils
{
public:
    static int StringToIntManual(const char* str);
    static int FindSubstringManual(const char* haystack, const char* needle);
    static void CopyStringManual(char* dest, const char* src, size_t dest_size);
    static int SplitLineManual(char* line, char fields[][100], int maxFields, char delimiter);
    static int CharArrayToIntManual(const char* str);
    static void IntToStringManual(int num, char* buffer, size_t buffer_size);
    static void GetCurrentDate(char* buffer, size_t buffer_size);
    static int CompareDates(const char* date1, const char* date2);
    static void GetCurrentDateManual(char* buffer, size_t buffer_size);
    static int CalculateDateDifference(const char* date1, const char* date2);
    static void ToUpperManual(char* str); 
    static void ToLowerManual(char* str); 
    static const int daysInMonth[13];
    static bool AskReturnToMenu();
    // Hàm mới: Phân tích cú pháp ngày "dd/mm/yyyy" thành tm
    static tm ParseDate(const string& dateStr);
    
    // Hàm mới: Định dạng tm thành chuỗi "dd/mm/yyyy"
    static string FormatDate(const tm& timeinfo);
    
    // Hàm mới: Cộng số ngày vào một ngày cho trước
    static tm AddDays(const tm& date, int days);
    
    // Hàm mới: So sánh hai ngày (trả về -1 nếu date1 < date2, 0 nếu bằng, 1 nếu date1 > date2)
    static int CompareDates(const tm& date1, const tm& date2);

    static bool IsLeap(int year)
    {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    static long long GetTotalDays(const char* date)
    {
        char temp[5]; 
        
        Utils::CopyStringManual(temp, date, 3); 
        int d = Utils::CharArrayToIntManual(temp);

        Utils::CopyStringManual(temp, date + 3, 3); 
        int m = Utils::CharArrayToIntManual(temp);

        Utils::CopyStringManual(temp, date + 6, 5); 
        int y = Utils::CharArrayToIntManual(temp);

        long long totalDays = 0;

        for (int i = 1; i < y; i++) {
            totalDays += 365;
            if (IsLeap(i)) {
                totalDays++;
            }
        }

        for (int i = 1; i < m; i++) {
            totalDays += daysInMonth[i];
        }
        
        if (m > 2 && IsLeap(y)) {
            totalDays++;
        }

        totalDays += d;
        
        return totalDays;
    }
};
bool Utils::AskReturnToMenu()
{
    char choice;
    cout << "\nBan co muon quay lai MENU khong? (Y/N): ";
    choice = cin.get(); 
    if (choice == '\n') {
        return true; 
    }
    cin.ignore(100, '\n'); 

    return (choice == 'Y' || choice == 'y');
}
bool Utils::AskReturnToMenu()
{
    char choice;
    cout << "\nBan co muon quay lai MENU khong? (Y/N): ";
    cin >> choice;
    cin.ignore(100, '\n');
    if (choice == 'Y' || choice == 'y')
        return true;
    return false;
}

void Utils::ToUpperManual(char* str)
{
    while (*str)
    {
        if (*str >= 'a' && *str <= 'z')
        {
            *str = *str - 32;
        }
        str++;
    }
}

void Utils::ToLowerManual(char* str)
{
    while (*str)
    {
        if (*str >= 'A' && *str <= 'Z')
        {
            *str = *str + 32;
        }
        str++;
    }
}

const int Utils::daysInMonth[13] = {
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

int Utils::CalculateDateDifference(const char* date1, const char* date2)
{
    long long days1 = GetTotalDays(date1);
    long long days2 = GetTotalDays(date2);
    return (int)(days1 - days2); 
}

void Utils::GetCurrentDateManual(char* buffer, size_t buffer_size)
{
    const char* defaultDate = "19/11/2025"; 
    CopyStringManual(buffer, defaultDate, buffer_size);
}

int Utils::CompareDates(const char* date1, const char* date2)
{
    int year1 = CharArrayToIntManual(date1 + 6);
    int year2 = CharArrayToIntManual(date2 + 6);
    if (year1 != year2) return year1 - year2;

    char monthStr1[3] = {date1[3], date1[4], '\0'};
    char monthStr2[3] = {date2[3], date2[4], '\0'};
    int month1 = CharArrayToIntManual(monthStr1);
    int month2 = CharArrayToIntManual(monthStr2);
    if (month1 != month2) return month1 - month2;

    char dayStr1[3] = {date1[0], date1[1], '\0'};
    char dayStr2[3] = {date2[0], date2[1], '\0'};
    int day1 = CharArrayToIntManual(dayStr1);
    int day2 = CharArrayToIntManual(dayStr2);
    return day1 - day2;
}

void Utils::IntToStringManual(int num, char* buffer, size_t buffer_size)
{
    if (buffer_size == 0) return;
    
    if (num == 0) {
        if (buffer_size > 1) {
            buffer[0] = '0';
            buffer[1] = '\0';
        } else if (buffer_size == 1) {
            buffer[0] = '\0';
        }
        return;
    }

    bool isNegative = false;
    if (num < 0) {
        isNegative = true;
        num = -num; 
    }

    int i = 0;
    while (num != 0 && i < buffer_size - 1) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (isNegative && i < buffer_size - 1) {
        buffer[i++] = '-';
    }
    buffer[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

int Utils::FindSubstringManual(const char* haystack, const char* needle)
{
    if (!*needle) return 0;

    while (*haystack)
    {
        const char* h = haystack;
        const char* n = needle;
        while (*n && *h == *n)
        {
            h++;
            n++;
        }
        if (!*n) return 1;
        haystack++;
    }
    return 0;
}

void Utils::CopyStringManual(char* dest, const char* src, size_t dest_size)
{
    size_t i = 0;
    while (i < dest_size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int Utils::SplitLineManual(char* line, char fields[][100], int maxFields, char delimiter)
{
    int fieldIndex = 0;
    int charIndex = 0;
    
    while (*line != '\0' && fieldIndex < maxFields)
    {
        if (*line == delimiter)
        {
            fields[fieldIndex][charIndex] = '\0';
            fieldIndex++;
            charIndex = 0;
        }
        else
        {
            if (charIndex < 99)
            {
                fields[fieldIndex][charIndex] = *line;
                charIndex++;
            }
        }
        line++;
    }
    fields[fieldIndex][charIndex] = '\0';
    return fieldIndex + 1;
}

int Utils::CharArrayToIntManual(const char* str)
{
    int res = 0;
    int sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    }
    while (*str != '\0') {
        if (*str >= '0' && *str <= '9') {
            res = res * 10 + (*str - '0');
        } else {
            break;
        }
        str++;
    }
    return res * sign;
}

void Utils::GetCurrentDate(char* buffer, size_t buffer_size)
{
    if (buffer_size < 11) {
        if (buffer_size > 0) buffer[0] = '\0';
        return;
    }

    time_t now = time(0);
    tm *ltm = localtime(&now);

    int written = sprintf(buffer, "%02d/%02d/%04d", 
                          ltm->tm_mday,
                          1 + ltm->tm_mon,
                          1900 + ltm->tm_year);

    if (written < buffer_size) {
        buffer[written] = '\0';
    } else {
        buffer[buffer_size - 1] = '\0';
    }
}

int Utils::StringToIntManual(const char* str)
{
    while (*str == ' ' || *str == '\t') {
        str++;
    }
    
    int res = 0;
    int sign = 1;

    if (*str == '-') {
        sign = -1;
        str++;
    }

    if (!(*str >= '0' && *str <= '9')) {
        return -1;
    }
    
    while (*str != '\0') {
        if (*str >= '0' && *str <= '9') {
            res = res * 10 + (*str - '0');
        } else if (*str == '\n' || *str == '\r') {
            break;
        } else {
            return -1; 
        }
        str++;
    }
    return res * sign;
}

tm Utils::ParseDate(const string& dateStr) {
    tm timeinfo = {};
    // Cố gắng phân tích cú pháp dd/mm/yyyy
    // Lưu ý: strptime không chuẩn trên Windows MSVC, có thể cần thay thế
    // Nếu bạn đang dùng Visual Studio, cần dùng sscanf_s hoặc parse thủ công
    #ifdef _WIN32
        int d, m, y;
        sscanf_s(dateStr.c_str(), "%d/%d/%d", &d, &m, &y);
        timeinfo.tm_mday = d;
        timeinfo.tm_mon = m - 1; // tm_mon is 0-11
        timeinfo.tm_year = y - 1900; // tm_year is years since 1900
    #else
        strptime(dateStr.c_str(), "%d/%m/%Y", &timeinfo);
    #endif
    mktime(&timeinfo); // Chuẩn hóa tm struct (ví dụ: đặt tm_wday, tm_yday)
    return timeinfo;
}

string Utils::FormatDate(const tm& timeinfo) {
    char buffer[11]; // dd/mm/yyyy\0
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", &timeinfo);
    return std::string(buffer);
}

tm Utils::AddDays(const tm& date, int days) {
    tm newDate = date;
    newDate.tm_mday += days;
    mktime(&newDate); // Chuẩn hóa lại sau khi thêm ngày
    return newDate;
}

int Utils::CompareDates(const tm& date1, const tm& date2) {
    time_t t1 = mktime(const_cast<tm*>(&date1));
    time_t t2 = mktime(const_cast<tm*>(&date2));
    if (t1 < t2) return -1;
    if (t1 > t2) return 1;
    return 0; // Equal
}
