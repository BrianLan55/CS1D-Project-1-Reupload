#ifndef CAMPUS_H
#define CAMPUS_H

#include <QString>
#include <iterator>
#include "souvenir.h"
#include <QVector>
#include <QDebug>


/*
struct Distance{
    QString destination;
    int mile;
};*/

//! A Campus class
/*!
    This class contains a list of souvenirs and
    A campus name.
*/
class Campus{
public:
    //! A constructor
    /*!
      Initializes all private data members other than
      vector of souvenirs.
    */
    Campus();
    //! A non-default constructor
    /*!
      Initializes all private data members other than
      vector of souvenirs.
    */
    Campus(QString inputName);
    //! A normal member taking one arguement
    /*!
     * \brief setCampusName
     * \param inputName name of campuse
     */

    void setCampusName(QString inputName);
    /*  Sets the value of the campus name with the passed string
     *  Post:
     *          campusName = inputName)
     */
    //! A normal member taking no arguement
    /*!
     * \brief signifies that campus has been visited
     */
    void visitCampus();
    /*  Sets the boolean value of visit to true
     *  Post:
     *          visit = true
     */
    //! A normal member taking returning a bool
    /*!
     * \brief isVisited checks if campus has been visited
     * \return bool representing if campus has been visited
     */
    bool isVisited();
    /*  Returns if the campus was visited
     *
     */
    //! a normal member taking two arguements
    /*!
     * \brief addSouvenir adds souvenir to vector which has
     * price associated
     * \param inputSouvenir souvenir name
     * \param inputPrice souvenir price
     * \param inputQuantity quantity of souvenir
     */
    void addSouvenir(QString inputSouvenir, double inputPrice, int inputQuantity);

    /*
    void addDistance(QString inputDestination, int inputDistance){
        Distance add;
        add.destination = inputDestination;
        add.mile = inputDistance;
        dList.append(add);
    }*/

    //! A normal member return a QString
    /*!
     * \brief getCampusName receives the name of campus
     * \return name of campus
     */
    QString getCampusName();

    //! A normal member
    /*!
     * \brief calculateTotalAmountSpent calculates total amount of souvenirs purchased
     */
    void calculateTotalAmountSpent();
    //! A normal member returning a double
    /*!
     * \brief getTotalAmountSpent receive amount spent at college
     * \return total amount spent on souvenirs
     */
    double getTotalAmountSpent();

    //! A normal member taking one argument
    /*!
     * \brief getItemQuantity getter for souvenir quanitity
     * \param inputItem souvenir name
     * \return quantity of souvenirs purchased
     */
    int getItemQuantity(QString inputItem);

    //! A normal member returning an int
    /*!
     * \brief getNumSouvenirs getter for souvenir amount
     * \return number of souvenirs
     */
    int getNumSouvenirs();

    //! A normal member takint two arguments
    /*!
     * \brief setItemQuantity set quantity to a souvenir
     * \param inputItem name of souvenir
     * \param inputQuantity quantity amount
     */
    void setItemQuantity(QString inputItem, int inputQuantity);
    //! A normal member taking two arguments
    /*!
     * \brief addItemQuantity sums up quantity of a particular item
     * \param inputItem souvenir name
     * \param inputQuantity souveir quantity
     */
    void addItemQuantity(QString inputItem, int inputQuantity);

    //! A normal member taking on argument
    /*!
     * \brief removeItem removes souvenir from a list of souvenirs
     * \param item souvenir name
     */
    void removeItem(QString item);
    //! A normal member returning an iterator pointing to the beginning of the list
    /*!
     * \brief getSouvenirListBegin gets an iterator at start of list
     * \return iterator beginning of list
     */
    QVector<Souvenir>::iterator getSouvenirListBegin();

    //! A normal member returning an iterator
    /*!
     * \brief getSouvenirListEnd get an interator the end of list
     * \return iterator of the end of the list
     */
    QVector<Souvenir>::iterator getSouvenirListEnd();
    //! A normal member returning a QVector
    /*!
     * \brief getSouvenirVector get souvenir list
     * \return souvenir list
     */
    QVector <Souvenir> getSouvenirVector();
    //! A normal member
    /*!
     * \brief clear clears the souvenir list
     */
    void clear();
    //! A normal member returning an int
    /*!
     * \brief getTotalItemQuantity get the total quantity
     * \return total quantity
     */
    int getTotalItemQuantity();

private:
    //QVector <Distance> dList;


    //! A private variable.
    /*!
        a vector that stores a list of souvenirs
    */
    QVector <Souvenir> sList;
    //! A private variable.
    /*!
        name of campuse
    */
    QString campusName;
    //! A private variable.
    /*!
        total amount of spent at college
    */
    double totalAmount;
    //! A private variable.
    /*!
        is campus visited?
    */
    bool visit;
    //! A private variable.
    /*!
        number of souvenirs purchased
    */
    int numSouvenirs;
};

#endif // CAMPUS_H
