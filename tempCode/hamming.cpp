#include <vector>
#include <string>
#include <iostream>
#include <bitset>

using namespace std;

string getHammingParity(std::string message)
{
	string codeMessage = "";
    for (int i = 0; i < message.length(); i++)
    {
        bitset<8> bits(message[i]);
        codeMessage += bits.to_string();
    }
    //1000001
    //codeMessage = "1101001100110101"; 
    int r = 0;
    int m = codeMessage.size(); // number of bits in the mesaage
    int i = 0;
    while (pow(2, i) <= m + 1)
    {
        i++;
    }
    r = i;        
    codeMessage.insert(0, "9");
    //cout << codeMessage << " " << r;
    // We now need to insert hamming bits at there correct posistions
    // 1000001 -> 0010001001
    vector<int> hammingBitsPositions;
    vector<bool> hammingBits(r, 0);
    for (int i = 0; i < r ;i++)
    {
        int x = pow(2, i);
        codeMessage.insert(x, "0");
        hammingBitsPositions.push_back(x);
        
    }
    for (int i = 0; i < hammingBits.size(); i++)
    {
        int stride = pow(2, i);
        int lastIndex = -1;
        //initial elements
        for (int j = stride; j < stride * 2; j++)
        {
            if (j < codeMessage.size())
            {
                cout << "for p" << i << " we use m" << j << endl;
                hammingBits[i] = hammingBits[i] ^ (codeMessage[j] == '1' ? 1 : 0);
                lastIndex = j + 1;
            }
        }
        //alternating part
        lastIndex += stride;
        // if stride == 1
        if (stride == 1)
        {
            for (int k = lastIndex; k < codeMessage.size(); k++)
            {
                if (k % 2 != 0)
                {
                    cout << "for p" << i << " we use m" << k << endl;
                    hammingBits[i] = hammingBits[i] ^ (codeMessage[k] == '1' ? 1 : 0);
                }
            }
        }
        else
        {
            int flag = lastIndex;
            while (flag < codeMessage.size())
            {
                int x = lastIndex;
                int y = lastIndex + stride;
                for (int k = x; k < y; k++)
                {
                    if (k < codeMessage.size())
                    {
                        cout << "for p" << i << " we use m" << k << endl;
                        hammingBits[i] = hammingBits[i] ^ (codeMessage[k] == '1' ? 1 : 0);
                        lastIndex = k + 1;
                    }
                }
                lastIndex += stride;
                flag += stride;
            }
        }
    }
    //insert into message
    for (int i = 0; i < hammingBits.size(); i++)
    {
        cout << hammingBits[i] << " ";
        codeMessage[hammingBitsPositions[i]] =  (hammingBits[i]? '1': '0');
    }
    codeMessage.erase(0, 1);
    cout << "code word is " << codeMessage << " with count " << codeMessage.size() << endl;
    string hammingBitsString = "";
    for (int i = 0; i < hammingBits.size(); i++)
        hammingBitsString += (hammingBits[i] ? '1' : '0');

    cout << hammingBitsString << endl;
    return hammingBitsString;
}