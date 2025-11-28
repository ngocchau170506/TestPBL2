#pragma once
#include <iostream>
#include <fstream>
#include "UserManager.h"
#include "BookManager.h"
#include "BorrowManager.h" 
#include "StatisticsManager.h"
#include "Utils.h"
#include "User.h"
#include <iomanip> 
#include <vector>

using namespace std;

class Admin
{
private:
    UserManager userManager;
    BookManager bookManager;
    BorrowManager borrowManager;
    StatisticsManager statsManager; 

    // XÓA dòng này: bool AskReturnToMenu();
    void UserMenu();
    void BookMenu();
    void BorrowBookMenu();
    void StatisticsMenu();

public:
    Admin();
    void Menu();
};

Admin::Admin() 
    : statsManager(userManager, bookManager, borrowManager) 
{
}

void Admin::StatisticsMenu()
{
    statsManager.ShowStatisticsMenu();
}

// XÓA TOÀN BỘ HÀM AskReturnToMenu() Ở ĐÂY

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
            Utils::AskReturnToMenu(); // SỬA Ở ĐÂY
        }
        else if (choice == 3)
        {
            int id;
            cout << "Nhap ID nguoi dung can sua: ";
            cin >> id;
            cin.ignore(100, '\n');
            userManager.UpdateUserByID(id);
            Utils::AskReturnToMenu(); // SỬA Ở ĐÂY
        }
        else if (choice == 4)
        {
            int id;
            cout << "Nhap ID nguoi dung can xoa: ";
            cin >> id;
            cin.ignore(100, '\n');
            userManager.DeleteUserByID(id);
            Utils::AskReturnToMenu(); // SỬA Ở ĐÂY
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
            Utils::AskReturnToMenu(); // SỬA Ở ĐÂY
        }
        else if (choice == 3)
        {
            // ... code hiện tại ...
            Utils::AskReturnToMenu(); // SỬA Ở ĐÂY
        }
        else if (choice == 4)
        {
            int id;
            cout << "Nhap ID sach can sua: ";
            cin >> id;
            cin.ignore(100, '\n');
            bookManager.UpdateBookByID(id);
            Utils::AskReturnToMenu(); // SỬA Ở ĐÂY
        }
        else if (choice == 5)
        {
            int id;
            cout << "Nhap ID sach can xoa: ";
            cin >> id;
            cin.ignore(100, '\n');
            bookManager.DeleteBookByID(id);
            Utils::AskReturnToMenu(); // SỬA Ở ĐÂY
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
        cout << "0. Quay lai MENU chinh\n";
        cout << "Chon: ";

        cin.getline(input, 10);
        choice = Utils::CharArrayToIntManual(input);

        if (choice == 1)
        {
            borrowManager.HandleBorrowBook(userManager, bookManager);
            Utils::AskReturnToMenu(); // SỬA Ở ĐÂY
        }
        else if (choice == 2)
        {
            borrowManager.HandleReturnBook(userManager, bookManager);
            Utils::AskReturnToMenu(); // SỬA Ở ĐÂY
        }
        else if (choice == 3)
        {
            bookManager.ShowAllBooks();
            Utils::AskReturnToMenu(); // SỬA Ở ĐÂY
        }
        else if (choice == 4)
        {
            // ... code hiện tại ...
            Utils::AskReturnToMenu(); // SỬA Ở ĐÂY
        }
        else if (choice == 5)
        {
            // ... code hiện tại ...
            Utils::AskReturnToMenu(); // SỬA Ở ĐÂY
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
            StatisticsMenu();
        }
        else if (choice == 0)
            cout << "Thoat chuong trinh.\n";
        else
            cout << "Lua chon khong hop le!\n";

    } while (choice != 0);
}