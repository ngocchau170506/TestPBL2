#pragma once
#include <iostream>
#include "UserManager.h"
#include "BookManager.h"
#include "BorrowManager.h"
#include "Utils.h"
#include "User.h"
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

class Admin
{
private:
    UserManager userManager;
    BookManager bookManager;

    bool AskReturnToMenu();
    void UserMenu();
    void BookMenu();
    void BorrowBookMenu();
    void HandleBorrowBook();
    void HandleReturnBook();
    void SaveBorrowInfoToFile(int userID, const string &userName, int bookID, const string &bookTitle, const string &returnDate);
    void LogBorrowReturnRecord(int userID, const string &userName,
                               int bookID, const string &bookTitle,
                               const char *borrowDate);

public:
    void Menu();
    void ShowBorrowRecords();
    void ShowHistoryBorrow();
    void ShowHistoryReturn();
    
};
// Trong file Admin.cpp
void Admin::ShowHistoryBorrow()
{
    cout << "\n============================================\n";
    cout << "       THONG KE DANH SACH MUON SACH       \n";
    cout << "============================================\n";

    ifstream inFile("BorrowRecords.txt"); // Mở file để đọc

    if (!inFile.is_open())
    {
        cout << "Thong bao: Chua co du lieu muon sach (File chua ton tai).\n";
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

    cin.ignore();
    cin.get();
}
void Admin::ShowHistoryReturn()
{
    cout << "\n============================================\n";
    cout << "       THONG KE DANH SACH MUON SACH       \n";
    cout << "============================================\n";

    ifstream inFile("BorrowReturn.txt"); // Mở file để đọc

    if (!inFile.is_open())
    {
        cout << "Thong bao: Chua co du lieu muon sach (File chua ton tai).\n";
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

    cin.ignore();
    cin.get();
}
void Admin::ShowBorrowRecords()
{
    cout << "\n==================================================================================\n";
    cout << "                           THONG KE DANH SACH MUON SACH                             \n";
    cout << "==================================================================================\n";

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
            if (isNewRecord)
            {
                // Nếu đang xử lý một record cũ mà gặp record mới, lưu record cũ lại
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

                value.erase(0, value.find_first_not_of(" \t\r\n"));
                value.erase(value.find_last_not_of(" \t\r\n") + 1);

                if (key.find("Thoi gian ghi nhan") != string::npos)
                {
                    currentRecord.timeStamp = value;
                }
                else if (key.find("ID nguoi dung") != string::npos)
                {
                    try
                    {
                        currentRecord.userID = stoi(value);
                    }
                    catch (...)
                    {
                        currentRecord.userID = 0;
                    }
                }
                else if (key.find("Ten nguoi dung") != string::npos)
                {
                    currentRecord.userName = value;
                }
                else if (key.find("ID sach") != string::npos)
                {
                    try
                    {
                        currentRecord.bookID = stoi(value);
                    }
                    catch (...)
                    {
                        currentRecord.bookID = 0;
                    }
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
    const int BOOK_TITLE_WIDTH = 35;
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

bool Admin::AskReturnToMenu()
{
    char choice;
    cout << "\nBan co muon quay lai MENU khong? (Y/N): ";
    cin >> choice;
    cin.ignore(100, '\n');
    if (choice == 'Y' || choice == 'y')
        return true;
    return false;
}

void Admin::UserMenu()
{
    int choice;
    char input[10];

    do
    {
        cout << "\n========= QUAN LY NGUOI DUNG =========\n";
        cout << "1. Them nguoi dung\n";
        cout << "2. Xem danh sach nguoi dung\n";
        cout << "3. Sua thong tin nguoi dung\n";
        cout << "4. Xoa nguoi dung\n";
        cout << "0. Quay lai MENU chinh\n";
        cout << "Chon: ";

        cin.getline(input, 10);
        choice = Utils::CharArrayToIntManual(input);

        if (choice == 1)
            userManager.AddUser();
        else if (choice == 2)
        {
            userManager.ShowAllUsers();
            AskReturnToMenu();
        }
        else if (choice == 3)
        {
            int id;
            cout << "Nhap ID nguoi dung can sua: ";
            cin >> id;
            cin.ignore(100, '\n');
            userManager.UpdateUserByID(id);
            AskReturnToMenu();
        }
        else if (choice == 4)
        {
            int id;
            cout << "Nhap ID nguoi dung can xoa: ";
            cin >> id;
            cin.ignore(100, '\n');
            userManager.DeleteUserByID(id);
            AskReturnToMenu();
        }
        else if (choice == 0)
            break;
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}

void Admin::BookMenu()
{
    int choice;
    char input[10];

    do
    {
        cout << "\n========= QUAN LY SACH =========\n";
        cout << "1. Them sach\n";
        cout << "2. Xem danh sach sach\n";
        cout << "3. Tim sach theo ten\n";
        cout << "4. Sua thong tin sach\n";
        cout << "5. Xoa sach\n";
        cout << "0. Quay lai MENU chinh\n";
        cout << "Chon: ";

        cin.getline(input, 10);
        choice = Utils::CharArrayToIntManual(input);

        if (choice == 1)
            bookManager.AddBook();
        else if (choice == 2)
        {
            bookManager.ShowAllBooks();
            AskReturnToMenu();
        }
        else if (choice == 3)
        {
            bookManager.SearchBookByTitle();
            AskReturnToMenu();
        }
        else if (choice == 4)
        {
            int id;
            cout << "Nhap ID sach can sua: ";
            cin >> id;
            cin.ignore(100, '\n');
            bookManager.UpdateBookByID(id);
            AskReturnToMenu();
        }
        else if (choice == 5)
        {
            int id;
            cout << "Nhap ID sach can xoa: ";
            cin >> id;
            cin.ignore(100, '\n');
            bookManager.DeleteBookByID(id);
            AskReturnToMenu();
        }
        else if (choice == 0)
            break;
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}

void Admin::BorrowBookMenu()
{
    int choice;
    char input[10];

    do
    {
        cout << "\n========= QUAN LY MUON/TRA SACH =========\n";
        cout << "1. Them thong tin muon sach\n";
        cout << "2. Them thong tin tra sach\n";
        cout << "3. Xem tinh trang sach trong thu vien\n";
        cout << "4. Xem sach dang duoc muon boi nguoi dung\n";
        cout << "5. Xem lich su muon sach\n";
        cout << "6. Xem lich su tra sach\n";
        cout << "0. Quay lai MENU chinh\n";
        cout << "Chon: ";

        cin.getline(input, 10);
        choice = Utils::CharArrayToIntManual(input);

        if (choice == 1)
        {
            HandleBorrowBook();
            AskReturnToMenu();
        }
        else if (choice == 2)
        {
            HandleReturnBook();
            AskReturnToMenu();
        }
        else if (choice == 3)
        {
            bookManager.ShowAllBooks();
            AskReturnToMenu();
        }
        else if (choice == 4)
        {
            char userID_str[10];
            cout << "Nhap ID nguoi dung can xem: ";
            cin.getline(userID_str, 10);
            int userID = Utils::CharArrayToIntManual(userID_str);

            Person *personPtr = userManager.GetUserByID(userID);
            User *user = static_cast<User *>(personPtr);

            if (user)
                user->ShowBorrowedBooks(bookManager);
            else
                cout << "Khong tim thay nguoi dung voi ID: " << userID << endl;

            AskReturnToMenu();
        }
        else if (choice == 5)
        {
            ShowHistoryBorrow();
            AskReturnToMenu();
        }
        else if (choice == 6)
        {
            ShowHistoryReturn();
            AskReturnToMenu();
        }
        else if (choice == 0)
            break;
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}

void Admin::LogBorrowReturnRecord(int userID, const string &userName,
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

    // --- GHI THEO FORM BẠN YÊU CẦU ---
    outFile << "=== THONG TIN MUON SACH ===" << endl;
    outFile << "Thoi gian ghi nhan: " << timeBuffer << endl;
    outFile << "ID nguoi dung: " << userID << endl;
    outFile << "Ten nguoi dung: " << userName << endl;
    outFile << "ID sach: " << bookID << endl;
    outFile << "Ten sach: " << bookTitle << endl;
    outFile << "Ngay muon: " << borrowDate << endl;
    outFile << "----------------------------------------" << endl;
    outFile << endl; // Xuống dòng để ngăn cách các lần ghi

    outFile.close();

    cout << "Da ghi thong tin muon sach vao BorrowRecords.txt.\n";
}

// void Admin::HandleBorrowBook()
// {
//     cout << "\n========= XU LY MUON SACH =========\n";

//     int userID = 0, bookID = 0;
//     char bufferID[20];

//     // --- B1: Nhập ID người dùng ---
//     cout << "Nhap ID nguoi dung muon sach: ";
//     if (!(cin >> userID)) {
//         cout << "Loi: ID nguoi dung khong hop le.\n";
//         cin.clear(); cin.ignore(100, '\n'); return;
//     }
//     cin.ignore(100, '\n');

//     // --- B2: Nhập ID sách ---
//     cout << "Nhap ID sach can muon: ";
//     if (!(cin >> bookID)) {
//         cout << "Loi: ID sach khong hop le.\n";
//         cin.clear(); cin.ignore(100, '\n'); return;
//     }
//     cin.ignore(100, '\n');

//     // --- B3: Tải thông tin người dùng ---
//     // User tempUser;
//     // Utils::IntToStringManual(userID, bufferID, sizeof(bufferID));
//     // tempUser.LoadUserByID(bufferID);

//     // if (tempUser.getID() == 0) {
//     //     cout << "Loi: Khong tim thay nguoi dung voi ID: " << userID << "\n";
//     //     return;
//     // }
//     Person* personPtr = userManager.GetUserByID(userID);
//     User* user = static_cast<User*>(personPtr);

//     if (user == nullptr) {
//         cout << "Loi: Khong tim thay nguoi dung voi ID: " << userID << "\n";
//         return;
//     }
//     // --- B4: Lấy thông tin sách ---
//     Book* book = bookManager.GetBookByID(bookID);

//     if (!book) {
//         cout << "Loi: Khong tim thay sach voi ID: " << bookID << "\n";
//         return;
//     }

//     if (book->getQuantity() <= 0) {
//         cout << "Loi: Sach '" << book->getTitle() << "' da het trong kho.\n";
//         return;
//     }

//     // --- B5: Xử lý mượn sách ---
//     if (tempUser.BorrowBook(bookID))
//     {
//         // Trừ số lượng
//         book->setQuantity(book->getQuantity() - 1);
//         bookManager.SaveBooksToFile();

//         // Lấy ngày mượn hiện tại
//         char borrowDate[20];
//         Utils::GetCurrentDate(borrowDate, sizeof(borrowDate));

//         // --- ĐOẠN CODE CẦN THÊM ĐỂ LẤY TÊN ---
//         string currentUserName = tempUser.getName(); // Lấy tên user
//         string currentBookTitle = book->getTitle();  // Lấy tên sách

//         // Gọi hàm ghi log với thông tin đầy đủ
//         LogBorrowReturnRecord(
//             userID,
//             currentUserName,  // Truyền tên user
//             bookID,
//             currentBookTitle, // Truyền tên sách
//             borrowDate
//         );
//         // -------------------------------------

//         cout << "MUON SACH THANH CONG!\n";
//         cout << "Sach: '" << book->getTitle() << "'\n";
//         cout << "Ngay muon: " << borrowDate << "\n";
//         cout << "So luong con lai: " << book->getQuantity() << "\n";
//     }
//     else {
//         // Lỗi đã được in trong BorrowBook
//     }
// }
void Admin::HandleBorrowBook()
{
    cout << "\n========= XU LY MUON SACH =========\n";

    int userID = 0, bookID = 0;
    // char bufferID[20]; // Không cần dùng cái này nữa

    // --- B1: Nhập ID người dùng ---
    cout << "Nhap ID nguoi dung muon sach: ";
    if (!(cin >> userID))
    {
        cout << "Loi: ID nguoi dung khong hop le.\n";
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    // --- B2: Nhập ID sách ---
    cout << "Nhap ID sach can muon: ";
    if (!(cin >> bookID))
    {
        cout << "Loi: ID sach khong hop le.\n";
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');

    // --- B3: Tải thông tin người dùng (SỬ DỤNG CON TRỎ TRỰC TIẾP) ---
    // Đoạn cũ đã comment là đúng, ta dùng đoạn dưới này:
    Person *personPtr = userManager.GetUserByID(userID);
    User *user = static_cast<User *>(personPtr);

    if (user == nullptr)
    {
        cout << "Loi: Khong tim thay nguoi dung voi ID: " << userID << "\n";
        return;
    }

    // --- B4: Lấy thông tin sách ---
    Book *book = bookManager.GetBookByID(bookID);

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

    // --- B5: Xử lý mượn sách (SỬA LẠI CHỖ NÀY) ---
    // Thay tempUser. bằng user->
    if (user->BorrowBook(bookID))
    {
        // 1. Trừ số lượng sách và lưu file sách
        book->setQuantity(book->getQuantity() - 1);
        bookManager.SaveBooksToFile();

        // 2. QUAN TRỌNG: Lưu danh sách User xuống file để cập nhật việc mượn
        // Nếu bạn có hàm SaveAllUsers() thì hãy gọi ở đây, ví dụ:
        // userManager.SaveAllUsers();

        // 3. Lấy ngày mượn hiện tại
        char borrowDate[20];
        Utils::GetCurrentDate(borrowDate, sizeof(borrowDate));

        // 4. Lấy thông tin ghi log (Dùng user-> thay vì tempUser.)
        string currentUserName = user->getName(); // <--- Đã sửa thành user->
        string currentBookTitle = book->getTitle();

        // Gọi hàm ghi log
        LogBorrowReturnRecord(
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
        // Lỗi đã được in trong hàm BorrowBook của class User
    }
}

void Admin::HandleReturnBook()
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

    // Nhập ngày trả sách
    // string returnDate;
    // cout << "Nhap ngay tra sach (dd/mm/yyyy): ";
    // getline(cin, returnDate);

    // if (returnDate.empty()) {
    //     cout << "Ngay tra sach khong duoc de trong.\n";
    //     return;
    // }
    char returnDate[20];
    Utils::GetCurrentDate(returnDate, sizeof(returnDate));

    User tempUser;
    Utils::IntToStringManual(userID, bufferID, sizeof(bufferID));
    tempUser.LoadUserByID(bufferID);

    if (tempUser.getID() == 0)
    {
        cout << "Loi: Khong tim thay nguoi dung voi ID: " << userID << "\n";
        return;
    }

    Book *book = bookManager.GetBookByID(bookID);
    string userName = tempUser.getName();
    string bookTitle = book ? book->getTitle() : "Unknown Book";

    // Gọi hàm ReturnBook với 1 tham số (giữ nguyên)
    if (tempUser.ReturnBook(bookID))
    {
        if (book)
        {
            book->setQuantity(book->getQuantity() + 1);
            cout << "TRA SACH THANH CONG!\n";
            cout << "Sach: '" << book->getTitle() << "'\n";
            cout << "Ngay tra: " << returnDate << "\n";
            cout << "So luong hien tai: " << book->getQuantity() << "\n";

            bookManager.SaveBooksToFile();
            SaveBorrowInfoToFile(userID, userName, bookID, bookTitle, returnDate);
        }
        else
        {
            cout << "TRA SACH THANH CONG.\n";
            cout << "Ngay tra: " << returnDate << "\n";
            SaveBorrowInfoToFile(userID, userName, bookID, "Unknown Book", returnDate);
        }
    }
    else
    {
        cout << "Loi: Nguoi dung ID " << userID << " khong muon sach ID " << bookID << "\n";
    }
}

// Hàm ghi thông tin mượn/trả sách ra file txt
void Admin::SaveBorrowInfoToFile(int userID, const string &userName,
                                 int bookID, const string &bookTitle,
                                 const string &returnDate)
{
    const string fileName = "ReturnRecords.txt"; // ← file bạn muốn ghi

    ofstream outFile(fileName, ios::app); // Mở file ở chế độ append

    if (!outFile.is_open())
    {
        cout << "Loi: Khong the mo file " << fileName << " de ghi thong tin muon/tra sach!\n";
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

void Admin::Menu()
{
    int choice;
    char input[10];

    do
    {
        cout << "\n========= HE THONG QUAN LY ADMIN =========\n";
        cout << "1. Quan ly nguoi dung\n";
        cout << "2. Quan ly sach\n";
        cout << "3. Quan ly muon/tra sach\n";
        cout << "4. Thong ke\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";

        cin.getline(input, 10);
        choice = Utils::CharArrayToIntManual(input);

        if (choice == 1)
            UserMenu();
        else if (choice == 2)
            BookMenu();
        else if (choice == 3)
            BorrowBookMenu();
        else if (choice == 4)
        {
            ShowBorrowRecords();
            AskReturnToMenu();
        }
        else if (choice == 0)
            cout << "Thoat chuong trinh.\n";
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}
