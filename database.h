#ifndef DATABASE_H
#define DATABASE_H

#include <QFileInfo>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QQueue>
#include <QStack>

#include <QDebug>

// Database Class
/**
 * \brief Database Class
 *
 * The Database class represents SQL database and the queries needed to display or chamge information
 *  stored in the database. Implemented as singleton.
 */
class Database : public QSqlDatabase{
private:
    //! A private variable
    /*!
     * \brief instance static pointer to siginify one instance of database
     */
    static Database* instance;  /** Static pointer to an instance of a database that is open */

    //! A constructor
    /*!
     * \brief Database initalizes database name
     */
    Database() : QSqlDatabase(addDatabase("QSQLITE")){


        this->setDatabaseName("./collegedata.db");

        QFileInfo existingDatabaseInfo("./collegedata.db");
    qDebug() << "hello";
        if(existingDatabaseInfo.exists()){
            qDebug() << "EXISTS";
        }
        else{
            qDebug() << "DOES NOT EXIST";
        }

        this->open();
    }

public:
    //! A normal member
    /*!
     * \brief getInstance returns a single instance of the database
     * \return returns a single instance of the database
     */
    static Database* getInstance(){
        if(instance == nullptr){
            instance = new Database;
        }
        return instance;
    }

    //How to call functions: Database::getInstance->FunctionName();

    //returns a table of the data that includes the starting campus, destination campus, and the distance between them
    //! A normal member returning a QSqlQueryModel pointer
    /*!
     * \brief displayCollegeAndDistanceList creates a model of all colleges and distances
     * \return model that displays all colleges and distances in distance table
     */
    QSqlQueryModel* displayCollegeAndDistanceList();

    //returns a list (table) of every college.
    //! A normal member returning a QSqlQueryModel pointer
    QSqlQueryModel* displayCollegeOnlyList();

    /**
     * @brief souvenirExistsAtCollege checks if a souvenir already exists at a particular college
     * @param souvenir name of souvenir
     * @param college name of college
     * @return
     */
    bool souvenirExistsAtCollege(QString souvenir, QString college);

    /**
     * @brief getCollegeNameStackStartingFrom creat a stack that contains all colleges in database  and college
     *        passed in at the top
     * @param campusName name of college going to top of stack
     * @return
     */
    QStack<QString> getCollegeNameStackStartingFrom(QString campusName);

    //Returns a list that displays the souvenirs and prices from the inputted college
    //! A normal member returning a QSqlQueryModel pointer
    /*!
     * \brief displaySouvenirList displays list of souvenirs at a specific college
     * \param inputCollege college name
     * \return model containing a table of souvenirs
     */
    QSqlQueryModel* displaySouvenirList(QString inputCollege);
    //! A normal member returning a QSqlQueryModel pointer
    /*!
     * \brief getDistanceQuery creates a model and stores a table containing distances at a specific college
     * \param startingCampus starting campus name
     * \return
     */
    QSqlQuery getDistanceQuery(QString startingCampus);
    //! A normal member that takes a QString returning a QSqlQueryModel pointer
    /*!
     * \brief displayCampusesExceptFor displays all campuses besides one
     * \param campus college name that is excluded from table
     * \return query from distance db that excludes one college
     */
    QSqlQueryModel* displayCampusesExceptFor(QString campus);
    //! A normal member that takes one argument and returns a QSqlQueryModel pointer
    /*!
     * \brief getDistancesAndCampuses distances and campuses for a particular college
     * \param college college name
     * \return model containing distances and campuses from a particular college
     */
    QSqlQueryModel* getDistancesAndCampuses(QString college);
    //! A normal member takes three arguments
    /*!
     * \brief addCollegeToDatabase insert college to database
     * \param startCampus starting campus
     * \param endCampus ending campuses
     * \param distances distances from starting campus to ending campus
     */
    bool addCollegeToDatabase(QString startCampus, const QVector<QString> &endCampus, const QVector<double> &distances);
    //! A normal member that takes three arguments
    /*!
     * \brief addSouvenirToDatabase adds souvenir to souvenir database
     * \param campus campus name
     * \param itemName name of souvenir
     * \param price price of souvenir
     */
    void addSouvenirToDatabase(QString campus, QString itemName, double price);
    //! A normal member that takes one argument
    /*!
     * \brief removeCollegeFromDatabase removes college from database
     * \param campus college name to be removed
     */
    void removeCollegeFromDatabase(QString campus);
    //! A normal member returning one argument
    /*!
     * \brief getCollegeNameVector
     * \return a vector of college names
     */
    bool removeSouvenir(QString college, QString souvenir);
    /**
     * @brief changeSouvenirPriceAtCollege change the price of a souvenir at a particular college in database
     * @param college college name
     * @param souvenir souvenir name
     * @param price souvenir price
     * @return
     */
    bool changeSouvenirPriceAtCollege(QString college, QString souvenir, double price);

    /**
     * @brief getCollegeNameVector gets a list of college names from database
     * @return vector of college names
     */
    QVector <QString> getCollegeNameVector();
    /**
     * @brief getCollegeNameQueueStartingFrom get a queue of colleges that starts from a specified college
     * @param campusName name of starting college
     * @return queue of college names
     */
    QQueue <QString> getCollegeNameQueueStartingFrom(QString campusName);

    /**
     * @brief collegeExists checks if college exists
     * @param college college name
     * @return bool representing if college exists or not
     */
    bool collegeExists(QString college);

    //Creates method that will call Database::getInstance->(name of method).
    //for example, you can return a sql table and display it

};

/*Brandon's note: The main thing we have to use is Query which uses sql language to grab from the database
 * Query.prepare("sql langauge");
 * Query.exec();
 *
*/
#endif // DATABASE_H


    /***********************************************************************************
    >>>>Go To Line 27 And Line 29

        >>>>> Replace "PATH TO SQL DATABASE GOES HERE"

    >>>>This Goes In Your .Pro File

        #-------------------------------------------------
        #
        # Project created by QtCreator 2017-03-16T11:55:53
        #
        #-------------------------------------------------

        QT     += core gui sql
        CONFIG += c++14
    ***********************************************************************************/
