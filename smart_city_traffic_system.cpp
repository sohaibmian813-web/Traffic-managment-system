#include<iostream>
#include<ctime>
#include<iomanip> 
#include<fstream>

using namespace std;

// constants

const int maxv = 100, maxvio = 200, maxs = 20,licensep = 12,redtime = 5,
          yellowtime = 2, greentime = 5,totaltime = redtime + yellowtime + greentime;

// structures

struct Vehicle {
    string ownerName;
    int vehicleNumber,licenseNumber,vehicleType,licensePoints,licenseValid,blacklisted;
    string vehicleTypeName; 
};

struct Violation {
    int challanID,vehicleNumber,violationType,fine,paid;
    string violationTypeName; 
    int year, month, day, hour, min, sec;
    string ampm;
};

struct TrafficSignal {
    int signalID;
    string location;
    int status,isManual;
};

//arrays of structures

Vehicle vehicles[maxv];
Violation violations[maxvio];
TrafficSignal signals[maxs];

// counters

int vehicle_count = 0;
int violation_count = 0;
int signal_count = 0;
int nextChallanID = 1;


string adminPassword ="sohaib";
string username = "sohaib123";
string trafficCondition = "Normal"; 
int violationFines[10] = {0,2500,5000,3000,2000,1500,2000,5000,4000,1000};
string vehicle_types[5] = {"", "Car", "Bike", "Bus", "Truck"};
string violation_types[10] = {"", "OverSpeed","SignalJump","WrongLane","NoHelmet","NoSeatBelt","MobileUse","NoLicense","Overload","Parking"};

// functions

int vehicle_types_names(string name){
    for(int i = 1; i <= 4; i++){
        if(vehicle_types[i] == name)
            return i;
    }
    return 0; // default if not found
}

int violation_types_names(string name){
    for(int i = 1; i <= 9; i++){
        if(violation_types[i] == name)
            return i;
    }
    return 0; // default if not found
}

int inputNumber(int min = 0, int max = 100000) {
    int x;
    while (true) {
        cin >> x;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout <<setw(61)<< "\033[31m Invalid input. Enter a number: \033[0m";
            continue;
        }
        // check if only integers are entered
        if (cin.peek() != '\n') {
            cin.ignore(1000, '\n');
            cout<<setw(60) << "\033[31m Invalid input.\033[0m  ";
            continue;
        }
        if (x < min || x > max) {
            cout<<setw(50) << "\033[31m Enter number between " << min << " and " << max << ":\033[0m ";
            continue;
        }
        cin.ignore(1000, '\n'); // clear newline
        return x;
    }
}

void pause() {
    cout <<"\n"<<setw(60)<< "\033[33m Press ENTER to continue.....\033[0m";
    cin.get();                // wait for Enter key
}

bool valid_vehicle_number(int vn) {
    int count = 0;
    while(vn != 0){
        vn /= 10;
        count++;
    }
    if (count != 3){
        return false;
    }
    else
    return true;
}
bool valid_license_number(int ln) {
    int count = 0;
    while(ln != 0){
        ln /= 10;
        count++;
    }
    if (count != 7){
        return false;
    }
    else 
    return true ;
}

void getCurrentDateTime(int &y, int &m, int &d,int &h, int &mi, int &s,string &ampm)
{
    time_t now = time(0);

    s  = now % 60;
    mi = (now / 60) % 60;
    int hour24 = (now / 3600) % 24;
    int days   = now / 86400;

    // Pakistan Standard Time (UTC +5)
    hour24 += 5;
    if(hour24 >= 24){
        hour24 -= 24;
        days++;
    }

    // Convert to 12-hour format
    if(hour24 == 0){
        h = 12;
        ampm = "AM";
    }
    else if(hour24 < 12){
        h = hour24;
        ampm = "AM";
    }
    else if(hour24 == 12){
        h = 12;
        ampm = "PM";
    }
    else{
        h = hour24 - 12;
        ampm = "PM";
    }

    // Year calculation
    y = 1970;
    while (days >= 365) {
        if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) {
            if (days >= 366) {
                days -= 366;
                y++;
            } else break;
        } else {
            days -= 365;
            y++;
        }
    }

    int monthDays[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)
        monthDays[1] = 29;

    m = 1;
    while (days >= monthDays[m - 1]) {
        days -= monthDays[m - 1];
        m++;
    }

    d = days + 1;
}

// admin functions

bool adminLogin(){
    string pass,user;
    cout<<setw(50)<<"Enter user name:";
    cin>>user;
    cout<<setw(57)<<"Enter admin password : ";
    cin>>pass;
    
    if (pass == adminPassword && user == username)
    return true;
    else 
    return false;
}

void changeAdminPassword(){
    cout<<"\n\n"<<setw(80)<<"\033[1m \033[34m--- CHANGE ADMIN USERNAME & PASSWORD ---\033[0m\n\n";
    string oldPass,newPass;
    string olduser,newuser;
    cout<<"\n"<<setw(50)<<"Enter old user name: ";
    cin>> olduser;
    cout<<setw(50)<<"Enter old password : ";
    cin>>oldPass;
    if(oldPass == adminPassword && olduser == username){
        cout<<"\n"<<setw(50)<<"Enter new user name: ";
        cin>>newuser;
        cout<<setw(50)<<"Enter new password : ";
        cin>>newPass;
    username = newuser;
    adminPassword = newPass;
    cout<<"\n"<<setw(55)<<" Password changed successfully."<<endl;
}
else 
    {
        cout<<setw(50)<<" Incorrect old password."<<endl;
    }
    
}

// violation functions

int getViolationFine(int type) {
    return violationFines[type];
}

void updateViolationFine() {
    cout<<endl;
    cout <<setw(75)<< "\033[1m \033[34m--- UPDATE VIOLATION FINES ---\033[0m"<<endl;
    cout<<endl;
    cout <<setw(52)<< "Violation Types:"<<endl;
    cout <<setw(50)<< "1. OverSpeed"<<endl;
    cout <<setw(52)<<"2. Signal Jump"<<endl;
    cout<<setw(51)<<"3. Wrong Lane"<<endl;
    cout<<setw(50)<<"4. No Helmet"<<endl;
    cout <<setw(53)<< "5. No Seat Belt"<<endl;
    cout<<setw(51)<<"6. Mobile Use"<<endl;
    cout<<setw(51)<<"7. No License"<<endl;
    cout<<setw(49)<<"8. Overload"<<endl;
    cout<<setw(48)<<"9. Parking\n"<<endl;
    cout<<setw(52)<<"Enter violation type:";
    int type = inputNumber(1, 9);
    cout <<"\n"<<setw(56)<< "Current Fine: Rs." << violationFines[type] <<endl;
    cout <<setw(62)<<"Enter New Fine Amount: ";
    int newFine = inputNumber(0, 100000);
    violationFines[type] = newFine;
    cout <<setw(65)<< "\033[32m Fine updated successfully!\033[0m\n";
}

int getViolationPoints(int type) {
    int points[10] = {0,-3,-4,-2,-2,-2,-2,-6,-4,-1};
    return points[type];
}

void updateVehicleStatus(int idx) {

    int unpaidCount = 0;

    // Count unpaid challans (FOR ALL VEHICLES)
    for(int i = 0; i < violation_count; i++) {
        if(violations[i].vehicleNumber == vehicles[idx].vehicleNumber) {
            if(violations[i].paid == 0)
                unpaidCount++;
        }
    }

    // Blacklisting
    vehicles[idx].blacklisted = (unpaidCount >= 3) ? 1 : 0;

    // update license 
    if(vehicles[idx].licenseNumber == 0) {
        vehicles[idx].licensePoints = 0;
        vehicles[idx].licenseValid  = 0;
        return;   
    }

    int totalPoints = licensep;

    for(int i = 0; i < violation_count; i++) {
        if(violations[i].vehicleNumber == vehicles[idx].vehicleNumber) {
            totalPoints += getViolationPoints(violations[i].violationType);
            if(totalPoints < 0)
                totalPoints = 0;
        }
    }

    vehicles[idx].licensePoints = totalPoints;
    vehicles[idx].licenseValid  = (totalPoints > 0) ? 1 : 0;
}

void printChallanReceipt(int vehicleIdx, int challanID) {
    int vIdx = vehicleIdx;
    int vioIdx = -1;

    for(int i = 0; i < violation_count; i++){
        if(violations[i].challanID == challanID){
            vioIdx = i;
            break;
        }
    }

    if(vioIdx == -1){
        cout << "\033[31mChallan not found.\n\033[0m";
        return;
    }

    cout << endl;
    cout<< setw(90) <<"\033[34m======================================================\033[0m"<<endl;
    cout << setw(90) << "\033[34m                   CHALLAN RECEIPT                  \033[0m" << endl;
    cout<< setw(90) <<"\033[34m======================================================\033[0m" <<endl;
    

    cout << setw(50) << "Challan ID: " << violations[vioIdx].challanID << endl;
    cout << setw(56) << "Vehicle number: ST" << vehicles[vIdx].vehicleNumber << endl;
    cout << setw(52) << "Vehicle type: " << vehicles[vIdx].vehicleTypeName << endl;
    cout << setw(50) << "Owner name: " << vehicles[vIdx].ownerName << endl;

    if(vehicles[vIdx].licenseNumber == 0){
        cout << setw(58) << "License: Not Present" << endl;
        cout << setw(70) << "License Points: N/A (No License)" << endl;
    } else {
        cout << setw(47) << "License: " << vehicles[vIdx].licenseNumber << endl;
        cout << setw(54) << "License Points: " << vehicles[vIdx].licensePoints << endl;
    }

    cout << setw(54) << "Violation type: " << violations[vioIdx].violationTypeName << endl;
    cout << setw(47) << "Fine: Rs." << violations[vioIdx].fine << endl;
    cout << setw(46) << "Status: " 
         << (violations[vioIdx].paid ? "Paid" : "Unpaid") << endl;

    cout << setw(44) << "Date: "
         << setw(2) << violations[vioIdx].day << "-"
         << setw(2) << violations[vioIdx].month << "-"
         << violations[vioIdx].year << endl;

    cout << setw(44) << "Time: "
         << setw(2) << violations[vioIdx].hour << ":"
         << setw(2) << violations[vioIdx].min << ":"
         << setw(2) << violations[vioIdx].sec << " "
         << violations[vioIdx].ampm << endl;
}

int findVehicleByNumber(int vn) {
    for(int i = 0; i < vehicle_count; i++){
        if(vehicles[i].vehicleNumber == vn)
            return i;  // Vehicle found
    }
    return -1;  // Not found
}

void issueChallan() {
  int vNum; 

do {
    cout<<"\n\n"<<setw(90)<<"\033[1m \033[34m==================Issue challan==================\033[0m\n\n";
    cout<<setw(70) << "Enter 3-digit Vehicle Number (ST___): ST"; 
    vNum = inputNumber(1, 999);
    if(!valid_vehicle_number(vNum))
        cout << "\033[31m Invalid Vehicle Number! Must be exactly 3 digits.\033[0m\n";
} while(!valid_vehicle_number(vNum));

    int idx = findVehicleByNumber(vNum);
// new vehicle
    Vehicle v;
    if(idx == -1){
        if(vehicle_count >= maxv) {
    cout<<setw(60)<<"\033[31mVehicles limit reached\033[0m";
    return;
}

        cout <<setw(42)<< "Owner Name: ";
        getline(cin , v.ownerName);

        v.vehicleNumber = vNum; 

        cout <<setw(70)<< "Does driver have license? (1=Yes 0=No): ";
        int hasLicense = inputNumber(0,1);

       if(hasLicense == 0){
    v.licenseNumber = 0;   
    v.licenseValid  = 0;   
    v.licensePoints = 0;   
}
     else {

            int license;
            do {
                cout <<setw(63)<< "Enter License Number (7 digits): ";
                license = inputNumber(1000000, 9999999); // Ensure 7-digit number
                if(!valid_license_number(license))
                    cout << "\033[31m Invalid License Number! Please try again.\033[0m\n";
                else {
                    v.licenseNumber = license;
                    v.licenseValid  = 1;
                    break;
                }
            } while(true);
        }

        cout <<setw(73)<< "Vehicle Type (1=Car 2=Bike 3=Bus 4=Truck): ";
        v.vehicleType = inputNumber(1,4);
        v.vehicleTypeName = vehicle_types[v.vehicleType];

       if(v.licenseNumber == 0){
    v.licensePoints = 0;
       }      
else{
    v.licensePoints = licensep;
}
    v.blacklisted = 0;

        vehicles[vehicle_count++] = v;
        idx = vehicle_count - 1;
    }
    else{
        v = vehicles[idx];
    }

    // violation
    Violation vio;
    vio.vehicleNumber = v.vehicleNumber; 

    cout <<setw(46)<< "Violation Type :"<<endl;
    cout <<setw(77)<<"1-OverSpeed 2-SignalJump 3-WrongLane 4-NoHelmet"<<endl;
    cout <<setw(82)<<" 5-NoSeat 6-Mobile 7-NoLicense 8-Overload 9-Parking: ";
    vio.violationType = inputNumber(1,9);
    vio.violationTypeName = violation_types[vio.violationType];
    vio.fine = getViolationFine(vio.violationType);

    string ampm;
getCurrentDateTime(vio.year, vio.month, vio.day,vio.hour, vio.min, vio.sec, vio.ampm);

    if(vio.hour>=22 || vio.hour<6)
        vio.fine *= 2;   // for night double fine

    // license check
    int noLicense = (v.licenseNumber == 0);

    if(noLicense && vio.violationType == 7){
        vio.fine = 5000;
        v.licensePoints = 0;
        v.licenseValid  = 0;
        vehicles[idx]   = v;
    }

    vio.challanID = nextChallanID++;

    vio.paid = 0;
    if(violation_count >= maxvio){
    cout << "\nViolation limit reached. Cannot issue more challans.\n";
    return;
}
    violations[violation_count++] = vio;

    updateVehicleStatus(idx);
    printChallanReceipt(idx,vio.challanID);  

}
void viewViolations() {
    if (violation_count == 0) {
        cout << setw(70) << "\033[31mNo violations recorded.\033[0m" << endl;
        return;
    }

    cout << endl;
    cout << setw(60) << "\033[1m \033[34m---*********** ALL VIOLATIONS REPORT ***********---\033[0m" << endl;
    cout << endl;

    cout << left 
         << setw(8)  << "ID"
         << setw(12) << "Vehicle"
         << setw(12) << "Type"
         << setw(15) << "Owner"
         << setw(15) << "Violation"
         << setw(12) << "Fine"
         << setw(10) << "Status"
         << "Date" 
         << endl;

    cout << "-----------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < violation_count; i++) {
        int idx = findVehicleByNumber(violations[i].vehicleNumber);
        
        cout << left 
             << setw(8)  << violations[i].challanID;

        cout << "ST" << setw(10) << violations[i].vehicleNumber; 

        cout << setw(12) << vehicles[idx].vehicleTypeName
             << setw(15) << (idx != -1 ? vehicles[idx].ownerName : "Unknown")
             << setw(15) << violations[i].violationTypeName
             << "Rs." << setw(9) << violations[i].fine
             << setw(10) << (violations[i].paid ? "Paid" : "Unpaid")
             << violations[i].year << "-" << violations[i].month << "-" << violations[i].day 
             << endl;
    }
    cout<<right;
    cout << endl;
}

 // payment function
void payFine() {
    int vn;
    cout<<setw(70) << "\033[1m \033[34m---*** PAY FINE ***---\033[0m\n";
    cout<<setw(85)<<"\033[1m \033[34m==========================================\033[0m\n\n";
    cout<<setw(62)<< "Enter 3 digits vehicle number: ST";
    vn = inputNumber(0, 999);  

    int idx = findVehicleByNumber(vn);
    if(idx == -1){
        cout<<setw(60) << "\033[31mNo record found.\n\033[0m";
        return;
    }

    bool hasUnpaid = false;
    cout<<endl;
    cout<<setw(80) << "\033[1m \033[34m---------*** Unpaid Challans ***---------\033[0m\n";
    cout<<endl;
    for(int i = 0; i < violation_count; i++){
        if(violations[i].vehicleNumber == vn && violations[i].paid == 0){
            hasUnpaid = true;
            cout <<setw(45)<< "Challan ID: " << violations[i].challanID
                 << " | Fine: Rs." << violations[i].fine << endl;
        }
    }

    if(!hasUnpaid){
        cout<<setw(60) << "\033[31mNo unpaid challans.\n\033[0m";
        return;
    }

    cout<<"\n"<<setw(58) <<"Enter Challan ID to pay: ";
    int cid = inputNumber(1, 100000);

    for(int i = 0; i < violation_count; i++){
        if(violations[i].vehicleNumber == vn && violations[i].challanID == cid && violations[i].paid == 0){

            cout<<setw(63) << "\033[1m Select Payment Method:\033[0m"<<endl;
            cout<<setw(51) << "1. Card"<<endl;
            cout<<setw(60)<<"2. Mobile Wallet"<<endl;
            cout<<setw(53)<<"Choice: ";
            int method = inputNumber(1, 2);

            bool paymentSuccess = false;

            if(method == 1){
                cout<<setw(60)<<"Enter 7 digits card number:";
                 inputNumber(1000000, 9999999);
                cout<<setw(60)<<"Enter 3 digits pin:";
                 inputNumber(100, 999);
                cout <<setw(64)<< "\033[32m Card Payment Successful!\033[0m\n";
                paymentSuccess = true;
            }
            else if(method == 2){
                cout<<setw(62)<<"Enter 8 digits wallet number:";
                 inputNumber(10000000, 99999999);
                cout<<setw(60)<<"Enter 4 digits pin:";
                 inputNumber(1000, 9999);
                cout<<setw(62) << "\033[32m Wallet Payment Successful!\033[0m\n";
                paymentSuccess = true;
            }

            if(paymentSuccess){
                cout<<setw(54) << "Fine: Rs." << violations[i].fine <<endl;
                 cout<<setw(65) << "\033[32m Payment Completed Successfully.\033[0m\n";
                violations[i].paid = 1;
                printChallanReceipt(idx,violations[i].challanID);
                updateVehicleStatus(idx);
                
                
            }

            return;
        }
    }

    cout<<setw(64) << "\033[31mChallan not found or already paid.\n\033[0m";
}

// traffic signal functions
void addSignal() {
    if(signal_count >= maxs){
        cout << "Cannot add more signals\n";
        return;
    }

    TrafficSignal s;
    s.signalID = signal_count + 1;
    cout<<endl;
    cout<<setw(65)<<"\033[1m \033[34m========Add signals========\033[0m"<<endl;
    cout<<endl;
    cout<<setw(45)<< "Signal Location: ";
    getline(cin , s.location);

    s.status = 1;      // Red
    s.isManual = 0;    // auto mode

    signals[signal_count++] = s;
    cout<<endl;
    cout<<setw(45) << "\033[32m Signal Added\033[0m"<<endl;
}

void setAllSignals(int color){
    for(int i = 0; i < signal_count; i++){
        signals[i].status = color;
        signals[i].isManual = 1; //  manual for ALL
    }
    cout<<endl;
    cout<< setw(60)<< "\033[32m All signals updated\033[0m\n";
}

void resumeCycle() {
    cout<<"\n\n"<<setw(70)<< "\033[1m \033[34m--- Resume Automatic Signal Cycle ---\033[0m"<<endl;
    for(int i = 0; i < signal_count; i++){
        signals[i].isManual = 0; // Back to auto
    }
    cout<<endl;
    cout <<setw(70)<< "\033[32m All signals resumed automatic cycle.\033[0m"<<endl;
}

void viewSignals() {
    if(signal_count == 0){
        cout << setw(70) << "No signals." << endl;
        return;
    }

    time_t now = time(0);
    int t = now % totaltime;

    cout << endl;
    cout << setw(70) << "\033[1m \033[34m--- VIEW ALL SIGNALS ---\033[0m" << endl;
    cout << endl;

    cout << left 
         << setw(10) << "ID"
         << setw(30) << "Location"
         << setw(15) << "Status"
         << setw(10) << "Mode"
         << endl;
    
    cout << "-----------------------------------------------------------------" << endl;

    for(int i = 0; i < signal_count; i++){
        if(signals[i].isManual == 0) { 
            if(t < redtime)
                signals[i].status = 1;
            else if(t < redtime + yellowtime)
                signals[i].status = 2;
            else
                signals[i].status = 3;
        }
        
        cout << left 
             << setw(10) << signals[i].signalID
             << setw(30) << signals[i].location;
        
        if(signals[i].status == 1) cout << "\033[31m" << setw(15) << "Red" << "\033[0m";
        else if(signals[i].status == 2) cout << "\033[33m" << setw(15) << "Yellow" << "\033[0m";
        else cout << "\033[32m" << setw(15) << "Green" << "\033[0m";

        cout << setw(10) << (signals[i].isManual ? "Manual" : "Auto") << endl;
    }
    cout << endl;
    cout << right;
}
void viewSignalsPublic() {
    if(signal_count == 0){
        cout << setw(52) << "\033[31mNo signals.\n\033[0m";
        return;
    }

    time_t now = time(0);
    int t = now % totaltime;

    cout << endl;
    cout << setw(70) << "\033[1m \033[34m---------***  SIGNALS DETAILS ***-------\033[0m \n\n";

    cout << left 
         << setw(30) << "Location"
         << setw(15) << "Status"
         << endl;
    
    cout << "---------------------------------------------------------------------------" << endl;

    for(int i = 0; i < signal_count; i++){
        if(signals[i].isManual == 0) {
            if(t < redtime)
                signals[i].status = 1;
            else if(t < redtime + yellowtime)
                signals[i].status = 2;
            else
                signals[i].status = 3;
        }
        
        cout << left << setw(30) << signals[i].location;

        if(signals[i].status == 1) cout << "\033[31m" << setw(15) << "Red" << "\033[0m" << endl;
        else if(signals[i].status == 2) cout << "\033[33m" << setw(15) << "Yellow" << "\033[0m" << endl;
        else cout << "\033[32m" << setw(15) << "Green" << "\033[0m" << endl;
    }
    cout << endl;
    cout << right;
}
void changeSignalIndividually() {
    if(signal_count == 0){
        cout<<endl;
        cout <<setw(50)<< "\033[31mNo signals to change\n\033[0m";
        return;
    }
    cout<<endl;
    cout <<setw(70)<< "\033[1m \033[34m--- Signals List ---\033[0m\n";
    viewSignals();

    cout <<setw(60)<< "Enter Signal ID to change: ";
    int id = inputNumber(1, 1000);

    for(int i = 0; i < signal_count; i++){
        if(signals[i].signalID == id){
            cout<<setw(70) << "Set Signal (1=Red 2=Yellow 3=Green): ";
            int s = inputNumber(1,3);

            signals[i].status = s;
            signals[i].isManual = 1; // set signal status

            cout <<setw(57)<< "\033[32m Signal Updated \033[0m\n";
            return;
        }
    }

    cout<<setw(60) << "\033[31mSignal not found\n\033[0m";
}

void deleteSignal(){
    if(signal_count==0){
         cout<<"\n" <<setw(50)<< "\033[31mNo signals to delete\n\033[0m";
          return;
         }
    cout<<endl;
    cout<<setw(70) << "\033[1m \033[34m---*** Signals List ***---\033[0m"<<endl;
     viewSignals();
    int id;
     cout <<setw(61)<< "Enter Signal ID to delete: ";
      cin >> id;
    for(int i=0;i<signal_count;i++){
        if(signals[i].signalID==id){
            for(int j=i;j<signal_count-1;j++)
             signals[j]=signals[j+1];
            signal_count--;
             cout <<setw(55)<< "\033[32m Signal Deleted\033[0m"<<endl;
              return;
        }
    }
    cout <<setw(60)<< "\033[31mSignal not found\033[0m"<<endl;
}

// admin traffic condition function

void updateTrafficCondition() {
    cout <<setw(60)<< "Enter Traffic Condition:"<<endl;
    cout<<setw(57)<<"1. No Traffic"<<endl;
    cout<<setw(53)<<"2. Normal"<<endl;
    cout<<setw(60)<<"3. Slightly Rush"<<endl;
    cout<<setw(51)<<"4. Rush"<<endl;
    cout<<setw(51)<<"Choice: ";
    int ch = inputNumber(1,4);
    if(ch==1)
     trafficCondition="No Traffic";
    else if(ch==2)
     trafficCondition="Normal";
    else if(ch==3)
     trafficCondition="Slightly Rush";
    else if(ch==4)
     trafficCondition="Rush";
    cout <<setw(65)<< "\033[32m Traffic condition updated to:\033[0m " << trafficCondition <<endl;
}

// violation report  function
void viewReports() {
    int revenue = 0, unpaid = 0;
    for(int i=0;i<violation_count;i++){
        if(violations[i].paid)
         revenue += violations[i].fine;
        else
         unpaid++;
    }
    cout<<endl;
    cout <<setw(60)<< "\033[1m \033[34m-------*** REPORT ***-------\033[0m\n";
    cout<<endl;
    cout <<setw(46)<< "Total Vehicles: " << vehicle_count << "\n";
    cout <<setw(48)<< "Total Violations: " << violation_count << "\n";
    cout <<setw(58)<< "Total Revenue Collected: Rs." << revenue << "\n";
    cout <<setw(50)<< "Total Unpaid Fines: " << unpaid << "\n";
}
// public functions
void checkChallanByVehicle() {
    int vn; 
    cout<<setw(80) << "\033[1m \033[34m---***** CHECK CHALLAN BY VEHICLE NUMBER *****---\033[0m\n";
    cout<<setw(85)<<"\033[1m \033[34m====================================================\033[0m\n\n";
    cout<<setw(60) << "Enter Vehicle Number (STxxx): ST";
    vn = inputNumber(1, 999); 

    int idx = findVehicleByNumber(vn);
    if(idx == -1){
        cout <<setw(60)<< "\033[31mNo record found\n\033[0m";
        return;
    }
    cout<<endl;
    cout<<setw(70) << "\033[1m \033[34m---*** Challans for Vehicle ***---\033[0m"<<endl;
    cout<<endl;
    for(int i = 0; i < violation_count; i++){
        if(violations[i].vehicleNumber == vn){
            cout<<setw(40)<< "Challan ID: " << violations[i].challanID<< " | Fine: Rs." << violations[i].fine
            << " | Status: " << (violations[i].paid ? "Paid" : "Unpaid") <<" | Date: " 
            << violations[i].year << "-" << violations[i].month << "-" << violations[i].day<<endl;
        }
    }

    if(vehicles[idx].licenseNumber == 0)
        cout<<setw(100) << "License: Not Present | Points: N/A | License Status: N/A | Blacklisted: "
             << (vehicles[idx].blacklisted ? "Yes" : "No") <<endl;
    else
        cout <<setw(37)<< "License: " << vehicles[idx].licenseNumber
             << " | Points: " << vehicles[idx].licensePoints
             << " | License Status: " << (vehicles[idx].licenseValid ? "Valid" : "Cancelled")
             << " | Blacklisted: " << (vehicles[idx].blacklisted ? "Yes" : "No") <<endl;
}

void checkChallanByID() {
    int id;
    cout<<setw(80) << "\033[1m \033[34m--- CHECK CHALLAN BY CHALLAN ID---\033[0m\n";
    cout<<setw(85)<<"\033[1m \033[34m==========================================\033[0m\n\n";
    cout<<setw(60) << "Enter Challan ID: ";
    id = inputNumber(1, 100000);  

    for(int i = 0; i < violation_count; i++){
        if(violations[i].challanID == id){
            int idx = findVehicleByNumber(violations[i].vehicleNumber);
            if(idx == -1){
                cout<<setw(60) << "Vehicle not found for this challan.\n";
                return;
            }
            cout<<endl;
            cout <<setw(75)<< "\033[1m \033[34m---*** Challan Details ***---\033[0m"<<endl;
            cout<<endl;
            cout <<setw(55)<< "Challan ID: " << violations[i].challanID << endl;
            cout <<setw(54)<< "Vehicle: ST" << violations[i].vehicleNumber << endl;
            cout <<setw(50)<< "Owner: " << vehicles[idx].ownerName << endl;
            
            if(vehicles[idx].licenseNumber == 0){
                cout<<setw(63) << "License: Not Present"<<endl;
                cout<<setw(75)<<"License Points: N/A (No License)"<<endl;
            }
            else {
                cout<<setw(52) << "License: " << vehicles[idx].licenseNumber << endl;
                cout<<setw(51)<< "Points: " << vehicles[idx].licensePoints << endl;
            }

            cout<<setw(52) << "Fine: Rs." << violations[i].fine << endl;
            cout<<setw(51) << "Status: " << (violations[i].paid ? "Paid" : "Unpaid") << endl;
            cout<<setw(49)<< "Date: " << violations[i].year << "-" << violations[i].month << "-" << violations[i].day << endl;
            return;
        }
    }

    cout<<setw(62) << "\033[31mChallan not found.\n\033[0m";
}

void trafficRules() {
    cout<<endl;
    cout<<setw(40) << "\033[1m \033[34m============== TRAFFIC RULES ===============\033[0m"<<endl;
    cout<<endl;
    cout << "1. Helmet is mandatory for all bike riders and pillion riders."<<endl;
    cout << "2. Seat belts are mandatory for all car drivers and passengers."<<endl;
    cout << "3. Do not jump a red traffic signal."<<endl;
    cout << "4. Do not drive in the wrong lane."<<endl;
    cout << "5. Using a mobile phone while driving is illegal."<<endl;
    cout << "6. Underage driving is strictly prohibited."<<endl;
    cout << "7. Driving without a valid license is illegal."<<endl;
    cout << "8. Overloading of vehicles beyond capacity is not allowed."<<endl;
    cout << "9. Illegal parking (blocking roads or private property) is prohibited."<<endl;
    cout << "10. Follow the speed limits for different zones (city, highway, school area)."<<endl;
    cout << "11. Always follow traffic signs and road markings."<<endl;
    cout << "12. Give way to emergency vehicles like ambulances, fire trucks, and police vehicles."<<endl;
    cout << "13. Do not drive under the influence of alcohol or drugs."<<endl;
    cout << "14. Maintain safe distance between vehicles to avoid accidents."<<endl;
    cout << "15. Use indicators before turning or changing lanes."<<endl;
    cout << "16. Stop for pedestrians at pedestrian crossings."<<endl;
    cout << "17. Avoid honking unnecessarily in residential areas."<<endl;
    cout << "18. Follow lane discipline at all times."<<endl;
    cout << "19. Use headlights properly at night or in foggy conditions."<<endl;
    cout << "20. Always carry vehicle registration, license, and insurance documents."<<endl;
}


void checkTrafficStatus() {
    cout<<"\n" <<setw(60)<< "Current Traffic Condition: " << trafficCondition <<endl;
}

// file handling functions
       // data saving functions
void savepassword(){
    ofstream file("password.txt");
    if(!file) return;
    
    file<<left<<setw(15)<<"\033[36mAdminPassword\033[0m"<<setw(15)<<"\033[36mUsername\033[0m"<<endl;
    file<<"====================================================="<<endl;
    file<<left<<setw(15)<< adminPassword;
    file<< username;    
    file.close();
}

void saveVehicles() {
    ofstream file("vehicles.txt");
    if (!file) return;

    file << left
         << setw(5)  << "ID"
         << setw(15) << "Owner_Name"
         << setw(18) << "Vehicle_Number"
         << setw(18) << "License_Number"
         << setw(15) << "Vehicle_Type"
         << setw(16) << "License_Points"
         << setw(15) << "License_Valid"
         << setw(13) << "Blacklisted"
         << "\n";

    file << "=================================================================================================================\n";

    for (int i = 0; i < vehicle_count; i++) {
        file << left
             << setw(5)  << (i + 1)
             << setw(15) << vehicles[i].ownerName
             << setw(18) << vehicles[i].vehicleNumber
             << setw(18) << vehicles[i].licenseNumber
             << setw(15) << vehicles[i].vehicleTypeName
             << setw(16) << vehicles[i].licensePoints
             << setw(15) << vehicles[i].licenseValid
             << setw(13) << vehicles[i].blacklisted
             << "\n";
    }

    file.close();
}

void saveViolations() {
    ofstream file("violations.txt");
    if (!file) return;

    file << left
         << setw(12) << "Challan_ID"
         << setw(18) << "Vehicle_Number"
         << setw(18) << "Violation_Type"
         << setw(8)  << "Fine"
         << setw(6)  << "Paid"
         << setw(15) << "Date"
         << setw(10) << "Time"
         << "\n";

    file << "====================================================================================\n";

    // Data
    for (int i = 0; i < violation_count; i++) {
        file << left
             << setw(12) << violations[i].challanID
             << setw(18) << violations[i].vehicleNumber
             << setw(18) << violations[i].violationTypeName
             << setw(8)  << violations[i].fine
             << setw(6)  << violations[i].paid
             << violations[i].year << "-"
             << violations[i].month << "-"
             << violations[i].day
             << setw(2) << " "
             << violations[i].hour << ":"
             << violations[i].min << ":"
             << violations[i].sec << " "
             << violations[i].ampm
             << "\n";
    }

    file.close();
}

void saveSignals() {
    ofstream file("signals.txt");
    if (!file) return;

    file << left << setw(5) << "ID"
         << setw(30) << "Location" << "\n";
    file << "===========================\n";

    for (int i = 0; i < signal_count; i++) {
        file << left
             << setw(5)  << signals[i].signalID
             << setw(30) << signals[i].location
             << "\n";
    }

    file.close();
}
          // data loading functions
void loadpassword(){
    ifstream file("password.txt");
    if (!file) return;
    string line;
    getline(file, line); // skip line
    getline(file, line);   
     while(file){
        file>>adminPassword;
        file>>username;
     }
   
   file.close();
}

void loadVehicles() {
    ifstream file("vehicles.txt");
    if (!file) return;

    string line;
    vehicle_count = 0;

    getline(file, line);
    getline(file, line);

    while(vehicle_count < maxv) {
        int id;
        Vehicle v;
        file >> id
             >> v.ownerName
             >> v.vehicleNumber
             >> v.licenseNumber
             >> v.vehicleTypeName
             >> v.licensePoints
             >> v.licenseValid
             >> v.blacklisted;

        if(file) {
            v.vehicleType = vehicle_types_names(v.vehicleTypeName);
            vehicles[vehicle_count++] = v;
        }
        else break;
    }
    file.close();
}

void loadViolations() {
    ifstream file("violations.txt");
    if (!file) return;

    string line;
    violation_count = 0;

    getline(file, line);
    getline(file, line);

    while (violation_count < maxvio) {
        char dash;
        Violation v;

        if (!(file >> v.challanID
                  >> v.vehicleNumber
                  >> v.violationTypeName
                  >> v.fine
                  >> v.paid
                  >> v.year >> dash
                  >> v.month >> dash
                  >> v.day
                  >> v.hour >> dash
                  >> v.min >> dash
                  >> v.sec >> v.ampm ))
            break;

        v.violationType = violation_types_names(v.violationTypeName);
        violations[violation_count++] = v;
    }

    // Update nextChallanID
    int maxID = 0;
    for(int i = 0; i < violation_count; i++){
        if(violations[i].challanID > maxID)
            maxID = violations[i].challanID;
    }
    nextChallanID = maxID + 1;
    file.close();
}

void loadSignals() {
    ifstream file("signals.txt");
    if (!file) return;

    signal_count = 0;
    string line;

    getline(file, line);
    getline(file, line);

    while (signal_count < maxs) {
        int id;
        string loc;

        file >> id;
        getline(file >> ws, loc);  

        if (file.fail()) break;

        signals[signal_count].signalID = id;
        signals[signal_count].location = loc;
        signal_count++;
    }

    file.close();
}

// menues
         // admin menue
void adminMenu() {
    int ch;
    do{
        cout<<endl;
        cout<<endl<<setw(75)<<"\033[1m \033[34m===============ADMIN MENU===============\033[0m\n"<<endl;
        cout <<"\033[32m"<<setw(45)<< " 1.Issue Challan"<<endl;
        cout<<setw(47)<<"2.View Violations"<<endl;
        cout<<setw(42)<<"3.Add Signal"<<endl;
        cout<<setw(44)<<"4.View Signals"<<endl;
        cout<<setw(58)<<"5.Change Signal Individually"<<endl;
        cout<<setw(45)<< "6.Delete Signal"<<endl;
        cout<<setw(55)<<"7.City Lockdown (All Red)"<<endl;
        cout<<setw(58)<<"8.Green Corridor (All Green)"<<endl;
        cout<<setw(54)<<"9.Resume Automatic Cycle"<<endl;
        cout<<setw(57)<<"10.Update Traffic Condition"<<endl;
        cout<<setw(48)<<"11.Change Password"<<endl;
        cout<<setw(45)<<"12.View Reports"<<endl;
        cout<<setw(55)<<"13.Update Violation Fines"<<endl;
        cout<<setw(44)<<"0.Logout \033[0m\n"<<endl;
        cout<<setw(38)<<"Choice: ";
        ch=inputNumber(0,13);
        system("cls");
        if(ch==1){
         issueChallan();
         saveViolations();
         saveVehicles();
         pause();
         system("cls");
    }
        else if(ch==2){
         viewViolations();
         pause();
         system("cls");
    }
        else if(ch==3){
         addSignal();
         saveSignals();
         pause();
         system("cls");
    }
        else if(ch==4){
         viewSignals();
         pause();
         system("cls");
    }
        else if(ch==5){
         changeSignalIndividually();
         pause();
         system("cls");
    }
        else if(ch==6){
         deleteSignal();
         saveSignals();
         pause();
         system("cls");
    }
        else if(ch==7){
         setAllSignals(1);
         pause();
         system("cls");
    }
        else if(ch==8){
         setAllSignals(3);
         pause();
         system("cls");
    }
        else if(ch==9){
         resumeCycle();
         pause(); 
         system("cls"); 
    }
        else if(ch==10){
         updateTrafficCondition();
         pause();
         system("cls");
    }
        else if(ch==11){
         changeAdminPassword();
         savepassword();
         pause();
         system("cls");
    }
        else if(ch==12){
         viewReports();
         pause();
         system("cls");
    }
        else if(ch==13){
         updateViolationFine();
         pause();
         system("cls");
    }
    }while(ch!=0);
}
         //pulic menue
void publicMenu() {
    int ch;
    do{
        cout<<"\n"<<setw(85)<<"\033[1m \033[34m===================PUBLIC MENU=================\033[0m\n"<<endl;
        cout<<"\033[32m"<<setw(58)<<"1.Check Challan by Vehicle"<<endl;
        cout<<setw(53)<<"2.Check Challan by ID"<<endl;
        cout<<setw(42)<<"3.Pay Fine"<<endl;
        cout<<setw(46)<<"4.View Signals"<<endl;
        cout<<setw(54)<<"5.Check Traffic Status"<<endl;
        cout<<setw(52)<<"6.View Traffic Rules"<<endl;
        cout<<setw(42)<<"0.Exit\033[0m"<<endl;
        cout<<setw(43)<<"Choice: ";
        ch=inputNumber(0,6);
        system("cls");
        if(ch==1){
         checkChallanByVehicle();
         pause();
         system("cls");
    }
        else if(ch==2){
         checkChallanByID();
         pause();
         system("cls");
        }
        else if(ch==3){
         payFine();
         saveViolations();
         saveVehicles();
         pause();
         system("cls");
    }
        else if(ch==4){
         viewSignalsPublic();
         pause();
         system("cls");
    }
        else if(ch==5){ 
        checkTrafficStatus();
        pause();
        system("cls");
        }
        else if(ch==6){
         trafficRules();
         pause();
         system("cls");
        }
    }while(ch!=0);
}
              // main menue
int main() {
    loadpassword();
    loadVehicles();
    loadViolations();
    loadSignals();
    int ch;
    do{
        cout <<"\n\n\n"<< setw(85)<<"\033[1m \033[34mSMART CITY TRAFFIC CONTROL AND VIOLATION MANAGMENT SYSTEM\033[0m\n"<<endl;
        cout<<"...............................................................................................\n\n";
        cout<<"\033[32m"<<setw(44)<<"1.Admin Login"<<endl;
        cout<< setw(46)<<"2.Public Access"<<endl;
        cout<< setw(42)<<"0.Exit\n\033[0m"<<endl;
        cout<< setw(42)<<"Choice: ";
        ch=inputNumber(0,2);
        if(ch==1){
            if(adminLogin()){
            cout<<endl;
             adminMenu();
            }
            else
             cout<<"\n" <<setw(65)<< "\033[31m Incorrect Password! \033[0m"<<endl;
        }
         else if(ch==2) 
         publicMenu();
    }while(ch!=0);
    cout <<setw(60)<< "\033[34m Exiting System...\033[0m\n";
    return 0;
}
