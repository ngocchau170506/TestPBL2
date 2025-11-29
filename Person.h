#pragma once
#include <iostream>
#include <sstream>
#include <cstring>

using namespace std;

class Person
{
private:
    int ID;
    char name[100];
    char date[11];
    char phone[11];
    char email[100];
    char password[50];

    void CopyStringManual(char* dest, const char* src, size_t dest_size);
    int CompareStringManual(const char* s1, const char* s2) const;
    void IntToStringManual(int num, char* buffer, size_t buffer_size);
    
public:
    Person();
    Person(int id, const char n[], const char d[], const char p[], const char e[], const char pw[]);
    virtual ~Person();

    void setName(const char n[]);
    void setDate(const char d[]);
    void setPhone(const char p[]);
    void setEmail(const char e[]);
    void setID(int id);
    void setPassword(const char pw[]);

    int getID() const;
    const char* getName() const;
    const char* getDate() const;
    const char* getPhone() const;
    const char* getEmail() const;
    const char* getPassword() const;

    bool ChangePassword(); 

    virtual void Show() const;
    void Update();
    void UpdateByUser();
    void InputAccountDetails();
    string getLastName() const;
    void InputUserDetails();

    friend ostream& operator<<(ostream& out, const Person& p);
    friend istream& operator>>(istream& in, Person& p);
};

string Person::getLastName() const {
    string fullName = this->getName();
    stringstream ss(fullName);
    string namePart;
    string lastName = "";

    // Duyệt qua từng từ trong tên và lấy từ cuối cùng
    while (getline(ss, namePart, ' ')) {
        // Bỏ qua các chuỗi trống do nhiều khoảng trắng
        if (!namePart.empty()) {
            lastName = namePart; 
        }
    }
    return lastName;
}

void Person::InputUserDetails()
{
    char tempStr[100];
    cout << "------------------------------------------\n";

    // 1. Nhập Tên
    do {
        cout << "Nhap Ten doc gia (VD: Nguyen Van A): ";
        cin.getline(tempStr, sizeof(tempStr));
        if (strlen(tempStr) == 0) {
            cout << "Ten khong duoc de trong.\n";
        }
    } while (strlen(tempStr) == 0);
    setName(tempStr);

    // 2. Nhập Ngày sinh
    do {
        cout << "Nhap Ngay sinh (DD/MM/YYYY): ";
        cin.getline(tempStr, sizeof(tempStr));
        if (strlen(tempStr) == 0) {
            cout << "Ngay sinh khong duoc de trong.\n";
        }
    } while (strlen(tempStr) == 0);
    setDate(tempStr);

    // 3. Nhập Số điện thoại
    do {
        cout << "Nhap So dien thoai: ";
        cin.getline(tempStr, sizeof(tempStr));
        if (strlen(tempStr) == 0) {
            cout << "So dien thoai khong duoc de trong.\n";
        }
    } while (strlen(tempStr) == 0);
    setPhone(tempStr);

    // 4. Nhập Email
    do {
        cout << "Nhap Email: ";
        cin.getline(tempStr, sizeof(tempStr));
        if (strlen(tempStr) == 0) {
            cout << "Email khong duoc de trong.\n";
        }
    } while (strlen(tempStr) == 0);
    setEmail(tempStr);
    
    cout << "------------------------------------------\n";
}
void Person::CopyStringManual(char* dest, const char* src, size_t dest_size) 
{
    size_t i = 0;
    while (i < dest_size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0'; 
}

int Person::CompareStringManual(const char* s1, const char* s2) const 
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void Person::IntToStringManual(int num, char* buffer, size_t buffer_size)
{
    if (buffer_size == 0) return;
    if (num == 0) {
        if (buffer_size > 1) {
            buffer[0] = '0';
            buffer[1] = '\0';
        } else if (buffer_size == 1) {
            buffer[0] = '\0';
        }
        return;
    }
    bool isNegative = false;
    if (num < 0) {
        isNegative = true;
        num = -num;
    }

    int i = 0;
    while (num != 0 && i < buffer_size - 1) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    if (isNegative && i < buffer_size - 1) {
        buffer[i++] = '-';
    }
    buffer[i] = '\0';
 
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}


Person::Person() 
{
    ID = 0;
    name[0] = '\0';
    date[0] = '\0';
    phone[0] = '\0';
    email[0] = '\0';
    password[0] = '\0';
}

Person::Person(int id, const char n[], const char d[], const char p[], const char e[], const char pw[]) 
{
    this->ID = id;
    CopyStringManual(name, n, sizeof(name));
    CopyStringManual(date, d, sizeof(date));
    CopyStringManual(phone, p, sizeof(phone));
    CopyStringManual(email, e, sizeof(email));
}

Person::~Person() 
{}

void Person::setName(const char n[]) 
{
    CopyStringManual(name, n, sizeof(name));
}
void Person::setDate(const char d[]) 
{
    CopyStringManual(date, d, sizeof(date));
}
void Person::setPhone(const char p[]) 
{
    CopyStringManual(phone, p, sizeof(phone));
}
void Person::setEmail(const char e[]) 
{
    CopyStringManual(email, e, sizeof(email));
}
void Person::setID(int id) 
{
    ID = id;
}
void Person::setPassword(const char p[])
{
    CopyStringManual(password, p, sizeof(password));
}

int Person::getID() const { return ID; }
const char* Person::getName() const { return name; }
const char* Person::getDate() const { return date; }
const char* Person::getPhone() const { return phone; }
const char* Person::getEmail() const { return email; }
const char* Person::getPassword() const { return password; }

void Person::InputAccountDetails() 
{
    char default_pw[50];
    IntToStringManual(ID, default_pw, sizeof(default_pw)); 
    setPassword(default_pw);
    cout << "Mat Khau ban dau la: " << default_pw << "\n";
    
    char input[100];

    cout << "Nhap Ho ten: ";
    cin.getline(input, sizeof(input));
    setName(input);

    char dateInput[11];
    cout << "Nhap Ngay sinh (dd/mm/yyyy): ";
    cin.getline(dateInput, sizeof(date));
    setDate(dateInput); 

    char phoneInput[11];
    cout << "Nhap So dien thoai: ";
    cin.getline(phoneInput, sizeof(phone));
    setPhone(phoneInput); 

    char emailInput[100];
    cout << "Nhap Email: ";
    cin.getline(emailInput, sizeof(email));
    setEmail(emailInput); 

    cout << "Tao tai khoan thanh cong!\n";
}

void Person::Show() const 
{
    cout << "--- Thong tin ca nhan ---\n";
    cout << "ID: " << ID << "\n";
    cout << "Ho Ten: " << name << "\n";
    cout << "Ngay Sinh: " << date << "\n";
    cout << "So dien Thoai: " << phone << "\n";
    cout << "Email: " << email << "\n";
}

ostream& operator<<(ostream& out, const Person& p)
{
    out << p.ID << "," << p.password << "," 
        << p.name << "," << p.date << "," << p.phone << "," << p.email;
    return out;
}

istream& operator>>(istream& in, Person& p) 
{
    cout << "Nhap ID: ";
    while (!(in >> p.ID)) {
        cout << "ID khong hop le. Nhap lai ID: ";
        in.clear();
        in.ignore(100, '\n');
    }
    in.ignore(); 

    cout << "Nhap Mat Khau: ";
    in.getline(p.password, sizeof(p.password));

    cout << "Nhap Ho ten: ";
    in.getline(p.name, sizeof(p.name));
    
    cout << "Nhap Ngay sinh: ";
    in.getline(p.date, sizeof(p.date));

    cout << "Nhap So dien thoai: ";
    in.getline(p.phone, sizeof(p.phone));

    cout << "Nhap Email: ";
    in.getline(p.email, sizeof(p.email));

    return in;
}

bool Person::ChangePassword()
{
    char oldPassword[50];
    char newPassword1[50];
    char newPassword2[50];

    cout << "Nhap Mat Khau cu: ";
    cin.getline(oldPassword, sizeof(oldPassword));

    if (CompareStringManual(oldPassword, password) != 0)
    {
        cout << "Khong thay doi mat khau!\n";
        return false;
    }

    do {
        cout << "Nhap Mat Khau moi: ";
        cin.getline(newPassword1, sizeof(newPassword1));

        cout << "Nhap lai Mat Khau moi: ";
        cin.getline(newPassword2, sizeof(newPassword2));

        if (CompareStringManual(newPassword1, newPassword2) == 0)
        {
            setPassword(newPassword1); 
            cout << "Thay doi mat khau thanh cong!\n";

            ifstream inFile("users.txt");
            if (!inFile.is_open()) {
                cout << "Loi: Khong the mo file users.txt de cap nhat!\n";
                return true; 
            }

            ofstream outFile("temp.txt");
            if (!outFile.is_open()) {
                cout << "Loi: Khong the tao file tam!\n";
                inFile.close();
                return true;
            }

            string line;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string idStr, pw, name, date, phone, email;
                getline(ss, idStr, ',');
                getline(ss, pw, ',');
                getline(ss, name, ',');
                getline(ss, date, ',');
                getline(ss, phone, ',');
                getline(ss, email, ',');

                int fileID = stoi(idStr);
                if (fileID == ID) {
                    outFile << ID << "," << password << "," << name << "," << date << "," << phone << "," << email << "\n";
                } else {
                    outFile << line << "\n";
                }
            }

            inFile.close();
            outFile.close();

            remove("users.txt");
            rename("temp.txt", "users.txt");

            return true;
        }
        else
        {
            cout << "Loi: Mat khau moi nhap lai khong khop. Vui long nhap lai.\n";
        }
    } while (true); 
}

void Person::Update() 
{
    char input[100];

    cout << "\n--- CAP NHAT THONG TIN CA NHAN ---\n";

    //cin.ignore(100, '\n');

    ChangePassword();

    cout << "Ten hien tai: " << name << endl;
    cout << "Nhap Ten moi: ";
    input[0] = '\0';
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') 
    {
        setName(input);
    }

    cout << "Ngay sinh hien tai: " << date << endl;
    cout << "Nhap Ngay sinh moi: ";
    input[0] = '\0';
    cin.getline(input, sizeof(date));
    if (input[0] != '\0') 
    {
        setDate(input);
    }

    cout << "SDT hien tai: " << phone << endl;
    cout << "Nhap So dien thoai moi: ";
    input[0] = '\0';
    cin.getline(input, sizeof(phone));
    if (input[0] != '\0') 
    {
        setPhone(input);
    }

    cout << "Email hien tai: " << email << endl;
    cout << "Nhap Email moi: ";
    input[0] = '\0';
    cin.getline(input, sizeof(email));
    if (input[0] != '\0') 
    {
        setEmail(input);
    }

    cout << "\nCap nhat thong tin ca nhan thanh cong!\n";
    Show();
}

void Person::UpdateByUser() 
{
    char input[100];

    cout << "\n--- CAP NHAT THONG TIN CA NHAN ---\n";

    //cin.ignore(100, '\n');

    cout << "Ten hien tai: " << name << endl;
    cout << "Nhap Ten moi: ";
    input[0] = '\0';
    cin.getline(input, sizeof(input));
    if (input[0] != '\0') 
    {
        setName(input);
    }

    cout << "Ngay sinh hien tai: " << date << endl;
    cout << "Nhap Ngay sinh moi: ";
    input[0] = '\0';
    cin.getline(input, sizeof(date));
    if (input[0] != '\0') 
    {
        setDate(input);
    }

    cout << "SDT hien tai: " << phone << endl;
    cout << "Nhap So dien thoai moi: ";
    input[0] = '\0';
    cin.getline(input, sizeof(phone));
    if (input[0] != '\0') 
    {
        setPhone(input);
    }

    cout << "Email hien tai: " << email << endl;
    cout << "Nhap Email moi: ";
    input[0] = '\0';
    cin.getline(input, sizeof(email));
    if (input[0] != '\0') 
    {
        setEmail(input);
    }

    cout << "\nCap nhat thong tin ca nhan thanh cong!\n";
    Show();
}