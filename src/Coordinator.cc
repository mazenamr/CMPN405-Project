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
#include "CoordinatorMessage_m.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>


Define_Module(Coordinator);

void Coordinator::initialize()
{
    std::ifstream fin(fileName);
    std::vector<std::string> inputStrings;
    std::string temp;
    while (fin >> temp)
    {
        inputStrings.push_back(temp);
    }

    std::vector<Instruction> instructions;
    for (int i = 0; i < inputStrings.size(); i += 2)
    {
        Instruction x(stoi(inputStrings[i]), inputStrings[i + 1], false, 0);

        if (i + 2 < inputStrings.size() && inputStrings[i + 2] == "start")
        {
            x.isStart = true;
            x.startTime = stoi(inputStrings[i + 3]);
            i += 2;
        }

        instructions.push_back(x);
    }

    for (int i = 0; i < par("n").intValue(); ++i)
    {
        CoordinatorMessage_Base *node = new CoordinatorMessage_Base("coordinator");
        node->setConfigFileName(instructions[i].fileName.c_str());
        node->setIsStart(instructions[i].isStart);
        node->setStartTime(instructions[i].isStart ? instructions[i].startTime : 0);
        std::cout << node->getName() << " "<< node->getConfigFileName() << " " << node->getIsStart() << " " << node->getStartTime() << std::endl;
        sendDelayed(node, instructions[i].startTime - simTime(), "outs", instructions[i].nodeId);
    }
}

void Coordinator::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}
