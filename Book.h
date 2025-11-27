#pragma once
#include <iostream>
#include "Person.h"
using namespace std;

class Book 
{
private:
    int bookID;
    char title[100];
    char author[100];
    char category[100];
    int pubYear;
    int quantity;
    bool isBorrowed;
    int year;
    int totalQuantity;

public:

    Book();
    Book(int id, const char* t, const char* a, const char* c, int y, int q, bool b);
    ~Book();

    int getID() const;
    const char* getTitle() const;
    const char* getAuthor() const;
    const char* getCategory() const;
    int getPubYear() const;
    int getQuantity() const;
    bool getBorrowed() const;
    int getTotalQuantity() const { return totalQuantity; }

    int getYear() const;   // Chỉ khai báo
    void setYear(int y);

    void setTotalQuantity(int total) { totalQuantity = total; }
    void setID(int id);
    void setTitle(const char* t);
    void setAuthor(const char* a);
    void setCategory(const char* c);
    void setPubYear(int y);
    void setQuantity(int q);
    void setBorrowed(bool b);

    void Show() const;
    void InputBook();
    void WriteToFile(ostream& out) const;
    void ReadToFile(istream& in);
};

int Book::getYear() const
{
    return year;
}
// Book.h:57:6:
void Book::setYear(int y)
{
    year = y;
}

Book::Book()
{
    bookID = 0;
    title[0] = author[0] = category[0] = '\0';
    pubYear = 0;
    quantity = 0;
    isBorrowed = false;
}

Book::Book(int id, const char* t, const char* a, const char* c, int y, int q, bool b)
{
    bookID = id;

    int i = 0;
    while (t[i] != '\0' && i < 99) { title[i] = t[i]; i++; }
    title[i] = '\0';

    i = 0;
    while (a[i] != '\0' && i < 99) { author[i] = a[i]; i++; }
    author[i] = '\0';

    i = 0;
    while (c[i] != '\0' && i < 99) { category[i] = c[i]; i++; }
    category[i] = '\0';

    pubYear = y;
    quantity = q;
    isBorrowed = b;
}

Book::~Book() {}

int Book::getID() const { return bookID; }
const char* Book::getTitle() const { return title; }
const char* Book::getAuthor() const { return author; }
const char* Book::getCategory() const { return category; }
int Book::getPubYear() const { return pubYear; }
int Book::getQuantity() const { return quantity; }
bool Book::getBorrowed() const { return isBorrowed; }

void Book::setTitle(const char* t) {
    int i = 0;
    while (t[i] != '\0' && i < 99) { title[i] = t[i]; i++; }
    title[i] = '\0';
}

void Book::setID(int id) { bookID = id;}
void Book::setAuthor(const char* a) {
    int i = 0;
    while (a[i] != '\0' && i < 99) { author[i] = a[i]; i++; }
    author[i] = '\0';
}

void Book::setCategory(const char* c) {
    int i = 0;
    while (c[i] != '\0' && i < 99) { category[i] = c[i]; i++; }
    category[i] = '\0';
}

void Book::setPubYear(int y) { pubYear = y; }
void Book::setQuantity(int q) { quantity = q; }
void Book::setBorrowed(bool b) { isBorrowed = b; }

void Book::Show() const 
{
    cout << "ID: " << bookID << endl;
    cout << "Ten sach: " << title << endl;
    cout << "Tac gia: " << author << endl;
    cout << "The loai: " << category << endl;
    cout << "Nam xuat ban: " << pubYear << endl;
    cout << "So luong: " << quantity << endl;
    cout << "Trang thai: " << (isBorrowed ? "Da muon" : "Con trong") << endl;
    cout << "\n";
}

void Book::InputBook() 
{
    cout << "Nhap ten sach: ";
    cin.getline(title, 100);

    cout << "Nhap ten tac gia: ";
    cin.getline(author, 100);

    cout << "Nhap the loai: ";
    cin.getline(category, 100);

    cout << "Nhap nam xuat ban: ";
    cin >> pubYear;

    cout << "Nhap so luong sach: ";
    cin >> quantity;

    isBorrowed = false;
    cout << "Them sach thanh cong!\n";
}

void Book::WriteToFile(ostream& out) const 
{
    out << bookID << ',' 
        << title << ',' 
        << author << ',' 
        << category << ',' 
        << pubYear << ',' 
        << quantity << ',' 
        << isBorrowed << '\n';
}

void Book::ReadToFile(istream& in) 
{
    char c;
    char temp[100];
    int idx = 0;
    int field = 0;

    while (in.get(c)) {
        if (c == ',' || c == '\n') {
            temp[idx] = '\0';
            switch (field) {
            case 0: bookID = atoi(temp); break;
            case 1: setTitle(temp); break;
            case 2: setAuthor(temp); break;
            case 3: setCategory(temp); break;
            case 4: pubYear = atoi(temp); break;
            case 5: quantity = atoi(temp); break;
            case 6: isBorrowed = (temp[0] == '1'); break;
            }
            idx = 0;
            field++;
            if (c == '\n') break;
        } else {
            if (idx < 99) temp[idx++] = c;
        }
    }
}
