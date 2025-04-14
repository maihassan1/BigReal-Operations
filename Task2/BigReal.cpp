#include <bits/stdc++.h>
using namespace std;

class BigReal{
private:
    string value;
    bool isValidReal(string realNumber);

public:
    BigReal(double realNumber );
    BigReal(string realNumber);
    BigReal(BigReal& other);
    BigReal& operator=(BigReal& other);

    void setNum(string& realNumber);
    int size() const;
    int sign() const;

    BigReal operator+(BigReal& other);
    BigReal operator-(BigReal& other);
    bool operator<(BigReal& anotherReal);
    bool operator>(BigReal& anotherReal);
    bool operator==(BigReal& anotherReal);

    friend ostream& operator<<(ostream& out,BigReal& num);


};
bool BigReal::isValidReal(string realNumber) {
    if(realNumber.empty()){
        return false;
    }
    int decimalPointCount=0;
    for(int i=0;i<realNumber.size();i++){
        if(!isdigit(realNumber[i])&& realNumber[i]!='.'
           && realNumber[i]!='+'&& realNumber[i]!='-')
            return false;
        if(realNumber[i]=='.'){
            decimalPointCount++;
        }
        if(decimalPointCount>1)
            return false;
    }

    return true;
}
BigReal::BigReal(double realNumber ) {

    value = to_string(realNumber);
    if(!isValidReal(value)){
        value = "0.0";
    }
}
BigReal::BigReal(string realNumber) {
if(isValidReal(realNumber)){
value=realNumber;
}
else
value="0.0";
}

BigReal::BigReal(BigReal &other) :value(other.value){}
BigReal& BigReal::operator=(BigReal& other){
    if (this != &other) {
        value = other.value;
    }
    return *this;
}
void BigReal::setNum(string &realNumber) {
    if(isValidReal(realNumber)){
        value= realNumber;
    }
    else
        value="0.0";
}
int BigReal::size() const {

        int decimalPointPosition = value.find('.');
        if (decimalPointPosition != -1) {
            return (value.size()) - 1;
        } else
            return (value.size());

}
int BigReal::sign() const {
    if(value[0] == '-'){
        return -1;
    }
    else if(value[0] == '+'){
        return +1;
    }
    else return 0;
}

bool BigReal::operator==(BigReal &anotherReal) {
    return value==anotherReal.value;
}
ostream& operator<<(ostream& out,BigReal& num){
    out<<num.value;
    return out;
}
BigReal BigReal:: operator+( BigReal& other) {
    BigReal r(stod(this->value) + stod(other.value));

    return r;
}
BigReal BigReal:: operator-( BigReal& other) {
    BigReal r(stod(this->value) - stod(other.value));

    return r;
}

bool BigReal::operator> (BigReal& other) {

    return stod(this->value) > stod(other.value);
}

bool BigReal::operator< (BigReal& other) {

    return stod(this->value) < stod(other.value);
}

int main(){
    BigReal r1("+3.3.3");
    BigReal r2("-22");

    cout<<r1.sign() <<" "<<r2.sign();

}
