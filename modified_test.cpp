/**************************************************************************************************************************
    Purpose: A simple student data manager that the Software Testing class can use to keep a record of student performance
             throughout the course. See documentation for additionally requirement to use this system.
****************************************************************************************************************************/
#define CATCH_CONFIG_MAIN

#include <stdio.h>
#include <vector>
#include <cstring>
//#include <string>//modified: delete
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <fstream>//modified:
#include <iostream>//modified:
#include "catch.hpp"

using namespace std;




string filename;
/*************************************************************************
 * A struct that hold a student's name, USF ID, email, presentation grade, essay grage and project grade.
 ***************************************************************************/
struct Student {
    //int name[41], USF_ID[11], email[41];
    char name[41], USF_ID[11], email[41];//modifed: vector<int>
    int presGrade, essayGrade, projectGrade;
    //Student constructor
    Student(char name_[41], char USF_ID_[11], char email_[41], int presGrade_, int essayGrade_, int projectGrade_)
    {
        strcpy(name, name_);
        strcpy(USF_ID, USF_ID_);
        strcpy(email, email_);
        presGrade = presGrade_;
        essayGrade = essayGrade_;
        projectGrade = projectGrade_;
    }
};

/*modified: declare some functions:*/
void addStudent(vector<Student> &studentVector);
vector<int> searchByName(vector<Student> studentVector);
void StudentUpdateMain(int index, vector<Student> &studentVector);
bool fileGradeCheck(string dataInfo, int &gradeUpdate);
int inputGradeCheck(string type);
int case_insensitive(string s1, string s2);
string stringInputCheck(string phrase, unsigned int size);
vector<Student> readDataFile(string filename, bool &opened); //modified: pass a reference bool
void writeToFile(string filename, vector<Student> studentData);
void updateAGradeType(string type, vector<Student> &studentVector);
bool deleteStudent(int index, vector<Student> &studentVector);
void updateInfo(string type, int index, vector<Student> &studentVector);
vector<int> searchByName(vector<Student> studentVector);
int chosenNameIndex(vector<int> studentIndexList, vector<Student> studentData);
int retrieveDataIndex(string searchBy, vector<Student> studentVector);
void printAllStudents(vector<Student> studentData);
void printOneStudent(int index, vector<Student> studentVector);


/*Main function*/
int old_main()//modified: change Main to main
{
    //dataFile, option;
    string dataFile, option;//modified
    //fileOpened;
    bool fileOpened;//modified: initial variable

    cout << "\nPlease enter the data file name: ";
    cin >> filename;
    //read data from the data file
    vector<Student> studentData = readDataFile(filename, fileOpened);
    if (!fileOpened) return 0;

    //next;
    bool next = true;//modified
    while (next) //while not exit the program
    {
        while (true) //while invalid menu input selection
        {
            //Main menu for the class-roll system
            cout << "\n\n**************** CEN4072  Class-Roll Maintenance ****************\n";
            cout << "\t1. Display all students info\n";
            cout << "\t2. Search/update/delete student by name [first last]\n";
            cout << "\t3. Search/update/delete student by USF ID [U-123455678]\n";
            cout << "\t4. Search/update/delete student by email\n";
            cout << "\t5. Add a student\n";
            cout << "\t6. Update all presentation grades\n";
            cout << "\t7. Update all essay grades\n";
            cout << "\t8. Update all project grades\n";
            cout << "\t9. Exit\n";
            cout << "*********************************************************************\n\n";
            cout << "Choose your menu option: ";

            //Accept menu selection
            cin >> option;
            if (option.length() != 1)
                cout << "ERROR: Invalid input.\n";
            else
                break;//modified: add break; to end loop

        }
        cout << endl;
        //nameList;
        vector<int> nameList;//modified

        //Perform functionality for the selected option
        switch (stoi(option))
        {
        //case 1:  addStudent(studentData);  //display all students
        case 1:  printAllStudents(studentData);//modified to call function printAllStudents
            break;//modified: add break
        case 2: //search student by name
            nameList = searchByName(studentData);
            if (nameList.size() == 0) //if no matched student found
                cout << "NO DATA: This Student Does Not Exist\n\n";
            else if (nameList.size() == 1) //if one matched student found
                StudentUpdateMain(nameList.at(0), studentData);
            else //if more than one matched students
                StudentUpdateMain(nameList.at(1), studentData);
            break;//modified: add break

        case 3: StudentUpdateMain(retrieveDataIndex("student USF ID", studentData), studentData);  //search student by USF ID
            break;//modified: add break
        case 4: StudentUpdateMain(retrieveDataIndex("student email", studentData), studentData); //search student by email
            break;//modified: add break
        //case 5: printAllStudents(studentData);   //add a new student
        case 5: addStudent(studentData);//modified to call function addStudent(studentData);
            break;//modified: add break
        case 6: updateAGradeType("presentation", studentData);  //update presentation grade for all student
            break;//modified: add break
        case 7: updateAGradeType("essay", studentData); //update essay grade for all student
            break;//modified: add break
        case 8: updateAGradeType("project", studentData);   //update project grade for all student
            break;//modified: add break
        case 9: //exit the program
            next = false;
            cout << "\nProgram Exit...";
            break;//modified: add break
        default: cout << "ERROR: Invalid Option.\n";

        }
    }
    return 0;
}

/*************************************************************************
 * Function that managing the update for a specific students
 **************************************************************************/
void StudentUpdateMain(int index, vector<Student> &studentVector)//modified: add symbol &, pass by reference
{
    if (index != -1)   //if student found
    {
        bool next = true;
        while (next)
        {
            //print a specific student's information and sub-action menu
            printOneStudent(index, studentVector);
            cout << "Student: " << studentVector[index].name << endl;
            cout << "1. Delete from Class-Roll System" << endl;
            cout << "2. Update Presentation Grade" << endl;
            cout << "3. Update Essay Grade" << endl;
            cout << "4. Update Project Grade" << endl;
            cout << "5. Back to Main Menu" << endl << endl;

            string sub_option;
            while (true) //while invalid input
            {
                cout << "Choose your action for student \"" << studentVector[index].name << "\": ";
                cin >> sub_option;
                if (sub_option.length() != 1)
                    cout << "ERROR: Invalid input.\n";
                else
                    break;//modified: add break to end the loop
            }

            //perform functionality for the selected sub-menu option
            switch (stoi(sub_option))
            {//modified: delete symbol &
            case 1: next = deleteStudent(index, studentVector) ? false : true; //delete this student
                break;//modified: add break 
            case 2: updateInfo("presentation", index, studentVector);    //update this student's presentation grade
                break;//modified: add break 
            case 3: updateInfo("essay", index, studentVector);   //update this student's essay grade
                break;//modified: add break 
            case 4: updateInfo("project", index, studentVector); //update this student's project grade
                break;//modified: add break 
            case 5: //back to main menu
                next = false;
                cout << "Note: Back to main menu\n";
                break;//modified: add break 
            default: cout << "ERROR: Invalid option\n";


            }
        }//modified add symbol }
    }//modified add symbol }
    else cout << "NO DATA: This Student Does Not Exist\n\n";  //if student not found
}

/*************************************************************************
 * Function that checks and returns if the grade in the data file are valid or not.
 * Return true for invalid data-to be skip, and false for valid data
 **************************************************************************/
bool fileGradeCheck(string dataInfo, int &gradeUpdate)//add & pass by reference
{
    if (dataInfo.length() == 0)    //if no data, then store -1 as the not graded yet
        gradeUpdate = -1;
    else if (dataInfo.length() == 1)  //check for grade range
    {
        gradeUpdate = stoi(dataInfo);
        if (gradeUpdate < -1 || gradeUpdate>4) return true;
    }
    else if (dataInfo.length() > 1)    //invalid data due to string size is more than 1
        return true;
    return false;
}

/*************************************************************************
 * Function that checks and returns the correct/valid input grade
 **************************************************************************/
int inputGradeCheck(string type)
{
    while (true) //while input grade is invalid, continue to ask for input
    {
        cout << "Enter new " << type << " grade [A=4,B=3,C=2,D=1,F=0,Not graded yet=-1]: ";
        string input;
        //bool bad;//modified: unreferenced local variable, delete it
        cin >> input;

        if (input.compare("-1") == 0)  //Check for input grade
            return -1;
        else if (input.compare("4") == 0 || input.compare("3") == 0 || input.compare("2") == 0 || input.compare("1") == 0 || input.compare("0") == 0)
            return stoi(input);
        else cout << "ERROR: Invalid Grade, Please Re-enter.\n";
    }
}

/*************************************************************************
 * Function that matching the string with case insensitive
 **************************************************************************/
int case_insensitive(string s1, string s2)
{
    //Convert both string to lower case and then compare
    transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
    transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
    //if (s1.compare(s2) = 0) return 1;
    if (s1.compare(s2) == 0) return 1;//modified
    return 0;
}

/*************************************************************************
 * Function that check and return the string if the length size are valid
 *************************************************************************/
string stringInputCheck(string phrase, unsigned int size)
{
    while (true) //while input string size and format are incorrect
    {
        cout << "Enter " << phrase;
        string input;
        getline(cin, input);

        if (size == 10) //check for USF-ID start with 'U-'
        {
            if (input.size() != 10 || input.at(0) != 'U' || input.at(1) != '-')
                cout << "ERROR: Invalid Format.\n";
            else return input;
        }
        else    //check for string(name, USF-ID and email) length size
        {
            if (input.size() > size)
                cout << "ERROR: Reached Maximum Characters Length Of " << size << endl;
            else return input;
        }
    }
}

/*************************************************************************
 * Function that read from the file that contains all students' data information and it returns a vector of Student struct
 **************************************************************************/
 //void readDataFile(string filename, bool opened)
vector<Student> readDataFile(string filename, bool &opened)//modified
{
    ifstream dataFile(filename);    //open data file
    string line, data;
    vector<Student> studentInfo;

    if (!dataFile.is_open())//modified: change !dataFile.is_open to !dataFile.is_open()
    {
        cout << "Could not open " << filename << ".\n";
        //false = opened;
        opened = false;//modified
        return studentInfo;
    }
    else opened = true; //modified: if it opened the file, opened is set to true
    cout << "\nStart reading the student data file " << filename << ".\n";

    if (dataFile.good())//modified change dataFile.good to dataFile.good()
        //If file opened, read the data line by line
    {
        int lineCount = 1;
        getline(dataFile, line);
        //int dataPresGrade, dataEssayGrade, dataProjectGrade;
        while (getline(dataFile, line))   //while still getting the line
        {
            bool skip = false, skipTemp;
            char dataName[101], dataID[101], dataEmail[101];
            int dataPresGrade, dataEssayGrade, dataProjectGrade;

            vector<string> tempData;
            lineCount++;

            //Check for data, if value store each field to the corresponding Student struct data member
            stringstream ss(line);
            getline(ss, data, ',');
            strcpy(dataName, data.c_str());
            if (data.length() > 40)skip = true;
            getline(ss, data, ',');
            strcpy(dataID, data.c_str());
            if (data.length() != 10)skip = true;
            getline(ss, data, ',');
            strcpy(dataEmail, data.c_str());
            if (data.length() > 40)skip = true;
            getline(ss, data, ',');
            skipTemp = fileGradeCheck(data, dataPresGrade);  
            if (skipTemp) skip = true;
            getline(ss, data, ',');
            skipTemp = fileGradeCheck(data, dataEssayGrade);
            if (skipTemp) skip = true;
            getline(ss, data, ',');
            skipTemp = fileGradeCheck(data, dataProjectGrade);
            if (skipTemp) skip = true;

            //If invalid data format or length size, skip it and print to the console
            if (skip) {
                cout << "Read Data Error: Line " << lineCount << " skip due to data length or data type\n";
                continue;
            }
            //store the valid student to the studentInfo vector
            Student s(dataName, dataID, dataEmail, dataPresGrade, dataEssayGrade, dataProjectGrade);
            studentInfo.push_back(s);
        }
    }
    dataFile.close();

    cout << "Finished reading the student data file " << filename << ".\n\n";
    return studentInfo;
}

/**************************************************************************
 * Function that write the update data to the file
 ***************************************************************************/
void writeToFile(string filename, vector<Student> studentData)
{
    ofstream dataFile(filename);
    
    unsigned int i;//modified: declare a variable outside of a loop
    if (!dataFile.is_open()) cout << "\nCould not open " << filename << ".\n";
    //if file is good
    if (dataFile.good()) {
        
        //write all student data to the file
        dataFile << "name,UID,email,presentation,essay,project\n";
        
        for ( i = 0; i < studentData.size(); i++)//modified: delete symbol ;
        {
            dataFile << studentData[i].name << "," << studentData[i].USF_ID << "," << studentData[i].email << ",";
            studentData[i].presGrade == -1 ? (dataFile << ",") : (dataFile << studentData[i].presGrade << ",");
            studentData[i].essayGrade == -1 ? (dataFile << ",") : (dataFile << studentData[i].essayGrade << ",");
            studentData[i].projectGrade == -1 ? (dataFile << "\n") : (dataFile << studentData[i].projectGrade << "\n");
        }
    }
    dataFile.close();
}

/*************************************************************************
 * Function that updates all students' presentation, essay or project grade
 **************************************************************************/
void updateAGradeType(string type, vector<Student> &studentVector)//modified: add symbol &, pass by reference
{
    int grade = inputGradeCheck(type);    //check for grade input validation
    string yORn;
    cout << "Are you sure you want to update all students' " << type << " grade to ";
    (grade == -1) ? cout << " ungraded?[yes or no]: " : cout << grade << "?[yes or no]: ";

    //If confirmed, update the corresponding field and write to the files
    cin >> yORn;
    unsigned int i;//modified: declare a variable outside of a loop
    if (case_insensitive(yORn, "yes")) {
        //for (int i = 0; i < studentVector.size(); i++);
        for (i = 0; i < studentVector.size(); i++) {//modified: add {} to for loop

            if (case_insensitive(type, "presentation"))
                studentVector.at(i).presGrade = grade;
            else if (case_insensitive(type, "essay"))
                studentVector.at(i).essayGrade = grade;
            else if (case_insensitive(type, "project"))
                studentVector.at(i).projectGrade = grade;
        }
        writeToFile(filename, studentVector);
        cout << "UPDATED: All " << type << " Grades are Updated." << endl << endl;
    }//modified: add {} for if statement
    else cout << "NOTE: No Update\n\n";
};

/*************************************************************************
 * Function that deletes a specific student from the data file and return true for successful and false for failure deletion
 **************************************************************************/
bool deleteStudent(int index, vector<Student> &studentVector)// modified: add symbol & pass by reference
{
    string yORn, name;
    name = studentVector[index].name;
    cout << "\nAre you sure you want to delete student \"" << name << "\" from the Class-Roll System?[yes or no]: ";
    cin >> yORn;
    //If confirmed to delete the student, update the vector and data file
    if (case_insensitive(yORn, "yes"))
    {
        studentVector.erase(studentVector.begin() + index);        
        writeToFile(filename, studentVector);
        
        cout << "REMOVED: Student \"" << name << "\" removed from the Class-Roll System" << endl << endl;
        return false;//modified: add return false if enter "yes"
    }
    else
    {
        cout << "NOTE: No student removed\n\n";
        return true;//modified: add return true if doesn't delete
    }
};

/*************************************************************************
 * Function that adds a specific student's data to the system and database
 **************************************************************************/
void addStudent(vector<Student> &studentVector)//modified: add symbol &, pass by reference
{
    string input, yORn;
    char name[41], ID[11], email[41];

    //bool bad;
    int presGrade, essayGrade, projectGrade;

    //Accept and check student data input
    cin.ignore();
    input = stringInputCheck("new student's name [up to 40 characters]: ", 40);
    strcpy(name, input.c_str());
    input = stringInputCheck("his/her USF ID [U-12345678]: ", 10);
    strcpy(ID, input.c_str());
    input = stringInputCheck("his/her email [up to 40 characters]: ", 40);
    strcpy(email, input.c_str());
    presGrade = inputGradeCheck("presentation");
    essayGrade = inputGradeCheck("essay");
    projectGrade = inputGradeCheck("project");

    //Verify input data
    cout << "\n\n----------------- INFO CORRECT? ------------------" << endl;
    cout << left << setw(28) << "|Name: " << name << endl;
    cout << setw(28) << "|USF ID:" << ID << endl;
    cout << setw(28) << "|Email:" << email << endl;
    cout << setw(28) << "|Presentation Grade:" << (char)((presGrade == -1) ? '-' : presGrade+48) << endl;
    cout << setw(28) << "|Essay Grade:" << (char)((essayGrade == -1) ? '-' : essayGrade+48) << endl;
    cout << setw(28) << "|Project Grade:" << (char)((projectGrade == -1) ? '-' : projectGrade+48) << endl;
    cout << "---------------------------------------------------\n\n";

    //If confirmed to add the student, update the vector and data file
    cout << "Is the above new student \"" << name << "\" info correct and ready to be add to the Class-Roll System?[yes or no]: ";
    cin >> yORn;
    if (case_insensitive(yORn, "yes")) {
        Student s(name, ID, email, presGrade, essayGrade, projectGrade);
        studentVector.push_back(s);

        writeToFile(filename, studentVector);
        cout << "ADDED: student \"" << name << "\" added to the Class-Roll System\n\n";
    }//modified: add {} for a if statement
    else cout << "NOTE: No student added\n\n";
}

/*************************************************************************
 * Function that updates the presentation, essay and project grade in the data file
 **************************************************************************/
void updateInfo(string type, int index, vector<Student> &studentVector)//modified: add symbol &, pass by reference
{
    cout << endl;
    int grade = inputGradeCheck("new " + type); //Accept and check input grade
    string yORn;
    //If confirmed the change, update the vector and data file
    cout << "Are you sure you want to made the change on the " << type << " grade for student \"" << studentVector[index].name << "\"?[yes or no]: ";
    cin >> yORn;
    if (case_insensitive(yORn, "yes")) {
        if (case_insensitive(type, "presentation"))
            studentVector.at(index).presGrade = grade;
        else if (case_insensitive(type, "essay"))
            studentVector.at(index).essayGrade = grade;
        else if (case_insensitive(type, "project"))
            studentVector.at(index).projectGrade = grade;

        writeToFile(filename, studentVector);
        cout << "UPDATE: student \"" << studentVector[index].name << "\" " << type << " Grade Updated\n";
    }//modified: add {} for a if statement
    else cout << "NOTE: No update\n\n";
}

/*************************************************************************
 * Function that returns all matching name of the students in a vector during the search
 **************************************************************************/
vector<int> searchByName(vector<Student> studentVector)
{
    cout << "Please enter name of the student: ";
    string name;
    vector<int> nameIndexList;
    cin.ignore();
    getline(cin, name);
    //Find all students have the same name and store the index of the student to a vector
    for (unsigned int i = 0; i < studentVector.size(); i++)//modified: change i<= to i <
    {
        if (case_insensitive(studentVector[i].name, name))
            nameIndexList.push_back(i);
    }
    return nameIndexList;
}

/*************************************************************************
 * Function that returns that the specific student's index when their are more than one student have the same name
 **************************************************************************/
int chosenNameIndex(vector<int> studentIndexList, vector<Student> studentData)
{
    if (studentIndexList.size() != 0)  //If there are one or more matched student
    {
        //Print all the matched students to the console
        cout << endl << left << setw(10) << "|Index" << setw(40) << "|Name" << setw(18) << "|USF ID" << setw(40) << "|Email" << setw(22) << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------\n";
        for (unsigned int i = 0; i <= studentIndexList.size(); i++)
        {
            cout << " " << left << setw(10) << i + 1 << setw(40) << studentData[studentIndexList.at(i)].name << setw(18) << studentData[studentIndexList.at(i)].USF_ID <<
                setw(40) << studentData[studentIndexList.at(i)].email << endl;
        }
        //Ask user to select the correct students from all the matched student list
        string index;
        while (true)
        {
            cout << "\nEnter the index number to view student info in details: ";
            cin >> index;
            if (index.length() != 1 || stoi(index) < 1 || stoi(index) > int(studentIndexList.size()))//modified: cast studentIndexList.size to int
                cout << "ERROR: Invalid input.\n";
            else return studentIndexList.at(stoi(index) - 1);
        }
    }
    return -1;
}

/*************************************************************************
 * Function that returns matching student's index number during the search
 **************************************************************************/
int retrieveDataIndex(string searchBy, vector<Student> studentVector)
{
    string searchCondition;
    cin.ignore();
    cout << "Please enter " << searchBy << ": ";
    getline(cin, searchCondition);
    //Find the matched student based on the search condition
    for (unsigned int i = 0; i <= studentVector.size(); i++)
    {
        if (case_insensitive(searchBy, "student name")) {
            if (case_insensitive(studentVector[i].name, searchCondition))
                return i;
        }
        else if (case_insensitive(searchBy, "student USF ID")) {
            if (case_insensitive(studentVector[i].USF_ID, searchCondition))
                return i;
        }
        else if (case_insensitive(searchBy, "student email")) {
            if (case_insensitive(studentVector[i].email, searchCondition))
                return i;
        }
    }
    return -1;
}

/*************************************************************************
* Function that prints all students in the class
*************************************************************************/
void printAllStudents(vector<Student> studentData)
{
    cout << "\t\t\t\t\t\t\t\t\t**** STUDENT LIST ****\n";
    cout << left << setw(40) << "|Name" << setw(18) << "|USF ID" << setw(40) << "|Email" << setw(22) << "|Presentation Grade" << setw(22) << "|Essay Grade" << setw(22) << "|Project Grade" << endl;
    cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    //Print all students' information
    for (unsigned int i = 0; i < studentData.size(); i++)//modified: change i <= to i <
    {
        cout << " " << left << setw(40) << studentData[i].name << setw(18) << studentData[i].USF_ID << setw(40) << studentData[i].email << setw(22) <<
            (char)((studentData[i].presGrade == -1) ? '-' : studentData[i].presGrade+48) << setw(22) <<//modified: added cast to char
            (char)((studentData[i].essayGrade == -1) ? '-' : studentData[i].essayGrade+48) << setw(22) <<//modified: added cast to char
            (char)((studentData[i].projectGrade == -1) ? '-' : studentData[i].projectGrade+48) << endl;//modified: added cast to char
    }
    cout << endl;
}

/*************************************************************************
* Function that prints a specific student's information
**************************************************************************/
void printOneStudent(int index, vector<Student> studentVector)
{
    if (index == -1) //If no matched student
    {
        cout << "NO DATA: This Student Does Not Exist" << endl;
        return;
    }
    //Print student based on the index
    cout << "\n\n\n------------------ STUDENT INFO -------------------" << endl;
    cout << left << setw(28) << "|Name: " << studentVector[index].name << endl;
    cout << setw(28) << "|USF ID:" << studentVector[index].USF_ID << endl;
    cout << setw(28) << "|Email:" << studentVector[index].email << endl;
    cout << setw(28) << "|Presentation Grade:" << (char)((studentVector[index].presGrade == -1) ? '-' : studentVector[index].presGrade+48) << endl;//modified: added cast to char
    cout << setw(28) << "|Essay Grade:" << (char)((studentVector[index].essayGrade == -1) ? '-' : studentVector[index].essayGrade+48) << endl;//modified: added cast to char
    cout << setw(28) << "|Project Grade:" << (char)((studentVector[index].projectGrade == -1) ? '-' : studentVector[index].projectGrade+48) << endl;//modified: added cast to char
    cout << "---------------------------------------------------\n\n";
}

/*************************************************************************
* Function that generates a specified number of students, puts them
* into a given vector of students, and writes it to a file of a given name
**************************************************************************/
void generateStudents(int numOfStudents, vector<Student> &studentData, string filename)
{
    vector<int> UID;
    int USFID;
    char name[41], ID[11], email[31];
    int presGrade, projGrade, essayGrade;

    ofstream dataFile(filename);
    if (!dataFile.is_open()) cout << "\nCould not open " << filename << ".\n";
    dataFile << "name,UID,email,presentation,essay,project\n";

    if (dataFile.good()) {
        for(int i = 0; i < numOfStudents; i++)
        {
            bool UIDDuplicate = true;
            while(UIDDuplicate)
            {
                UIDDuplicate = false;
                USFID = rand() % 1000000 + 10000000;
                for(int j = 0; j < UID.size(); j++)
                {
                    if(USFID == UID[j])
                    {
                        UIDDuplicate = true; 
                        break;
                    }   
                }
                UID.push_back(USFID);
            }
            strcpy(name, ("User" + to_string(i)).c_str());
            strcpy(ID, ("U-"+ to_string(USFID)).c_str());
            strcpy(email, ("User" + to_string(i) + "@usf.edu").c_str());

            //Randomly put in blank grades
            if(rand() % 5 > 3) presGrade = -1;
            else presGrade = rand() % 5;
            if(rand() % 5 > 3) projGrade = -1;
            else projGrade = rand() % 5;
            if(rand() % 5 > 3) essayGrade = -1;
            else essayGrade = rand() % 5;     

            Student s(name, ID, email, presGrade, projGrade, essayGrade);
            studentData.push_back(s);
            
            dataFile << name << "," << ID << "," << email << ",";
            presGrade == -1 ? (dataFile << ",") : (dataFile << presGrade << ",");
            projGrade == -1 ? (dataFile << ",") : (dataFile << projGrade << ",");
            essayGrade == -1 ? (dataFile << "," << endl) : (dataFile << essayGrade << endl);
        }
    }
    dataFile.close();
}

bool makeStudentsFile(vector<Student> studentData, string filename)
{
    string name, ID, email;
    int presGrade, projGrade, essayGrade;

    ofstream dataFile(filename);
    if (!dataFile.is_open()){
        cout << "\nCould not open " << filename << ".\n";
        return false;
    }
    dataFile << "name,UID,email,presentation,essay,project\n";

    if (dataFile.good()) {
        for(int i = 0; i < studentData.size(); i++)
        {
            name = studentData[i].name;
            ID = studentData[i].USF_ID;
            email = studentData[i].email;
            presGrade = studentData[i].presGrade;
            projGrade = studentData[i].projectGrade;
            essayGrade = studentData[i].essayGrade;
            
            dataFile << name << "," << ID << "," << email << ",";
            presGrade == -1 ? (dataFile << ",") : (dataFile << presGrade << ",");
            projGrade == -1 ? (dataFile << ",") : (dataFile << projGrade << ",");
            essayGrade == -1 ? (dataFile << "," << endl) : (dataFile << essayGrade << endl);
        }
    }
    else return false;
    dataFile.close();
    return true;
}

/*************************************************************************
* BASIC VECTOR TEST
**************************************************************************/
TEST_CASE("vector of students can be added to, read from, and removed from", "[readDataFile]"){
    vector<Student> studentData;
    char name[41] = "Banner", ID[11] = "U-00000000", email[30] = "strongestAvenger@usf.edu";

    Student s(name, ID, email, 4, 4, 4);
    studentData.push_back(s);

    REQUIRE(studentData.size() == 1);
    CHECK(strcmp(studentData[0].name, name) == 0);
    CHECK(strcmp(studentData[0].USF_ID, ID) == 0);
    CHECK(strcmp(studentData[0].email, email) == 0);
    CHECK(studentData[0].presGrade == 4);
    CHECK(studentData[0].projectGrade == 4);
    CHECK(studentData[0].essayGrade == 4);

    studentData.erase(studentData.begin() + 0);  //This is the same logic as is found in deleteStudent

    REQUIRE(studentData.size() == 0);
}

/*************************************************************************
* Testing int case_insensitive(string s1, string s2); 
* case_insenitive checks if the strings match in lowercase.
**************************************************************************/
SCENARIO("strings are compared case insensitive", "[case_insensitive]"){
    GIVEN("A string with uppercase, an identical string with lowercase, and a similar string with lowercase"){
        string one_upper = "test!s TRING...\n";
        string one_lower = "test!s tring...\n";
        string two = "test1s tring...\n";
        WHEN("identical but different cases are compared"){
            THEN("the strings are considered identical"){
                CHECK(case_insensitive(one_upper, one_lower) == 1);
            }
        }
        WHEN("identical are compared"){
            THEN("the strings are considered identical"){
                CHECK(case_insensitive(two, two) == 1);
            }
        }
        WHEN("similar but different lowercase strings are compared"){
            THEN("the strings are different"){
                CHECK(case_insensitive(one_lower, two) == 0);
            }
        }
        WHEN("similar but different uppercase and lowercase strings are compared"){
            THEN("the strings are different"){
                CHECK(case_insensitive(one_upper, two) == 0);
            }
        }
    }
}

/*************************************************************************
 * Testing bool fileGradeCheck(string dataInfo, int &gradeUpdate);
 * Function that checks and returns if the grade in the data file are valid or not.
 * Return true for invalid data-to be skip, and false for valid data
 **************************************************************************/
SCENARIO("Valid grades are 0-4 or nothing", "[fileGradeCheck]"){
    GIVEN("Some grades"){
        int grade;
        string good_grade = "4", bad_grade = "5", bad_grade_long = "11", negative_bad_grade = "-1", nothing = "";
        WHEN("a grade is invalid"){
            THEN("the grade checker says true, it is invalid"){
                CHECK(fileGradeCheck(bad_grade, grade) == true);
                CHECK(fileGradeCheck(bad_grade_long, grade) == true);
                CHECK(fileGradeCheck(negative_bad_grade, grade) == true);
            }
        }
        WHEN("a grade is not invalid"){
            THEN("the grade checker says false, it is not invalid"){
                CHECK(fileGradeCheck("0", grade) == false);
                CHECK(fileGradeCheck("1", grade) == false);
                CHECK(fileGradeCheck("2", grade) == false);
                CHECK(fileGradeCheck("3", grade) == false);
                CHECK(fileGradeCheck(good_grade, grade) == false);
            }
            THEN("the grade is passed to the given address"){
                CHECK(grade == 4);
            }
        }
        WHEN("a grade is nothing"){
            THEN("the grade is valid"){
                CHECK(fileGradeCheck(nothing, grade) == false);
            }
            THEN("the given address contains -1"){
                CHECK(grade == -1);
            }
        }
    }
}

/*************************************************************************
 * Testing vector<Student> readDataFile(string filename, bool &opened);
 * Function that read from the file that contains all students' data information and it returns a vector of Student struct
 **************************************************************************/
SCENARIO("A test file is read", "[readDataFile]"){
    GIVEN("A test file"){
        string fileName = "student_test.csv";
        vector<Student> studentData;
        vector<Student> functionData;
        WHEN("has four students that shouldn't be skipped"){
            bool opened;
            string fileBasic = "student_basic.csv";
            THEN("the returned student vector is empty"){
                CHECK(readDataFile(fileBasic, opened).size() == 4);
            }
            THEN("the file was opened"){
                CHECK(opened == true);
            }
        }
        WHEN("has only the header"){
            bool opened;
            REQUIRE(makeStudentsFile(studentData, fileName));
            THEN("the returned student vector is empty"){
                CHECK(readDataFile(fileName, opened).size() == 0);
            }
            THEN("the file was opened"){
                CHECK(opened == true);
            }
        }
        WHEN("has one valid student"){
            bool opened;
            char name[41] = "Banner", ID[11] = "U-00000000", email[30] = "strongestAvenger@usf.edu";
            Student s(name, ID, email, 4, 4, 4);
            studentData.push_back(s);
            REQUIRE(makeStudentsFile(studentData, fileName));
            functionData = readDataFile(fileName, opened);
            THEN("the file was opened"){
                REQUIRE(opened == true);
            }
            THEN("the returned student vector has as many students as given"){
                REQUIRE(functionData.size() == studentData.size());
            }
            THEN("the students in the vector match what was in the file"){
                CHECK(strcmp(functionData[0].name, studentData[0].name) == 0);
                CHECK(strcmp(functionData[0].USF_ID, studentData[0].USF_ID) == 0);
                CHECK(strcmp(functionData[0].email, studentData[0].email) == 0);
                CHECK(functionData[0].presGrade == studentData[0].presGrade);
                CHECK(functionData[0].projectGrade == studentData[0].projectGrade);
                CHECK(functionData[0].essayGrade == studentData[0].essayGrade);
            }
        }
        WHEN("has one valid student and a student with a name that is one character too long"){
            bool opened;
            string fileLongName = "student_long_name_test.csv";
            functionData = readDataFile(fileLongName, opened);
            THEN("the file was opened"){
                REQUIRE(opened == true);
            }
            THEN("the returned student vector has only 1 student"){
                REQUIRE(functionData.size() == 1);
            }
        } 
        WHEN("doesn't exist"){
            bool opened;
            fileName = "1.csv";
            THEN("the returned student vector is empty"){
                CHECK(readDataFile(fileName, opened).size() == 0);
            }
            THEN("the file was not opened"){
                CHECK(opened == false);
            }
        }
    }
}

/**************************************************************************
 * Testing void writeToFile(string filename, vector<Student> studentData);
 * Function that writes the update data to the file
 ***************************************************************************/
// Give it some data that you've written to a file. Open the file it writes to and compare each line with the file you made.
SCENARIO("A vector of students is wrritten into a file", "[writeToFile]"){
    GIVEN("A test file"){
        string fileName = "one_student.csv";
        vector<Student> studentData;
        vector<Student> functionData;
        char name[41] = "Banner", ID[11] = "U-00000000", email[30] = "strongestAvenger@usf.edu";
        studentData.push_back(Student(name, ID, email, 3, 4, 3));
        WHEN("one_student.csv"){
            bool opened;
            writeToFile(fileName, studentData);
            studentData.clear();
            THEN("check if it prints out everything"){
                functionData = readDataFile(fileName, opened);
                CHECK(readDataFile(fileName, opened).size() == 1);
                CHECK(strcmp(studentData[0].name, name) == 0);
                CHECK(strcmp(studentData[0].USF_ID, ID) == 0);
                CHECK(strcmp(studentData[0].email, email) == 0);
                CHECK(functionData[0].presGrade == 3);
                CHECK(functionData[0].essayGrade == 4);
                CHECK(functionData[0].projectGrade == 3);
            }
        }
    }
}
/**************************************************************************
 * Testing void printOneStudent(int index, vector<Student> studentVector)
 * Function that prints a specific students information
 ***************************************************************************/
SCENARIO("A specific student's information is printed out", "[printOneStudent]"){
  GIVEN("A vector of students"){
      vector<Student> studentData;
      char name[41] = "Banner", ID[11] = "U-00000000", email[30] = "strongestAvenger@usf.edu";
      studentData.push_back(Student(name, ID, email, 3, 4, 3));
      char name1[41] = "Jack", ID1[11] = "U-11111111", email1[30] = "jack@usf.edu";
      studentData.push_back(Student(name1, ID1, email1, 3, 4, 3));
      // Check that it doesn't fail when given -1 (which means that the student doesn't exist)
      WHEN("student doesn't exist"){
          printOneStudent(-1, studentData);
      }
  
// - - - doesn't fail on a good index
      WHEN("student does exist"){
          printOneStudent(1, studentData);
      }
// - - - prints properly (probably have to cout what it should look like and let user compare)
      WHEN("student does exist and prints student info"){
          cout << "\n\n\n------------------ COMPARE THE TWO TABLES BELOW, SHOULD BE THE SAME -------------------\n" << endl;
          printOneStudent(1, studentData);
              //Print student based on the index
          cout << "\n------------------ STUDENT INFO -------------------" << endl;
          cout << left << setw(28) << "|Name: " << studentData[1].name << endl;
          cout << setw(28) << "|USF ID:" << studentData[1].USF_ID << endl;
          cout << setw(28) << "|Email:" << studentData[1].email << endl;
          cout << setw(28) << "|Presentation Grade:" << (char)((studentData[1].presGrade == -1) ? '-' : studentData[1].presGrade+48) << endl;//modified: added cast to char
          cout << setw(28) << "|Essay Grade:" << (char)((studentData[1].essayGrade == -1) ? '-' : studentData[1].essayGrade+48) << endl;//modified: added cast to char
          cout << setw(28) << "|Project Grade:" << (char)((studentData[1].projectGrade == -1) ? '-' : studentData[1].projectGrade+48) << endl;//modified: added cast to char
          cout << "---------------------------------------------------\n\n";
          cout << "---------------------------------------------------------------------------------------------\n\n";
      }
  }
}

/**************************************************************************
 * Testing void printAllStudents(vector<Student> studentData)
 * Function that prints all the students in the class
 ***************************************************************************/
 SCENARIO("All students' information is printed out", "[printAllStudents]"){
   GIVEN("A vector of students"){
       vector<Student> studentData;

      //add students to vector 
      char name[41] = "Banner", ID[11] = "U-00000000", email[30] = "strongestAvenger@usf.edu";
      studentData.push_back(Student(name, ID, email, 3, 4, 3));
      char name1[41] = "Jack", ID1[11] = "U-11111111", email1[30] = "jack@usf.edu";
      studentData.push_back(Student(name1, ID1, email1, 4, 4, 3));
// - - - - - - - a good vector of students
      WHEN("vector of students is not empty"){
          printAllStudents(studentData);
      }
// - - - prints properly (probably have to cout what it should look like and let user compare)
      WHEN("vector of students is not empty and print students' info"){
          cout << "\n\n\n------------------ COMPARE THE TWO TABLES BELOW, SHOULD BE THE SAME -------------------\n" << endl;
          printAllStudents(studentData);
          cout << "\t\t\t\t\t\t\t\t\t**** STUDENT LIST ****\n";
          cout << left << setw(40) << "|Name" << setw(18) << "|USF ID" << setw(40) << "|Email" << setw(22) << "|Presentation Grade" << setw(22) << "|Essay Grade" << setw(22) << "|Project Grade" << endl;
          cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
          //Print all students' information
          for (unsigned int i = 0; i < studentData.size(); i++)
          {
              cout << " " << left << setw(40) << studentData[i].name << setw(18) << studentData[i].USF_ID << setw(40) << studentData[i].email << setw(22) <<
                      (char)((studentData[i].presGrade == -1) ? '-' : studentData[i].presGrade+48) << setw(22) <<//modified: added cast to char
                      (char)((studentData[i].essayGrade == -1) ? '-' : studentData[i].essayGrade+48) << setw(22) <<//modified: added cast to char
                      (char)((studentData[i].projectGrade == -1) ? '-' : studentData[i].projectGrade+48) << endl;//modified: added cast to char
          }
          cout << endl;
          cout << "---------------------------------------------------------------------------------------------\n\n"; 
      }
  }
}
