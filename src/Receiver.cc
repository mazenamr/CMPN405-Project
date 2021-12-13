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

#include "Receiver.h"
#include "MyMessage_m.h"
Define_Module(Receiver);

void Receiver::initialize()
{
    // TODO - Generated method body
}

void Receiver::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    MyMessage_Base *mmsg = check_and_cast<MyMessage_Base *>(msg);
    EV<<"received message with sequence number ...   ";
    EV << mmsg->getSeq_Num();
    EV<<"  and payload of ... ";
    EV<< mmsg->getM_Payload();
    EV<<"   and check bits of ...";
    EV<< mmsg->getMycheckbits().to_string();

    std::string message = mmsg->getM_Payload();
    std::vector<std::bitset<8> > vec;
    for (int i = 0; i < message.size() - 1; i++)
    {
        std::bitset<8> letter(message[i]);
        vec.push_back(letter);
    }
    std::bitset<8> parity(0);
    parity = parity ^ vec[0];
    for(int i = 1; i < vec.size(); i++)
    {
        parity = parity ^ vec[i];
    }
    std::bitset<8> oldParity(message[message.size() - 1]);
    if(parity.to_ulong() != oldParity.to_ulong())
    {
        cMessage* nack = new cMessage("NACK");
        send(nack, "out");
        std::cout << "Error" << std::endl;
    }
    else
    {
        cMessage* ack = new cMessage("ACK");
        send(ack, "out");
        std::cout << "Correct and the message is " << message <<std::endl;
    }
}
