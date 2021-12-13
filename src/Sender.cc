//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Sender.h"
#include "MyMessage_m.h"
Define_Module(Sender);

void Sender::initialize()
{
   scheduleAt(simTime() + 5, new cMessage(""));
}

void Sender::handleMessage(cMessage *msg)
{
    static int seqNumber = 0;
    if(strcmp(msg->getName(), "NACK") != 0)
    {
        EV << "Negative ack" << endl;
    }
    else
    {
        EV << "Pos ack" << endl;
    }
    // TODO
    //1) get the string from the user
    //2) calculate the char count
    //3) define the vector of the bitsets
    //4) append the charcount to the vector
    //5) loop on the characters and append them to the vector
    //and calculate the paritty check simultaniouly
    //6) loop on the vector print the message and convert every bitset to char
    //and append them to the final string message
    // 7) do the modification to single bit
    std::string userInput = "";
    std::cin >> userInput;
    MyMessage_Base * mmsg = new MyMessage_Base();
    mmsg->setM_Type(0);
    mmsg->setSeq_Num(seqNumber);
    //std::bitset<8> temp('A');
    //msg->setMycheckbits(temp);

    std::vector<std::bitset<8> > vec;
    std::bitset<8> header(userInput.size());
    vec.push_back(header);
    std::cout << (char)vec[0].to_ulong() <<std::endl;
    for(int i = 0; i < userInput.size(); i++)
    {
        std::bitset<8> letter(userInput[i]);
        vec.push_back(letter);
    }
    //add parity
    std::bitset<8> parity(0);
    parity = parity ^ vec[0];
    for(int i = 1; i < vec.size(); i++)
    {
        parity = parity ^ vec[i];
    }
    vec.push_back(parity);
    std::string final="";
    for(int i = 0; i < vec.size(); i++)
    {
        std::cout<<(char)vec[i].to_ulong();
    }
    std::cout<<std::endl;
    float prob = uniform(0,1);
    if(prob >= 0.5)
    {
        int bit = uniform(0, userInput.size() - 1 );
        vec[bit][0] = !vec[bit][0];
    }
    for(int i = 0; i < vec.size(); i++)
    {
         final = final + (char)vec[i].to_ulong();
    }
    std::cout << final << std::endl;

    mmsg->setM_Payload(final.c_str());
    mmsg->setMycheckbits(parity);

    //cancelAndDelete(msg);
     // should add the frame header payload, and the trailer to the string final
    seqNumber++;
    send((cMessage*)mmsg,"out");
}
