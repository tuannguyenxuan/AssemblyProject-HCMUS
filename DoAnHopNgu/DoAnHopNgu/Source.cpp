#include"QInt.h"
int main()
{
    QInt q1("57C3320B182E4DADB0AD346C9896EF1A", "hex");
    QInt q2("57C3320B182E4DADB0AD346C9896EF1A", "hex");
    //cout << "q1 ror =  " << endl;
  // cout << " "<<(q1).toDec() << endl;
   cout << " " << (q1+q2).toBin() << endl;
   // cout << string("11000011101110110100100110100100000111111110111101000001111100100101011001011010111001110111100010110011000100100000100010000110").size();

   // cout << (q1.ror()).toDec();
    cout << endl;
	
} 

