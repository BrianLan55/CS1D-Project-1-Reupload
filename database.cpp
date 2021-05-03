#include "database.h"

//returns a table of the data that includes the starting campus, destination campus, and the distance between them
QSqlQueryModel* Database::displayCollegeAndDistanceList(){
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT * FROM Distances");
    query.exec(); //executes prepare statement

    model->setQuery(query);

    return model;
}

//returns a list (table) of every college.
QSqlQueryModel* Database::displayCollegeOnlyList(){
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT DISTINCT StartingCampus FROM Distances");
    query.exec();

    model->setQuery(query);

    return model;

}

//Returns a list that displays the souvenirs and prices from the inputted college
QSqlQueryModel* Database::displaySouvenirList(QString inputCollege){
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT Item, Price FROM Souvenirs WHERE Campus = :college");
    query.bindValue(":college", inputCollege);
    query.exec();

    model->setQuery(query);

    return model;
}
// returns an active query for all columns at a particular college
QSqlQuery Database::getDistanceQuery(QString startingCampus){

    QSqlQuery query;

    query.prepare("SELECT * FROM Distances WHERE StartingCampus = :college ORDER BY Distance");
    query.bindValue(":college", startingCampus);
    query.exec();


    qDebug() << "getDistanceQuery (" << startingCampus << ")";
//    qDebug() << "query's position from getDistanceQuery: " << query.at(); //return -1 (empty), so this isnt working
    return query;
}
QSqlQueryModel* Database::getDistancesAndCampuses(QString college) {

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM Distances WHERE StartingCampus = :college ORDER BY Distance");
    query.bindValue(":college", college);
    query.exec();
    model->setQuery(query);
    return model;

}
// This function adds starting campus, a list of ending campuses and distances to the Distances table
bool Database::addCollegeToDatabase(QString startCampus,        // IN - starting campuse
                                    const QVector<QString> &endCampus, // IN - list of ending campuses
                                    const QVector<double> &distances) {// IN - list of corresponding distances
    QSqlQuery query;
    // There must be the same number of ending campuses as distances to add to database, otherwise return false
    if(distances.size() != endCampus.size()) {
        return false;
    }
    // FOR - runs until all ending campuses and distances have been inserted into the database the start campuse
    //       will stay the same
    for(int i = 0; i < distances.size(); ++i) {
        // return false if college ending campus doesnt exist
        if(!collegeExists(endCampus[i])) {
            return false;
        }
        // Insert into distance table a new start campus and a list of ending campuses and distances
        query.prepare("INSERT INTO Distances(StartingCampus, EndingCampus, Distance)\n"
                      "Values(:start, :end, :dist)");
        query.bindValue(":start", startCampus);
        query.bindValue(":end", endCampus[i]);
        query.bindValue(":dist", distances[i]);
        query.exec();
    }
    // for every pre-existing college in database, add a new ending campus(startCampus) and a corresponding distance
    for(int i = 0; i < distances.size(); ++i) {
        query.prepare("INSERT INTO Distances(StartingCampus, EndingCampus, Distance) Values(:start, :end, :dist)");
        query.bindValue(":start", endCampus[i]);
        query.bindValue(":end", startCampus);
        query.bindValue(":dist", distances[i]);
        query.exec();
     }
     return true;
}
// adds a souveir to the Souvenir Table
void Database::addSouvenirToDatabase(QString campus,   // IN - campus name
                                     QString itemName, // IN - souvenir name
                                     double price){    // IN - price of souvenir
    QSqlQuery query;
    // INSERTS souvenir data into db
    query.prepare("INSERT INTO Souvenirs(Campus, Item, Price)\n"
                  "VALUES(:campus, :item, :price);");
    query.bindValue(":campus", campus);
    query.bindValue(":item", itemName);
    query.bindValue(":price", price);
    query.exec();
}
// don't need thi function
//void Database::removeCollegeFromDatabase(QString campus){
//    QSqlQuery query;
//    query.prepare("DELETE FROM Distances WHERE StartingCampus = :college");
//    query.bindValue(":college" , campus);
//    query.exec();
//    query.prepare("DELETE FROM Distances WHERE EndingCampus = :college");
//    query.bindValue(":college" , campus);
//    query.exec();
//    query.prepare("DELETE FROM Souvenirs WHERE Campus = :college");
//    query.bindValue(":college" , campus);
//    query.exec();
//}

QSqlQueryModel* Database::displayCampusesExceptFor(QString campus){
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT StartingCampus FROM Distances WHERE EndingCampus = :college");
    query.bindValue(":college" , campus);
    query.exec();
    model->setQuery(query);
    return model;
}

QVector <QString> Database::getCollegeNameVector(){
    QSqlQuery query;
    QVector <QString> list;

    query.prepare("SELECT DISTINCT StartingCampus FROM Distances");
    query.exec();
    while(query.next()){
        list.append(query.value(0).toString());
    }

//    for(int i=0; i<list.size(); ++i ){
//        qDebug() << list[i];
//    }

    return list;
}
// returns a queue of colleges starting from a desired college (parameter)
QQueue <QString> Database::getCollegeNameQueueStartingFrom(QString campusName) // IN - starting college name
{
    QSqlQuery query;
    QQueue <QString> list;
    if(!collegeExists(campusName)) {
        throw QString("College Doesn't Exist");
    }
    // enqueue start college(parameter) then retrieve all other colleges and return the queue
    list.enqueue(campusName);
    query.prepare("SELECT Distinct StartingCampus FROM Distances WHERE StartingCampus != :campusName");
    query.bindValue(":campusName", campusName);
    query.exec();
    while(query.next()) {
        list.enqueue(query.value(0).toString());
    }
    return list;
}
// creates a stack that contains all colleges in the database and puts the campusName param at the top of the stack.
QStack<QString> Database::getCollegeNameStackStartingFrom(QString campusName) // IN - starting college name
{
    QSqlQuery query;
    QStack<QString> list;
    if(!collegeExists(campusName)) {
        throw QString("College Doesn't Exist");
    }
    // query for distinct college names except parameter and then add parameter to top of stack.
    query.prepare("SELECT Distinct StartingCampus FROM Distances WHERE StartingCampus != :campusName");
    query.bindValue(":campusName", campusName);
    query.exec();
    while(query.next()) {
        list.push(query.value(0).toString());
    }
    list.push(campusName);
    return list;
}
// checks if college exists
bool Database::collegeExists(QString college) // IN - college name
{
    QSqlQuery query;
    query.prepare("SELECT Distinct StartingCampus FROM Distances WHERE UPPER(StartingCampus) = UPPER(:campusName)");
    query.bindValue(":campusName", college);
    query.exec();
    if(query.next()) {
        return true;
    }
    return false;
}
// checks if souvenir exists at a particular college,
bool Database::souvenirExistsAtCollege(QString souvenir, // souvenir name
                                       QString college) {// college name
    QSqlQuery query;

    query.prepare("SELECT Distinct Item FROM Souvenirs WHERE UPPER(Item) = UPPER(:souvenir) and UPPER(Campus) = UPPER(:college)");
    query.bindValue(":souvenir", souvenir);
    query.bindValue(":college", college);
    query.exec();
    if(query.next()) {
        return true;
    }
    return false;
}
// removes souvenir from Souvenir table
bool Database::removeSouvenir(QString college,      // IN - college name
                              QString souvenir) {   // IN - souvenir
    // checks if college and souvenir exist
    if(!collegeExists(college) || !souvenirExistsAtCollege(souvenir, college)) {
        return false;
    }
    // delete souvenir from table
    QSqlQuery query;
    query.prepare("DELETE FROM Souvenirs WHERE Item = :item and Campus = :campus");
    query.bindValue(":item", souvenir);
    query.bindValue(":campus", college);
    query.exec();
    return true;
}
// changes the price of the souvenir from the Souvenir table
bool Database::changeSouvenirPriceAtCollege(QString college,  // IN - college name
                                            QString souvenir, // IN - souvenir name
                                            double newPrice) {// IN - new price of souvenir
    if(!collegeExists(college) || !souvenirExistsAtCollege(souvenir, college)) {
        return false;
    }
    QSqlQuery query;
    // changes price in souvenir table
    query.prepare("UPDATE Souvenirs\n"
                  "SET Price = :price\n"
                  "WHERE Campus = :campus\n"
                  "AND Item = :item");
    query.bindValue(":price", newPrice);
    query.bindValue(":campus", college);
    query.bindValue(":item", souvenir);
    query.exec();
    return true;

}



