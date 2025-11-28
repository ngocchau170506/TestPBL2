#pragma once
#include "Person.h"
#include "BorrowedItem.h"
#include "Utils.h"
#include "UserManager.h" 
#include "User.h"
#include "Book.h"
#include "BookManager.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_BORROWED_BOOKS 5

class User : public Person
{
private:
    vector<BorrowedItem> transactionHistory; 
    
    void LoadTransactionHistory();
    void SaveTransactionHistory() const;
    int FindActiveBorrowIndex(int bookID) const;
    
public:
    User();
    User(int id, const char n[], const char d[], const char p[], const char e[], const char pw[]);
    virtual ~User();

    int getCurrentBorrowedCount() const; 
    
    void Show() const override;
    void LoadUserByID(const string &id);
    bool AskReturnToMenu();
    
    bool ReturnBook(int bookID);
    bool BorrowBook(int bookID);
    const vector<BorrowedItem>& getTransactionHistory() const {
        return transactionHistory;
    }
    
    void ShowTransactionHistory(BookManager& bookManager);
    void Menu(UserManager &manager, BookManager &bm);
};

const int MAX_STREAM_SIZE_MANUAL = 100;

int User::getCurrentBorrowedCount() const {
    int count = 0;
    for (const auto& item : transactionHistory) {
        if (!item.getIsReturned()) {
            count++;
        }
    }
    return count;
}

int User::FindActiveBorrowIndex(int bookID) const
{
    for (size_t i = 0; i < transactionHistory.size(); ++i)
    {
        if (transactionHistory[i].getBookID() == bookID && !transactionHistory[i].getIsReturned())
            return i;
    }
    return -1;
}

void User::SaveTransactionHistory() const
{
    if (getID() == 0) return;

    string filename = "user_" + to_string(getID()) + "_history.txt";
    ofstream outFile(filename);

    if (!outFile.is_open())
    {
        cerr << "Loi: Khong the mo tep de luu lich su giao dich: " << filename << endl;
        return;
    }

    outFile << transactionHistory.size() << "\n";

    for (const auto& item : transactionHistory)
    {
        outFile << item.getBookID() << ","
                << item.getBorrowDate() << ","
                << item.getReturnDate() << ","
                << item.getIsReturned() << "\n";
    }

    outFile.close();
}

void User::LoadTransactionHistory()
{
    if (getID() == 0) {
        transactionHistory.clear();
        return;
    }

    string filename = "user_" + to_string(getID()) + "_history.txt";
    ifstream inFile(filename);

    if (!inFile.is_open())
    {
        transactionHistory.clear();
        return;
    }

    string line;
    int totalTransactions = 0;
    
    if (getline(inFile, line))
    {
        try {
            totalTransactions = stoi(line);
        } catch (...) {
            totalTransactions = 0;
        }
    }
    
    transactionHistory.clear();
    int loadedCount = 0;
    
    while (getline(inFile, line) && loadedCount < totalTransactions)
    {
        stringstream ss(line);
        string bookID_str, borrowDate_str, returnDate_str, isReturned_str;

        if (getline(ss, bookID_str, ',') && 
            getline(ss, borrowDate_str, ',') && 
            getline(ss, returnDate_str, ',') && 
            getline(ss, isReturned_str))
        {
            try
            {
                int bookID = stoi(bookID_str);
                bool isReturned = (stoi(isReturned_str) != 0);

                transactionHistory.emplace_back(
                    bookID, 
                    borrowDate_str, 
                    returnDate_str, 
                    isReturned
                );
                loadedCount++;
            }
            catch (...)
            {
                // Bo qua dong bi loi
            }
        }
    }

    inFile.close();
}

User::User() : Person()
{
    transactionHistory = vector<BorrowedItem>();
}

User::User(int id, const char n[], const char d[], const char p[], const char e[], const char pw[])
    : Person(id, n, d, p, e, pw)
{
    transactionHistory = vector<BorrowedItem>();
    LoadTransactionHistory(); 
}

User::~User()
{
    SaveTransactionHistory();
}

bool User::BorrowBook(int bookID)
{
    int currentCount = getCurrentBorrowedCount();
    int activeIndex = FindActiveBorrowIndex(bookID);
    
    if (currentCount >= MAX_BORROWED_BOOKS)
    {
        cout << "Loi: Nguoi dung da muon toi da " << MAX_BORROWED_BOOKS << " cuon sach.\n";
        return false;
    }
    if (activeIndex != -1)
    {
        cout << "Loi: Nguoi dung da co cuon sach ID " << bookID << " nay trong danh sach dang muon.\n";
        return false;
    }
    
    char dateBuffer[20];
    Utils::GetCurrentDate(dateBuffer, sizeof(dateBuffer));
    
    transactionHistory.emplace_back(bookID, dateBuffer);
    SaveTransactionHistory();
    
    return true;
}

bool User::ReturnBook(int bookID)
{
    int index = FindActiveBorrowIndex(bookID);
    if (index == -1)
    {
        cout << "Loi: Nguoi dung khong muon cuon sach ID " << bookID << " nay.\n";
        return false;
    }
    
    char dateBuffer[20];
    Utils::GetCurrentDate(dateBuffer, sizeof(dateBuffer));
    
    transactionHistory[index].MarkAsReturned(dateBuffer);
    SaveTransactionHistory();
    return true;
}

bool User::AskReturnToMenu()
{
    char choiceInput[10];
    cout << "\nBan co muon quay lai MENU khong? (Y/N): ";
    
    if (cin.getline(choiceInput, sizeof(choiceInput)).fail()) {
        cin.clear();
        cin.ignore(MAX_STREAM_SIZE_MANUAL, '\n');
        return false; 
    }
    return (choiceInput[0] == 'Y' || choiceInput[0] == 'y');
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

            LoadTransactionHistory();
            file.close();
            return;
        }
    }
    file.close();
}

void User::ShowTransactionHistory(BookManager& bookManager)
{
    cout << "\n--- LICH SU GIAO DICH CUA: " << this->getName() << " ---\n";

    if (transactionHistory.empty()) 
    {
        cout << "Hien tai nguoi dung nay chua co giao dich nao.\n";
        return;
    }

    const int DEFAULT_BORROW_DAYS = 14; 
    char currentDateBuffer[20];
    Utils::GetCurrentDate(currentDateBuffer, sizeof(currentDateBuffer));
    tm current_tm = Utils::ParseDate(currentDateBuffer);

    cout << left << setw(10) << "ID Sach"
         << left << setw(35) << "Ten Sach"
         << left << setw(15) << "Ngay muon"
         << left << setw(15) << "Ngay tra"
         << left << setw(15) << "Han tra"
         << left << setw(15) << "Trang thai"
         << endl;
    cout << setfill('-') << setw(105) << "-" << setfill(' ') << endl; 

    for (const auto& item : transactionHistory)
    {
        int bookID = item.getBookID();
        string borrowDateStr = item.getBorrowDate();
        string returnDateStr = item.getReturnDate();
        Book* book = bookManager.GetBookByID(bookID);

        cout << left << setw(10) << bookID;

        string bookTitle = (book) ? book->getTitle() : "Sach da bi xoa khoi he thong";
        cout << left << setw(35) << bookTitle;
        cout << left << setw(15) << borrowDateStr;
        cout << left << setw(15) << (item.getIsReturned() ? returnDateStr : "--");

        tm borrow_tm = Utils::ParseDate(borrowDateStr);
        tm dueDate_tm = Utils::AddDays(borrow_tm, DEFAULT_BORROW_DAYS);
        string dueDateStr = Utils::FormatDate(dueDate_tm);
        cout << left << setw(15) << dueDateStr;

        string status;
        if (item.getIsReturned()) {
            status = "Da tra";
            tm return_tm = Utils::ParseDate(returnDateStr);
            if (Utils::CompareDates(return_tm, dueDate_tm) > 0) {
                 status += " (Tre)";
            }
        } else {
            status = "Dang muon";
            if (Utils::CompareDates(current_tm, dueDate_tm) > 0) {
                status = "QUA HAN";
            }
        }
        cout << left << setw(15) << status;
        cout << endl;
    }
    cout << setfill('-') << setw(105) << "-" << setfill(' ') << endl;
}

void User::Show() const
{
    cout << "\n===== THONG TIN CA NHAN DOC GIA =====\n";
    Person::Show();
    cout << "So sach dang muon: " << getCurrentBorrowedCount() << " / " << MAX_BORROWED_BOOKS << "\n"; 
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
        cout << "3. Xem lich su giao dich (Muon & Tra)\n"; 
        cout << "4. Xem thong tin ca nhan\n";
        cout << "5. Thay doi thong tin ca nhan\n";
        cout << "6. Doi mat khau\n";
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
            ShowTransactionHistory(bm); 
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
                cin.ignore(MAX_STREAM_SIZE_MANUAL, '\n');
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
        case 6:
            ChangePassword();
            AskReturnToMenu();
            break;
        case 0:
            cout << "Dang xuat thanh cong. Tam biet!\n";
            break;
        default:
            cout << "Lua chon khong hop le! Vui long chon lai.\n";
            break;
        }

    } while (choice != 0);
}