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

#include "Node.h"
#include "MyMessage_m.h"
Define_Module(Node);

void Node::initialize()
{
    // TODO - Generated method body
    if ( strcmp(getName(),"Tic")==0)
    {
        MyMessage_Base * msg = new MyMessage_Base("Hello");
        msg->setM_Payload("hi there!");
        msg->setM_Type(1);
        msg->setSeq_Num(12);
        std::bitset<8> temp('A');
        msg->setMycheckbits(temp);
      send(msg,"out");
    }
}

void Node::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    MyMessage_Base *mmsg = check_and_cast<MyMessage_Base *>(msg);
    EV<<"received message with sequence number ...   ";
    EV << mmsg->getSeq_Num();
    EV<<"  and payload of ... ";
    EV<< mmsg->getM_Payload();
    EV<<"   and check bits of ...";
    EV<< mmsg->getMycheckbits().to_string();

    int rand=uniform(0,1)*10;
       EV<<"rand is "<<std::to_string(rand)<<endl;
       if(rand<8) // prob to delay the message
       {
           std::string mypayload= mmsg->getM_Payload();
           mypayload[0]=mypayload[0]+5;
           mmsg->setM_Payload(mypayload.c_str());
       }

    send(mmsg,"out");

}



