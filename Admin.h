#pragma once
#include <iostream>
#include <fstream> // Cần thiết cho các hàm còn lại (ví dụ AskReturnToMenu)
#include "UserManager.h"
#include "BookManager.h"
#include "BorrowManager.h" 
#include "Utils.h"
#include "User.h"
#include <iomanip> 
//#include <limits>
//#include <string>
#include <vector>

using namespace std;

class Admin
{
private:
    UserManager userManager;
    BookManager bookManager;
    BorrowManager borrowManager; 

    bool AskReturnToMenu();
    void UserMenu();
    void BookMenu();
    void BorrowBookMenu();
    

public:
    void Menu();
};

// --- CÁC HÀM CỦA ADMIN ---

bool Admin::AskReturnToMenu()
{
    char choice;
    cout << "\nBan co muon quay lai MENU khong? (Y/N): ";
    // Sử dụng cin.get() thay cho cin >> choice để tránh lỗi buffer
    choice = cin.get(); 
    if (choice == '\n') { // Nếu người dùng chỉ nhấn Enter
        return true; 
    }
    // Xóa bộ đệm sau khi đọc ký tự (đảm bảo choice chỉ lấy 1 ký tự)
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
        cout << "3. Tim sach\n";
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
                    bookManager.SearchBookByTitle();
                else if (subChoice == 2)
                {
                    char author[100];
                    cout << "Nhap ten tac gia: ";
                    cin.getline(author, sizeof(author));
                    bookManager.SearchBookByAuthor(author);
                }
                else if (subChoice == 3)
                {
                    char idStr[20];
                    int bookID;
                    cout << "Nhap ID sach: ";
                    cin.getline(idStr, sizeof(idStr));
                    bookID = Utils::CharArrayToIntManual(idStr);
                    const Book *bookPtr = bookManager.GetBookByID(bookID);
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
        cout << "2. Ghi nhan tra sach\n";
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
            // Ủy thác xử lý mượn sách cho BorrowManager
            borrowManager.HandleBorrowBook(userManager, bookManager);
            AskReturnToMenu();
        }
        else if (choice == 2)
        {
            // Ủy thác xử lý trả sách cho BorrowManager
            borrowManager.HandleReturnBook(userManager, bookManager);
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
                // Giả định User::ShowBorrowedBooks chấp nhận BookManager
                user->ShowTransactionHistory(bookManager); 
            else
                cout << "Khong tim thay nguoi dung voi ID: " << userID << endl;

            AskReturnToMenu();
        }
        else if (choice == 5)
        {
            // Ủy thác hiển thị lịch sử mượn thô cho BorrowManager
            borrowManager.ShowAllUsersTransactionHistory(userManager, bookManager);
            AskReturnToMenu();
        }
        else if (choice == 6)
        {
            // Ủy thác hiển thị lịch sử trả thô cho BorrowManager
            borrowManager.ShowHistoryReturn();
            AskReturnToMenu();
        }
        else if (choice == 0)
            break;
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
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
        cout << "4. Thong ke\n"; // Đổi tên cho rõ ràng
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
            // Ủy thác thống kê cho BorrowManager
            borrowManager.ShowBorrowRecords();
            AskReturnToMenu();
        }
        else if (choice == 0)
            cout << "Thoat chuong trinh.\n";
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}