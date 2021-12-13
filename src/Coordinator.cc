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

#include "Coordinator.h"
#include <fstream>
#include <vector>
#include <string>

Define_Module(Coordinator);

void Coordinator::initialize()
{
    std::ifstream myfile(fileName);

    for (auto&& i: A)
    {
        int tempNodeId, tempStart;
        std::string nodeFileName, start;

        myfile >> tempId >> nodeFileName >> start;
        bool sender = std::strcmp("start", start) == 0 ? true : false;
        if (sender)
        {
            myfile >> tempStart;
            instructions.push_back(new instruction(tempId, nodeFileName, sender, tempStart));
        }
        else
            instructions.push_back(new instruction(tempId, nodeFileName, sender, -1));
    }

}

void Coordinator::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}
