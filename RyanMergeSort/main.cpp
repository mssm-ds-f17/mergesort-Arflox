#include <iostream>
#include <functional>
#include <vector>

#include "gtest/gtest.h"

using namespace std;

class Thing {
public:
    int id;
    int thingNum;
    string name;
//    Thing();
    Thing(int id = 0);
};

Thing::Thing(int id) {
    this->id = id;
}

// return number from 1 to maxValue inclusive
int random(int maxValue) {
    return rand() % maxValue + 1;
}

// ids will range from 1 to maxId
// thingNum will be ascending order
vector<Thing> makeRandomThings(int count, int maxId)
{
    vector<Thing> things;

    for (int i = 0; i < count; i++) {
        things.push_back(Thing{random(maxId)});
    }

    for (int i = 0; i < count; i++) {
        things[i].thingNum = i+1;
    }

    return things;
}

void printVector(vector<Thing> values)
{
    for (unsigned int i = 0; i < values.size(); i++) {
        cout << values[i].thingNum << ", " << values[i].id << endl;
    }
}

// assuming already sorted
bool isStable(vector<Thing> values)
{
    for (int i = 0; i < (int)values.size()-1; i++) {
        if (values[i].id == values[i+1].id) {
            if (values[i].thingNum > values[i+1].thingNum) {
                return false;
            }
        }
    }
    return true;
}

// one option we won't use this time
bool operator<(const Thing& a, const Thing& b) {
    return a.id < b.id;
}

// return true if thing a is less than thing b
bool compareThingsById(const Thing& a, const Thing& b) {
    return a.id < b.id;
}

bool isSorted(const vector<Thing>& values, function<bool(const Thing& a, const Thing& b)> comp) {
    for (unsigned int i = 1; i < values.size(); i++) {
        if (comp(values[i], values[i-1])) {
            return false;
        }
    }
    return true;
}

// assumes that things is sorted (precondition)
// postcondition things is still sorted and 'value' is in the vector
void insertValue(const Thing& value, vector<Thing>& things)
{
    // find spot to put the value
    unsigned int insertionPoint = 0;
    for (; insertionPoint < things.size(); insertionPoint++) {
        if (value < things[insertionPoint]) {
            break;
        }
    }

    things.resize(things.size()+1); // create a space at the end

    // I now know the spot

    // move everything from there to the end up by one space

    for (unsigned int i = things.size()-1; i > insertionPoint+1; i--) {
        things[i] = things[i-1];
    }

    // put the value in that spot
}

void insertionSort(vector<Thing>& values, function<bool(const Thing& a, const Thing& b)> comp) {

    // create an empty output vector
    // loop through first vector
    //     insert into output vector in the right spot

    vector<Thing> output;

    for (Thing& t : values) {
        insertValue(t, output);
    }

    values = output;
}

/*vector<vector<Thing>> split(vector<Thing>& vals){
    vector<vector<Thing>> a;
     for (int i = 0; i < vals.size(); i++) {
   a.pushback(vector<Thing> b);
    a[i][0].id = vals[i].id;
}
return a;
}*/
void mergeFunc(vector<Thing>& values, int bottom, int top, int mid, function<bool(Thing, Thing)> comp){

    vector<Thing> newValues;

    int lowIndex = bottom;
    int midIndex = mid + 1;

    while(lowIndex <= mid && midIndex <= top){

        if(comp(values[lowIndex], values[midIndex])){
            newValues.push_back(values[lowIndex]);
            lowIndex++;
        }
        else{
            newValues.push_back(values[midIndex]);
            midIndex++;
        }
    }

    while(lowIndex <= mid){
        newValues.push_back(values[lowIndex]);
        lowIndex++;
    }

    while(midIndex <= top){
        newValues.push_back(values[midIndex]);
        midIndex++;
    }

    for (int i = bottom; i <= top; i++){
        values[i] = newValues[i-bottom];
    }
}

void RecursiveMerge(vector<Thing>& values, int lowestIndex, int highestIndex, function<bool(Thing, Thing)> comp){

    int middleIndex;

    if(lowestIndex < highestIndex){
        middleIndex = (lowestIndex+highestIndex)/2;

        RecursiveMerge(values, lowestIndex, middleIndex, comp);
        RecursiveMerge(values, middleIndex+1, highestIndex, comp);

        mergeFunc(values, lowestIndex, highestIndex, middleIndex, comp);
    }

}
void mergeSort(vector<Thing>& values, function<bool(Thing, Thing)> comp){
    int lowestIndex = 0;
    int highestIndex = values.size()-1;

    RecursiveMerge(values, lowestIndex, highestIndex, comp);
}



TEST(SortThing, sortNone) {
    vector<Thing> v;
    mergeSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, sortOne) {
    vector<Thing> v { 5 };
    mergeSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, sortTwoA) {
    vector<Thing> v { 3, 4 };
    mergeSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, sortTwoB) {
    vector<Thing> v { 4, 3 };
    mergeSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, sortTwoC) {
    vector<Thing> v { 4, 4 };
    mergeSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortThing, sortThree) {
    vector<Thing> v{ 3, 1, 2 };
    mergeSort(v, compareThingsById);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortTest, isEmptyVectorSorted) {
    vector<Thing> v;
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortTest, isSingleValueVectorSorted) {
    vector<Thing> v { 1 };
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortTest, isSortedVectorSorted) {
    vector<Thing> v { 1,3,5,5,9,100 };
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortTest, isUnsortedVectorSorted) {
    vector<Thing> v { 1,3,5,1,9,100 };
    ASSERT_FALSE(isSorted(v, compareThingsById));
}

int main(int argc, char **argv) {
    Thing a;
    Thing b;
    Thing c;
    Thing d;
     a.id = 0;
     b.id = 5;
     c.id = 3;
     d.id = 10;
vector<Thing> v {a, b, c, d};
mergeSort(v,compareThingsById);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

