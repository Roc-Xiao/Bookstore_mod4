//
// Created by 肖鹏楊 on 24-12-19.
//
#include<iostream>
#include<iomanip>
#include<string>
#include <utility>
#include <windows.h> //UTF-8设置

using namespace std;

class BookData  //定义类
{
private:
    string isbn;
    string bookTitle;
    int qtyOnHand;
    double retail;

public:
    explicit BookData(string i = "", string t = "", int q = 0, double r = 0.0)   //初始化BookData
    {
        isbn = move(i);
        bookTitle = move(t);
        qtyOnHand = q;
        retail = r;
    }

    void setISBN(const string& i)   //输入isbn
    {
        isbn = i;
    }
    void setTitle(const string& t)  //输入书名
    {
        bookTitle = t;
    }
    void setQty(int q)  //输入库存
    {
        qtyOnHand = q;
    }
    void setRetail(double r)    //输入零售价
    {
        retail = r;
    }

    [[nodiscard]] string getISBN() const  //输出isbn
    {
        return isbn;
    }
    [[nodiscard]] string getTitle() const //输出书名
    {
        return bookTitle;
    }
    [[nodiscard]] int getQty() const  //输出库存
    {
        return qtyOnHand;
    }
    [[nodiscard]] double getRetail() const    //输出零售价
    {
        return retail;
    }

    void BookInfo() const   //显示书籍信息
    {
        cout << setw(50) << setfill('-') << " " << endl;
        cout << setw(35) << "长空御风图书管理系统" << endl;
        cout << setw(42) << endl;
        cout << setw(15) << "ISBN 号:" << isbn << endl;
        cout << setw(15) << "书名:" << bookTitle << endl;
        cout << setw(15) << "库存量:" << qtyOnHand << endl;
        cout << setw(15) << "零售价: RMB " << fixed << setprecision(2) << retail << endl;
        cout << setw(50) << setfill('-') << " " << endl;
    }
};

const int MAX_BOOKS = 100;
BookData books[MAX_BOOKS]; //创建书库
const double TAX = 0.06;

void mainMenu();
void Sale(BookData books[], int& bookCount);
void bookMenu(BookData books[], int& bookCount);
void table(BookData books[], int bookCount);
int findBookByISBN(BookData books[], int bookCount, const string& isbn);
void deleteBook(int& bookCount);
void lookUpBook();
void editBook();
void addBook(int& bookCount);

int main()
{
    // 设置控制台输出和输入编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    BookData books[MAX_BOOKS];
    int bookCount = 0;  //初始化库存
    int choice;

    do {
        mainMenu();
        cin >> choice;

        switch (choice)
        {
        case 1: //选项一，收银模块
            Sale(books, bookCount);
            break;
        case 2: //选项二，书库管理模块
            bookMenu(books, bookCount);
            break;
        case 3: //选项三，报表模块
            table(books, bookCount);
            break;
        case 4: //选项四，退出系统
            cout << "退出系统" << endl;
            break;
        default:
            cout << "输入有误，请重新输入";
        }
    } while (choice != 4);

    return 0;
}

void mainMenu()  //主菜单
{
    cout << setw(40) << "长空御风图书管理系统" << endl;
    cout << setw(25) << "主菜单" << endl << endl;
    cout << left << setw(0) << "1.收银模块" << endl;
    cout << left << setw(0) << "2.书库管理模块" << endl;
    cout << left << setw(0) << "3.报表模块" << endl;
    cout << left << setw(0) << "4.退出系统" << endl;
    cout << "输入选择" << endl;
    cout << "请输入1-4之间的数" << endl;
    cout << setw(45) << setfill('-') << " " << endl;
}

void Sale(BookData books[], int& bookCount)   //收银模块
{
    string isbn;
    int quantity;
    double price, total, tax, finalAmount;
    //销售模块菜单
    cout << setw(50) << setfill('-') << " " << endl;
    cout << endl << "前台销售模块" << endl;
    cout << "请输入书的ISBN号: ";
    cin >> isbn;
    int index = findBookByISBN(books, bookCount, isbn);
    if (index == -1)
    {
        cout << "未找到此书" << endl;
        return;
    }
    cout << "请输入购买数量: ";
    cin >> quantity;
    if (quantity > books[index].getQty())
    {
        cout << "库存不足，当前库存量为: " << books[index].getQty() << endl;
        return;
    }
    //计算金额
    price = books[index].getRetail();
    total = price * quantity;
    tax = total * TAX;
    finalAmount = total + tax;

    //更新库存
    books[index].setQty(books[index].getQty() - quantity);

    //输出收据
    cout << endl << "销售收据" << endl;
    cout << "ISBN 号: " << isbn << endl;
    cout << "书名: " << books[index].getTitle() << endl;
    cout << "单价: RMB " << fixed << setprecision(2) << price << endl;
    cout << "数量: " << quantity << endl;
    cout << "销售合计: RMB " << total << endl;
    cout << "零售税: RMB " << tax << endl;
    cout << "应付总额: RMB " << finalAmount << endl << endl;
    cout << "谢谢光临！" << endl;
}

//书库管理模块
void bookMenu(BookData books[], int& bookCount)
{
    int choice;
    do {
        cout << setw(40) << "长空御风图书管理系统" << endl;
        cout << setw(22) << "书库管理模块" << endl << endl;
        cout << left << setw(0) << "1.查找某本书的信息" << endl;
        cout << left << setw(0) << "2.增加书" << endl;
        cout << left << setw(0) << "3.修改书的信息" << endl;
        cout << left << setw(0) << "4.删除书" << endl;
        cout << left << setw(0) << "5.返回到主菜单" << endl;
        cout << "输入选择：" << endl;
        cout << setw(45) << setfill('-') << " " << endl;

        cin >> choice;

        switch (choice) {
            case 1: lookUpBook(); break;
            case 2: addBook(bookCount); break;
            case 3: editBook(); break;
            case 4: deleteBook(bookCount); break;
            case 5: break;
            default: cout << "输入错误，请重新输入！" << endl;
        }
    } while (choice != 5);
}

//报表模块
void table(BookData books[], int bookCount)
{
    cout << "书库报表";
    cout << setw(15) << "ISBN" << setw(20) << "书名" << setw(10) << "库存量"
         << setw(10) << "零售价";
    for (int i = 0; i < bookCount; ++i)
    {
        cout << setw(15) << books[i].getISBN() << setw(20) << books[i].getTitle()
             << setw(10) << books[i].getQty()
             << "RMB " << fixed << setprecision(2) << books[i].getRetail() << endl;
    }
}

int findBookByISBN(BookData books[], int bookCount, const string& isbn) //通过isbn查找
{
    for (int i = 0; i < bookCount; ++i) {
        if (books[i].getISBN() == isbn) {
            return i;
        }
    }
    return -1;
}

//查找书籍
void lookUpBook() {
    string isbn;
    cout << "请输入 ISBN 号: ";
    cin >> isbn;

    int index = findBookByISBN(books, MAX_BOOKS, isbn);
    if (index == -1) {
        cout << "未找到该书籍！" << endl;
    } else {
        books[index].BookInfo();
    }
}

//添加书籍
void addBook(int& bookCount) {
    if (bookCount >= MAX_BOOKS) {
        cout << "书库已满，无法增加！" << endl;
        return;
    }

    string isbn, title;
    int qty;
    double retail;

    cout << "请输入书籍信息: " << endl;

    cout << "ISBN 号: ";
    cin >> isbn;
    cin.ignore();  // 忽略上一个输入留下的换行符

    cout << "书名: ";
    getline(cin, title);

    cout << "库存量: ";
    cin >> qty;

    cout << "零售价: ";
    cin >> retail;

    books[bookCount] = BookData(isbn, title, qty, retail);
    bookCount++;

    cout << "书籍添加成功！" << endl;
}

// 修改书籍信息
void editBook() {
    string isbn;
    cout << "请输入要修改的书的 ISBN 号: ";
    cin >> isbn;

    int index = findBookByISBN(books, MAX_BOOKS, isbn);
    if (index == -1) {
        cout << "未找到该书籍！" << endl;
        return;
    }

    cout << "输入新库存量: ";
    int qty;
    cin >> qty;
    books[index].setQty(qty);

    cout << "输入新零售价: ";
    int retail;
    cin >> retail;
    books[index].setRetail(retail);

    cout << "书籍信息已更新！" << endl;
}

// 删除书籍
void deleteBook(int& bookCount) {
    string isbn;
    cout << "请输入要删除的书的 ISBN 号: ";
    cin >> isbn;

    int index = findBookByISBN(books, MAX_BOOKS, isbn);
    if (index == -1) {
        cout << "未找到该书籍！" << endl;
        return;
    }

    for (int i = index; i < bookCount - 1; i++) {
        books[i] = books[i + 1];
    }
    bookCount--;
    cout << "书籍已删除" << endl;
}
