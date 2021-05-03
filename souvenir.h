#ifndef SOUVENIR_H
#define SOUVENIR_H

#include <QString>

/**
 * @brief Souvenir Class
 *
 * Contains the information for a souvenir
 * including its name, price, and the quantity sold.
 */
class Souvenir{
public:
    /**
     * @brief Souvenir constructor that initializes the data members of a
     *        souvenir object to the default values.
     */
    Souvenir(){
        quantity = 0;
        price = 0;
        item = "Default Value";
    }

    /**
     * @brief Returns the name of the souvenir.
     * @return Name of souvenir
     */
    QString getItemName(){
        return item;
    }

    /**
     * @brief Returns the price of the souvenir.
     * @return Price of the souvenir
     */
    double getItemPrice(){
        return price;
    }

    /**
     * @brief Returns the quantity sold.
     * @return Quantity sold
     */
    int getItemQuantity(){
        return quantity;
    }

    /**
     * @brief Sets the name of a Souvenir object.
     * @param input - Name of the souvenir.
     */
    void setItemName(QString input){
        item = input;
    }

    /**
     * @brief Sets the price of a Souvenir object.
     * @param input - Price of souvenir
     */
    void setItemPrice(double input){
        price = input;
    }

    /**
     * @brief Sets the quantity sold of a Souvenir object.
     * @param input - Quantity sold
     */
    void setItemQuantity(int input){
        quantity = input;
    }

    /**
     * @brief Adds another quantity to the current quantity sold of a souvnir.
     * @param input - Quantity to be added to current quantity
     */
    void addItemQuantity(int input){
        quantity += input;
    }

    /**
     * @brief Initializes Souvenir object's data members to the default values.
     */
    void clear(){
        item  = "Default Value";
        price = 0;
        quantity = 0;
    }

private:
    //! A private variable.
    /*!
     * @brief Souvenir name
     */
    QString item;

    //! A private variable.
    /*!
     * @brief Souvenir price
     */
    double price;

    //! A private variable.
    /*!
     * @brief Quantity sold
     */
    int quantity;
};

#endif // SOUVENIR_H
