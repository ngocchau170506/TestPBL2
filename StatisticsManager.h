#pragma once
#include "UserManager.h"
#include "BookManager.h"
#include "BorrowManager.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class StatisticsManager {
private:
    UserManager& userManager;
    BookManager& bookManager;
    BorrowManager& borrowManager;

public:
    StatisticsManager(UserManager& um, BookManager& bm, BorrowManager& bmgr)
        : userManager(um), bookManager(bm), borrowManager(bmgr) {}

    // Menu chính thống kê
    void ShowStatisticsMenu();
    
    // Thống kê sách (với menu con)
    void ThongKeSach();
    void ThongKeTongQuanSach();
    void ThongKeTheoTheLoai();
    void ThongKeTheoTacGia();
    void ThongKeTinhTrangMuon();
    void ThongKeSachMuonNhieu();
    
    // Thống kê độc giả
    void ThongKeDocGia();
    
    // Thống kê mượn/trả sách
    void ThongKeMuonTra();
    
    // Báo cáo tổng hợp
    void BaoCaoTongHop();
};

void StatisticsManager::ShowStatisticsMenu() {
    int statChoice;
    
    do {
        cout << "\n=== THONG KE THU VIEN ===";
        cout << "\n1. Thong ke sach";
        cout << "\n2. Thong ke doc gia"; 
        cout << "\n3. Thong ke muon/tra sach";
        cout << "\n0. Quay lai menu chinh";
        cout << "\n=========================";
        cout << "\nNhap lua chon: ";
        
        if (!(cin >> statChoice)) {
            cin.clear();
            cin.ignore(100, '\n');
            statChoice = -1;
        }
        cin.ignore(100, '\n');

        switch(statChoice) {
            case 1:
                ThongKeSach();
                Utils::AskReturnToMenu();
                break;
            case 2:
                ThongKeDocGia();
                Utils::AskReturnToMenu();
                break;
            case 3:
                ThongKeMuonTra();
                Utils::AskReturnToMenu();
                break;
            case 0:
                cout << "\nQuay lai menu chinh...";
                break;
            default:
                cout << "\nLua chon khong hop le!";
        }
        
    } while (statChoice != 0);
}

void StatisticsManager::ThongKeSach() {
    int choice;
    do {
        cout << "\n=== THONG KE SACH ===";
        cout << "\n1. Tong quan sach";
        cout << "\n2. Theo the loai";
        cout << "\n3. Theo tac gia";
        cout << "\n4. Theo tinh trang muon";
        cout << "\n5. Sach duoc muon nhieu nhat";
        cout << "\n0. Quay lai";
        cout << "\nChon: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(100, '\n');
            choice = -1;
        }
        cin.ignore(100, '\n');

        switch(choice) {
            case 1: 
                ThongKeTongQuanSach();
                Utils::AskReturnToMenu(); 
                break;
            case 2: 
                ThongKeTheoTheLoai(); 
                Utils::AskReturnToMenu();
                break;
            case 3: 
                ThongKeTheoTacGia(); 
                Utils::AskReturnToMenu();
                break;
            case 4: 
                ThongKeTinhTrangMuon(); 
                Utils::AskReturnToMenu();
                break;
            case 5: 
                ThongKeSachMuonNhieu();
                Utils::AskReturnToMenu(); 
                break;
            case 0:
                cout << "Quay lai menu thong ke.\n";
                break;
            default:
                cout << "Lua chon khong hop le!\n";
        }
        
        if (choice != 0) {
            cout << "\nNhan Enter de tiep tuc...";
            cin.get();
        }
        
    } while(choice != 0);
}

void StatisticsManager::ThongKeTongQuanSach() {
    cout << "\n=== TONG QUAN SACH ===\n";
    
    // Sử dụng hàm có sẵn từ BookManager
    bookManager.ShowStockReport();
    
    // Thêm thống kê chi tiết hơn
    int totalBooks = 0;
    int availableBooks = 0;
    int lowStockBooks = 0; // Sách sắp hết (số lượng <= 2)
    
    // Duyệt qua tất cả sách (giả sử có cách lấy danh sách sách)
    for (int i = 0; i < 100; i++) {
        Book* book = bookManager.GetBookByID(i + 1); // Giả sử ID từ 1-100
        if (book) {
            totalBooks++;
            if (book->getQuantity() > 0) {
                availableBooks++;
            }
            if (book->getQuantity() <= 2 && book->getQuantity() > 0) {
                lowStockBooks++;
            }
        }
    }
    
    cout << "\n--- CHI TIET TINH TRANG SACH ---\n";
    cout << "Tong so dau sach: " << totalBooks << endl;
    cout << "So sach con trong kho: " << availableBooks << endl;
    cout << "So sach sap het (<= 2): " << lowStockBooks << endl;
    
    if (totalBooks > 0) {
        double availableRate = (double)availableBooks / totalBooks * 100;
        cout << fixed << setprecision(1);
        cout << "Ty le sach con trong kho: " << availableRate << "%" << endl;
    }
}

void StatisticsManager::ThongKeTheoTheLoai() {
    cout << "\n=== THONG KE SACH THEO THE LOAI ===\n";
    
    map<string, int> categoryStats;
    
    // Thống kê sách theo thể loại
    for (int i = 0; i < 100; i++) {
        Book* book = bookManager.GetBookByID(i + 1);
        if (book && book->getCategory()[0] != '\0') {
            string category = book->getCategory();
            categoryStats[category]++;
        }
    }
    
    if (categoryStats.empty()) {
        cout << "Khong co du lieu the loai.\n";
        return;
    }
    
    cout << left << setw(25) << "THE LOAI" << setw(15) << "SO LUONG" << "TY LE" << endl;
    cout << setfill('-') << setw(50) << "-" << setfill(' ') << endl;
    
    int totalBooks = 0;
    for (const auto& stat : categoryStats) {
        totalBooks += stat.second;
    }
    
    // Sắp xếp theo số lượng giảm dần
    vector<pair<string, int>> sortedCategories(categoryStats.begin(), categoryStats.end());
    sort(sortedCategories.begin(), sortedCategories.end(), 
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });
    
    for (const auto& stat : sortedCategories) {
        double percentage = (double)stat.second / totalBooks * 100;
        cout << left << setw(25) << stat.first 
             << setw(15) << stat.second 
             << fixed << setprecision(1) << percentage << "%" << endl;
    }
    
    cout << setfill('-') << setw(50) << "-" << setfill(' ') << endl;
    cout << left << setw(25) << "TONG CONG" << setw(15) << totalBooks << "100%" << endl;
}

void StatisticsManager::ThongKeTheoTacGia() {
    cout << "\n=== THONG KE SACH THEO TAC GIA ===\n";
    
    map<string, int> authorStats;
    
    // Thống kê sách theo tác giả
    for (int i = 0; i < 100; i++) {
        Book* book = bookManager.GetBookByID(i + 1);
        if (book && book->getAuthor()[0] != '\0') {
            string author = book->getAuthor();
            authorStats[author]++;
        }
    }
    
    if (authorStats.empty()) {
        cout << "Khong co du lieu tac gia.\n";
        return;
    }
    
    cout << left << setw(30) << "TAC GIA" << setw(15) << "SO SACH" << "TY LE" << endl;
    cout << setfill('-') << setw(55) << "-" << setfill(' ') << endl;
    
    int totalBooks = 0;
    for (const auto& stat : authorStats) {
        totalBooks += stat.second;
    }
    
    // Sắp xếp theo số sách giảm dần
    vector<pair<string, int>> sortedAuthors(authorStats.begin(), authorStats.end());
    sort(sortedAuthors.begin(), sortedAuthors.end(), 
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });
    
    int count = 0;
    for (const auto& stat : sortedAuthors) {
        if (count >= 15) break; // Chỉ hiển thị top 15
        
        double percentage = (double)stat.second / totalBooks * 100;
        cout << left << setw(30) << stat.first 
             << setw(15) << stat.second 
             << fixed << setprecision(1) << percentage << "%" << endl;
        count++;
    }
    
    cout << setfill('-') << setw(55) << "-" << setfill(' ') << endl;
    cout << left << setw(30) << "TONG CONG" << setw(15) << totalBooks << "100%" << endl;
}

void StatisticsManager::ThongKeTinhTrangMuon() {
    cout << "\n=== TINH TRANG MUON SACH HIEN TAI ===\n";
    
    map<int, int> bookBorrowCount; // bookID -> số lượng đang mượn
    
    const Person* allUsers = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();
    
    // Đếm số sách đang được mượn
    for (int i = 0; i < userCount; i++) {
        User user;
        user.LoadUserByID(to_string(allUsers[i].getID()));
        
        const vector<BorrowedItem>& history = user.getTransactionHistory();
        for (const auto& item : history) {
            if (!item.getIsReturned()) {
                bookBorrowCount[item.getBookID()]++;
            }
        }
    }
    
    if (bookBorrowCount.empty()) {
        cout << "Khong co sach nao dang duoc muon.\n";
        return;
    }
    
    cout << left << setw(8) << "ID SACH" << setw(40) << "TEN SACH" << setw(20) << "SO LUONG DANG MUON" << endl;
    cout << setfill('-') << setw(68) << "-" << setfill(' ') << endl;
    
    // Sắp xếp theo số lượng mượn giảm dần
    vector<pair<int, int>> sortedBooks(bookBorrowCount.begin(), bookBorrowCount.end());
    sort(sortedBooks.begin(), sortedBooks.end(), 
         [](const pair<int, int>& a, const pair<int, int>& b) {
             return a.second > b.second;
         });
    
    for (const auto& stat : sortedBooks) {
        Book* book = bookManager.GetBookByID(stat.first);
        if (book) {
            string title = book->getTitle();
            if (title.length() > 39) {
                title = title.substr(0, 36) + "...";
            }
            cout << left << setw(8) << stat.first 
                 << setw(40) << title 
                 << setw(20) << stat.second << endl;
        }
    }
    
    cout << setfill('-') << setw(68) << "-" << setfill(' ') << endl;
    cout << "Tong so sach dang duoc muon: " << bookBorrowCount.size() << endl;
}

void StatisticsManager::ThongKeSachMuonNhieu() {
    cout << "\n=== TOP SACH DUOC MUON NHIEU NHAT (ALL TIME) ===\n";
    
    map<int, int> bookBorrowCount; // bookID -> tổng số lần mượn
    
    const Person* allUsers = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();
    
    // Đếm tổng số lần mượn của từng sách
    for (int i = 0; i < userCount; i++) {
        User user;
        user.LoadUserByID(to_string(allUsers[i].getID()));
        
        const vector<BorrowedItem>& history = user.getTransactionHistory();
        for (const auto& item : history) {
            bookBorrowCount[item.getBookID()]++;
        }
    }
    
    if (bookBorrowCount.empty()) {
        cout << "Chua co thong tin muon sach.\n";
        return;
    }
    
    cout << left << setw(5) << "STT" << setw(8) << "ID" << setw(40) << "TEN SACH" << setw(25) << "TONG SO LAN MUON" << endl;
    cout << setfill('-') << setw(78) << "-" << setfill(' ') << endl;
    
    // Sắp xếp theo tổng số lần mượn giảm dần
    vector<pair<int, int>> sortedBooks(bookBorrowCount.begin(), bookBorrowCount.end());
    sort(sortedBooks.begin(), sortedBooks.end(), 
         [](const pair<int, int>& a, const pair<int, int>& b) {
             return a.second > b.second;
         });
    
    int rank = 1;
    for (const auto& stat : sortedBooks) {
        if (rank > 10) break; // Top 10
        
        Book* book = bookManager.GetBookByID(stat.first);
        if (book) {
            string title = book->getTitle();
            if (title.length() > 39) {
                title = title.substr(0, 36) + "...";
            }
            cout << left << setw(5) << rank++
                 << setw(8) << stat.first 
                 << setw(40) << title 
                 << setw(25) << stat.second << endl;
        }
    }
    
    cout << setfill('-') << setw(78) << "-" << setfill(' ') << endl;
}

void StatisticsManager::ThongKeDocGia() {
    cout << "\n=== THONG KE DOC GIA ===\n";
    
    const Person* allUsers = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();
    
    cout << "Tong so doc gia: " << userCount << endl;
    
    // Thống kê độc giả có sách mượn
    int usersWithBorrows = 0;
    int totalBorrowedBooks = 0;
    int usersWithOverdue = 0;
    
    for (int i = 0; i < userCount; i++) {
        User user;
        user.LoadUserByID(to_string(allUsers[i].getID()));
        
        int borrowedCount = user.getCurrentBorrowedCount();
        if (borrowedCount > 0) {
            usersWithBorrows++;
            totalBorrowedBooks += borrowedCount;
            
            // Kiểm tra quá hạn (cần thêm logic kiểm tra ngày)
            // if (user.HasOverdueBooks()) {
            //     usersWithOverdue++;
            // }
        }
    }
    
    cout << "\n--- TINH TRANG MUON SACH ---\n";
    cout << "So doc gia dang muon sach: " << usersWithBorrows << endl;
    cout << "So doc gia khong muon sach: " << userCount - usersWithBorrows << endl;
    cout << "Tong so sach dang duoc muon: " << totalBorrowedBooks << endl;
    cout << "So doc gia co sach qua han: " << usersWithOverdue << endl;
    
    if (userCount > 0) {
        double borrowRate = (double)usersWithBorrows / userCount * 100;
        cout << fixed << setprecision(1);
        cout << "Ty le doc gia dang muon sach: " << borrowRate << "%" << endl;
    }
}

void StatisticsManager::ThongKeMuonTra() {
    cout << "\n=== THONG KE MUON/TRA SACH ===\n";
    
    int choice;
    cout << "1. Danh sach muon sach\n";
    cout << "2. Lich su muon sach\n"; 
    cout << "3. Lich su tra sach\n";
    cout << "4. Lich su giao dich tat ca doc gia\n";
    cout << "Chon: ";
    
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
    cin.ignore(100, '\n');
    
    switch(choice) {
        case 1:
            borrowManager.ShowBorrowRecords();
            Utils::AskReturnToMenu();
            break;
        case 2:
            borrowManager.ShowHistoryBorrow();
            Utils::AskReturnToMenu();
            break;
        case 3:
            borrowManager.ShowHistoryReturn();
            Utils::AskReturnToMenu();
            break;
        case 4:
            borrowManager.ShowAllUsersTransactionHistory(userManager, bookManager);
            Utils::AskReturnToMenu();
            break;
        default:
            cout << "Lua chon khong hop le!\n";
    }
}

void StatisticsManager::BaoCaoTongHop() {
    cout << "\n=== BAO CAO TONG HOP THU VIEN ===\n";
    
    // Thống kê tổng quan
    const Person* allUsers = userManager.GetAllUsers();
    int userCount = userManager.GetUserCount();
    
    // Đếm tổng số sách
    int totalBooks = 0;
    int totalBorrowed = 0;
    
    for (int i = 0; i < 100; i++) {
        Book* book = bookManager.GetBookByID(i + 1);
        if (book) {
            totalBooks++;
            // Giả sử số lượng mượn = totalQuantity - quantity
            // int borrowed = book->getTotalQuantity() - book->getQuantity();
            // totalBorrowed += borrowed;
        }
    }
    
    // Đếm số sách đang mượn từ users
    int actualBorrowed = 0;
    for (int i = 0; i < userCount; i++) {
        User user;
        user.LoadUserByID(to_string(allUsers[i].getID()));
        actualBorrowed += user.getCurrentBorrowedCount();
    }
    
    cout << "=== TONG QUAN ===\n";
    cout << "Tong so doc gia: " << userCount << endl;
    cout << "Tong so dau sach: " << totalBooks << endl;
    cout << "Tong so sach dang muon: " << actualBorrowed << endl;
    
    if (totalBooks > 0) {
        double borrowRate = (double)actualBorrowed / totalBooks * 100;
        cout << fixed << setprecision(1);
        cout << "Ty le muon sach: " << borrowRate << "%" << endl;
    }
    
    cout << "\n=== HOAT DONG GAN DAY ===\n";
    // Có thể thêm thống kê hoạt động trong tháng
    cout << "So giao dịch trong tháng: (Du lieu dang phat trien)" << endl;
}