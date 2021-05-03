#include "campus.h"

Campus::Campus(){
    totalAmount = 0;
    campusName  = "Default Value";
    visit = false;
    numSouvenirs = 0;
}

Campus::Campus(QString inputName){
    campusName  = inputName;
    totalAmount = 0;
    visit = false;
    numSouvenirs = 0;
}

void Campus::setCampusName(QString inputName){
    campusName = inputName;
}

void Campus::visitCampus(){
    qDebug() << this->getCampusName() << "is visited.";
    visit = true;
}

bool Campus::isVisited(){
    return visit;
}

void Campus::addSouvenir(QString inputSouvenir, double inputPrice, int inputQuantity){
    Souvenir add;
    add.setItemName(inputSouvenir);
    add.setItemPrice(inputPrice);
    add.setItemQuantity(inputQuantity);
    sList.append(add);
}

QString Campus::getCampusName(){
    return campusName;
}

void Campus::calculateTotalAmountSpent(){
    totalAmount = 0;

    QVector <Souvenir>::iterator ptr = sList.begin();
    while(ptr != sList.end()){ //end() is the last element after the last data element
        totalAmount += (ptr->getItemPrice() * ptr->getItemQuantity());
        ptr++;
    }
}

double Campus::getTotalAmountSpent(){
    this->calculateTotalAmountSpent();
    return totalAmount;
}

int Campus::getNumSouvenirs()
{
    QVector<Souvenir>::iterator souvenirPtr = sList.begin();

    numSouvenirs = 0;

    while(souvenirPtr != sList.end())
    {
        numSouvenirs += souvenirPtr->getItemQuantity();

        souvenirPtr++;
    }

    return numSouvenirs;
}

int Campus::getItemQuantity(QString inputItem){
   QVector <Souvenir>::iterator ptr = sList.begin();
   bool found = false;
   while(!found && ptr != sList.end()){
        if(inputItem == ptr->getItemName()){
            found = true;
        }
        else ptr++;
   }
   return ptr->getItemQuantity();
}

void Campus::setItemQuantity(QString inputItem, int inputQuantity){
    QVector <Souvenir>::iterator ptr = sList.begin();
    bool found = false;
    while(!found && ptr != sList.end()){
        if(inputItem == ptr->getItemName()){
            found = true;
        }
        else ptr++;
    }

    if(found){
        ptr->setItemQuantity(inputQuantity); //Souvenir class has same function name
    }
}

void Campus::addItemQuantity(QString inputItem, int inputQuantity){
    QVector <Souvenir>::iterator ptr = sList.begin();
    bool found = false;
    while(!found && ptr != sList.end()){
        if(inputItem == ptr->getItemName()){
            found = true;
        }
        else ptr++;
    }

    if(found){
        ptr->addItemQuantity(inputQuantity); //Souvenir class has same function name
    }
}

QVector<Souvenir>::iterator Campus::getSouvenirListBegin(){
     return sList.begin();
}

QVector<Souvenir>::iterator Campus::getSouvenirListEnd(){
    return sList.end();
}

void Campus::clear(){
    sList.clear();

//    for (QVector <Souvenir>::iterator ptr = sList.begin(); ptr != sList.end(); ++ptr){
//        *ptr->clear();
//    }
    campusName = "Default Value";
    totalAmount = 0;
    visit = false;
}

QVector <Souvenir> Campus::getSouvenirVector(){
    return sList;
}

void Campus::removeItem(QString item){
    QVector<Souvenir>::iterator ptr = sList.begin();
    bool found = false;
    while(ptr!= sList.end() && !found){
        found = ptr->getItemName() == item;
        if(!found) ++ ptr;
    }
    sList.erase(ptr);
}

int Campus::getTotalItemQuantity(){
    int sum = 0;
    QVector<Souvenir>::iterator ptr = sList.begin();
    while(ptr != sList.end()){
        sum += ptr->getItemQuantity();
        ++ptr;
    }
    return sum;
}
