#pragma once
#include "Person.h"
#include "BorrowedItem.h"
#include "Utils.h"
#include "UserManager.h" 
#include <iostream>
#include <fstream>
#include <iomanip>  
#include <vector>   

using namespace std;

#define MAX_BORROWED_BOOKS 5

class User : public Person
{
private:
    BorrowedItem borrowedBooks[MAX_BORROWED_BOOKS];
    int currentBorrowedCount;

    void LoadBorrowedBooks();
    void SaveBorrowedBooks() const;
    int FindBorrowedBookIndex(int bookID) const;
    vector<int> borrowedBookIDs;

public:
    User();
    User(int id, const char n[], const char d[], const char p[], const char e[], const char pw[]);
    virtual ~User();

    int getCurrentBorrowedCount() const { return currentBorrowedCount; }
    const BorrowedItem *getBorrowedBooks() const { return borrowedBooks; }

    void Show() const override;
    void LoadUserByID(const string &id);
    bool AskReturnToMenu();
    bool ReturnBook(int bookID);
    bool BorrowBook(int bookID);
    void ShowBorrowedBooks(BookManager& bookManager);

    void Menu(UserManager &manager, BookManager &bm);
};

const int MAX_STREAM_SIZE_MANUAL = 100;

User::User() : Person()
{
    this->currentBorrowedCount = 0;
}

User::User(int id, const char n[], const char d[], const char p[], const char e[], const char pw[])
    : Person(id, n, d, p, e, pw)
{
    currentBorrowedCount = 0;
    LoadBorrowedBooks();
}

User::~User()
{
    SaveBorrowedBooks();
}

int User::FindBorrowedBookIndex(int bookID) const
{
    for (int i = 0; i < currentBorrowedCount; ++i)
    {
        if (borrowedBooks[i].getBookID() == bookID)
            return i;
    }
    return -1;
}

void User::SaveBorrowedBooks() const
{
    if (getID() == 0)
        return;

    string filename = "borrows.txt";
    ofstream outFile(filename);

    if (!outFile.is_open())
    {
        cerr << "Loi: Khong the mo tep de luu thong tin muon sach: " << filename << endl;
        return;
    }

    outFile << currentBorrowedCount << "\n";

    for (int i = 0; i < currentBorrowedCount; ++i)
    {
        outFile << borrowedBooks[i].getBookID() << "," << borrowedBooks[i].getBorrowDate();
        if (i < currentBorrowedCount - 1)
        {
            outFile << "\n";
        }
    }

    outFile.close();
}

void User::LoadBorrowedBooks()
{
    if (getID() == 0)
        return;

    string filename = "borrows.txt";
    ifstream inFile(filename);

    if (!inFile.is_open())
    {
        currentBorrowedCount = 0;
        return;
    }

    string line;
    if (getline(inFile, line))
    {
        try
        {
            currentBorrowedCount = stoi(line);
        }
        catch (...)
        {
            currentBorrowedCount = 0;
        }
    }
    else
    {
        currentBorrowedCount = 0;
        inFile.close();
        return;
    }

    int i = 0;
    while (getline(inFile, line) && i < MAX_BORROWED_BOOKS)
    {
        stringstream ss(line);
        string bookID_str, borrowDate_str;

        if (getline(ss, bookID_str, ',') && getline(ss, borrowDate_str))
        {
            try
            {
                borrowedBooks[i].setBookID(stoi(bookID_str));
                borrowedBooks[i].setBorrowDate(borrowDate_str.c_str());
                i++;
            }
            catch (...)
            {
            }
        }
    }

    currentBorrowedCount = i;
    inFile.close();
}

bool User::BorrowBook(int bookID)
{
    if (currentBorrowedCount >= MAX_BORROWED_BOOKS)
    {
        cout << "Loi: Nguoi dung da muon toi da " << MAX_BORROWED_BOOKS << " cuon sach.\n";
        return false;
    }
    if (FindBorrowedBookIndex(bookID) != -1)
    {
        cout << "Loi: Nguoi dung da co cuon sach ID " << bookID << " nay trong danh sach muon.\n";
        return false;
    }
    char dateBuffer[20];
    Utils::GetCurrentDate(dateBuffer, sizeof(dateBuffer));
    borrowedBooks[currentBorrowedCount].setBookID(bookID);
    borrowedBooks[currentBorrowedCount].setBorrowDate(dateBuffer);
    currentBorrowedCount++;

    SaveBorrowedBooks();
    return true;
}

bool User::ReturnBook(int bookID)
{
    int index = FindBorrowedBookIndex(bookID);
    if (index == -1)
    {
        cout << "Loi: Nguoi dung khong muon cuon sach ID " << bookID << " nay.\n";
        return false;
    }

    for (int i = index; i < currentBorrowedCount - 1; ++i)
    {
        borrowedBooks[i] = borrowedBooks[i + 1];
    }

    currentBorrowedCount--;
    SaveBorrowedBooks();
    return true;
}

bool User::AskReturnToMenu()
{
    char choice;
    cout << "\nBan co muon quay lai MENU khong? (Y/N): ";
    cin >> choice;
    cin.ignore(MAX_STREAM_SIZE_MANUAL, '\n');
    return (choice == 'Y' || choice == 'y');
}

void User::LoadUserByID(const string &id)
{
    ifstream file("users.txt");
    if (!file.is_open())
    {
        cout << "Khong the mo file users.txt\n";
        return;
    }

    string line;
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);

        string userID, password_str, name_str, dob_str, phone_str, email_str;

        if (!getline(ss, userID, ',') || !getline(ss, password_str, ',') ||
            !getline(ss, name_str, ',') || !getline(ss, dob_str, ',') ||
            !getline(ss, phone_str, ',') || !getline(ss, email_str, ','))
            continue;

        if (userID == id)
        {
            setID(stoi(userID));
            setPassword(password_str.c_str());
            setName(name_str.c_str());
            setDate(dob_str.c_str());
            setPhone(phone_str.c_str());
            setEmail(email_str.c_str());

            LoadBorrowedBooks();

            file.close();
            return;
        }
    }
    file.close();
}

// void User::ShowBorrowedBooks(BookManager& bookManager)
// {
//     if (currentBorrowedCount == 0)
//     {
//         cout << "Ban hien khong muon cuon sach nao.\n";
//         return;
//     }

//     cout << "\n===== DANH SACH SACH DANG MUON (" << currentBorrowedCount << " cuon) =====\n";

//     for (int i = 0; i < currentBorrowedCount; ++i)
//     {
//         int bookID = borrowedBooks[i].getBookID();
//         const Book *bookPtr = bm.GetBookByID(bookID);

//         cout << "Sach thu " << i + 1 << " - ID: " << bookID << "\n";
//         cout << "   Ngay muon: " << borrowedBooks[i].getBorrowDate() << "\n";

//         if (bookPtr)
//         {
//             bookPtr->Show();
//         }
//         else
//         {
//             cout << "Loi: Khong tim thay chi tiet sach ID: " << bookID << " trong he thong.\n";
//         }
//     }
// }
// Trong file User.cpp

void User::ShowBorrowedBooks(BookManager& bookManager)
{
    cout << "\n--- DANH SACH SACH DANG MUON CUA: " << this->getName() << " ---\n";

    if (currentBorrowedCount == 0) 
    {
        cout << "Hien tai nguoi dung nay khong muon cuon sach nao.\n";
        return;
    }

    cout << left << setw(10) << "ID Sach" 
         << left << setw(40) << "Ten Sach" 
         << left << setw(20) << "Ngay muon" << endl;
    cout << setfill('-') << setw(70) << "-" << setfill(' ') << endl;

    for (int i = 0; i < currentBorrowedCount; i++)
    {
        int bookID = borrowedBooks[i].getBookID(); 
        string borrowDate = borrowedBooks[i].getBorrowDate();
        Book* book = bookManager.GetBookByID(bookID);

        cout << left << setw(10) << bookID;

        if (book) {
            cout << left << setw(40) << book->getTitle();
            cout << left << setw(20) << borrowDate; 
        }
        else {
            cout << left << setw(40) << "Sach da bi xoa khoi he thong";
            cout << left << setw(20) << borrowDate;
        }
        cout << endl;
    }
    cout << setfill('-') << setw(70) << "-" << setfill(' ') << endl;
}

void User::Show() const
{
    cout << "\n===== THONG TIN CA NHAN DOC GIA =====\n";
    Person::Show();
    cout << "So sach dang muon: " << currentBorrowedCount << " / " << MAX_BORROWED_BOOKS << "\n";
}

void User::Menu(UserManager &manager, BookManager &bm)
{
    int choice;
    char input[10];

    do
    {
        cout << "\n========== MENU DOC GIA (" << getName() << ") ==========\n";
        cout << "1. Tim kiem sach\n";
        cout << "2. Xem tat ca sach\n";
        cout << "3. Xem danh sach sach da muon\n";
        cout << "4. Xem thong tin ca nhan\n";
        cout << "5. Thay doi thong tin ca nhan\n";
        cout << "0. Dang xuat\n";
        cout << "Chon: ";

        if (cin.getline(input, sizeof(input)).fail())
        {
            cin.clear();
            cin.ignore(MAX_STREAM_SIZE_MANUAL, '\n');
            choice = -1;
        }
        else
        {
            choice = Utils::StringToIntManual(input);
            if (choice == 0 && input[0] != '0' && input[0] != '\0')
            {
                choice = -1;
            }
        }

        switch (choice)
        {
        case 1:
        {
            int subChoice;
            char subInput[10];
            do
            {
                cout << "\n--- MENU TIM KIEM SACH ---\n";
                cout << "1. Theo ten\n";
                cout << "2. Theo tac gia\n";
                cout << "3. Theo ID\n";
                cout << "0. Quay lai\n";
                cout << "Chon: ";
                cin.getline(subInput, sizeof(subInput));
                subChoice = Utils::StringToIntManual(subInput);

                if (subChoice == 1)
                    bm.SearchBookByTitle();
                else if (subChoice == 2)
                {
                    char author[100];
                    cout << "Nhap ten tac gia: ";
                    cin.getline(author, sizeof(author));
                    bm.SearchBookByAuthor(author);
                }
                else if (subChoice == 3)
                {
                    char idStr[20];
                    int bookID;
                    cout << "Nhap ID sach: ";
                    cin.getline(idStr, sizeof(idStr));
                    bookID = Utils::CharArrayToIntManual(idStr);
                    const Book *bookPtr = bm.GetBookByID(bookID);
                    if (bookPtr)
                        bookPtr->Show();
                    else
                        cout << "Khong tim thay sach co ID " << bookID << "\n";
                }
                else if (subChoice == 0)
                    break;
                else
                    cout << "Lua chon khong hop le!\n";

            } while (subChoice != 0);

            AskReturnToMenu();
            break;
        }
        case 2:
            bm.ShowAllBooks();
            AskReturnToMenu();
            break;
        case 3:
            ShowBorrowedBooks(bm);
            AskReturnToMenu();
            break;
        case 4:
            Show();
            AskReturnToMenu();
            break;
        case 5:
        {
            int id;
            cout << "Nhap lai ID de xac nhan sua thong tin: ";
            if (!(cin >> id))
            {
                cin.clear();
                id = -1;
            }
            cin.ignore(MAX_STREAM_SIZE_MANUAL, '\n');
            if (id == getID())
            {
                manager.UpdateByID(id);
                cout << "Cap nhat hoan tat! Vui long dang nhap lai.\n";
            }
            else
            {
                cout << "ID xac nhan khong khop.\n";
            }
            AskReturnToMenu();
            break;
        }
        case 0:
            cout << "Dang xuat thanh cong. Tam biet!\n";
            break;
        default:
            cout << "Lua chon khong hop le! Vui long chon lai.\n";
            break;
        }

    } while (choice != 0);
}
