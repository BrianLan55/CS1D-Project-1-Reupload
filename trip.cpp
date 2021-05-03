#include "trip.h"

Trip::Trip(){
    totalDistance = 0;
    totalPrice = 0;
    startingCollege = "Default";
    }

Trip::Trip(QString inputStartCollege){
    totalDistance = 0;
    totalPrice = 0;
    startingCollege = inputStartCollege;
    trackNewCollege(startingCollege);
}

void Trip::visitNewCollege(QString inputCollege){
    trackNewCollege(inputCollege);
    if(startingCollege == "Default"){
        totalDistance = 0;
        totalPrice = 0;
        startingCollege = inputCollege;
    }
}

void Trip::calculateTotalPrice(){
    totalPrice = 0;
    QVector <Campus>::iterator ptr = trackPurchase.begin();
    while(ptr != trackPurchase.end()){
        totalPrice += ptr->getTotalAmountSpent();
        ptr++;
    }
}

double Trip::getTotalPrice(){
    this->calculateTotalPrice();
    return totalPrice;
}

double Trip::getTotalAmountSpentAtCollege(QString inputCollege){
   QVector <Campus>::iterator ptr = trackPurchase.begin();
   bool found = false;
   while(!found && ptr != trackPurchase.end()){
       if(inputCollege == ptr->getCampusName()){
           found = true;
       }
       else ptr++;
   }
   return ptr->getTotalAmountSpent();
}

//pass the inputs using the info from the database
void Trip::buyItem(QString inputCollege, QString inputItem, int quantity, double price){
    QVector <Campus>::iterator campusPtr = trackPurchase.begin();
    bool foundCollege = false;
    bool foundItem = false;
//    QString temp;
    while(!foundCollege && campusPtr != trackPurchase.end()){

//         temp = campusPtr->getCampusName(); //have to overide

        foundCollege = campusPtr->getCampusName() == inputCollege;
        if(foundCollege){


           QVector <Souvenir>::iterator itemPtr = campusPtr->getSouvenirListBegin();

           while(!foundItem && itemPtr != campusPtr->getSouvenirListEnd()){
//               QString item = itemPtr->getItemName();
               if(inputItem == itemPtr->getItemName()){
                    foundItem = true;
//                    qDebug() << ("Found");
               }
               else itemPtr++;
           }
       }
       if(!foundCollege) campusPtr++;

    }
    if(foundCollege && foundItem){
        campusPtr->setItemQuantity(inputItem, quantity);
        qDebug() << "setItemQuantity";
    }
    else{
        campusPtr->addSouvenir(inputItem, price, quantity);
        qDebug() << "addSouvenir";
    }

}

void Trip::addItem(QString inputCollege, QString inputItem, int quantity, double price){
    QVector <Campus>::iterator campusPtr = trackPurchase.begin();
    bool foundCollege = false;
    bool foundItem = false;
//    QString temp;
    while(!foundCollege && campusPtr != trackPurchase.end()){

//         temp = campusPtr->getCampusName(); //have to overide

        foundCollege = campusPtr->getCampusName() == inputCollege;
        if(foundCollege){

           QVector <Souvenir>::iterator itemPtr = campusPtr->getSouvenirListBegin();

           while(!foundItem && itemPtr != campusPtr->getSouvenirListEnd()){
//               QString item = itemPtr->getItemName();
               if(inputItem == itemPtr->getItemName()){
                    foundItem = true;
               }
               else itemPtr++;
           }
       }

       if(!foundCollege) campusPtr++;

    }

    if(foundCollege && foundItem){
        campusPtr->addItemQuantity(inputItem, quantity);
        qDebug() << "addItemQuantity";
    }
    else{
        campusPtr->addSouvenir(inputItem, price, quantity);
        qDebug() << "addSouvenir";
    }
}

void Trip::outputCollegeTripSmallestRoute(){
    QVector<QString>::iterator ptr = tripRoute.begin();
    while(ptr != tripRoute.end()){
        QDebug cout = qDebug();
        cout.noquote();
        cout << *ptr;
        ++ptr;
    }
}

void Trip::outputCollegeTripNoOrder(){
    QVector<QString>::iterator ptr = collegeList.begin();
    while(ptr != collegeList.end()){
        QDebug cout = qDebug();
        cout.noquote();
        cout << *ptr;
        ++ptr;
    }
}

void Trip::findSmallestRoute(){
    totalDistance = calculateSmallestRoute(totalDistance, collegeList.begin());
}
void Trip::findSmallestRoute(int collegeAmount) {
    totalDistance = calculateSmallestRoute(totalDistance, collegeList.begin(), collegeAmount);

}
double Trip::getTravelDistance(){
    if(totalDistance == 0 && startingCollege != "Default"){
         this->calculateSmallestRoute(totalDistance, collegeList.begin());
         qDebug() << "totalDistance == 0 && startingCollege != Default";
        return totalDistance;

    }
    else if (totalDistance != 0 && startingCollege != "Default"){
        qDebug() << "totalDistance != 0 && startingCollege != Default";
        return totalDistance;
    }
    else {
        qDebug() << "ERROR on findSmallestRoute\n";
        return 0;
    }
}

void Trip::trackNewCollege(QString inputCollege){
    Campus temp(inputCollege);
    trackPurchase.append(temp);

    collegeList.append(inputCollege);
}

bool Trip::isAllCampusVisited(){
    bool value = true;
    QVector <Campus>::iterator campusPtr = trackPurchase.begin();
    while(campusPtr != trackPurchase.end() && value){
        value = campusPtr->isVisited();
        ++campusPtr;
    }

    if (value) qDebug() << "Visited all campus";

    return value;
}

//When using this function, ALWAYS pass in totalDistance collegeList.begin();
//DO NOT CALL THIS FUNCTION
double Trip::calculateSmallestRoute(double distance, QVector <QString>::iterator startCollegeLocation){

    tripRoute.append(*startCollegeLocation);

    //Use Sql database to find the smallest distance between the campuses.
    //iterate to the ending college and mark the college that collegePtr points to as visited
    //repeat all of the campuses in trackPurchase is visited;

    QSqlQuery query = Database::getInstance()->getDistanceQuery(*startCollegeLocation);

    QVector <Campus>::iterator campusPtr = trackPurchase.begin();
    QVector <QString> visitedCollege;

    //gets list that includes visited college
    while(campusPtr != trackPurchase.end()){
        qDebug() << campusPtr->getCampusName();
         if(campusPtr->isVisited()){
             visitedCollege.append(campusPtr->getCampusName());
              qDebug() << "Adding " << campusPtr->getCampusName() << " to visitedCollege";
         }
        ++campusPtr;
    }

    double smallestDistance = 0;
    QString nextCollegeLocation;
    QVector <QString>::iterator checkCollegeList;
    bool found = false;
    //this entire while loop loops throuugh the query to get the shortest distance and the campus name
    while(query.next() && !found){ //advances to the next data row until the end
        qDebug() << "query's position: " << query.at();
        qDebug() << "Value: " << query.value(2).toDouble();

        bool invalidCollege;
        QVector<QString>::iterator ptr;

        checkCollegeList = collegeList.begin();
        //iterates through the college list
        while(checkCollegeList != collegeList.end()){

            invalidCollege = false;
            ptr = visitedCollege.begin();
            //first if statement executes first and only once
            //It starts the search for the smallest distance
            if (query.value(1) == *checkCollegeList && smallestDistance == 0){

               //invalidCollege is true the visited college equals to the campus in the college list ( vector private data member)
                while (!invalidCollege && ptr != visitedCollege.end()){
                    invalidCollege = *ptr == *checkCollegeList;
                    ++ptr;
                }
                //if the campus in checkCollegeList is not visited, initializes the smallest distance
                //and nextCollegeLocation
                if(!invalidCollege){
                    smallestDistance = query.value(2).toDouble();
                    nextCollegeLocation = query.value(1).toString();
                    found = true;
                }
            }
            //The else if statements after the above if statement is executed
            //it continues the search for the smallest distance
            //only the query with a smaller distance will get execute the else if statement
            else if (query.value(1) == *checkCollegeList && query.value(2).toDouble() < smallestDistance){

                //invalidCollege is true the visited college equals to the campus in the college list ( vector private data member)
                while (!invalidCollege && ptr != visitedCollege.end()){
                    invalidCollege = *ptr == *checkCollegeList;
                    ++ptr;
                }
                //if the campus is checkCollegeList is not visited and its distance is smaller,
                //replace the value of the smallest distance and campus name.
                if(!invalidCollege){
                    smallestDistance = query.value(2).toDouble();
                    nextCollegeLocation = query.value(1).toString();
                    found = true;
                }

            }
            ++checkCollegeList;
        }
    }

    //checks off the starting campus as visited.
    this->checkOffCollege(*startCollegeLocation);

    bool findEndingCollege = false;
    startCollegeLocation = collegeList.begin();
    qDebug() << nextCollegeLocation;
    //iterates to the destination college and exits when the iterator (startCollegeLocation) at the element
    while(!findEndingCollege && startCollegeLocation != collegeList.end()){
        findEndingCollege = *startCollegeLocation == nextCollegeLocation;
        if(!findEndingCollege) ++startCollegeLocation;
    }

    distance = smallestDistance;
    qDebug() << "distance: " << distance;

    if(this->isAllCampusVisited()){
        qDebug() << "end of recursion function";
         return distance;
     }
     else{
        qDebug() << "Calling calculateSmallestRoute again";
         return distance + calculateSmallestRoute(distance, startCollegeLocation);
     }

}
// Recursive function used for finding the shortest route of distances from university of michigan
double Trip::calculateSmallestRoute(double distance, // OUT - total distance in trip
                                 QVector <QString>::iterator startCollegeLocation, // IN -iterator pointing to current college
                                 int campusesLeft) {//IN - campuses left to find closest college excluding colleges already in trip
    tripRoute.append(*startCollegeLocation);

    //Use Sql database to find the smallest distance between the campuses.
    //iterate to the ending college and mark the college that collegePtr points to as visited
    //repeat all of the campuses in trackPurchase is visited;

    QSqlQuery query = Database::getInstance()->getDistanceQuery(*startCollegeLocation);

    QVector <Campus>::iterator campusPtr = trackPurchase.begin();
    QVector <QString> visitedCollege;

    //gets list that includes visited college
    while(campusPtr != trackPurchase.end()){
        qDebug() << campusPtr->getCampusName();
         if(campusPtr->isVisited()){
             visitedCollege.append(campusPtr->getCampusName());
              qDebug() << "Adding " << campusPtr->getCampusName() << " to visitedCollege";
         }
        ++campusPtr;
    }

    double smallestDistance = 0;
    QString nextCollegeLocation;
    QVector <QString>::iterator checkCollegeList;

    //this entire while loop loops throuugh the query to get the shortest distance and the campus name
    while(query.next()){ //advances to the next data row until the end
        qDebug() << "query's position: " << query.at();
        qDebug() << "Value: " << query.value(2).toDouble();


        bool invalidCollege;
        QVector<QString>::iterator ptr;

        checkCollegeList = collegeList.begin();
        //iterates through the college list
        while(checkCollegeList != collegeList.end()){

            invalidCollege = false;
            ptr = visitedCollege.begin();
            //first if statement executes first and only once
            //It starts the search for the smallest distance
            if (query.value(1) == *checkCollegeList && smallestDistance == 0){

               //invalidCollege is true the visited college equals to the campus in the college list ( vector private data member)
                while (!invalidCollege && ptr != visitedCollege.end()){
                    invalidCollege = *ptr == *checkCollegeList;
                    ++ptr;
                }
                //if the campus in checkCollegeList is not visited, initializes the smallest distance
                //and nextCollegeLocation
                if(!invalidCollege){
                    smallestDistance = query.value(2).toDouble();
                    nextCollegeLocation = query.value(1).toString();
                }
            }
            //The else if statements after the above if statement is executed
            //it continues the search for the smallest distance
            //only the query with a smaller distance will get execute the else if statement
            else if (query.value(1) == *checkCollegeList && query.value(2).toDouble() < smallestDistance){

                //invalidCollege is true the visited college equals to the campus in the college list ( vector private data member)
                while (!invalidCollege && ptr != visitedCollege.end()){
                    invalidCollege = *ptr == *checkCollegeList;
                    ++ptr;
                }
                //if the campus is checkCollegeList is not visited and its distance is smaller,
                //replace the value of the smallest distance and campus name.
                if(!invalidCollege){
                    smallestDistance = query.value(2).toDouble();
                    nextCollegeLocation = query.value(1).toString();
                }

            }
            ++checkCollegeList;
        }
    }

    //checks off the starting campus as visited.
    this->checkOffCollege(*startCollegeLocation);

    bool findEndingCollege = false;
    startCollegeLocation = collegeList.begin();
    qDebug() << nextCollegeLocation;
    //iterates to the destination college and exits when the iterator (startCollegeLocation) at the element
    while(!findEndingCollege && startCollegeLocation != collegeList.end()){
        findEndingCollege = *startCollegeLocation == nextCollegeLocation;
        if(!findEndingCollege) ++startCollegeLocation;
    }

    distance = smallestDistance;
    qDebug() << "distance: " << distance;

    if(this->isAllCampusVisited() || campusesLeft == 1){
        qDebug() << "end of recursion function";
         return distance;
     }
     else{
        qDebug() << "Calling calculateSmallestRoute again";
         return distance + calculateSmallestRoute(distance, startCollegeLocation, campusesLeft-1);
     }

}


void Trip::checkOffCollege(QString inputCollege){
    QVector <Campus>::iterator campusPtr = trackPurchase.begin();
    bool found = false;
    while(campusPtr != trackPurchase.end() && !found){
         if(campusPtr->getCampusName() == inputCollege){
             qDebug() << "visitCampus";
             campusPtr->visitCampus();
             found = true; //makes the algorithm faster.
         }
        ++campusPtr;
    }
}

void Trip::printCollegeList()
{
    QVector<QString>::iterator collegePtr = collegeList.begin();

    while(collegePtr != collegeList.end())
    {
        qDebug() << *collegePtr << endl;

        collegePtr++;
    }
}

void Trip::clear(){
    collegeList.clear();
    tripRoute.clear();
    trackPurchase.clear();
//    for(QVector<Campus>::iterator ptr = trackPurchase.begin();ptr!=trackPurchase.end();++ptr){
//        ptr->clear();
//    }
    totalDistance = 0;
    totalPrice = 0;

}

QVector <QString> Trip::getRoute()
{
    return tripRoute;
}

QVector <Souvenir> Trip::getSouvenirVector(QString campusName)
    {
    QVector <Campus>::iterator ptr = trackPurchase.begin();
    bool found = false;
    while(ptr!= trackPurchase.end() && !found)
    {
        found = campusName == ptr->getCampusName();
        if (!found) ++ptr;
        qDebug() << ptr->getCampusName();
    }

    return ptr->getSouvenirVector();
}

QVector <Campus> Trip::getCampusVector(){
    return trackPurchase;
}

int Trip::getTotalItemAtCollege(QString inputCollege){
    QVector<Campus>::iterator ptr = trackPurchase.begin();
    bool found = false;
    int sum = 0;
    while(ptr != trackPurchase.end()){
        found = inputCollege == ptr->getCampusName();
        if(!found) ++ptr;
    }

    if(found){
        sum = ptr->getTotalItemQuantity();
    }

    return sum;
}
