#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    // create a more readable type name for function pointer from mainwindow
    typedef void (MainWindow::*functionName)();
    // name of function
    functionName m_func;
    // reference to mainwindow
    MainWindow &parentWindow;

    explicit Login(MainWindow &parent, functionName func);
    ~Login();

private slots:
    void on_radioButton_showPassword_toggled(bool checked);

    void on_pushButton_login_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::Login *ui;
    //AdminWindow *adminWindow;
};

#endif // LOGIN_H
