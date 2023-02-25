#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include<string>
#include<math.h>
#include<bitset>
#include<sstream>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
const unsigned int  maxUInt = 4294967295;
const unsigned int  minUInt = -4294967295;
const int  maxInt = 2147483647;
const int  minInt = -2147483647;

// 'x' - '0' = x ;
class QInt
{
	vector<unsigned int> num;
	int First_num;

public:

	QInt();
	QInt(string sbin, string type);
	QInt(const QInt& q);
	~QInt();
	void print();
	string print(string);
	QInt operator ~ ();
	QInt rol();
	QInt ror();
	QInt& operator=(const QInt& q1);
	friend QInt operator << (const QInt& q1, int k);
	QInt operator + (const QInt& q1);
	friend QInt operator >> (const QInt& q1, int k);
	QInt operator - (const QInt& q1);
	QInt operator * (QInt q1);
	QInt operator / (QInt q1);
	QInt operator & (const QInt& q1);
	QInt operator | (const QInt& q1);
	QInt operator ^ (const QInt& q1);
	QInt operator - ();
	QInt operator - () const;
	bool isNegative() const;
	bool isEqualZero();
	// convert
	string toBin();
	string toDec();
	string toHex();

};
string addZero(string s);
string deleteZero(string s);
bool isZero(string s);
char getHexaChar(char HexC);
//string mul2(string a);
string StringSum(string a, string b);
string Div2(const string& Src);


