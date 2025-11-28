#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <limits> // For numeric_limits
#include "BorrowManager.h"
#include "UserManager.h" // Cần include đầy đủ để gọi các hàm của Manager
#include "BookManager.h" // Cần include đầy đủ để gọi các hàm của Manager
#include "Utils.h"       // Cần Utils::CharArrayToIntManual, GetCurrentDate, IntToStringManual
#include "User.h"        // Cần class User để gọi BorrowBook/ReturnBook
#include "Book.h"        // Cần class Book

// Khai báo forward các lớp Manager để tránh lỗi include vòng lặp
class UserManager;
class BookManager;

using namespace std;

// Struct dùng để lưu trữ dữ liệu lịch sử mượn sách
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
    // Chuyển các hàm ghi log giao dịch từ Admin sang Private
    void LogBorrowRecord(int userID, const string &userName,
                         int bookID, const string &bookTitle,
                         const char *borrowDate);
    
    // Đổi tên SaveBorrowInfoToFile thành LogReturnRecord cho rõ ràng hơn
    void LogReturnRecord(int userID, const string &userName,
                         int bookID, const string &bookTitle,
                         const string &returnDate);
    
public:
    // Chuyển các hàm xử lý giao dịch từ Admin
    void HandleBorrowBook(UserManager& um, BookManager& bm);
    void HandleReturnBook(UserManager& um, BookManager& bm);
        
    // Chuyển các hàm thống kê/báo cáo từ Admin
    void ShowBorrowRecords();
    void ShowRawBorrowHistory(const string& filename, const string& title);
    
    // Public wrappers cho các hàm thô
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

    // Lấy thông tin users từ UserManager
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

    // Header cho bảng
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

    int borrowID = 1; // ID mượn tự tăng

    for (int i = 0; i < userCount; i++) {
        const Person& person = allUsers[i];
        
        // Tạo User object và tải transaction history
        User user;
        user.LoadUserByID(to_string(person.getID()));
        
        // Giả sử User có getter cho transactionHistory
        const vector<BorrowedItem>& history = user.getTransactionHistory();
        
        if (history.empty()) {
            continue; // Bỏ qua user không có giao dịch
        }

        // Duyệt qua lịch sử của user hiện tại
        for (const auto& item : history) {
            int bookID = item.getBookID();
            string borrowDateStr = item.getBorrowDate();
            string returnDateStr = item.getReturnDate();
            Book* book = bookManager.GetBookByID(bookID);

            // Cột ID mượn
            cout << left << setw(8) << borrowID++;

            // Thông tin user
            cout << left << setw(10) << person.getID()
                 << left << setw(20) << person.getName();

            // Thông tin sách
            cout << left << setw(10) << bookID;

            string bookTitle = (book) ? book->getTitle() : "Sach da bi xoa";
            if (bookTitle.length() > 34) {
                bookTitle = bookTitle.substr(0, 31) + "...";
            }
            cout << left << setw(35) << bookTitle;
            
            // Ngày mượn, ngày trả
            cout << left << setw(12) << borrowDateStr;
            cout << left << setw(12) << (item.getIsReturned() ? returnDateStr : "--");

            // Tính toán hạn trả
            tm borrow_tm = Utils::ParseDate(borrowDateStr);
            tm dueDate_tm = Utils::AddDays(borrow_tm, DEFAULT_BORROW_DAYS);
            string dueDateStr = Utils::FormatDate(dueDate_tm);
            cout << left << setw(12) << dueDateStr;

            // Xác định trạng thái
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

    // Thống kê tổng quan
    cout << "\n=== TONG QUAN HE THONG ===\n";
    cout << "Tong so doc gia: " << userCount << "\n";
    cout << "Tong so giao dich: " << totalTransactions << "\n";
    cout << "So sach dang muon: " << activeBorrows << "\n";
    cout << "So sach qua han: " << overdueBorrows << "\n";
}
// void BorrowManager::ShowAllUsersTransactionHistory(UserManager& userManager, BookManager& bookManager) {
//     cout << "\n=== LICH SU GIAO DICH CUA TAT CA DOC GIA ===\n";

//     // Lấy thông tin users từ UserManager
//     const Person* allUsers = userManager.GetAllUsers();
//     int userCount = userManager.GetUserCount();
    
//     if (userCount == 0) {
//         cout << "Khong co doc gia nao trong he thong.\n";
//         return;
//     }

//     const int DEFAULT_BORROW_DAYS = 14;
//     char currentDateBuffer[20];
//     Utils::GetCurrentDate(currentDateBuffer, sizeof(currentDateBuffer));
//     tm current_tm = Utils::ParseDate(currentDateBuffer);

//     int totalTransactions = 0;
//     int activeBorrows = 0;
//     int overdueBorrows = 0;

//     for (int i = 0; i < userCount; i++) {
//         const Person& person = allUsers[i];
        
//         // Tạo User object và tải transaction history
//         User user;
//         user.LoadUserByID(to_string(person.getID()));
        
//         // Giả sử User có getter cho transactionHistory
//         // Nếu không có, bạn cần điều chỉnh theo implementation thực tế
//         const vector<BorrowedItem>& history = user.getTransactionHistory();
        
//         if (history.empty()) {
//             continue; // Bỏ qua user không có giao dịch
//         }

//         cout << "\n"
//              << " DOC GIA: " << left << setw(37) << person.getName() << "\n"
//              << " ID: " << left << setw(40) << person.getID() << "\n";

//         // Header cho bảng
//         cout << left << setw(10) << "ID Sach"
//              << left << setw(35) << "Ten Sach"
//              << left << setw(15) << "Ngay muon"
//              << left << setw(15) << "Ngay tra"
//              << left << setw(15) << "Han tra"
//              << left << setw(15) << "Trang thai"
//              << endl;
//         cout << setfill('-') << setw(105) << "-" << setfill(' ') << endl;

//         // Duyệt qua lịch sử của user hiện tại
//         for (const auto& item : history) {
//             int bookID = item.getBookID();
//             string borrowDateStr = item.getBorrowDate();
//             string returnDateStr = item.getReturnDate();
//             Book* book = bookManager.GetBookByID(bookID);

//             cout << left << setw(10) << bookID;

//             string bookTitle = (book) ? book->getTitle() : "Sach da bi xoa";
//             if (bookTitle.length() > 34) {
//                 bookTitle = bookTitle.substr(0, 31) + "...";
//             }
//             cout << left << setw(35) << bookTitle;
//             cout << left << setw(15) << borrowDateStr;
            
//             // Hiển thị ngày trả
//             cout << left << setw(15) << (item.getIsReturned() ? returnDateStr : "--");

//             // Tính toán hạn trả
//             tm borrow_tm = Utils::ParseDate(borrowDateStr);
//             tm dueDate_tm = Utils::AddDays(borrow_tm, DEFAULT_BORROW_DAYS);
//             string dueDateStr = Utils::FormatDate(dueDate_tm);
//             cout << left << setw(15) << dueDateStr;

//             // Xác định trạng thái
//             string status;
//             if (item.getIsReturned()) {
//                 status = "Da tra";
//                 tm return_tm = Utils::ParseDate(returnDateStr);
//                 if (Utils::CompareDates(return_tm, dueDate_tm) > 0) {
//                     status += " (Tre)";
//                     overdueBorrows++;
//                 }
//             } else {
//                 status = "Dang muon";
//                 activeBorrows++;
//                 if (Utils::CompareDates(current_tm, dueDate_tm) > 0) {
//                     status = "QUA HAN";
//                     overdueBorrows++;
//                 }
//             }
//             cout << left << setw(15) << status;
//             cout << endl;

//             totalTransactions++;
//         }
//         cout << setfill('-') << setw(105) << "-" << setfill(' ') << endl;
//     }

//     // Thống kê tổng quan
//     cout << "\n=== TONG QUAN HE THONG ===\n";
//     cout << "Tong so doc gia: " << userCount << "\n";
//     cout << "Tong so giao dich: " << totalTransactions << "\n";
//     cout << "So sach dang muon: " << activeBorrows << "\n";
//     cout << "So sach qua han: " << overdueBorrows << "\n";
// }
// --- 1. LOGIC GHI LOG (Private) ---

void BorrowManager::LogBorrowRecord(int userID, const string &userName,
                                    int bookID, const string &bookTitle,
                                    const char *borrowDate)
{
    // Dùng ofstream với ios::app để ghi nối tiếp và TỰ ĐỘNG TẠO FILE nếu chưa có
    ofstream outFile("BorrowRecords.txt", ios::app);

    if (!outFile.is_open())
    {
        cout << "LOI: Khong the mo hoac tao file BorrowRecords.txt.\n";
        return;
    }

    // Lấy thời gian thực tế hệ thống
    time_t now = time(0);
    tm *localTime = localtime(&now);
    char timeBuffer[80];
    strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", localTime);

    // Ghi theo định dạng
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

    // Lấy ngày giờ hiện tại
    time_t now = time(0);
    tm *localTime = localtime(&now);
    char timeBuffer[80];
    strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", localTime);

    // Ghi thông tin vào file
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

// --- 2. LOGIC GIAO DỊCH (Public) ---

// void BorrowManager::HandleBorrowBook(UserManager& um, BookManager& bm)
// {
//     cout << "\n========= XU LY MUON SACH =========\n";

//     int userID = 0, bookID = 0;

//     // --- B1: Nhập ID người dùng ---
//     cout << "Nhap ID nguoi dung muon sach: ";
//     if (!(cin >> userID))
//     {
//         cout << "Loi: ID nguoi dung khong hop le.\n";
//         cin.clear();
//         cin.ignore(100, '\n');
//         return;
//     }
//     cin.ignore(100, '\n');

//     // --- B2: Nhập ID sách ---
//     cout << "Nhap ID sach can muon: ";
//     if (!(cin >> bookID))
//     {
//         cout << "Loi: ID sach khong hop le.\n";
//         cin.clear();
//         cin.ignore(100, '\n');
//         return;
//     }
//     cin.ignore(100, '\n');

//     // --- B3: Tải thông tin người dùng ---
//     Person *personPtr = um.GetUserByID(userID);
//     User *user = static_cast<User *>(personPtr);

//     if (user == nullptr)
//     {
//         cout << "Loi: Khong tim thay nguoi dung voi ID: " << userID << "\n";
//         return;
//     }
    
//     // --- B4: Lấy thông tin sách ---
//     Book *book = bm.GetBookByID(bookID);

//     if (!book)
//     {
//         cout << "Loi: Khong tim thay sach voi ID: " << bookID << "\n";
//         return;
//     }

//     if (book->getQuantity() <= 0)
//     {
//         cout << "Loi: Sach '" << book->getTitle() << "' da het trong kho.\n";
//         return;
//     }

//     // --- B5: Xử lý mượn sách ---
//     if (user->BorrowBook(bookID))
//     {
//         // 1. Trừ số lượng sách và lưu file sách
//         book->setQuantity(book->getQuantity() - 1);
//         bm.SaveBooksToFile(); // Gọi hàm lưu sách từ BookManager

//         // 2. Lấy ngày mượn hiện tại
//         char borrowDate[20];
//         Utils::GetCurrentDate(borrowDate, sizeof(borrowDate));

//         // 3. Lấy thông tin ghi log
//         string currentUserName = user->getName();
//         string currentBookTitle = book->getTitle();

//         // 4. Gọi hàm ghi log (private)
//         LogBorrowRecord(
//             userID,
//             currentUserName,
//             bookID,
//             currentBookTitle,
//             borrowDate);

//         cout << "MUON SACH THÀNH CÔNG!\n";
//         cout << "Sach: '" << book->getTitle() << "'\n";
//         cout << "Ngay muon: " << borrowDate << "\n";
//         cout << "So luong con lai: " << book->getQuantity() << "\n";
        
//         // 5. Cần lưu lại thông tin User sau khi mượn (để cập nhật BorrowedBooks list)
//         // Lưu ý: User::BorrowBook có thể đã tự gọi save nếu được thiết kế như vậy,
//         // nếu không, cần thêm hàm SaveUserByID(userID) vào UserManager và gọi ở đây.
//         // Tạm thời bỏ qua bước này vì không rõ User.h/UserManager.h
//     }
//     else
//     {
//         // Lỗi đã được in trong hàm BorrowBook của class User
//     }
    
// }
void BorrowManager::HandleBorrowBook(UserManager& um, BookManager& bm)
{
    cout << "\n========= XU LY MUON SACH =========\n";

    int userID = 0, bookID = 0;

    // --- Nhập ID người dùng ---
    cout << "Nhap ID nguoi dung muon sach: ";
    if (!(cin >> userID))
    {
        cout << "Loi: ID nguoi dung khong hop le.\n";
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    // --- Nhập ID sách ---
    cout << "Nhap ID sach can muon: ";
    if (!(cin >> bookID))
    {
        cout << "Loi: ID sach khong hop le.\n";
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    // --- Tải và kiểm tra người dùng ---
    // FIXED: Tạo user cục bộ thay vì dùng con trỏ
    User user;
    user.LoadUserByID(to_string(userID));
    
    // Kiểm tra xem user có được tải thành công không
    if (user.getID() == 0) {
        cout << "Loi: Khong tim thay nguoi dung voi ID: " << userID << "\n";
        return;
    }

    // cout << "[DEBUG] User loaded: " << user.getName() << ", ID: " << user.getID() << "\n";

    // --- Lấy thông tin sách ---
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

    // --- Xử lý mượn sách ---
    cout << "[DEBUG] Before calling BorrowBook\n";
    if (user.BorrowBook(bookID))
    {
        cout << "[DEBUG] BorrowBook returned true\n";
        
        // 1. Trừ số lượng sách và lưu file sách
        book->setQuantity(book->getQuantity() - 1);
        bm.SaveBooksToFile();

        // 2. Lấy ngày mượn hiện tại
        char borrowDate[20];
        Utils::GetCurrentDate(borrowDate, sizeof(borrowDate));

        // 3. Lấy thông tin ghi log
        string currentUserName = user.getName();
        string currentBookTitle = book->getTitle();

        // 4. Gọi hàm ghi log
        LogBorrowRecord(
            userID,
            currentUserName,
            bookID,
            currentBookTitle,
            borrowDate);

        cout << "MUON SACH THÀNH CÔNG!\n";
        cout << "Sach: '" << book->getTitle() << "'\n";
        cout << "Ngay muon: " << borrowDate << "\n";
        cout << "So luong con lai: " << book->getQuantity() << "\n";
        
        // 5. Cập nhật thông tin user trong UserManager
        // (Nếu cần đồng bộ với danh sách users trong UserManager)
    }
    else
    {
        cout << "[DEBUG] BorrowBook returned false\n";
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

    // Lấy ngày trả sách hiện tại
    char returnDate[20];
    Utils::GetCurrentDate(returnDate, sizeof(returnDate));
    string returnDateStr(returnDate);

    // --- B1: Tải thông tin người dùng (Sử dụng tạm LoadUserByID do thiếu User.h) ---
    // Tuy nhiên, để đúng logic OOP hơn, nên dùng GetUserByID từ um
    
    Person *personPtr = um.GetUserByID(userID);
    User *user = static_cast<User *>(personPtr);
    
    // Nếu không dùng GetUserByID do tempUser dùng LoadUserByID trong code cũ,
    // phải đảm bảo class User có hàm LoadUserByID(char*)
    User tempUser;
    Utils::IntToStringManual(userID, bufferID, sizeof(bufferID));
    tempUser.LoadUserByID(bufferID);

    if (tempUser.getID() == 0)
    {
        cout << "Loi: Khong tim thay nguoi dung voi ID: " << userID << "\n";
        return;
    }

    // --- B2: Lấy thông tin sách ---
    Book *book = bm.GetBookByID(bookID);
    string userName = tempUser.getName();
    string bookTitle = book ? book->getTitle() : "Unknown Book";

    // --- B3: Xử lý trả sách ---
    if (tempUser.ReturnBook(bookID))
    {
        if (book)
        {
            // Tăng số lượng sách và lưu file sách
            book->setQuantity(book->getQuantity() + 1);
            cout << "TRA SACH THANH CÔNG!\n";
            cout << "Sach: '" << book->getTitle() << "'\n";
            cout << "Ngay tra: " << returnDateStr << "\n";
            cout << "So luong hien tai: " << book->getQuantity() << "\n";

            bm.SaveBooksToFile(); // Gọi hàm lưu sách từ BookManager
            LogReturnRecord(userID, userName, bookID, bookTitle, returnDateStr); // Ghi log
        }
        else
        {
            // Trường hợp sách bị mất khỏi database nhưng vẫn còn trong hồ sơ người dùng
            cout << "TRA SACH THANH CÔNG.\n";
            cout << "Ngay tra: " << returnDateStr << "\n";
            LogReturnRecord(userID, userName, bookID, "Unknown Book (Deleted)", returnDateStr); // Ghi log
        }
        // Cần lưu lại thông tin User sau khi trả (để cập nhật BorrowedBooks list)
        // Tương tự như Borrow, cần hàm SaveUserByID/SaveAllUsers trong UserManager.
    }
    else
    {
        cout << "Loi: Nguoi dung ID " << userID << " khong muon sach ID " << bookID << "\n";
    }
}

// --- 3. LOGIC BÁO CÁO (Public) ---

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

    // Đọc từng dòng trong file và in ra
    while (getline(inFile, line))
    {
        cout << line << endl;
        isEmpty = false;
    }

    if (isEmpty) {
        cout << "Thong bao: File lich su hien dang trong.\n";
    }

    inFile.close(); // Đóng file sau khi đọc xong

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

    // --- 1. Đọc và Phân tích Cú pháp (Parsing) ---
    while (getline(inFile, line))
    {
        // 1.1. Bắt đầu một Khối Giao dịch mới
        if (line.find("=== THONG TIN MUON SACH ===") != string::npos)
        {
            // Nếu đang xử lý một record cũ mà gặp record mới, lưu record cũ lại
            // (Chỉ xảy ra khi file không kết thúc bằng "-------------------")
            if (isNewRecord)
            {
                records.push_back(currentRecord);
            }
            currentRecord = BorrowRecordData(); // Reset record mới
            isNewRecord = true;
        }
        // 1.2. Trích xuất các trường dữ liệu
        else if (isNewRecord)
        {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos)
            {
                string key = line.substr(0, colon_pos);
                string value = line.substr(colon_pos + 1);

                // Cắt khoảng trắng đầu và cuối
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
        // 1.3. Kết thúc một Khối Giao dịch (hoặc khoảng trống ngăn cách)
        else if (line.find("----------------------------------------") != string::npos)
        {
            if (isNewRecord)
            {
                records.push_back(currentRecord);
            }
            isNewRecord = false;
        }
    }

    // Nếu vòng lặp kết thúc mà vẫn còn 1 record đang chờ (đến EOF)
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

    // --- 2. Hiển thị dưới dạng bảng ---
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