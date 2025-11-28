#pragma once
#include <string>
#include <cstring>
#include <iostream>
using namespace std;

class BorrowedItem
{
private:
    int bookID;
    string borrowDate;
    string returnDate;
    bool isReturned;

public:
    BorrowedItem()
        : bookID(0), borrowDate(""), returnDate(""), isReturned(false) {}

    BorrowedItem(int id, const string& bDate)
        : bookID(id), borrowDate(bDate), returnDate(""), isReturned(false) {}

    BorrowedItem(int id, const string& bDate, const string& rDate, bool returned)
        : bookID(id), borrowDate(bDate), returnDate(rDate), isReturned(returned) {}

    // FIXED: Trả về string thay vì const char*
    int getBookID() const { return bookID; }
    string getBorrowDate() const { return borrowDate; }
    string getReturnDate() const { return returnDate; }
    bool getIsReturned() const { return isReturned; }

    void setBookID(int id) { this->bookID = id; }
    void setBorrowDate(const char* date) { this->borrowDate = date; }
    void setReturnDate(const char* date) { this->returnDate = date; }
    void setIsReturned(bool returned) { this->isReturned = returned; }

    void MarkAsReturned(const char* date) {
        setReturnDate(date);
        setIsReturned(true);
    }
};
