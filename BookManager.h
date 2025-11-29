#pragma once
#include "Book.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class BookManager
{
private:
    Book books[100];
    int bookCount;
    int nextAvailableID;
    
    char filePath[100];
    bool IsBookIDExist(int id);
    char userFilePath[100];

    char toLower(char c);
    void trim(char* str);
    bool containsIgnoreCase(const char* text, const char* keyword);
    string GenerateNextBookID();
public:
    BookManager();

    void LoadBooksFromFile();
    void SaveBooksToFile() const;

    void AddBook();
    void ShowAllBooks() const;
    void SearchBookByTitle();
    void SearchBookByAuthor(const char* author) const;
    void DeleteBookByID(int id);
    void UpdateBookByID(int id);
    void ShowStockReport() const;
    void ShowStockReportPerBook() const;
    
    const Book* GetBookByID(int id) const; 
    Book* GetBookByID(int id);
};

string BookManager::GenerateNextBookID()
{
    int nextIDNumber = bookCount + 1;
    int newID = nextAvailableID;
    
    stringstream ss;
    ss << setfill('0') << newID;
    nextAvailableID++;
    
    return ss.str();
}    

void BookManager::ShowStockReport() const
{
    int totalCopies = 0;
    int availableCopies = 0;
    int borrowedCopies = 0; 
    
    for (int i = 0; i < bookCount; ++i) {
        totalCopies += books[i].getTotalQuantity(); 
        availableCopies += books[i].getQuantity();
    }
    
    borrowedCopies = totalCopies - availableCopies;
    
    cout << "\n========== BAO CAO THONG KE SACH ==========\n";
    cout << "Tong so dau sach: " << bookCount << "\n";
    cout << "Tong so ban sao (copies): " << totalCopies << "\n";
    cout << "   - So ban sao CON LAI: " << availableCopies << "\n";
    cout << "   - So ban sao DANG MUON: " << borrowedCopies << "\n";
    cout << "===========================================\n";
}

void BookManager::SearchBookByAuthor(const char* author) const
{
    cout << "\n--- KET QUA TIM KIEM TAC GIA: " << author << " ---\n";
    int foundCount = 0;
    
    for (int i = 0; i < bookCount; ++i) {
        
        if (Utils::FindSubstringManual(books[i].getAuthor(), author)) 
        {
            books[i].Show();
            foundCount++;
        }
    }
    
    if (foundCount == 0) {
        cout << "Khong tim thay sach nao cua tac gia '" << author << "'.\n";
    }
}
Book* BookManager::GetBookByID(int id)
{
    for (int i = 0; i < bookCount; ++i)
    {
        if (books[i].getID() == id)
            return &books[i]; 
    }
    return nullptr;
}

BookManager::BookManager() 
{
    bookCount = 0;
    Utils::CopyStringManual(userFilePath, "book.txt", sizeof(userFilePath));
    LoadBooksFromFile();
}

bool BookManager::IsBookIDExist(int id)
{
    for (int i = 0; i < bookCount; ++i)
        if (books[i].getID() == id)
            return true;
    return false;
}

void BookManager::LoadBooksFromFile()
{
    ifstream inFile(userFilePath);
    if (!inFile)
        return;

    bookCount = 0;
    char line[512];
    while (inFile.getline(line, sizeof(line)))
    {
        if (line[0] == '\0')
            continue;

        char fields[7][200];
        int fld = 0, pos = 0;
        for (int i = 0; line[i] != '\0'; ++i)
        {
            if (line[i] == ',')
            {
                fields[fld][pos] = '\0';
                fld++;
                pos = 0;
            }
            else
            {
                if (pos < 199)
                    fields[fld][pos++] = line[i];
            }
        }
        fields[fld][pos] = '\0'; 

        if (fld < 6)
            continue; 

        int id = atoi(fields[0]);
        int pubYear = atoi(fields[4]);
        int quantity = atoi(fields[5]);
        bool borrowed = (fields[6][0] == '1');

        books[bookCount] = Book(id, fields[1], fields[2], fields[3], pubYear, quantity, borrowed);
        bookCount++;
        if (bookCount >= 100)
            break;
    }

    inFile.close();
}

void BookManager::SaveBooksToFile() const
{
    ofstream outFile(userFilePath);
    for (int i = 0; i < bookCount; ++i)
    {
        outFile << books[i].getID() << ","
                << books[i].getTitle() << ","
                << books[i].getAuthor() << ","
                << books[i].getCategory() << ","
                << books[i].getPubYear() << ","
                << books[i].getQuantity() << ","
                << (books[i].getBorrowed() ? 1 : 0);
        if (i < bookCount - 1)
            outFile << "\n";
    }
    outFile.close();
}

void BookManager::AddBook()
{
    char choice;
    do
    {
        cout << "\n--- THEM SACH MOI ---\n";
        string newIDStr = GenerateNextBookID();
        int id = Utils::StringToIntManual(newIDStr.c_str());
        
        cout << "ID sach tu dong: " << newIDStr << "\n";
        
        cin.ignore();
        books[bookCount].setID(id);
        books[bookCount].InputBook();
        books[bookCount].setBorrowed(false); 
        bookCount++;

        SaveBooksToFile();

        cout << "Ban co muon them sach khac khong? (Y/N): ";
        cin >> choice;
        cin.ignore(100, '\n');
        if (choice >= 'a' && choice <= 'z')
            choice -= 32;

    } while (choice == 'Y');
}

// void BookManager::ShowAllBooks() const
// {
//     if (bookCount == 0)
//     {
//         cout << "Chua co sach nao!\n";
//         return;
//     }

//     for (int i = 0; i < bookCount; ++i)
//     {
//         cout << "\nSach thu " << i + 1 << ":\n";
//         books[i].Show();
//     }
// }
void BookManager::ShowAllBooks() const
{
    if (bookCount == 0)
    {
        cout << "Chua co sach nao!\n";
        return;
    }

    // Thiết lập độ rộng cột cho bảng
    const int ID_WIDTH = 8;
    const int TITLE_WIDTH = 45;
    const int AUTHOR_WIDTH = 30;
    const int PUBLISHER_WIDTH = 30;
    const int YEAR_WIDTH = 10;
    const int QUANTITY_WIDTH = 10;
    const int PRICE_WIDTH = 15;
    const int TOTAL_WIDTH = ID_WIDTH + TITLE_WIDTH + AUTHOR_WIDTH + PUBLISHER_WIDTH  + QUANTITY_WIDTH  + 5;

    cout << "\n========== DANH SACH TAT CA SACH (" << bookCount << " cuon) ==========\n";

    // --- 1. In Tiêu đề Bảng ---
    cout << setfill(' '); // Đặt ký tự lấp đầy là khoảng trắng

    cout << left << setw(ID_WIDTH) << "ID"
         << left << setw(TITLE_WIDTH) << "Ten sach"
         << left << setw(AUTHOR_WIDTH) << "Tac gia"
         << left << setw(PUBLISHER_WIDTH) << "NXB"
        //  << left << setw(YEAR_WIDTH) << "Nam XB"
         << left << setw(QUANTITY_WIDTH) << "So luong"
        //  << right << setw(PRICE_WIDTH) << "Gia"
         << endl;

    // --- 2. In Dòng Phân cách ---
    cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;

    // --- 3. Duyệt và In Dữ liệu ---
    for (int i = 0; i < bookCount; ++i)
    {
        // Lấy thông tin sách
        const Book& book = books[i];

        // In từng cột (Giả định class Book có các hàm getter tương ứng)
        cout << left << setw(ID_WIDTH) << book.getID()
             << left << setw(TITLE_WIDTH) << book.getTitle()
             << left << setw(AUTHOR_WIDTH) << book.getAuthor()
             << left << setw(YEAR_WIDTH) << book.getYear()
             << left << setw(QUANTITY_WIDTH) << book.getQuantity()
             << endl;
    }

    // --- 4. In Dòng Kết thúc ---
    cout << setfill('-') << setw(TOTAL_WIDTH) << "" << setfill(' ') << endl;
}

char BookManager::toLower(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

void BookManager::trim(char* str) {
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t') start++;

    int end = 0;
    while (str[end] != '\0') end++;
    end--; 
    while (end >= start && (str[end] == ' ' || str[end] == '\t')) end--;

    int j = 0;
    for (int i = start; i <= end; ++i) {
        str[j++] = str[i];
    }
    str[j] = '\0';
}

bool BookManager::containsIgnoreCase(const char* text, const char* keyword) {
    for (int i = 0; text[i] != '\0'; ++i) {
        int j = 0;
        while (keyword[j] != '\0' && text[i + j] != '\0' &&
                toLower(text[i + j]) == toLower(keyword[j])) {
            j++;
        }
        if (keyword[j] == '\0') return true; 
    }
    return false;
}

void BookManager::SearchBookByTitle() {
    char keyword[100];
    cout << "Nhap ten sach can tim: ";
    cin.ignore();
    cin.getline(keyword, 100);
    trim(keyword);

    bool found = false;
    for (int i = 0; i < bookCount; ++i) {
        const char* title = books[i].getTitle();
        if (containsIgnoreCase(title, keyword)) {
            cout << "\n==> Tim thay sach:\n";
            books[i].Show();
            found = true;
        }
    }

    if (!found) cout << "Khong tim thay sach phu hop.\n";
}

void BookManager::DeleteBookByID(int id)
{
    char choice;
    do
    {
        int pos = -1;
        for (int i = 0; i < bookCount; ++i)
            if (books[i].getID() == id)
            {
                pos = i;
                break;
            }

        if (pos == -1)
        {
            cout << "Khong tim thay sach co ID " << id << endl;
        }
        else
        {
            cout << "\nBan co chac chan muon xoa sach sau khong?\n";
            books[pos].Show();
            cout << "Ban co chac chan muon xoa sach nay khong? (Y/N): ";
            char confirm;
            cin >> confirm;
            cin.ignore(100, '\n');
            if (confirm >= 'a' && confirm <= 'z')
                confirm -= 32;

            if (confirm == 'Y')
            {
                for (int i = pos; i < bookCount - 1; ++i)
                    books[i] = books[i + 1];
                bookCount--;
                SaveBooksToFile();
                cout << "Xoa sach thanh cong!\n";
            }
            else
            {
                cout << "Da huy thao tac xoa.\n";
            }
        }

        cout << "\nBan co muon xoa sach khac khong? (Y/N): ";
        cin >> choice;
        cin.ignore(100, '\n');
        if (choice >= 'a' && choice <= 'z')
            choice -= 32;

        if (choice == 'Y')
        {
            cout << "Nhap ID sach can xoa: ";
            if (!(cin >> id))
            {
                cin.clear();
                cin.ignore(100, '\n');
                break;
            }
        }

    } while (choice == 'Y');
}
void BookManager::ShowStockReportPerBook() const
{
    if (bookCount == 0) {
        cout << "Thu vien chua co sach nao!\n";
        return;
    }

    cout << "\n===== BAO CAO THEO TUNG SACH =====\n";
    cout << "ID\tTen sach\t\tTong ban\tCon lai\tDang muon\n";
    cout << "-------------------------------------------------------------\n";

    for (int i = 0; i < bookCount; ++i)
    {
        int totalCopies = books[i].getTotalQuantity();
        int available = books[i].getQuantity();
        int borrowed = totalCopies - available;

        cout << books[i].getID() << "\t"
             << books[i].getTitle() << "\t\t"
             << totalCopies << "\t\t"
             << available << "\t"
             << borrowed << "\n";
    }

    cout << "=============================================================\n";
}


// Trong BookManager.cpp
// Trong BookManager.cpp

void BookManager::UpdateBookByID(int id)
{
    // Giả định hàm GetBookByID non-const đã có và trả về Book*
    Book* book = GetBookByID(id); 
    if (!book) {
        cout << "Khong tim thay sach co ID: " << id << "\n";
        return;
    }

    cout << "\n--- CAP NHAT SACH ID: " << id << " ---\n";
    cout << "--- THONG TIN CU ---\n";
    book->Show(); // Hiển thị thông tin cũ
    cout << "--------------------\n";

    char input[200];
    
    // 1. Cập nhật Tên sách (Title)
    cout << "Nhap Ten sach moi: ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') {
        book->setTitle(input);
    }
    
    // 2. Cập nhật Tác giả (Author)
    cout << "Nhap Tac gia moi: ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') {
        book->setAuthor(input);
    }
    
  
    cout << "Nhap Nam xuat ban moi: ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') {
        int newYear = Utils::CharArrayToIntManual(input);
        if (newYear > 0) { 
            book->setYear(newYear); 
        } else {
            cout << "Canh bao: Nam nhap vao khong hop le hoac <= 0. Giu nguyen gia tri cu.\n";
        }
    }
    
    // 4. Cập nhật Thể loại (Category)
    cout << "Nhap The loai moi: ";
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') {
        book->setCategory(input);
    }

   
    cout << "\nCap nhat thong tin sach thanh cong!\n";
    SaveBooksToFile(); 
}

const Book* BookManager::GetBookByID(int id) const
{
    for (int i = 0; i < bookCount; ++i)
    {
        if (books[i].getID() == id)
            return &books[i];
    }
    return nullptr;
}