#pragma once
#include "Person.h"
#include "BorrowedItem.h" // PHIÊN BẢN MỚI: có returnDate và isReturned
#include "Utils.h"
#include "UserManager.h" 
#include "User.h"
#include "Book.h"        // Giả định: Cần có Book.h
#include "BookManager.h" // Giả định: Cần có BookManager.h
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm> // Cần cho vector

using namespace std;

#define MAX_BORROWED_BOOKS 5 // Giới hạn số sách mượn CÙNG MỘT LÚC

class User : public Person
{
private:
    // --- THAY ĐỔI CẤU TRÚC DỮ LIỆU ---
    // Lưu TOÀN BỘ lịch sử giao dịch (bao gồm cả sách đã trả)
    vector<BorrowedItem> transactionHistory; 
    
    // --- CẬP NHẬT TÊN HÀM FILE I/O VÀ FIND ---
    void LoadTransactionHistory();
    void SaveTransactionHistory() const;
    
    // Chỉ tìm kiếm các giao dịch ĐANG MƯỢN
    int FindActiveBorrowIndex(int bookID) const;
    
public:
    User();
    User(int id, const char n[], const char d[], const char p[], const char e[], const char pw[]);
    virtual ~User();

    // getCurrentBorrowedCount tính toán dựa trên transactionHistory (số sách chưa trả)
    int getCurrentBorrowedCount() const; 
    
    void Show() const override;
    void LoadUserByID(const string &id);
    bool AskReturnToMenu();
    
    // Logic mượn/trả được cập nhật
    bool ReturnBook(int bookID);
    bool BorrowBook(int bookID);
    const vector<BorrowedItem>& getTransactionHistory() const {
        return transactionHistory;
    }
    // Đổi tên hàm hiển thị: Hiển thị TOÀN BỘ LỊCH SỬ GIAO DỊCH
    
    void ShowTransactionHistory(BookManager& bookManager);

    void Menu(UserManager &manager, BookManager &bm);
    
};


const int MAX_STREAM_SIZE_MANUAL = 100;

// Tinh toan lai so sach dang muon (chua tra)
   int User::getCurrentBorrowedCount() const {
    cout << "[DEBUG] getCurrentBorrowedCount - Vector size: " << transactionHistory.size() << "\n";
    
    int count = 0;
    for (const auto& item : transactionHistory) {
        if (!item.getIsReturned()) {
            count++;
        }
    }
    cout << "[DEBUG] getCurrentBorrowedCount - Result: " << count << "\n";
    return count;
}
int User::FindActiveBorrowIndex(int bookID) const
{
    for (size_t i = 0; i < transactionHistory.size(); ++i)
    {
        // Kiem tra ID sach VA trang thai chua tra
        if (transactionHistory[i].getBookID() == bookID && !transactionHistory[i].getIsReturned())
            return i; // Tra ve index trong vector
    }
    return -1;
}

// SaveTransactionHistory: Luu TOAN BO lich su giao dich vao file rieng cua User
void User::SaveTransactionHistory() const
{
    if (getID() == 0) return;

    // Sua ten file de moi user co file lich su rieng
    string filename = "user_" + to_string(getID()) + "_history.txt";
    ofstream outFile(filename);

    if (!outFile.is_open())
    {
        cerr << "Loi: Khong the mo tep de luu lich su giao dich: " << filename << endl;
        return;
    }

    outFile << transactionHistory.size() << "\n"; // Ghi tong so giao dich

    for (const auto& item : transactionHistory)
    {
        // Dinh dang luu: BookID,BorrowDate,ReturnDate,IsReturned(0/1)
        outFile << item.getBookID() << ","
                << item.getBorrowDate() << ","
                << item.getReturnDate() << ","
                << item.getIsReturned() << "\n";
    }

    outFile.close();
}

// LoadTransactionHistory: Tai TOAN BO lich su giao dich tu file rieng cua User
// void User::LoadTransactionHistory()
// {
    
//     if (getID() == 0) return;

//     string filename = "user_" + to_string(getID()) + "_history.txt";
//     ifstream inFile(filename);

//     if (!inFile.is_open())
//     {
//         cout << "[DEBUG] Khong tim thay file lich su: " << filename << " - Tao moi\n";
//         transactionHistory.clear();
//         return;
//     }

//     string line;
//     int totalTransactions = 0;
    
//     if (getline(inFile, line))
//     {
//         try {
//             totalTransactions = stoi(line);
//         } catch (...) {
//             totalTransactions = 0;
//         }
//     }
    
//     transactionHistory.clear();
//     int i = 0;
//     while (getline(inFile, line) && i < totalTransactions)
//     {
//         stringstream ss(line);
//         string bookID_str, borrowDate_str, returnDate_str, isReturned_str;

//         if (getline(ss, bookID_str, ',') && 
//             getline(ss, borrowDate_str, ',') && 
//             getline(ss, returnDate_str, ',') && 
//             getline(ss, isReturned_str)) // Doc 4 truong
//         {
//             try
//             {
//                 int bookID = stoi(bookID_str);
//                 bool isReturned = (stoi(isReturned_str) != 0); // 1 = true, 0 = false

//                 // Dung constructor day du de them vao lich su
//                 transactionHistory.emplace_back(
//                     bookID, 
//                     borrowDate_str, 
//                     returnDate_str, 
//                     isReturned
//                 );
//                 i++;
//             }
//             catch (...)
//             {
//                 // Bo qua dong bi loi
//             }
//         }
//     }

//     inFile.close();
// }
void User::LoadTransactionHistory()
{
    cout << "[DEBUG] LoadTransactionHistory called for user ID: " << getID() << "\n";
    
    if (getID() == 0) {
        cout << "[DEBUG] User ID is 0, skipping load\n";
        transactionHistory.clear();
        return;
    }

    string filename = "user_" + to_string(getID()) + "_history.txt";
    cout << "[DEBUG] Looking for file: " << filename << "\n";
    
    ifstream inFile(filename);

    if (!inFile.is_open())
    {
        cout << "[DEBUG] File not found, creating new transaction history\n";
        transactionHistory.clear(); // Đảm bảo vector rỗng
        return;
    }

    string line;
    int totalTransactions = 0;
    
    if (getline(inFile, line))
    {
        try {
            totalTransactions = stoi(line);
            cout << "[DEBUG] Total transactions in file: " << totalTransactions << "\n";
        } catch (...) {
            totalTransactions = 0;
            cout << "[DEBUG] Failed to parse transaction count\n";
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
                cout << "[DEBUG] Loaded transaction: BookID=" << bookID 
                     << ", BorrowDate=" << borrowDate_str 
                     << ", ReturnDate=" << returnDate_str 
                     << ", IsReturned=" << isReturned << "\n";
            }
            catch (...)
            {
                cout << "[DEBUG] Failed to parse transaction line: " << line << "\n";
            }
        }
    }

    inFile.close();
    cout << "[DEBUG] Finished loading. Total loaded: " << loadedCount 
         << ", Current vector size: " << transactionHistory.size() << "\n";
}


User::User() : Person()
{
    transactionHistory = vector<BorrowedItem>();
    // Bo: this->currentBorrowedCount = 0;
}

User::User(int id, const char n[], const char d[], const char p[], const char e[], const char pw[])
    : Person(id, n, d, p, e, pw)
{
    // Đảm bảo vector được khởi tạo rỗng
    transactionHistory = vector<BorrowedItem>();
    LoadTransactionHistory(); 
}

User::~User()
{
    // --- THAY DOI ---
    SaveTransactionHistory();
}

// Ham FindBorrowedBookIndex cu da bi thay the bang FindActiveBorrowIndex

// Ham LoadBorrowedBooks va SaveBorrowedBooks cu da bi thay the bang cac ham TransactionHistory moi

bool User::BorrowBook(int bookID)
{
    cout << "\n[DEBUG] ---- BorrowBook Start ----\n";
    cout << "[DEBUG] bookID = " << bookID << "\n";
    cout << "[DEBUG] User ID = " << getID() << "\n";
    cout << "[DEBUG] Transaction history size = " << transactionHistory.size() << "\n";
    
    int currentCount = getCurrentBorrowedCount();
    cout << "[DEBUG] currentCount = " << currentCount << "\n";
    
    int activeIndex = FindActiveBorrowIndex(bookID);
    cout << "[DEBUG] activeIndex = " << activeIndex << "\n";
    
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
    cout << "[DEBUG] Borrow date = " << dateBuffer << "\n";
    
    // Thêm item mới
    transactionHistory.emplace_back(bookID, dateBuffer);
    cout << "[DEBUG] After add - Transaction history size = " << transactionHistory.size() << "\n";
    
    SaveTransactionHistory();
    cout << "[DEBUG] Save completed\n";
    
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
    
    // Gia dinh: Tai day ban se goi BookManager de tang so luong sach
    // bookManager.IncreaseBookQuantity(bookID); 

    char dateBuffer[20];
    Utils::GetCurrentDate(dateBuffer, sizeof(dateBuffer));
    
    // --- THAY DOI LON: CAP NHAT TRANG THAI TRONG LICH SU ---
    transactionHistory[index].MarkAsReturned(dateBuffer); // Danh dau da tra va ghi ngay tra

    SaveTransactionHistory();
    return true;
}

// bool User::AskReturnToMenu()
// {
//     char choice;
//     cout << "\nBan co muon quay lai MENU khong? (Y/N): ";
//     cin >> choice;
//     cin.ignore(MAX_STREAM_SIZE_MANUAL, '\n');
//     return (choice == 'Y' || choice == 'y');
// }
bool User::AskReturnToMenu()
{
    char choiceInput[10];
    cout << "\nBan co muon quay lai MENU khong? (Y/N): ";
    
    // Đảm bảo đọc cả dòng input
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

            // --- THAY DOI ---
            LoadTransactionHistory(); 

            file.close();
            return;
        }
    }
    file.close();
}

// ShowTransactionHistory: Ham hien thi lich su day du (bao gom Da tra)
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

    // Cap nhat do rong cot
    cout << left << setw(10) << "ID Sach"
         << left << setw(35) << "Ten Sach"
         << left << setw(15) << "Ngay muon"
         << left << setw(15) << "Ngay tra"      // Cot moi
         << left << setw(15) << "Han tra"       // Cot moi
         << left << setw(15) << "Trang thai"    // Cot moi (Da tra, Qua han, Dang muon)
         << endl;
    cout << setfill('-') << setw(105) << "-" << setfill(' ') << endl; 

    // Duyet qua TOAN BO lich su giao dich
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
        
        // Hien thi ngay tra (Neu chua tra, hien thi "--")
        cout << left << setw(15) << (item.getIsReturned() ? returnDateStr : "--");

        // Tinh toan han tra
        tm borrow_tm = Utils::ParseDate(borrowDateStr);
        tm dueDate_tm = Utils::AddDays(borrow_tm, DEFAULT_BORROW_DAYS);
        string dueDateStr = Utils::FormatDate(dueDate_tm);
        cout << left << setw(15) << dueDateStr;

        // Xac dinh Trang thai
        string status;
        if (item.getIsReturned()) {
            status = "Da tra";
            // Neu da tra, kiem tra xem co qua han khi tra khong
            tm return_tm = Utils::ParseDate(returnDateStr);
            if (Utils::CompareDates(return_tm, dueDate_tm) > 0) {
                 status += " (Tre)"; // Da tra nhung tre han
            }
        } else {
            // Dang muon, kiem tra qua han
            status = "Dang muon";
            if (Utils::CompareDates(current_tm, dueDate_tm) > 0) {
                status = "QUA HAN"; // Dang muon va qua han
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
    // getCurrentBorrowedCount gio tinh toan tu transactionHistory
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
        // --- THAY DOI MUC MENU ---
        cout << "3. Xem lich su giao dich (Muon & Tra)\n"; 
        // -------------------------
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
            // --- THAY DOI HAM GOI ---
            ShowTransactionHistory(bm); 
            // ------------------------
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