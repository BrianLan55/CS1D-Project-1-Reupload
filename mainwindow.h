#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "trip.h"
#include <QTableWidgetItem>
#include<QMessageBox>

#include <QStack>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void SetUpUserCart(Trip &obj, QString collegeName);
    void SetUpUserCart(Trip *trip);
    ~MainWindow();

private slots:

 void on_comboBox_home_activated(int index);

 void on_tableView_souvenirs_doubleClicked(const QModelIndex &index);

 void on_pushButton_back_souvenirs_clicked();

 void on_pushButton_back_saddleback_clicked();

 void on_pushButton_plan_trip_page_back_clicked();

 void on_pushButton_back_select_colleges_clicked();

 void on_pushButton_trip_saddleback_clicked();

 void on_tableView_select_colleges_doubleClicked(const QModelIndex &index);

 void on_pushButton_trip_custom_clicked();

 void on_pushButton_back_short_trip_clicked();

 void on_pushButton_add_item_short_trip_clicked();

 void on_tableWidget_short_trip_itemClicked(QTableWidgetItem *item);

 void on_comboBox_custom_trip_activated(const QString &arg1);

 void on_pushButton_clear_selected_colleges_clicked();

 void on_pushButton_begin_custom_trip_clicked();

 void on_pushButton_delete_clicked();

 void on_pushButton_quantity_clicked();

 void on_pushButton_backToHome_clicked();

 void on_pushButton_view_edit_cart_clicked();

 void on_pushButton_trip_irvine_clicked();

 void on_pushButton_next_short_trip_clicked();

 void on_pushButton_back_summary_clicked();

 void on_pushButton_exit_clicked();

 void on_pushButton_trip_michigan_clicked();

 void on_pushButton_getCollegeQuantity_clicked();

 void on_pushButton_backToSelectTrip_clicked();

 void on_pushButton_RunAsAdmin_clicked();

 void openAdminPage();

 void on_pushButton_adminLogout_clicked();

 void on_pushButton_back_edit_souvenir_clicked();

 void on_pushButton_logout_edit_souvenirs_clicked();

 void on_pushButton_back_add_college_clicked();

 void on_comboBox_admin_currentIndexChanged(int index);

 void on_tableWidget_edit_souvenirs_clicked(const QModelIndex &index);

 void on_pushButton_confirmPrice_clicked();

 void on_pushButton_back_clicked();

QString on_tableWidget_addSouvenirCollegeList_itemClicked(QTableWidgetItem *item);

void on_pushButton_addSouvenir_clicked();

void on_pushButton_deleteSouvenir_clicked();

void on_tableWidget_addSouvenirCollegeList_clicked(const QModelIndex &index);

void on_pushButtonEnterDistance_clicked();

void on_pushButton_ConfirmNewCollege_clicked();

void on_doubleSpinBoxAddCollege_editingFinished();


private:
    Ui::MainWindow *ui;
    //Login *login;
    Trip saddlebackTrip;
    Trip michiganTrip;
    Trip irvineTrip;
    Trip customTrip;
    Trip *summary;

    QQueue<QString> inputSaddlebackTrip;
    QStack<QString> inputIrvineTrip;
};

#endif // MAINWINDOW_H
