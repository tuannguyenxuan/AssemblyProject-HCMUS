#include "QInt.h"

QInt::QInt()
{
	First_num = 0;
	num.resize(3);
	num[0] = num[1] = num[2] = 0;
}
QInt::QInt(const QInt& q)
{
	First_num = q.First_num;
	this->num = q.num;
}
QInt::QInt(string in, string type)
{
	num.resize(3);
	First_num = 0;
	if (type == "bin")
	{
		in = addZero(in);
		for (int i = 0; i < 4; i++)
		{
			bitset<32> bit(in.substr(i * 32, 32));
			if (i == 0) First_num = (int)(bit.to_ulong());
			else num[i - 1] = bit.to_ulong(); //Luu day bit duoi dang unsigned long
		}
	}
	else if (type == "dec")
	{
		string Result;
		bool isNegative = false;

		if (in[0] == '-') //Kiểm tra số âm.
		{
			in.erase(0, 1); //xóa dấu trừ
			isNegative = true;
		}

		while (in != "")
		{
			Result.push_back(((in[in.size() - 1] - '0') % 2) + '0'); //Chia hết cho 2 -> 0 else 1
			in = Div2(in);
		}

		reverse(Result.begin(), Result.end()); // Đảo ngược để lấy dãy bit đúng

		//Nếu là chuỗi ban đầu là số âm.
		Result = addZero(Result); // padding để đủ 128bit
		for (int i = 0; i < 4; i++)
		{
			bitset<32> bit(Result.substr(i * 32, 32));
			if (i == 0) First_num = (int)(bit.to_ulong());
			else num[i - 1] = bit.to_ulong(); //Luu day bit duoi dang unsigned long
		}
		if (isNegative == true)	*this = -(*this); //Nếu âm thì chuyển sang bù 2

	}
	else if (type == "hex")
	{
		//Lấy 1 char từ string hex (từng nhóm 4 bit) đưa vào QInt
		for (int i = 0; i < in.length(); i++)
		{
			*this = *this << 4; //chuẩn bị 4 bit 0 ở cuối
			num[2] = num[2] | getHexaChar(in[i]); //Thay 4 bit 0 bằng bit của ký tự hex
		}
	}
}
QInt& QInt::operator =(const QInt& q)
{
	First_num = q.First_num;
	this->num = q.num;
	return *this;
}
QInt::~QInt()
{

}
void QInt::print()
{
	bitset<32> bit;
	bit = First_num;
	cout << First_num << " : ";
	cout << bit.to_string() << endl;

	for (int i = 0; i < 3; i++)
	{
		cout << num[i] << " : ";
		bit = num[i];
		cout << bit.to_string() << endl;
	}
	cout << endl;
}
string QInt::print(string type)
{
	if (type == "2") return toBin();
	if (type == "10") return toDec();
	if (type == "16") return toHex();
	return "";
}
QInt operator << (const QInt& q1, int k)
{
	QInt temp(q1);
	while (k > 31)
	{
		temp.First_num = (temp.First_num << 31) | (temp.num[0] >> 1);
		temp.num[0] = (temp.num[0] << 31) | (temp.num[1] >> 1);
		temp.num[1] = (temp.num[1] << 31) | (temp.num[2] >> 1);
		temp.num[2] = (temp.num[2] << 31);
		k -= 31;
	}
	temp.First_num = (temp.First_num << k) | (temp.num[0] >> (32 - k));
	temp.num[0] = (temp.num[0] << k) | (temp.num[1] >> (32 - k));
	temp.num[1] = (temp.num[1] << k) | (temp.num[2] >> (32 - k));
	temp.num[2] = (temp.num[2] << k);
	return temp;
}
QInt operator >> (const QInt& q1, int k)
{
	QInt temp(q1);
	while (k > 31)
	{
		temp.num[2] = (temp.num[2] >> 31) | (temp.num[1] << 1);
		temp.num[1] = (temp.num[1] >> 31) | (temp.num[0] << 1);
		temp.num[0] = (temp.num[0] >> 31) | (temp.First_num << 1);
		temp.First_num = (temp.First_num >> 31);
		k -= 31;
	}
	temp.num[2] = (temp.num[2] >> k) | (temp.num[1] << (32 - k));
	temp.num[1] = (temp.num[1] >> k) | (temp.num[0] << (32 - k));
	temp.num[0] = (temp.num[0] >> k) | (temp.First_num << (32 - k));
	temp.First_num = (temp.First_num >> k);

	return temp;
}
QInt QInt::operator + (const QInt& q1)
{
	unsigned long long kq;
	QInt temp;
	int carry = 0;
	for (int i = 2; i >= 0; i--)
	{
		kq = (unsigned long long)this->num[i] + q1.num[i] + carry;
		if (kq <= maxUInt)
		{
			temp.num[i] = kq;
			carry = 0;
		}
		else
		{
			temp.num[i] = kq - maxUInt - 1;
			carry = 1;
		}
	}
	long long kq2 = 0;
	kq2 = (long long)this->First_num + (long long)q1.First_num + carry;
	cout << endl << kq2<<" "<<(long long)First_num<<" "<<(long long)q1.First_num<<endl;
	if (kq2 > maxInt || kq2 < minInt) {
		QInt zero;
		return zero;
	}
	temp.First_num = kq2;
	return temp;
}
QInt QInt::operator - (const QInt& q1)
{
	QInt temp = q1;
	return *this + (-temp);
}
QInt QInt::rol()
{
	QInt temp = *this;
	int First_bit = (temp.First_num >> 31) & 1;
	temp = temp << 1;
	temp.num[2] = temp.num[2] | First_bit;
	return temp;
}
QInt QInt::ror()
{
	QInt temp = *this;
	long long bit = temp.num[2] & 1;
	temp = temp >> 1;
	if (bit == 1) temp.First_num = temp.First_num | (1 << 31);  // bat bit 31
	if (bit == 0) temp.First_num = temp.First_num & (~(1 << 31));//tat bit 31
	
	return temp;
}

QInt QInt::operator ~ ()
{
	QInt temp = *this;
	temp.First_num = ~temp.First_num;
	temp.num[0] = ~temp.num[0];
	temp.num[1] = ~temp.num[1];
	temp.num[2] = ~temp.num[2];
	return temp;
}
QInt QInt::operator & (const QInt& q1)
{
	QInt temp = *this;
	temp.First_num = this->First_num & q1.First_num;
	temp.num[0] = this->num[0] & q1.num[0];
	temp.num[1] = this->num[1] & q1.num[1];
	temp.num[2] = this->num[2] & q1.num[2];
	return temp;
}
QInt QInt::operator | (const QInt& q1)
{
	QInt temp = *this;
	temp.First_num = this->First_num | q1.First_num;
	temp.num[0] = this->num[0] | q1.num[0];
	temp.num[1] = this->num[1] | q1.num[1];
	temp.num[2] = this->num[2] | q1.num[2];
	return temp;
}
QInt QInt::operator ^ (const QInt& q1)
{
	QInt temp = *this;
	temp.First_num = this->First_num ^ q1.First_num;
	temp.num[0] = this->num[0] ^ q1.num[0];
	temp.num[1] = this->num[1] ^ q1.num[1];
	temp.num[2] = this->num[2] ^ q1.num[2];
	return temp;
}
QInt QInt::operator -()
{
	QInt temp = *this;
	QInt one;
	one.num[2] = 1; //one = 1 
	temp = ~temp + one; 	//Chuyển sang dạng bù 2
	return temp;
}
QInt QInt::operator -() const
{
	QInt temp = *this;

	QInt one;
	one.num[2] = 1;
	temp = ~temp + one;
	return temp;
}
QInt QInt::operator * (QInt q1)
{
	QInt temp = *this;
	QInt res;
	//Xử lý dấu
	if (temp.isEqualZero() == 1 || q1.isEqualZero() == 1) return res;
	bool isNegative = true;
	if (temp.isNegative() == q1.isNegative()) isNegative = false;
	if (temp.isNegative() == 1) temp = -(temp);
	if (q1.isNegative() == 1) q1 = -q1;
	//
	while (!q1.isEqualZero())
	{
		if ((q1.num[2] & 1) == 1) res = res + temp;
		temp = temp + temp;
		if (temp.isEqualZero() == 1) return temp;
		q1 = q1 >> 1;
	}
	if (isNegative == true) res = -res;
	return res;
}
QInt QInt::operator / (QInt q1)
{
	QInt temp = *this;
	QInt r;
	if (temp.isEqualZero() == 1 || q1.isEqualZero() == 1) return r;
	bool isNegative = true;
	if (temp.isNegative() == q1.isNegative()) isNegative = false;
	if (temp.isNegative() == 1) temp = -(temp);
	if (q1.isNegative() == 1) q1 = -q1;
	int k = 128;
	while (k > 0)
	{
		// Shift left (SHL) [r,temp]
		//r.print();
		r = r << 1;
		r.num[2] = r.num[2] | ((temp.First_num >> 31) & 1);
		temp = temp << 1;
		//r.print();

		r = r - q1;
		if (r.isNegative() == true)
			r = r + q1;

		else temp.num[2] = temp.num[2] | 1;
		k--;
	}
	if (isNegative == true) {
		temp = -temp;
	}
	return temp;
}


char getHexaChar(char HexC)
{
	switch (HexC)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'A':
		return 10;
	case 'B':
		return 11;
	case 'C':
		return 12;
	case 'D':
		return 13;
	case 'E':
		return 14;
	case 'F':
		return 15;
	default:
		break;
	}
}
bool QInt::isNegative() const
{
	if ((this->First_num >> 31 & 1) == 1) return true;
	return false;
}
bool QInt::isEqualZero() {

	if (this->First_num == 0 && this->num[0] == 0 && this->num[1] == 0 && this->num[2] == 0) return true;
	return false;
}
string addZero(string s)
{
	int k = 128 - s.length();
	string temp(k, '0');
	temp += s;
	return temp;
}
bool isZero(string s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] != '0') return false;
	}
	return true;
}
string deleteZero(string s)
{
	int count = 0;
	for (int i = 0; i < s.length() - 1; i++)
	{
		if (s[i] == '0') count++;
		if (s[i] != '0') break;
	}
	string res;
	res = s.erase(0, count);
	return res;

}
// convert
string QInt::toBin()
{
	string res = "";
	bitset<32> bit;
	for (int i = 0; i < 4; i++)
	{
		if (i == 0) bit = First_num;
		else bit = num[i - 1];
		res += bit.to_string();
	}
	//res = deleteZero(res);
	return res;
}
char Bit4ToChar(string bin)
{
	if (bin == "0000") return '0';
	else if (bin == "0001") return '1';
	else if (bin == "0010") return '2';
	else if (bin == "0011") return '3';
	else if (bin == "0100") return '4';
	else if (bin == "0101") return '5';
	else if (bin == "0110") return '6';
	else if (bin == "0111") return '7';
	else if (bin == "1000") return '8';
	else if (bin == "1001") return '9';
	else if (bin == "1010") return 'A';
	else if (bin == "1011") return 'B';
	else if (bin == "1100") return 'C';
	else if (bin == "1101") return 'D';
	else if (bin == "1110") return 'E';
	else if (bin == "1111") return 'F';
}
string QInt::toHex()
{
	string res = "";
	string bin = toBin();
	bin = addZero(bin);
	for (int i = 0; i < 128; i += 4)
		res += Bit4ToChar(bin.substr(i, 4));
	res = deleteZero(res);
	return res;
}
string QInt::toDec()
{
	QInt temp = *this;
	string res = "0";
	bool isNegative = false;
	if (temp.isNegative() == true)
	{
		isNegative = true;
		temp = -temp;
	}
	string bin = temp.toBin();
	for (int i = 0; i < bin.length(); i++)
	{
		if (bin[i] == '1')
			res = StringSum(StringSum(res, res), "1");
		else
			res = StringSum(res, res);
	}
	if (isNegative == true)
		res = "-" + res;

	return res;
}
//string mul2(string a)
//{
//	int j = a.size() - 1;
//	for (int i = a.size() - 1; i >= 0; i--, j--)
//		a[j] += (a[i] - '0');
//
//	for (int i = a.size() - 1; i > 0; i--)
//	{
//		if (a[i] > '9')
//		{
//			int t = a[i] - '0';
//			a[i - 1] = ((a[i - 1] - '0') + t / 10) + '0';
//			a[i] = (t % 10) + '0';
//		}
//	}
//	if (a[0] > '9')
//	{
//		string k;
//		k += a[0];
//		a[0] = ((a[0] - '0') % 10) + '0';
//		k[0] = ((k[0] - '0') / 10) + '0';
//		a = k + a;
//	}
//	return a;
//}
string StringSum(string a, string b)
{
	if (a.size() < b.size())
		swap(a, b);

	int j = a.size() - 1;
	for (int i = b.size() - 1; i >= 0; i--, j--)
		a[j] += (b[i] - '0');

	for (int i = a.size() - 1; i > 0; i--)
	{
		if (a[i] > '9')
		{
			int d = a[i] - '0';
			a[i - 1] = ((a[i - 1] - '0') + d / 10) + '0';
			a[i] = (d % 10) + '0';
		}
	}
	if (a[0] > '9')
	{
		string k;
		k += a[0];
		a[0] = ((a[0] - '0') % 10) + '0';
		k[0] = ((k[0] - '0') / 10) + '0';
		a = k + a;
	}
	return a;
}
string Div2(const string& dividend)
{
	// Hàm này chia chuỗi số bị chia (dividend) cho 2
	// Bằng cách chia từng chữ số từ trái qua phải
	// char -> int: int = char - '0'
	string Result;
	int div_digit = 0, remainder = 0; // div_digit: Chữ số lấy ra để chia, remainder: số dư
	for (int i = 0; i < dividend.size(); i++)
	{
		div_digit = (dividend[i] - '0') + remainder * 10;  //Lấy từng chữ số từ dividend
							 //Nếu trước đó là một phép chia có dư (remainder = 1) thì sẽ cộng thêm 10
		Result.push_back((div_digit / 2) + '0'); // Pushback kết quả chia vào Result
		remainder = div_digit % 2; //Lấy dư
	}
	if (Result[0] == '0') Result.erase(0, 1); // Xóa 0 ở đầu kết quả
	return Result;
}