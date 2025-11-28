#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <limits>
#include "BorrowManager.h"
#include "UserManager.h"
#include "BookManager.h"
#include "Utils.h"
#include "User.h"
#include "Book.h"

class UserManager;
class BookManager;

using namespace std;

struct BorrowRecordData
{
    int userID;
    string userName;
    int bookID;
    string bookTitle;
    string borrowDate;
    string timeStamp;
};

class BorrowManager
{
private:
    void LogBorrowRecord(int userID, const string &userName,
                         int bookID, const string &bookTitle,
                         const char *borrowDate);
    
    void LogReturnRecord(int userID, const string &userName,
                         int bookID, const string &bookTitle,
                         const string &returnDate);
    
public:
    void HandleBorrowBook(UserManager& um, BookManager& bm);
    void HandleReturnBook(UserManager& um, BookManager& bm);
        
    void ShowBorrowRecords();
    void ShowRawBorrowHistory(const string& filename, const string& title);
    
    void ShowHistoryBorrow() {
        ShowRawBorrowHistory("BorrowRecords.txt", "THONG KE DANH SACH MUON SACH (RAW)");
    }
    void ShowHistoryReturn() {
        ShowRawBorrowHistory("ReturnRecords.txt", "THONG KE DANH SACH TRA SACH (RAW)");
    }
    void ShowAllUsersTransactionHistory(UserManager& userManager, BookManager& bookManager);
    
};

void BorrowManager::ShowAllUsersTransactionHistory(UserManager& userManager, BookManager& bookManager) {
    cout << "\n=== LICH SU GIAO DICH CUA TAT CA DOC GIA ===\n";

    const Person* allUsers = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();
    
    if (userCount == 0) {
        cout << "Khong co doc gia nao trong he thong.\n";
        return;
    }

    const int DEFAULT_BORROW_DAYS = 14;
    char currentDateBuffer[20];
    Utils::GetCurrentDate(currentDateBuffer, sizeof(currentDateBuffer));
    tm current_tm = Utils::ParseDate(currentDateBuffer);

    int totalTransactions = 0;
    int activeBorrows = 0;
    int overdueBorrows = 0;

    cout << left << setw(8) << "ID Muon"
         << left << setw(10) << "ID User"
         << left << setw(20) << "Ten User"
         << left << setw(10) << "ID Sach"
         << left << setw(35) << "Ten Sach"
         << left << setw(12) << "Ngay muon"
         << left << setw(12) << "Ngay tra"
         << left << setw(12) << "Han tra"
         << left << setw(15) << "Trang thai"
         << endl;
    cout << setfill('-') << setw(135) << "-" << setfill(' ') << endl;

    int borrowID = 1;

    for (int i = 0; i < userCount; i++) {
        const Person& person = allUsers[i];
        
        User user;
        user.LoadUserByID(to_string(person.getID()));
        
        const vector<BorrowedItem>& history = user.getTransactionHistory();
        
        if (history.empty()) {
            continue;
        }

        for (const auto& item : history) {
            int bookID = item.getBookID();
            string borrowDateStr = item.getBorrowDate();
            string returnDateStr = item.getReturnDate();
            Book* book = bookManager.GetBookByID(bookID);

            cout << left << setw(8) << borrowID++;

            cout << left << setw(10) << person.getID()
                 << left << setw(20) << person.getName();

            cout << left << setw(10) << bookID;

            string bookTitle = (book) ? book->getTitle() : "Sach da bi xoa";
            if (bookTitle.length() > 34) {
                bookTitle = bookTitle.substr(0, 31) + "...";
            }
            cout << left << setw(35) << bookTitle;
            
            cout << left << setw(12) << borrowDateStr;
            cout << left << setw(12) << (item.getIsReturned() ? returnDateStr : "--");

            tm borrow_tm = Utils::ParseDate(borrowDateStr);
            tm dueDate_tm = Utils::AddDays(borrow_tm, DEFAULT_BORROW_DAYS);
            string dueDateStr = Utils::FormatDate(dueDate_tm);
            cout << left << setw(12) << dueDateStr;

            string status;
            if (item.getIsReturned()) {
                status = "Da tra";
                tm return_tm = Utils::ParseDate(returnDateStr);
                if (Utils::CompareDates(return_tm, dueDate_tm) > 0) {
                    status += " (Tre)";
                    overdueBorrows++;
                }
            } else {
                status = "Dang muon";
                activeBorrows++;
                if (Utils::CompareDates(current_tm, dueDate_tm) > 0) {
                    status = "QUA HAN";
                    overdueBorrows++;
                }
            }
            cout << left << setw(15) << status;
            cout << endl;

            totalTransactions++;
        }
    }

    cout << setfill('-') << setw(135) << "-" << setfill(' ') << endl;

    cout << "\n=== TONG QUAN HE THONG ===\n";
    cout << "Tong so doc gia: " << userCount << "\n";
    cout << "Tong so giao dich: " << totalTransactions << "\n";
    cout << "So sach dang muon: " << activeBorrows << "\n";
    cout << "So sach qua han: " << overdueBorrows << "\n";
}

void BorrowManager::LogBorrowRecord(int userID, const string &userName,
                                    int bookID, const string &bookTitle,
                                    const char *borrowDate)
{
    ofstream outFile("BorrowRecords.txt", ios::app);

    if (!outFile.is_open())
    {
        cout << "LOI: Khong the mo hoac tao file BorrowRecords.txt.\n";
        return;
    }

    time_t now = time(0);
    tm *localTime = localtime(&now);
    char timeBuffer[80];
    strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", localTime);

    outFile << "=== THONG TIN MUON SACH ===" << endl;
    outFile << "Thoi gian ghi nhan: " << timeBuffer << endl;
    outFile << "ID nguoi dung: " << userID << endl;
    outFile << "Ten nguoi dung: " << userName << endl;
    outFile << "ID sach: " << bookID << endl;
    outFile << "Ten sach: " << bookTitle << endl;
    outFile << "Ngay muon: " << borrowDate << endl;
    outFile << "----------------------------------------" << endl;
    outFile << endl;

    outFile.close();

    cout << "Da ghi thong tin muon sach vao BorrowRecords.txt.\n";
}

void BorrowManager::LogReturnRecord(int userID, const string &userName,
                                    int bookID, const string &bookTitle,
                                    const string &returnDate)
{
    const string fileName = "ReturnRecords.txt";

    ofstream outFile(fileName, ios::app);

    if (!outFile.is_open())
    {
        cout << "Loi: Khong the mo file " << fileName << " de ghi thong tin tra sach!\n";
        return;
    }

    time_t now = time(0);
    tm *localTime = localtime(&now);
    char timeBuffer[80];
    strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", localTime);

    outFile << "=== THONG TIN TRA SACH ===" << endl;
    outFile << "Thoi gian ghi nhan: " << timeBuffer << endl;
    outFile << "ID nguoi dung: " << userID << endl;
    outFile << "Ten nguoi dung: " << userName << endl;
    outFile << "ID sach: " << bookID << endl;
    outFile << "Ten sach: " << bookTitle << endl;
    outFile << "Ngay tra: " << returnDate << endl;
    outFile << "----------------------------------------" << endl;
    outFile << endl;

    outFile.close();

    cout << "Da ghi thong tin tra sach vao file: " << fileName << "\n";
}

void BorrowManager::HandleBorrowBook(UserManager& um, BookManager& bm)
{
    cout << "\n========= XU LY MUON SACH =========\n";

    int userID = 0, bookID = 0;

    cout << "Nhap ID nguoi dung muon sach: ";
    if (!(cin >> userID))
    {
        cout << "Loi: ID nguoi dung khong hop le.\n";
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    cout << "Nhap ID sach can muon: ";
    if (!(cin >> bookID))
    {
        cout << "Loi: ID sach khong hop le.\n";
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    User user;
    user.LoadUserByID(to_string(userID));
    
    if (user.getID() == 0) {
        cout << "Loi: Khong tim thay nguoi dung voi ID: " << userID << "\n";
        return;
    }

    Book *book = bm.GetBookByID(bookID);
    if (!book)
    {
        cout << "Loi: Khong tim thay sach voi ID: " << bookID << "\n";
        return;
    }

    if (book->getQuantity() <= 0)
    {
        cout << "Loi: Sach '" << book->getTitle() << "' da het trong kho.\n";
        return;
    }

    if (user.BorrowBook(bookID))
    {
        book->setQuantity(book->getQuantity() - 1);
        bm.SaveBooksToFile();

        char borrowDate[20];
        Utils::GetCurrentDate(borrowDate, sizeof(borrowDate));

        string currentUserName = user.getName();
        string currentBookTitle = book->getTitle();

        LogBorrowRecord(
            userID,
            currentUserName,
            bookID,
            currentBookTitle,
            borrowDate);

        cout << "MUON SACH THANH CONG!\n";
        cout << "Sach: '" << book->getTitle() << "'\n";
        cout << "Ngay muon: " << borrowDate << "\n";
        cout << "So luong con lai: " << book->getQuantity() << "\n";
    }
    else
    {
        cout << "Muon sach that bai!\n";
    }
}

void BorrowManager::HandleReturnBook(UserManager& um, BookManager& bm)
{
    cout << "\n========= XU LY TRA SACH =========\n";
    int userID = 0, bookID = 0;
    char bufferID[20];

    cout << "Nhap ID nguoi dung tra sach: ";
    if (!(cin >> userID))
    {
        cout << "Nhap ID nguoi dung khong hop le.\n";
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    cout << "Nhap ID sach can tra: ";
    if (!(cin >> bookID))
    {
        cout << "Nhap ID sach khong hop le.\n";
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    char returnDate[20];
    Utils::GetCurrentDate(returnDate, sizeof(returnDate));
    string returnDateStr(returnDate);

    User tempUser;
    Utils::IntToStringManual(userID, bufferID, sizeof(bufferID));
    tempUser.LoadUserByID(bufferID);

    if (tempUser.getID() == 0)
    {
        cout << "Loi: Khong tim thay nguoi dung voi ID: " << userID << "\n";
        return;
    }

    Book *book = bm.GetBookByID(bookID);
    string userName = tempUser.getName();
    string bookTitle = book ? book->getTitle() : "Unknown Book";

    if (tempUser.ReturnBook(bookID))
    {
        if (book)
        {
            book->setQuantity(book->getQuantity() + 1);
            cout << "TRA SACH THANH CONG!\n";
            cout << "Sach: '" << book->getTitle() << "'\n";
            cout << "Ngay tra: " << returnDateStr << "\n";
            cout << "So luong hien tai: " << book->getQuantity() << "\n";

            bm.SaveBooksToFile();
            LogReturnRecord(userID, userName, bookID, bookTitle, returnDateStr);
        }
        else
        {
            cout << "TRA SACH THANH CONG.\n";
            cout << "Ngay tra: " << returnDateStr << "\n";
            LogReturnRecord(userID, userName, bookID, "Unknown Book (Deleted)", returnDateStr);
        }
    }
    else
    {
        cout << "Loi: Nguoi dung ID " << userID << " khong muon sach ID " << bookID << "\n";
    }
}

void BorrowManager::ShowRawBorrowHistory(const string& filename, const string& title)
{
    cout << "\n============================================\n";
    cout << "       " << title << "       \n";
    cout << "============================================\n";

    ifstream inFile(filename);

    if (!inFile.is_open())
    {
        cout << "Thong bao: Chua co du lieu (File " << filename << " chua ton tai).\n";
        return;
    }

    string line;
    bool isEmpty = true;

    while (getline(inFile, line))
    {
        cout << line << endl;
        isEmpty = false;
    }

    if (isEmpty) {
        cout << "Thong bao: File lich su hien dang trong.\n";
    }

    inFile.close();

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void BorrowManager::ShowBorrowRecords()
{
    cout << "\n======================================================================================================\n";
    cout << "                                  THONG KE DANH SACH MUON SACH                                          \n";
    cout << "======================================================================================================\n";

    ifstream inFile("BorrowRecords.txt");

    if (!inFile.is_open())
    {
        cout << "Thong bao: Chua co du lieu muon sach (File chua ton tai).\n";
        return;
    }

    vector<BorrowRecordData> records;
    string line;
    BorrowRecordData currentRecord;
    bool isNewRecord = false;

    while (getline(inFile, line))
    {
        if (line.find("=== THONG TIN MUON SACH ===") != string::npos)
        {
            if (isNewRecord)
            {
                records.push_back(currentRecord);
            }
            currentRecord = BorrowRecordData();
            isNewRecord = true;
        }
        else if (isNewRecord)
        {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos)
            {
                string key = line.substr(0, colon_pos);
                string value = line.substr(colon_pos + 1);

                value.erase(0, value.find_first_not_of(" \t\r\n"));
                value.erase(value.find_last_not_of(" \t\r\n") + 1);

                if (key.find("Thoi gian ghi nhan") != string::npos)
                {
                    currentRecord.timeStamp = value;
                }
                else if (key.find("ID nguoi dung") != string::npos)
                {
                    try { currentRecord.userID = stoi(value); }
                    catch (...) { currentRecord.userID = 0; }
                }
                else if (key.find("Ten nguoi dung") != string::npos)
                {
                    currentRecord.userName = value;
                }
                else if (key.find("ID sach") != string::npos)
                {
                    try { currentRecord.bookID = stoi(value); }
                    catch (...) { currentRecord.bookID = 0; }
                }
                else if (key.find("Ten sach") != string::npos)
                {
                    currentRecord.bookTitle = value;
                }
                else if (key.find("Ngay muon") != string::npos)
                {
                    currentRecord.borrowDate = value;
                }
            }
        }
        else if (line.find("----------------------------------------") != string::npos)
        {
            if (isNewRecord)
            {
                records.push_back(currentRecord);
            }
            isNewRecord = false;
        }
    }

    if (isNewRecord)
    {
        records.push_back(currentRecord);
    }

    inFile.close();

    if (records.empty())
    {
        cout << "Thong bao: File lich su hien dang trong.\n";
        cout << "\n(Nhan Enter de quay lai menu...)";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    const int TIME_WIDTH = 25;
    const int USER_ID_WIDTH = 10;
    const int USER_NAME_WIDTH = 25;
    const int BOOK_ID_WIDTH = 8;
    const int BOOK_TITLE_WIDTH = 40;
    const int DATE_WIDTH = 15;
    const int TOTAL_WIDTH = TIME_WIDTH + USER_ID_WIDTH + USER_NAME_WIDTH + BOOK_ID_WIDTH + BOOK_TITLE_WIDTH + DATE_WIDTH + 6;

    cout << setfill(' ');
    cout << left << setw(TIME_WIDTH) << "Thoi gian ghi nhan"
         << left << setw(USER_ID_WIDTH) << "ID Doc gia"
         << left << setw(USER_NAME_WIDTH) << "Ten Doc gia"
         << left << setw(BOOK_ID_WIDTH) << "ID Sach"
         << left << setw(BOOK_TITLE_WIDTH) << "Ten Sach"
         << left << setw(DATE_WIDTH) << "Ngay Muon"
         << endl;

    cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;

    for (const auto &rec : records)
    {
        cout << left << setw(TIME_WIDTH) << rec.timeStamp
             << left << setw(USER_ID_WIDTH) << rec.userID
             << left << setw(USER_NAME_WIDTH) << rec.userName
             << left << setw(BOOK_ID_WIDTH) << rec.bookID
             << left << setw(BOOK_TITLE_WIDTH) << rec.bookTitle
             << left << setw(DATE_WIDTH) << rec.borrowDate
             << endl;
    }

    cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}