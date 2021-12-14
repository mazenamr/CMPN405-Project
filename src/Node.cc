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

void Node::sendMessage(std::string messageName)
{
    if (index >= messages.size())
        return;

    NodeMessage_Base *nmsg = new NodeMessage_Base(messageName.c_str());
    if (!messages[index].loss)
    {
        double delay = messages[index].delay ? par("delay").doubleValue() : 0;
        nmsg->setPayload(messages[index].content.c_str());
        messageHeader header;
        header.messageId = index;
        header.sendingTime = simTime().raw();
        nmsg->setHeader(header);

        if (messages[index].duplicated)
            sendDelayed(nmsg->dup(), 0.01 + delay, "out");

        // normal send
        sendDelayed(nmsg->dup(), delay, "out");
    }
    std::cout << messages[index].modification << messages[index].loss
            << messages[index].duplicated << messages[index].delay
            << " " << messages[index].content << std::endl;
    index++;
}

void Node::byteStuffing()
{
    for(int i = 0; i < messages.size(); ++i)
    {
        for(int j = 0; j < messages[i].content.size(); ++j)
        {
            if(messages[i].content[j] == '$' || messages[i].content[j] == '/')
            {
                messages[i].content.insert(j, "/");
                j++;
            }
        }
    }
}

void Node::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if (strcmp(msg->getName(), "coordinator") == 0)
    {
        CoordinatorMessage_Base *cmsg = check_and_cast<CoordinatorMessage_Base*>(msg);
        char path[] = "../inputs/";
        bool isStart = cmsg->getIsStart();
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
            message.pop_back();
            messages.push_back(Message(bits, message));
            if (i != inputStrings.size())
                bits = inputStrings[i];
        }

        byteStuffing();

        if(isStart)
            sendMessage("starter");

        return;
    }

    if (strcmp(msg->getName(), "starter") == 0)
    {
        NodeMessage_Base *receivedMsg = check_and_cast<NodeMessage_Base*>(msg);
        // Error detection and/or correction happen here.
        std::cout << "Received: " << receivedMsg->getPayload() << std::endl;
        NodeMessage_Base *nmsg = new NodeMessage_Base(index % 2 == 0 ? "NACK" : "ACK");
        send(nmsg, "out");
        index++;
    }
    else
        sendMessage("starter");

}



