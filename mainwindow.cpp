#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include "campus.h"
#include "trip.h"
#include <QInputDialog>
#include "login.h"

#include <QMessageBox>

Database* Database::instance = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set up regex/validator

    //Limits the admin to use only letters and the parenthesis when adding a new campus
    QRegExp limit("[A-Za-z(-) ,]{1,100}");
    ui->lineEditAddCollege->setValidator(new QRegExpValidator(limit, this));

    ui->stackedWidget->setCurrentWidget(ui->page_home);

    //ui->lineEdit_quantity->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    irvineTrip.clear();
    saddlebackTrip.clear();
    customTrip.clear();
    michiganTrip.clear();
}
// combobox on home page will be activated and index of selection will be passed in
void MainWindow::on_comboBox_home_activated(int index)
{
    // SWITCH - in each case there will be a new page opened with a corresponding list of college information
    switch(index) {
    // open new page and display list of colleges and distances from saddleback
    case 1: {
            QString str = "Saddleback College";
            ui->stackedWidget->setCurrentWidget(ui->page_display_saddleback);
            ui->tableView_saddleback->setModel(Database::getInstance()->getDistancesAndCampuses(str));
            // resizes the model so that the table displays all info without cutting it off
            ui->tableView_saddleback->resizeColumnsToContents();
            // update combo box to display the "<Select Here>"
            ui->comboBox_home->setCurrentIndex(0);

            int tableWidth_saddleback = ui->tableView_saddleback->columnWidth(0) + ui->tableView_saddleback->columnWidth(1) +
                                        ui->tableView_saddleback->columnWidth(2) + 24;

            ui->tableView_saddleback->setMaximumWidth(tableWidth_saddleback);
        }
        break;
    // opens new page and displays a list of colleges. resizes columns so that table is displayed nicely
    case 2: {
              ui->stackedWidget->setCurrentWidget(ui->page_display_souvenirs);
              ui->tableView_souvenirs->setModel(Database::getInstance()->displayCollegeOnlyList());
              ui->tableView_souvenirs->resizeColumnsToContents();
              // update combo box to display the "<Select Here>"
              ui->comboBox_home->setCurrentIndex(0);

              int width_souvenirs = ui->tableView_souvenirs->columnWidth(0) + 24;

              ui->tableView_souvenirs->setMinimumWidth(width_souvenirs);
            }
        break;
    // opens plan a trip page and sets combobox back to default
    case 3:{
            ui->stackedWidget->setCurrentWidget(ui->page_plan_a_trip);
            ui->comboBox_home->setCurrentIndex(0);
            } break;
        //sets up the cart page on the stack widget
    }
}

void MainWindow::SetUpUserCart(Trip *trip){
    QVector<Souvenir> souvenirList;
    QVector<Souvenir>::iterator s;

    QVector<Campus> campusList = trip->getCampusVector();
    QVector<Campus>::iterator campusPtr = campusList.begin();

    QString Souvenir;
    double Price;
    int Quantity;

    QString stringPrice;
    QString stringQuantity;

//    for(campusPtr; campusPtr!= campusList.end(); ++campusPtr){
      while(campusPtr != campusList.end()){
//        qDebug() << campusPtr->getCampusName();
        souvenirList = campusPtr->getSouvenirVector();
        s = souvenirList.begin();
//        for(s;s!=souvenirList.end();++s){
        while(s != souvenirList.end()){

            Souvenir = s->getItemName();
            Price    = s->getItemPrice();
            Quantity = s->getItemQuantity();
            stringPrice = QString::number(Price);
            stringQuantity = QString::number(Quantity);

            ui->tableWidget_userCart->insertRow(ui->tableWidget_userCart->rowCount());

            ui->tableWidget_userCart->setItem(ui->tableWidget_userCart->rowCount()-1,0,new QTableWidgetItem(Souvenir));
            ui->tableWidget_userCart->setItem(ui->tableWidget_userCart->rowCount()-1,1,new QTableWidgetItem(stringPrice));
            ui->tableWidget_userCart->setItem(ui->tableWidget_userCart->rowCount()-1,2,new QTableWidgetItem(stringQuantity));
            ui->tableWidget_userCart->setItem(ui->tableWidget_userCart->rowCount()-1,3,new QTableWidgetItem( QString::number((Price*Quantity))));
            ui->tableWidget_userCart->setItem(ui->tableWidget_userCart->rowCount()-1,4,new QTableWidgetItem(campusPtr->getCampusName()));

            ++s;
        }

        souvenirList.clear();
        ++campusPtr;
    }

}

/*******************************************************************************
*deletes the selected souvenir from the user's cart by row number              *
*gets the selected row from a line edit in the ui and checks for valid input   *
*******************************************************************************/
void MainWindow::on_pushButton_delete_clicked()
{
    //wont delete anything if nothing is selected
    if(ui->tableWidget_userCart->currentRow() > -1){
        QTableWidgetItem *item = ui->tableWidget_userCart->item(ui->tableWidget_userCart->currentRow(),0);
        QTableWidgetItem *campus = ui->tableWidget_userCart->item(ui->tableWidget_userCart->currentRow(),4);

        QString deleteItem = item->data(0).toString();

        QMessageBox::StandardButton confirmDeletion;

        confirmDeletion = QMessageBox::warning(this, "Confirm Deletion", "Are you sure you would like to\ndelete " + deleteItem
                                               + " from your cart?", QMessageBox::Yes | QMessageBox::No);

        qDebug() << item->data(0);
        qDebug() << campus->data(0);

        if(confirmDeletion == QMessageBox::Yes){
        QVector<Campus>::iterator ptr = summary->getCampusVector().begin();
        bool found = false;
        while(ptr != summary->getCampusVector().end() && !found){
            found = campus->data(0).toString() == ptr->getCampusName();
            if(!found) ++ptr;
        }

        ptr->removeItem(item->data(0).toString());

        ui->tableWidget_userCart->setRowCount(0);
        SetUpUserCart(summary);

        QMessageBox::information(this, "Deletion Successful", deleteItem + " was successfully deleted from the cart.");
        }
        else
        {
            QMessageBox::information(this, "Deletion Cancelled", deleteItem + " was not deleted.");
        }
    }
    else{
        QMessageBox::information(this, "No Item Selected!", "Please select the item you would\nlike to delete from the cart.");
    }
    ui->comboBox_quantitySelecter->setCurrentIndex(0);
}

/***************************************************
 *Allows the user to edit the quanity of a souvenir*
 * that is currently in their cart.                *
 **************************************************/
void MainWindow::on_pushButton_quantity_clicked()
{

    if(ui->tableWidget_userCart->currentRow() > -1 && ui->comboBox_quantitySelecter->currentIndex() > 0){
        QTableWidgetItem *item = ui->tableWidget_userCart->item(ui->tableWidget_userCart->currentRow(),0);
        QTableWidgetItem *price = ui->tableWidget_userCart->item(ui->tableWidget_userCart->currentRow(),1);
        QTableWidgetItem *campus = ui->tableWidget_userCart->item(ui->tableWidget_userCart->currentRow(),4);

        QMessageBox::StandardButton changeQuantity;

        QString itemName = item->data(0).toString();

        changeQuantity = QMessageBox::information(this, "Confirm Change", itemName + "\'s quantity will be changed to " +
                                                  ui->comboBox_quantitySelecter->currentText() + ".", QMessageBox::Yes | QMessageBox::No);


        qDebug() << item->data(0);
        qDebug() << price->data(0);
        qDebug() << campus->data(0);

        if(changeQuantity == QMessageBox::Yes)
        {
        summary->buyItem(campus->data(0).toString(), item->data(0).toString(), ui->comboBox_quantitySelecter->currentText().toInt(), price->data(0).toDouble());

        ui->tableWidget_userCart->setRowCount(0);
        SetUpUserCart(summary);

        QMessageBox::information(this, "Quantity Changed", itemName + "\'s quantity was successfully changed.");
        }
        else
        {
            QMessageBox::information(this, "No Change", itemName + "\'s quantity was not changed.");
        }
    }
    else{
        QMessageBox::information(this, "No Item Selected", "Please select an item to change\nit's quantity.");

        qDebug() << "no row or quantity was selected";
    }
    ui->comboBox_quantitySelecter->setCurrentIndex(0);
}

/***********************************************************************************************************
*IN: const QModelIndex &index                                                                              *
* ---------------------------------------------------------------------------------------------------------*
* This function passes in the index of the item that was double clicked on the table and converts it       *
*into a school name(QString) then passes it into the database which grabs all the souvenirs for that school*
*it then displays the list of souvenirs.                                                                   *
***********************************************************************************************************/
void MainWindow::on_tableView_souvenirs_doubleClicked(const QModelIndex &index)
{
    // store index from tableview selection
    QModelIndex campusIndex = index.sibling(index.row(), 0);
    // convert selection into a qstring
    QString campusName = ui->tableView_souvenirs->model()->data(campusIndex).toString();

    // displays souvenirs neatly in a tableview
    ui->tableView_souvenirs_2->setModel(Database::getInstance()->displaySouvenirList(campusName));
    ui->tableView_souvenirs_2->setColumnWidth(0, 150);
    ui->tableView_souvenirs_2->setColumnWidth(1, 105);

    int width_souvenirs_2 = ui->tableView_souvenirs_2->columnWidth(0) + ui->tableView_souvenirs_2->columnWidth(1) + 10;
    ui->tableView_souvenirs_2->setMinimumWidth(width_souvenirs_2);
}
// returns the user back to the home page (page where souvenirs are shown for any given college)
// and empties tableview
void MainWindow::on_pushButton_back_souvenirs_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    ui->stackedWidget->setCurrentWidget(ui->page_home);
    ui->tableView_souvenirs_2->setModel(model);
}
// returns the user back to the homepage from campuses from saddleback page
void MainWindow::on_pushButton_back_saddleback_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_home);
}
// returns user back to homepage from plan a trip page
void MainWindow::on_pushButton_plan_trip_page_back_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->page_home);
}

void MainWindow::on_pushButton_trip_saddleback_clicked()
{

    inputSaddlebackTrip = Database::getInstance()->getCollegeNameQueueStartingFrom("Saddleback College");
    while(!inputSaddlebackTrip.empty()) {
        saddlebackTrip.visitNewCollege(inputSaddlebackTrip.dequeue());
    }

    saddlebackTrip.findSmallestRoute();

    QVector <QString> route = saddlebackTrip.getRoute();

    int count = 0;
    ui->tableWidget_short_trip->setRowCount(route.size());
    ui->tableWidget_short_trip->setColumnWidth(0,300);
    for(QVector<QString>::iterator ptr = route.begin(); ptr!= route.end(); ++ptr){
        qDebug() << *ptr;
        QTableWidgetItem *obj = new QTableWidgetItem(*ptr);
        ui->tableWidget_short_trip->setItem(count,0,obj);
        ++count;

    }

    ui->lineEdit_distance_short_trip->setText(QString::number(saddlebackTrip.getTravelDistance()));
    ui->stackedWidget->setCurrentWidget(ui->page_short_trip);
    //points to saddleback trip (for setting up the cart page)
    summary = &saddlebackTrip;
}

void MainWindow::on_pushButton_trip_custom_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_select_colleges);

    ui->comboBox_custom_trip->insertItem(0, "<Select Starting Campus>");

    QVector <QString> list = Database::getInstance()->getCollegeNameVector();
    for(int i=0; i<list.size(); ++i ){
        ui->comboBox_custom_trip->insertItem(i+1, list[i]);
    }

    summary = &customTrip;

}

void MainWindow::on_pushButton_back_short_trip_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_plan_a_trip);
    ui->tableWidget_short_trip->clearContents();
    ui->tableWidget_short_trip->setRowCount(0);
    //resets table view
    QSqlQueryModel *model = new QSqlQueryModel;
    ui->tableView_short_trip->setModel(model);
    ui->lineEdit_distance_short_trip->clear();
    ui->spinBox_short_trip->setValue(0);

    saddlebackTrip.clear();
    customTrip.clear();
    michiganTrip.clear();
    irvineTrip.clear();
    summary = NULL;

    ui->comboBox_custom_trip->clear();
//    QSqlQueryModel *model = new QSqlQueryModel();
    ui->tableView_select_colleges->setModel(model);
    ui->listWidget_select_colleges->clear();
}

void MainWindow::on_pushButton_add_item_short_trip_clicked()
{
    if(ui->tableView_short_trip->currentIndex().row() >-1 && ui->spinBox_short_trip->text().toInt() > 0){
//        qDebug() << ui->tableWidget_short_trip->currentIndex().data(0);
//        qDebug() << ui->tableView_short_trip->currentIndex().sibling(ui->tableView_short_trip->currentIndex().row(), 0).data(0);
//        qDebug() << ui->tableView_short_trip->currentIndex().sibling(ui->tableView_short_trip->currentIndex().row(), 1).data(0);
        QString campus = ui->tableWidget_short_trip->currentIndex().data(0).toString();
        QString item = ui->tableView_short_trip->currentIndex().sibling(ui->tableView_short_trip->currentIndex().row(), 0).data(0).toString();
        double price = ui->tableView_short_trip->currentIndex().sibling(ui->tableView_short_trip->currentIndex().row(), 1).data(0).toDouble();

        qDebug() << campus;
        qDebug() << item;
        qDebug() << price;

       summary->addItem(campus, item, ui->spinBox_short_trip->text().toInt(), price);

       QMessageBox::information(this, "Added to Cart", item + " was successfully added to your cart.");
    }
    else{
        QMessageBox::warning(this, "No Item Selected", "Please select the item you wish to\npurchase and the quantity.");

         qDebug() << "Souvenir or Quantity was not selected";
    }
    ui->spinBox_short_trip->setValue(0);
}

void MainWindow::on_tableWidget_short_trip_itemClicked(QTableWidgetItem *item)
{
    qDebug() << item->text();
    ui->tableView_short_trip->setModel(Database::getInstance()->displaySouvenirList(item->text()));
    ui->tableView_short_trip->setColumnWidth(0,155);
    ui->tableView_short_trip->setColumnWidth(1,155);
}

void MainWindow::on_pushButton_back_select_colleges_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_plan_a_trip);
    ui->comboBox_custom_trip->clear();
    QSqlQueryModel *model = new QSqlQueryModel();
    ui->tableView_select_colleges->setModel(model);
    ui->listWidget_select_colleges->clear();
}

void MainWindow::on_comboBox_custom_trip_activated(const QString &arg1)
{
    ui->listWidget_select_colleges->clear();
    if(arg1 != "<Select Starting Campus>"){
        //unhides every row
        for(int i=0; i<Database::getInstance()->getCollegeNameVector().size(); ++i){
            if(ui->tableView_select_colleges->isRowHidden(i)){
                ui->tableView_select_colleges->showRow(i);
            }
        }
        qDebug() << "plan a trip double clicked " << arg1;
        ui->tableView_select_colleges->setModel(Database::getInstance()->displayCampusesExceptFor(arg1));
        ui->tableView_select_colleges->horizontalHeader()->hide();
    }
    else{
        QSqlQueryModel *model = new QSqlQueryModel();
        ui->tableView_select_colleges->setModel(model);
    }
    ui->tableView_select_colleges->setColumnWidth(0, 300);
}

void MainWindow::on_tableView_select_colleges_doubleClicked(const QModelIndex &index)
{
    ui->tableView_select_colleges->hideRow(index.row());
    QModelIndex campusIndex = index.sibling(index.row(), 0);
   // qDebug() << campusIndex.model()->data(index);
   // QTableWidgetItem *obj = new QTableWidgetItem(campusIndex.model()->data(index).toString());
    ui->listWidget_select_colleges->addItem(campusIndex.model()->data(index).toString());
}

void MainWindow::on_pushButton_clear_selected_colleges_clicked()
{
     ui->listWidget_select_colleges->clear();
     //unhides every row
     for(int i=0; i<Database::getInstance()->getCollegeNameVector().size(); ++i){
         if(ui->tableView_select_colleges->isRowHidden(i)){
             ui->tableView_select_colleges->showRow(i);
         }
     }
}

void MainWindow::on_pushButton_begin_custom_trip_clicked()
{
    qDebug() << ui->comboBox_custom_trip->currentText();

    if(ui->comboBox_custom_trip->currentText() != "<Select Starting Campus>" && ui->listWidget_select_colleges->count() > 0){
        customTrip.visitNewCollege(ui->comboBox_custom_trip->currentText());
        for(int i=0; i<ui->listWidget_select_colleges->count(); ++i){
               customTrip.visitNewCollege(ui->listWidget_select_colleges->item(i)->text());
        }
        customTrip.findSmallestRoute();

        QVector <QString> route = customTrip.getRoute();

        int count = 0;
        ui->tableWidget_short_trip->setRowCount(route.size());
        ui->tableWidget_short_trip->setColumnWidth(0,300);
        for(QVector<QString>::iterator ptr = route.begin(); ptr!= route.end(); ++ptr){
            qDebug() << *ptr;
            QTableWidgetItem *obj = new QTableWidgetItem(*ptr);
            ui->tableWidget_short_trip->setItem(count,0,obj);
            ++count;
        }

         ui->lineEdit_distance_short_trip->setText(QString::number(customTrip.getTravelDistance()));

         ui->stackedWidget->setCurrentWidget(ui->page_short_trip);

    }
}


void MainWindow::on_pushButton_backToHome_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_short_trip);
    ui->tableWidget_userCart->setRowCount(0);
    ui->comboBox_quantitySelecter->setCurrentIndex(0);
    ui->tableWidget_short_trip->setColumnWidth(0,300);
}

void MainWindow::on_pushButton_view_edit_cart_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_cart);
    ui->comboBox_home->setCurrentIndex(0);
    ui->tableWidget_userCart->selectRow(0);
    ui->spinBox_short_trip->setValue(0);
    //passes pointer to trip object
    SetUpUserCart(summary);
    ui->tableWidget_short_trip->setColumnWidth(0,200);
    ui->tableWidget_userCart->setColumnWidth(4,270);
}
/* ***************************************************************
 * Function: on_pushButton_trip_irvine_clicked
 * ---------------------------------------------------------------
 * Displays 11 initial campuses sorted by the shortest trip route
 * starting from uc irvine. Sets purchase summary pointer to
 * UC Irvine.
 ******************************************************************/
void MainWindow::on_pushButton_trip_irvine_clicked()
{
    inputIrvineTrip = Database::getInstance()->getCollegeNameStackStartingFrom("University of California, Irvine (UCI)");
    while(!inputIrvineTrip.empty()){
        irvineTrip.visitNewCollege(inputIrvineTrip.top());
        inputIrvineTrip.pop();
    }

    irvineTrip.findSmallestRoute();

    // stores ideal trip route in route
    QVector <QString> route = irvineTrip.getRoute();

    // sets formatting for table displaying ideal trip route
    int count = 0;
    ui->tableWidget_short_trip->setRowCount(route.size());
    ui->tableWidget_short_trip->setColumnWidth(0,300);
    // runs from the starting point of the route until the ending point of the route
    for(QVector<QString>::iterator ptr = route.begin(); ptr!= route.end(); ++ptr){
        qDebug() << *ptr;
        // inserts a the college into the table
        QTableWidgetItem *obj = new QTableWidgetItem(*ptr);
        ui->tableWidget_short_trip->setItem(count,0,obj);
        ++count;

    }
    //
    ui->lineEdit_distance_short_trip->setText(QString::number(irvineTrip.getTravelDistance()));
    ui->stackedWidget->setCurrentWidget(ui->page_short_trip);
    //points to UC irvine trip (for setting up the cart page)
    summary = &irvineTrip;

}

void MainWindow::on_pushButton_next_short_trip_clicked()
{
    // Displays summary page by changing the current widget of the stackedWidget.
    ui->stackedWidget->setCurrentWidget(ui->page_purchase_summary);

    // Vector of campus that are being visited and iterator pointing to the start of the vector.
    QVector<Campus> campusList = summary->getCampusVector();
    QVector<Campus>::iterator campusPtr = campusList.begin();

    // Sets the data in the table widget.
    while(campusPtr != campusList.end())
    {
        QString campusName = campusPtr->getCampusName();
        QString numSouvenirs = QString::number(campusPtr->getNumSouvenirs());
        QString totalSpent = QString::number(campusPtr->getTotalAmountSpent(), 'd', 2);

        // Only displays the college name, number of souvenirs purchased, and total spent if at least 1
        // souvenir was purchased.
        if(numSouvenirs.toInt() > 0)
        {
            ui->tableWidget_purchase_summary->insertRow(ui->tableWidget_purchase_summary->rowCount());

            ui->tableWidget_purchase_summary->setItem(ui->tableWidget_purchase_summary->rowCount() - 1, 0, new QTableWidgetItem(campusName));
            ui->tableWidget_purchase_summary->setItem(ui->tableWidget_purchase_summary->rowCount() - 1, 1, new QTableWidgetItem(numSouvenirs));
            ui->tableWidget_purchase_summary->setItem(ui->tableWidget_purchase_summary->rowCount() - 1, 2, new QTableWidgetItem(totalSpent));
        }

        campusPtr++;
    }

    // Displays the total spent at all colleges in the label under the table.
    ui->label_grand_total->setText("$" + QString::number(summary->getTotalPrice(), 'd', 2));

    // Sets the font size of the grand total and makes it bold.
    QFont font = ui->label_grand_total->font();
    font.setPointSize(12);
    font.setBold(true);

    ui->label_grand_total->setFont(font);

    ui->tableWidget_purchase_summary->resizeColumnsToContents();

    // Sets the width of the table to be the total of the widths of all columns.
    int summaryTableWidth = ui->tableWidget_purchase_summary->columnWidth(0) + ui->tableWidget_purchase_summary->columnWidth(1) +
                            ui->tableWidget_purchase_summary->columnWidth(2) + 17;

    ui->tableWidget_purchase_summary->setMaximumWidth(summaryTableWidth);
}

void MainWindow::on_pushButton_back_summary_clicked()
{
    // Brings the user back to the trip page.
    ui->stackedWidget->setCurrentWidget(ui->page_short_trip);

    // Deletes the values stored in the table widget.
    ui->tableWidget_purchase_summary->setRowCount(0);
}

void MainWindow::on_pushButton_exit_clicked()
{
    QApplication::quit();
}
// Planning a trip from michigan to any number of colleges
void MainWindow::on_pushButton_trip_michigan_clicked()
{
    int index;

    QVector <QString> list = Database::getInstance()->getCollegeNameVector();

    ui->stackedWidget->setCurrentWidget(ui->page_michiganPage);
    for(index = 1;index<=list.size();index++)
    {
        ui->comboBox_collegeQuantity->addItem(QString::number(index),index);
    }


    //CHRISTIAN'S CODE
    // gets list of colleges that are going to be visited starting with University of Michigan
    QQueue<QString> collegesToVisit = Database::getInstance()->getCollegeNameQueueStartingFrom("University of Michigan");
    while(!collegesToVisit.empty()) {
        michiganTrip.visitNewCollege(collegesToVisit.dequeue());
    }
    // use this method to get smallest route replace 8 with whatever amount of colleges they want to visit
    //michiganTrip.findSmallestRoute(8);

}

//USES FUNCTIONS FROM OTHER TRIPS
void MainWindow::on_pushButton_getCollegeQuantity_clicked()
{
    int collegeNumber = ui->comboBox_collegeQuantity->currentText().toInt();
    qDebug() << collegeNumber << "IS THE COLLEGE NUMBER\n";
    ui->comboBox_collegeQuantity->clear();

    ui->stackedWidget->setCurrentWidget(ui->page_select_colleges);

    //calling functions from other trips//

    // stores ideal trip route in route
    michiganTrip.findSmallestRoute(collegeNumber);
    QVector <QString> route = michiganTrip.getRoute();


    // sets formatting for table displaying ideal trip route
    int count = 0;
    ui->tableWidget_short_trip->setRowCount(route.size());
    ui->tableWidget_short_trip->setColumnWidth(0,300);
    // runs from the starting point of the route until the ending point of the route
    for(QVector<QString>::iterator ptr = route.begin(); ptr!= route.end(); ++ptr){
        qDebug() << *ptr;
        // inserts a the college into the table
        QTableWidgetItem *obj = new QTableWidgetItem(*ptr);
        ui->tableWidget_short_trip->setItem(count,0,obj);
        ++count;

    }
    //
    ui->lineEdit_distance_short_trip->setText(QString::number(michiganTrip.getTravelDistance()));
    ui->stackedWidget->setCurrentWidget(ui->page_short_trip);
    //points to UC irvine trip (for setting up the cart page)
    summary = &michiganTrip;
}

void MainWindow::on_pushButton_backToSelectTrip_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_plan_a_trip);
    ui->comboBox_collegeQuantity->clear();
}

void MainWindow::on_pushButton_RunAsAdmin_clicked()
{
    // create new instance of login and pass in Mainwindow class pointer and openAdmin page function
    Login *login = new Login(*this, &MainWindow::openAdminPage);

    // execute login;
    login->exec();

    delete login;
}
void MainWindow::openAdminPage() {
    ui->stackedWidget->setCurrentWidget(ui->page_admin);
}

void MainWindow::on_pushButton_adminLogout_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_home);

    // Resets the index of te combo box to 0.
    ui->comboBox_admin->setCurrentIndex(0);

    // Clears the tables.
    ui->tableWidget_edit_souvenirs->setRowCount(0);

    QSqlQueryModel *model = new QSqlQueryModel;
    ui->tableView_edit_souvenirs->setModel(model);
}

void MainWindow::on_pushButton_back_edit_souvenir_clicked()
{
    // Goes back to the admin homepage.
    ui->stackedWidget->setCurrentWidget(ui->page_admin);

    // Resets the index of the combo box to 0.
    ui->comboBox_admin->setCurrentIndex(0);

    // Clears the tables.
    ui->tableWidget_edit_souvenirs->setRowCount(0);

    QSqlQueryModel *model = new QSqlQueryModel;
    ui->tableView_edit_souvenirs->setModel(model);

    ui->doubleSpinBox_newPrice->setValue(0);
}

void MainWindow::on_pushButton_logout_edit_souvenirs_clicked()
{
    on_pushButton_adminLogout_clicked();
}

void MainWindow::on_pushButton_back_add_college_clicked()
{
    on_pushButton_back_edit_souvenir_clicked();

    ui->comboBox_admin->setCurrentIndex(0);
    ui->tableWidgetAddCollege->setRowCount(0);
    ui->lineEditAddCollege->clear();
    ui->doubleSpinBoxAddCollege->setValue(0);
}


void MainWindow::on_comboBox_admin_currentIndexChanged(int index)
{
    switch(index)
    {
    case 1:
    {
        ui->stackedWidget->setCurrentWidget(ui->page_add_college);

         QVector<QString> collegeList = Database::getInstance()->getCollegeNameVector();

        for(QVector<QString>::iterator ptr = collegeList.begin(); ptr != collegeList.end();++ptr) {
             ui->tableWidgetAddCollege->insertRow(ui->tableWidgetAddCollege->rowCount());
             ui->tableWidgetAddCollege->setItem(ui->tableWidgetAddCollege->rowCount()-1, 0, new QTableWidgetItem(*ptr));
             ui->tableWidgetAddCollege->setItem(ui->tableWidgetAddCollege->rowCount()-1, 1, new QTableWidgetItem("Enter Distance"));\
        }
        ui->tableWidgetAddCollege->setColumnWidth(0, 200);
    }
        break;
    case 2:
    {
        // Sets the index of the stacked widget to the change price page.
        ui->stackedWidget->setCurrentWidget(ui->page_edit_souvenirs);

        QVector<QString> collegeList = Database::getInstance()->getCollegeNameVector();

        // Displays the list of colleges in the database on the table widget.
        for(QVector<QString>::iterator it = collegeList.begin(); it != collegeList.end(); it++)
        {
            ui->tableWidget_edit_souvenirs->insertRow(ui->tableWidget_edit_souvenirs->rowCount());

            ui->tableWidget_edit_souvenirs->setItem(ui->tableWidget_edit_souvenirs->rowCount() - 1, 0, new QTableWidgetItem(*it));
        }

        ui->tableWidget_edit_souvenirs->setColumnWidth(0, 300);
    }
        break;
    case 3:
    {
        ui->stackedWidget->setCurrentWidget(ui->page_add_souvenir);
        ui->tableWidget_addSouvenirCollegeList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableWidget_addSouvenirCollegeList->setColumnCount(1);
        QStringList titles;
        titles << "Colleges";
        ui->tableWidget_addSouvenirCollegeList->setHorizontalHeaderLabels(titles);

        QVector<QString> collegeList = Database::getInstance()->getCollegeNameVector();

        // Displays the list of colleges in the database on the table widget.
        for(QVector<QString>::iterator it = collegeList.begin(); it != collegeList.end(); it++)
        {
            ui->tableWidget_addSouvenirCollegeList->insertRow(ui->tableWidget_addSouvenirCollegeList->rowCount());

            ui->tableWidget_addSouvenirCollegeList->setItem(ui->tableWidget_addSouvenirCollegeList->rowCount() - 1, 0, new QTableWidgetItem(*it));
        }

        ui->tableWidget_addSouvenirCollegeList->setColumnWidth(0, 300);
    }
        break;
    }
}

void MainWindow::on_tableWidget_edit_souvenirs_clicked(const QModelIndex &index)
{
    // Gets the index of the selected college.
    QModelIndex campusIndex = index.sibling(index.row(), index.column());

    // Stores the name of the selected college.
    QString campusName = ui->tableWidget_edit_souvenirs->model()->data(campusIndex).toString();

    // Displays the list of souvenirs for the selected college.
    ui->tableView_edit_souvenirs->setModel(Database::getInstance()->displaySouvenirList(campusName));

    ui->tableView_edit_souvenirs->setColumnWidth(0, 200);
    ui->tableView_edit_souvenirs->setColumnWidth(1, 149);
}

void MainWindow::on_pushButton_confirmPrice_clicked()
{
    if(ui->tableWidget_edit_souvenirs->currentRow() == -1 || ui->tableView_edit_souvenirs->currentIndex().row() == -1){
        QMessageBox::critical(this, "Error", "Please select a college and souvenir.");
    }
    else{
        // Gets the new price of the souvenir.
        double newPrice = ui->doubleSpinBox_newPrice->text().toDouble();

        // Doesn't allow the user to enter a negative price or a price greater than $500.
        if(newPrice == 0.00)
        {
            QMessageBox::critical(this, "Invalid Price", "Please enter a price!");
        }
        else
        {
            // Gets the row and column of the selected souvenir.
            int row = ui->tableView_edit_souvenirs->currentIndex().row();
            int column = ui->tableView_edit_souvenirs->currentIndex().column();

            // Sets the index of the selected souvenir.
            QModelIndex index = ui->tableView_edit_souvenirs->model()->index(row, column, QModelIndex());

            // Stores the name of the souvenir.
            QString souvenir = ui->tableView_edit_souvenirs->model()->data(index).toString();

            // Gets the row and column of the selected college.
            int collegeRow = ui->tableWidget_edit_souvenirs->currentIndex().row();
            int collegeColumn = ui->tableWidget_edit_souvenirs->currentIndex().column();

            // Stores the index of the selected college.
            QModelIndex campusIndex = ui->tableWidget_edit_souvenirs->model()->index(collegeRow, collegeColumn, QModelIndex());

            // Stores the name of the college.
            QString collegeName = ui->tableWidget_edit_souvenirs->model()->data(campusIndex).toString();

            // Doesn't allow the souvenir's price to be changed if no souvenir is selected.
            if(souvenir == "")
            {
                QMessageBox::warning(this, "No Souvenir Selected", "Please select a souvenir!");
            }
            else
            {
                QMessageBox::StandardButton confirmChange;

                confirmChange = QMessageBox::warning(this, "", "Confirm new price.",
                                                     QMessageBox::Yes | QMessageBox::No);

                // Changes the price of the souvenir if yes is clicked on the Messagebox.
                if(confirmChange == QMessageBox::Yes)
                {
                    qDebug() << collegeName << " " << souvenir << " " << newPrice;

                    // Call change price DB function function.
                    Database::getInstance()->changeSouvenirPriceAtCollege(collegeName, souvenir, newPrice);

                    // Updates table.
                    on_tableWidget_edit_souvenirs_clicked(campusIndex);

                    ui->doubleSpinBox_newPrice->setValue(0);
                }
            }
        }
    }
}




///
/// \This method takes the admin from the add souvenir page to the main
/// \admin window
///
void MainWindow::on_pushButton_back_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_admin);
    ui->lineEdit_addSouvenirName->clear();
    ui->doubleSpinBox_addSouvenirPrice->setValue(0);
    ui->comboBox_admin->setCurrentIndex(0);
    ui->tableWidget_addSouvenirCollegeList->setRowCount(0);
    ui->tableView_displaySouvenirs->setModel(new QSqlQueryModel);
}

///
/// \This method allows the admin to select a college that will be used
/// \to add a new souvenir
///
QString MainWindow::on_tableWidget_addSouvenirCollegeList_itemClicked(QTableWidgetItem *item)
{
    QString campusName = item->text();
    return campusName;
}

///
/// \This method allows the admin to add a souvenir
///
void MainWindow::on_pushButton_addSouvenir_clicked()
{

    if(ui->tableWidget_addSouvenirCollegeList->currentRow() == -1){
        QMessageBox::critical(this, "Error", "Please select a college and souvenir.");
    }
    else{
        QTableWidgetItem *item = ui->tableWidget_addSouvenirCollegeList->currentItem();

        //bool validPrice = ui->lineEdit_addSouvenirPrice->text().toDouble();

        // Prevents the addition of more than 6 souvenirs per college.
        if(ui->tableView_displaySouvenirs->model()->rowCount() == 6)
        {
            QMessageBox::critical(this, "Error", "Maximum number of souvenirs have\nalready been added. Please delete\na souvenir and try again!");

            return;
        }

        //if the line_edit fields are empty
        if(ui->lineEdit_addSouvenirName->text().isEmpty() || ui->doubleSpinBox_addSouvenirPrice->text().isEmpty())
        {
            QMessageBox::critical(this,"Input Error","Please fill in all fields.");
            return;
        }
        //else if no college was selected from the table widget
        else if(!item)
        {
           QMessageBox::critical(this,"Input Error","Please select a campus to continue.");
           return;
        }
        else if(ui->doubleSpinBox_addSouvenirPrice->text().isEmpty() || ui->doubleSpinBox_addSouvenirPrice->text().toDouble() == 0){
            QMessageBox::critical(this,"Input Error","Please enter the price.");
            return;
        }
        else if(Database::getInstance()->souvenirExistsAtCollege(ui->lineEdit_addSouvenirName->text(), on_tableWidget_addSouvenirCollegeList_itemClicked(item))){
            QMessageBox::critical(this,"Input Error","Souvenir already exists.");
            ui->lineEdit_addSouvenirName->clear();
            ui->doubleSpinBox_addSouvenirPrice->setValue(0);
            return;
        }
        //else process the souvenir to be added
        else
        {
            double souvenirPrice = ui->doubleSpinBox_addSouvenirPrice->text().toDouble();

            QString campusName = on_tableWidget_addSouvenirCollegeList_itemClicked(item);
            QString souvenirName = ui->lineEdit_addSouvenirName->text();

            QMessageBox::StandardButton confirmSouvenirAddition;
            confirmSouvenirAddition = QMessageBox::warning(this, "Add Souvenir", "Do you want to add [" + souvenirName +
                                                           " - $"  + QString::number(souvenirPrice) + "] to "  + campusName +" ?",
                                                           QMessageBox::Yes | QMessageBox::No);


            if(confirmSouvenirAddition == QMessageBox::Yes)
            {
                QMessageBox::information(this,"Souvenir Added", "Souvenir successfully added.");
                Database::getInstance()->addSouvenirToDatabase(campusName,souvenirName,souvenirPrice);
                ui->lineEdit_addSouvenirName->clear();
                ui->doubleSpinBox_addSouvenirPrice->setValue(0);

                QModelIndex collegeIndex = ui->tableWidget_addSouvenirCollegeList->model()->index(ui->tableWidget_addSouvenirCollegeList->currentIndex().row(),
                                                                                                  ui->tableWidget_addSouvenirCollegeList->currentIndex().column(), QModelIndex());

                on_tableWidget_addSouvenirCollegeList_clicked(collegeIndex);
            }
            ui->lineEdit_addSouvenirName->clear();
        }
    }
}

void MainWindow::on_pushButton_deleteSouvenir_clicked()
{

    if(ui->tableWidget_edit_souvenirs->currentRow() == -1){
        QMessageBox::critical(this, "Error", "Please select both a college and a souvenir!");
    }
    else{
        // Gets the name of the college.
        int collegeRow = ui->tableWidget_edit_souvenirs->currentIndex().row();
        int collegeCol = ui->tableWidget_edit_souvenirs->currentIndex().column();

        QModelIndex collegeIndex = ui->tableWidget_edit_souvenirs->model()->index(collegeRow, collegeCol, QModelIndex());

        QString souvenirCollege = ui->tableWidget_edit_souvenirs->model()->data(collegeIndex).toString();

        // Gets the name of the souvenir.
        int souvenirRow = ui->tableView_edit_souvenirs->currentIndex().row();
        int souvenirCol = ui->tableView_edit_souvenirs->currentIndex().column();

        QModelIndex souvenirIndex = ui->tableView_edit_souvenirs->model()->index(souvenirRow, souvenirCol, QModelIndex());

        QString souvenirName = ui->tableView_edit_souvenirs->model()->data(souvenirIndex).toString();

        // Deletes the souvenir if a college and souvenir were selected, else displays an error message.
        if(souvenirCollege != "" && souvenirName != "")
        {
            QMessageBox::StandardButton confirmDeletion = QMessageBox::warning(this, "Confirm Changes", "Are you sure you would "
                                                                               "like to\ndelete " + souvenirName + "?", QMessageBox::Yes | QMessageBox::No);

            if(confirmDeletion == QMessageBox::Yes)
            {
                Database::getInstance()->removeSouvenir(souvenirCollege, souvenirName);

                QMessageBox::information(this, "Change Successful", souvenirName + " was successfully deleted.");

                // Updates table.
                on_tableWidget_edit_souvenirs_clicked(collegeIndex);

                ui->doubleSpinBox_newPrice->setValue(0);
            }
        }
        else
        {
            QMessageBox::critical(this, "Error", "Please select both a college and a souvenir!");
        }

    }

}

void MainWindow::on_tableWidget_addSouvenirCollegeList_clicked(const QModelIndex &index)
{
    QModelIndex campusIndex = index.sibling(index.row(), index.column());

    QString campusName = ui->tableWidget_addSouvenirCollegeList->model()->data(campusIndex).toString();

    ui->tableView_displaySouvenirs->setModel(Database::getInstance()->displaySouvenirList(campusName));

    ui->tableView_displaySouvenirs->setColumnWidth(0, 200);
    ui->tableView_displaySouvenirs->setColumnWidth(1, 116);
}

void MainWindow::on_pushButtonEnterDistance_clicked()
{
    if((ui->doubleSpinBoxAddCollege->value() == 0 || ui->doubleSpinBoxAddCollege->text().isEmpty()) && ui->lineEditAddCollege->text().isEmpty()){
        QMessageBox::critical(this,"Input Error","Please enter a distance and the name of the new college.");
    }
    else if(ui->doubleSpinBoxAddCollege->value() == 0 || ui->doubleSpinBoxAddCollege->text().isEmpty()){
        QMessageBox::critical(this,"Input Error","Please enter a distance.");
    }
    else if(ui->lineEditAddCollege->text().isEmpty()){
        QMessageBox::critical(this,"Input Error","Please enter the name of the new college.");
    }
    else if(ui->tableWidgetAddCollege->currentRow() < 0){
        QMessageBox::critical(this,"Input Error","Select a college on the table.");
    }
    //if all of the checks are valid, edit the distance on the table
    else{
        ui->tableWidgetAddCollege->setItem(ui->tableWidgetAddCollege->currentRow(), 1, new QTableWidgetItem(ui->doubleSpinBoxAddCollege->text()));
        ui->doubleSpinBoxAddCollege->clear();
        ui->tableWidgetAddCollege->selectRow(ui->tableWidgetAddCollege->currentRow()+1);
    }
    ui->doubleSpinBoxAddCollege->setFocus();
}

void MainWindow::on_pushButton_ConfirmNewCollege_clicked()
{
    QVector<QString> nameVector;
    QVector<double> distanceVector;

    bool valid = !ui->lineEditAddCollege->text().isEmpty();

    if(!valid){
         QMessageBox::critical(this,"Input Error","Please enter the name of the new college.");
         return;
    }

    if(Database::getInstance()->collegeExists(ui->lineEditAddCollege->text())){
        QMessageBox::critical(this,"Input Error","College already exists.");
        return;
    }

    int i=0;
    while(valid && i<ui->tableWidgetAddCollege->rowCount()){

        valid = ui->tableWidgetAddCollege->item(i, 1)->text() != "Enter Distance";
        qDebug() << ui->tableWidgetAddCollege->item(i, 1)->text();

        if (valid){
            nameVector.append(ui->tableWidgetAddCollege->item(i,0)->text());
            distanceVector.append(ui->tableWidgetAddCollege->item(i,1)->text().toDouble());
            ++i;
        }
    }

    if(valid){
        Database::getInstance()->addCollegeToDatabase(ui->lineEditAddCollege->text(), nameVector, distanceVector);
        ui->stackedWidget->setCurrentWidget(ui->page_admin);
        ui->tableWidgetAddCollege->setRowCount(0);
        ui->lineEditAddCollege->clear();
        ui->doubleSpinBoxAddCollege->setValue(0);
        ui->comboBox_admin->setCurrentIndex(0);
    }
    else{
        QMessageBox::critical(this,"Input Error","Enter the distance to each campus.");
    }
}

void MainWindow::on_doubleSpinBoxAddCollege_editingFinished()
{
    ui->pushButtonEnterDistance->setFocus();
}
