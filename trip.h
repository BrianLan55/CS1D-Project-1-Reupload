#ifndef TRIP_H
#define TRIP_H

#include "campus.h"
#include "database.h"
//#define INITIAL_CAMPUS_AMOUNT 13
//#define ORIGINAL_CAMPUS_AMOUNT 11
/**
 * @brief Trip Class
 *
 * Contains the information for a trip including the list of colleges
 * to be visited, the most efficient order to visit them in, the
 * total distance of the trip, the total amount spent during the trip,
 * and the souvenirs purchased from each college.
 */
class Trip{
public:
    /**
     * @brief Trip default constructor that initializes data members to the
     *        default values.
     */
    Trip();

    /**
     * @brief Trip overloaded constructor that allows the specification of a starting
     *        college.
     * @param inputStartCollege - College where trip will begin.
     */
    Trip(QString inputStartCollege);

    /**
     * @brief Adds another college to the vector of colleges being visited.
     * @param inputCollege - Next college that will be visited.
     */
    void visitNewCollege(QString inputCollege);

    /**
     * @brief Calculates the grand total spent during the trip.
     */
    void calculateTotalPrice();

    /**
     * @brief Returns the value calculated by calculateTotalPrice()
     * @return Grand total spent during the trip
     */
    double getTotalPrice();

    /**
     * @brief Returns the total amount spent at a specifed college
     * @param inputCollege - College whose total amount spent is desired
     * @return Total amount spent at inputCollege
     */
    double getTotalAmountSpentAtCollege(QString inputCollege);

    /**
     * @brief Returns the number of souvenirs that were purchased at a specified college
     * @param inputCollege - College whose total number of souvenirs purchased is desired.
     * @return Total number of souvenirs purchased at inputCollege
     */
    int getTotalItemAtCollege(QString inputCollege);

    //pass the inputs using the info from the database
    //sets quantity
    /**
     * @brief Purchases an item by adding it to the trackPurchase vector.
     * @param inputCollege - Name of college where souvenir was purchased
     * @param inputItem - Name of souvenir purchased
     * @param quantity - Quantity sold of souvenir
     * @param price - Price of souvenir
     */
    void buyItem(QString inputCollege, QString inputItem, int quantity, double price);

    //adds to the quantity
    /**
     * @brief Increases the quantity of the souvenir if it is purchased at two different times
     * @param inputCollege - Name of college where souvenir was purchased
     * @param inputItem - Name of souvenir purchased
     * @param quantity - Quantity sold of souvenir
     * @param price - Price of souvenir
     */
    void addItem(QString inputCollege, QString inputItem, int quantity, double price);

    /**
     * @brief Finds the next college to be visited that is the closest to the current
     *        college.
     */
    void findSmallestRoute();

    /**
     * @brief findShortestRoute finds the shortest route of the colleges visited by calling recursive function
     * @param amount of colleges to visit
     */
    void findSmallestRoute(int collegeAmount);

    /**
     * @brief Returns the total distance travelled during the trip.
     * @return Total distance travelled during the trip.
     */
    double getTravelDistance();

    /**
     * @brief Outputs the trip in the most efficient order.
     */
    void outputCollegeTripSmallestRoute();

    /**
     * @brief Outputs the trip in the order the colleges were added to the
     *        collegeList vector.
     */
    void outputCollegeTripNoOrder();

    /**
     * @brief Outputs the values stored in the collegList vector.
     */
    void printCollegeList();

    /**
     * @brief Sets all private data members to their default values.
     */
    void clear();

    /**
     * @brief Returns the vector of Souvenirs purchased at a specified college.
     * @param campusName - College whose souvenir list is desired
     * @return Vector of Souvenirs for a specified college.
     */
    QVector <Souvenir> getSouvenirVector(QString campusName);

    /**
     * @brief Returns the vector of colleges that will be visited during the trip.
     * @return Vector of college names to be visited.
     */
    QVector<Campus> getCampusVector();

    /**
     * @brief Returns the most efficient route that should be taken during the trip.
     * @return Vector of college names in the most efficient order.
     */
    QVector <QString> getRoute();


private:


    //! A private variable.
    /*!
        Name of college where trip will begin
    */
    QString startingCollege; //must append first;

    //! A private variable.
    /*!
        Vector of colleges to be visited during the trip
    */
    QVector <QString> collegeList;

    //! A private variable.
    /*!
       Vector of colleges to be visited sorted in the most efficient order.
    */
    QVector <QString> tripRoute;

    //! A private variable.
    /*!
        Vector of Campus objects that keeps track of the souvenirs purchased
        at each college.
    */
    QVector <Campus> trackPurchase;

    //! A private variable.
    /*!
        Total distance travelled during the trip.
    */
    double totalDistance;

    //! A private variable.
    /*!
        Grand total spent during the trip.
    */
    double totalPrice;

    /**
     * A private method.
     *
     * @brief Adds the college to be visited onto the trackPurchase and collegeList
     *        vectors.
     * @param inputCollege - College that will be visited.
     */
    void trackNewCollege(QString inputCollege);

    /**
     * A private method.
     *
     * @brief Checks to see if all colleges in the trackPurchase vector have been
     *        visited when calculating the most efficient route.
     * @return
     */
    bool isAllCampusVisited();

    //When using this function, ALWAYS pass in totalDistance collegeList.begin();
    //DO NOT CALL THIS FUNCTION
    /**
     * A private method.
     *
     * @brief Recursive function that calculates the most effient trip route.
     * @param sum - Total distance travelled.
     * @param startCollegeLocation - College whose proximity to the next closest college
     *                               will be determined.
     * @return Total distance travelled during trip.
     */
    double calculateSmallestRoute(double sum, QVector <QString>::iterator startCollegeLocation);

    /**
     * @brief calculateSmallestRoute recursive function to find ideal trip route given a list of colleges and distances
     * @param distance total distance being accumulated
     * @param startCollegeLocation iterator that determines where the list will begin upon each iteration
     * @param campusesLeft amount of campuses left
     * @return total distance traveled
     */
    double calculateSmallestRoute(double distance, QVector <QString>::iterator startCollegeLocation, int campusesLeft);

    /**
     * A private method.
     *
     * @brief Checks off a college once it is visited.
     * @param inputCollege - College whose visited status is being determined.
     */
    void checkOffCollege(QString inputCollege);

};

#endif // TRIP_H
