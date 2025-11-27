
// #pragma once
// #include <iostream>
// #include "UserManager.h"
// #include "BookManager.h"
// #include "Utils.h"
// #include "User.h"
// using namespace std;

// class BorrowManager
// {
// private:

//     void SaveBorrowInfoToFile(int userID, const string &userName, int bookID, const string &bookTitle, const string &returnDate);
//     void ShowBorrowRecords();
    
// };
// void BorrowManager::ShowBorrowRecords()
// {
//     cout << "\n==================================================================================\n";
//     cout << "                           THONG KE DANH SACH MUON SACH                             \n";
//     cout << "==================================================================================\n";

//     ifstream inFile("BorrowRecords.txt");

//     if (!inFile.is_open())
//     {
//         cout << "Thong bao: Chua co du lieu muon sach (File chua ton tai).\n";
//         return;
//     }

//     vector<BorrowRecordData> records;
//     string line;
//     BorrowRecordData currentRecord;
//     bool isNewRecord = false;

//     // --- 1. Đọc và Phân tích Cú pháp (Parsing) ---
//     while (getline(inFile, line))
//     {
//         // 1.1. Bắt đầu một Khối Giao dịch mới
//         if (line.find("=== THONG TIN MUON SACH ===") != string::npos)
//         {
//             if (isNewRecord)
//             {
//                 // Nếu đang xử lý một record cũ mà gặp record mới, lưu record cũ lại
//                 records.push_back(currentRecord);
//             }
//             currentRecord = BorrowRecordData(); // Reset record mới
//             isNewRecord = true;
//         }
//         // 1.2. Trích xuất các trường dữ liệu
//         else if (isNewRecord)
//         {
//             size_t colon_pos = line.find(':');
//             if (colon_pos != string::npos)
//             {
//                 string key = line.substr(0, colon_pos);
//                 string value = line.substr(colon_pos + 1);

//                 value.erase(0, value.find_first_not_of(" \t\r\n"));
//                 value.erase(value.find_last_not_of(" \t\r\n") + 1);

//                 if (key.find("Thoi gian ghi nhan") != string::npos)
//                 {
//                     currentRecord.timeStamp = value;
//                 }
//                 else if (key.find("ID nguoi dung") != string::npos)
//                 {
//                     try
//                     {
//                         currentRecord.userID = stoi(value);
//                     }
//                     catch (...)
//                     {
//                         currentRecord.userID = 0;
//                     }
//                 }
//                 else if (key.find("Ten nguoi dung") != string::npos)
//                 {
//                     currentRecord.userName = value;
//                 }
//                 else if (key.find("ID sach") != string::npos)
//                 {
//                     try
//                     {
//                         currentRecord.bookID = stoi(value);
//                     }
//                     catch (...)
//                     {
//                         currentRecord.bookID = 0;
//                     }
//                 }
//                 else if (key.find("Ten sach") != string::npos)
//                 {
//                     currentRecord.bookTitle = value;
//                 }
//                 else if (key.find("Ngay muon") != string::npos)
//                 {
//                     currentRecord.borrowDate = value;
//                 }
//             }
//         }

//         else if (line.find("----------------------------------------") != string::npos)
//         {
//             if (isNewRecord)
//             {
//                 records.push_back(currentRecord);
//             }
//             isNewRecord = false;
//         }
//     }

//     if (isNewRecord)
//     {
//         records.push_back(currentRecord);
//     }

//     inFile.close();

//     if (records.empty())
//     {
//         cout << "Thong bao: File lich su hien dang trong.\n";
//         cout << "\n(Nhan Enter de quay lai menu...)";
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         cin.get();
//         return;
//     }

//     const int TIME_WIDTH = 25;
//     const int USER_ID_WIDTH = 10;
//     const int USER_NAME_WIDTH = 25;
//     const int BOOK_ID_WIDTH = 8;
//     const int BOOK_TITLE_WIDTH = 35;
//     const int DATE_WIDTH = 15;
//     const int TOTAL_WIDTH = TIME_WIDTH + USER_ID_WIDTH + USER_NAME_WIDTH + BOOK_ID_WIDTH + BOOK_TITLE_WIDTH + DATE_WIDTH + 6;

//     cout << setfill(' ');
//     cout << left << setw(TIME_WIDTH) << "Thoi gian ghi nhan"
//          << left << setw(USER_ID_WIDTH) << "ID Doc gia"
//          << left << setw(USER_NAME_WIDTH) << "Ten Doc gia"
//          << left << setw(BOOK_ID_WIDTH) << "ID Sach"
//          << left << setw(BOOK_TITLE_WIDTH) << "Ten Sach"
//          << left << setw(DATE_WIDTH) << "Ngay Muon"
//          << endl;

//     cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;

//     for (const auto &rec : records)
//     {
//         cout << left << setw(TIME_WIDTH) << rec.timeStamp
//              << left << setw(USER_ID_WIDTH) << rec.userID
//              << left << setw(USER_NAME_WIDTH) << rec.userName
//              << left << setw(BOOK_ID_WIDTH) << rec.bookID
//              << left << setw(BOOK_TITLE_WIDTH) << rec.bookTitle
//              << left << setw(DATE_WIDTH) << rec.borrowDate
//              << endl;
//     }

//     cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;

//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
//     cin.get();
// }
// void BorrowManager::SaveBorrowInfoToFile(int userID, const string &userName,
//                                  int bookID, const string &bookTitle,
//                                  const string &returnDate)
// {
//     const string fileName = "ReturnRecords.txt"; // ← file bạn muốn ghi

//     ofstream outFile(fileName, ios::app); // Mở file ở chế độ append

//     if (!outFile.is_open())
//     {
//         cout << "Loi: Khong the mo file " << fileName << " de ghi thong tin muon/tra sach!\n";
//         return;
//     }

//     // Lấy ngày giờ hiện tại
//     time_t now = time(0);
//     tm *localTime = localtime(&now);
//     char timeBuffer[80];
//     strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", localTime);

//     // Ghi thông tin vào file
//     outFile << "=== THONG TIN TRA SACH ===" << endl;
//     outFile << "Thoi gian ghi nhan: " << timeBuffer << endl;
//     outFile << "ID nguoi dung: " << userID << endl;
//     outFile << "Ten nguoi dung: " << userName << endl;
//     outFile << "ID sach: " << bookID << endl;
//     outFile << "Ten sach: " << bookTitle << endl;
//     outFile << "Ngay tra: " << returnDate << endl;
//     outFile << "----------------------------------------" << endl;
//     outFile << endl;

//     outFile.close();

//     cout << "Da ghi thong tin tra sach vao file: " << fileName << "\n";
// }