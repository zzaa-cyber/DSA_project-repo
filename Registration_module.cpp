//Programmed by: Russel 
// Registration Module
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
using namespace std;

struct Record{
    string account_name;
    string account_number; // 5 digits (better na string kesa int, cause di panget maglagay if 0 unang digits)
    string birthday; //format: MM/DD/YYYY
    string contact_number;
    double initial_deposit; //minimum 5000
    string pinCode;
};

struct Node{
    Record data;
    struct Node* next;
    Node(Record x): data(x){}
};

class Registration_list{
    private:
    Node* head;
    public:
    //constructor
    Registration_list(): head(NULL){}
    //destructor
    ~Registration_list(){
        Node* p;
        while(head != NULL){
            p = head;
            head = head->next;
            delete(p);
        }
    }
    //Class methods
    string generate_Account_Num();
    string inp_PIN();
    void add_record(Record x);
    bool is_Account_Num_taken(string acc_No); // Checks if account number is taken by previous inputs
    void display();
    void save();
    void retrieve();
};

//Other Functions kineme
int menu();
bool is_Numeric(string s);
string inp_PIN();
string encrypt_PIN(string pin);
string decrypt_PIN(string pin);

//Defined class functions outside
string Registration_list::generate_Account_Num(){
    string acc_No;
    int num;
    do{
        num = rand() % 100000; //0 to 99999
        stringstream ss;
        ss << setw(5) << setfill('0') << num; 
        acc_No = ss.str();
    }
    while(is_Account_Num_taken(acc_No));
    return acc_No;
}

void Registration_list::add_record(Record x){
    Node* p;
    Node* q;
    Node* newNode;
    p=q=head;
    newNode = new Node(x);
    while(p != NULL){
        q=p;
        p=p->next;
    }
    if(p == head){
        head = newNode;
    }
    else{
        q->next = newNode;
    }
    newNode->next = p;
}
bool Registration_list::is_Account_Num_taken(string acc_No){
    Node* p;
    p=head;
    while(p != NULL){
        if(p->data.account_number == acc_No){
            return true;
        }
        p=p->next;
    }
    return false;
}
void Registration_list::display(){
Node *p;
p=head;
system("cls");
cout<<left<<setw(12)<<"Acc.No."
<<setw(20)<<"Name"
<<setw(12)<<"Birthday"
<<setw(15)<<"Contact"
<<setw(12)<<"Balance"<<endl;
while(p!=NULL){
    cout<<left<<setw(12)<<p->data.account_number
<<setw(20)<<p->data.account_name
<<setw(12)<<p->data.birthday
<<setw(15)<<p->data.contact_number
<<setw(12)<<fixed<<setprecision(2)<<p->data.initial_deposit<<endl;
    p=p->next;
}
system("pause");
}

void Registration_list::save(){
    ofstream file("accounts.csv");
    if(!file){
        cout << "File error." << endl;
        return;
    }
    else{
        Node* p = head;
        while(p != NULL){
            file << p->data.account_number << ","
            << p->data.account_name << ","
            << p->data.birthday << ","
            << p->data.contact_number << ","
            << p->data.initial_deposit << ","
            << p->data.pinCode << endl;
            p = p->next;
        }
    }
    file.close();
}

void Registration_list::retrieve(){
    ifstream file("accounts.csv");
    if(!file){
        cout << "File error." << endl;
        return;
    }
    else{
        Record x; string line; string deposit;
        while(getline(file, line)){
            if(line.empty())
                continue;
            stringstream ss(line);
            getline(ss, x.account_number, ',');
            getline(ss, x.account_name, ',');
            getline(ss, x.birthday, ',');
            getline(ss, x.contact_number, ',');
            getline(ss, deposit, ',');
            x.initial_deposit = atof(deposit.c_str());
            getline(ss, x.pinCode, ',');
            add_record(x);
            
        }
        file.close();
    }
}

//Free functions definitions
int menu(){
    int op;
    system("cls");
    cout<<"MENU\n";
    cout<<"[1]. Add Account\n";
    cout<<"[2]. Display All\n";
    cout<<"[3]. Exit\n";
    cout<<"Select option [1-3]: \n";
    cin>>op;
    if(cin.fail()){
        cin.clear();
        cin.ignore(1000,'\n');
        op = 0;
    }
    return op;
}

bool is_Numeric(string s){ //checks for letters/numbers inputs
    for(int i=0; i<s.length();i++){
        if(!isdigit(s[i])){
            return false;
        }
    }
    return true;
}
string inp_PIN(){
    string pin = "";
    char ch;
    cout << "Input your PIN code (4-6 digits): ";
    while(true){
        ch = getch(); 
        if(ch == '\r'){
            if(pin.length() >= 4){ //check if pin is inputted
                break;
            }
        }
        else if(isdigit(ch) && pin.length() < 6){ // if pin is too short;
        pin += ch;
        cout << '*';
        if(pin.length() == 6){
            break; // di na mag eenter, auto-submit na
            }
        }
    }
    cout << endl;
    return pin;
}

int PIN_KEY = 7; // for encrpytion/decryption
string encrypt_PIN(string pin){
    string enc = pin;
    for(int i=0; i<enc.length(); i++){
        int digit = enc[i] - '0';
        digit = (digit + PIN_KEY) % 10;
        enc[i] = digit + '0';
    }
    return enc;
}

string decrypt_PIN(string pin){
    string dec = pin;
    for(int i=0; i<dec.length(); i++){
        int digit = dec[i] - '0';
        digit = (digit - PIN_KEY + 10) % 10;
        dec[i] = digit + '0';
    }
    return dec;
}

string detect_Flash_drive(){
    char drive[] = "A:\\";
    char letter;
    for(letter = 'A'; letter <= 'Z'; letter++){
        drive[0] = letter; //starting index
        UINT type = GetDriveTypeA(drive);
        if(type == DRIVE_REMOVABLE){
            string d = " ";
            d += letter;
            d += ":\\";
            return d;
        }
    }
    return "";
}

void write_ATM_card(string drive_Letter, string acc_No, string encrypted_Pin){
    string path = drive_Letter + "pin.code";
    ofstream card(path.c_str());
        if(!card){
            cout << "Error writing to flash drive!" << endl;
            return;
        }
        card << acc_No << "," << encrypted_Pin << endl;
        card.close();
    }


int main(){
    srand(time(NULL));
    Record r;
    Registration_list reg;
    reg.retrieve();
    while(true){
        switch(menu()){
            case 1:{
            system("cls");
            cout<<"Registration Mode" << endl;
            // number validation - 5 digits exact, di pwede duplicate
                r.account_number = reg.generate_Account_Num();
                cout << "Your assigned Account Number is: " << r.account_number << endl;
           
                cout << "Input Account name: "; cin.ignore(); getline(cin,r.account_name);
                cout << "Input Birthday (MM/DD/YYYY): "; getline(cin,r.birthday);
                cout << "Input Contact Number: "; getline(cin,r.contact_number);
                //Initial deposit checker - minimum is 5000
                do{
                    cout <<"Input Initial Deposit (Min. 5000): ";cin>>r.initial_deposit;
                    if(cin.fail()){
                        cin.clear();
                        cin.ignore(1000,'\n');
                        cout << "Invalid input. Please enter a number." << endl;
                        continue;
                    }
                    if(r.initial_deposit < 5000){
                        cout <<"Invalid amount. Minimum deposit is 5000." << endl;
                    }
                    else{
                        break;
                    }
                }
                    while(true);
                    r.pinCode = inp_PIN();
                    r.pinCode = encrypt_PIN(r.pinCode);
                    string drive;
                    do{
                        drive = detect_Flash_drive();
                        if(drive == ""){
                            cout << "Please insert card." << endl;
                            system("pause");
                        }

                    }while(drive == "");
                    write_ATM_card(drive, r.account_number, r.pinCode);
                    cout << "ATM card written to drive " << drive << endl; 
                    reg.add_record(r);
                    cout << "\n Account Registered Succesfully. Welcome " << r.account_name << endl;
                    system("pause");
                    break;
            }
            case 2:
            reg.display();
            break;
            case 3:
            cout << "Exiting program...\n";
            reg.save();
            system("pause");
            exit(0);
            default:
            cout << "Input choices 1 to 3 only.\n"; system("pause");
        }
    }
    
    return 0;
}
