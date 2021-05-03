#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
// Constructor
Login::Login(MainWindow &parent, // reference to mainwindow instance
             functionName func) :// name of function from mainwindow
    m_func(func),
    parentWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}

Login::~Login()
{
    delete ui;
}

void Login::on_radioButton_showPassword_toggled(bool checked)
{
    if(checked)
    {
        ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    }
}

void Login::on_pushButton_login_clicked()
{
    // case invalid
    if(ui->lineEdit_username->text().isEmpty() || ui->lineEdit_password->text().isEmpty())
    {
        QMessageBox::information(this, "Invalid Login Info", "Please fill out all fields!");
    }
    else
    {
        // case valid
        if(ui->lineEdit_username->text() == "a" && ui->lineEdit_password->text() == "a")
        {
            QMessageBox::information(this, "Login Success", "Login Successful");
            // close qmessage box
            close();
            // calls showAdminPage function from mainwindow.cpp
            (parentWindow.*m_func)();
        }
        else
        {
            QMessageBox::critical(this, "Invalid Login Info", "Invalid Username or Password");
        }
    }
}

void Login::on_pushButton_cancel_clicked()
{
    close();
}
