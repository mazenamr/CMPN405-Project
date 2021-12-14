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
#include "CoordinatorMessage_m.h"
#include "NodeMessage_m.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

Define_Module(Node);

void Node::initialize()
{
    // TODO - Generated method body

}

void Node::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if (strcmp(msg->getName(), "coordinator") == 0)
    {
        CoordinatorMessage_Base *cmsg = check_and_cast<CoordinatorMessage_Base*>(msg);
        char path[] = "../inputs/";
        std::strcat(path, cmsg->getConfigFileName());
        std::ifstream fin(path);
        std::vector<std::string> inputStrings;
        while (fin)
        {
            std::string temp;
            fin >> temp;
            inputStrings.push_back(temp);
        }

        std::string bits = inputStrings[0];
        for (int i = 1; i < inputStrings.size(); ++i)
        {

            std::string message = "";
            while (inputStrings[i][0] != '0' && inputStrings[i][0] != '1')
            {
                message += (inputStrings[i] + " ");
                ++i;
                if (i == inputStrings.size())
                    break;
            }
            messages.push_back(Message(bits, message));
            if (i != inputStrings.size())
                bits = inputStrings[i];
        }
    }
    // TO DO: BYTE STUFFING
    // CRC checksum bytes
    for (int i = 0; i < messages.size(); ++i)
    {
        NodeMessage_Base *nmsg = new NodeMessage_Base("node1");
        nmsg->setPayload(messages[i].content.c_str());
        messageHeader header;
        header.messageId = i;
        //header.sendingTime = simTime();
        nmsg->setHeader(header);

        send(nmsg, "out");

        std::cout << messages[i].modification << messages[i].loss << messages[i].duplicated << messages[i].delay << " " << messages[i].content << std::endl;
        /*if (messages[i].duplicated)
            sendDelayed(nmsg, 0.01, "out");

        if (messages[i].delay)
            sendDelayed(nmsg, par("delay").doubleValue(), "out");*/
    }

}



