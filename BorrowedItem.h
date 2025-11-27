#pragma once
#include <string>
#include <cstring>
#include <iostream>

using namespace std;

class BorrowedItem
{
private:
    int bookID;
    char borrowDate[20];

public:
    char dateBorrowed[11];
    char dateDue[11];

    BorrowedItem() : bookID(0)
    {
        strcpy(borrowDate, "");
    }

    // Getter cho BookID
    int getBookID() const
    {
        return bookID;
    }

    // Setter cho BookID
    void setBookID(int id)
    {
        bookID = id;
    }


    const char *getBorrowDate() const
    {
        return borrowDate;
    }

    void setBorrowDate(const char date[])
    {
        strncpy(borrowDate, date, 19);
        borrowDate[19] = '\0'; 
    }

};