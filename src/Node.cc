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

        for (int i = 0; i < inputStrings.size(); ++i)
        {

            std::string bits = inputStrings[i];
            std::string message = "";
            while (inputStrings[i][0] != '0' || inputStrings[i][0] != '1')
            {
                message += (inputStrings[i] + " ");
                ++i;
                if (i == inputStrings.size())
                    break;
            }
            messages.push_back(Message(bits, message));
            --i;
        }

        for (int i = 0; i < messages.size(); ++i)
        {
            NodeMessage_Base *nmsg = new NodeMessage_Base("node1");
        }
        //delay(200);
    }

}



