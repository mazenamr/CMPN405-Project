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


Define_Module(Coordinator);

void Coordinator::initialize()
{
    std::ifstream fin(fileName);
    std::vector<std::string> inputStrings(par("n").intValue()*3);

    for (auto&& i: inputStrings)
    {
        fin >> i;
    }

    std::vector<Instruction> instructions(par("n").intValue());
    instructions[0].nodeId = stoi(inputStrings[0]);
    instructions[0].fileName = inputStrings[1];
    instructions[0].isStart = false;
    instructions[0].startTime = -1;
    if (inputStrings[2] == "start")
    {
        instructions[0].isStart = true;
        instructions[0].startTime = stoi(inputStrings[3]);
        instructions[1].nodeId = stoi(inputStrings[4]);
        instructions[1].fileName = inputStrings[5];
        instructions[1].isStart = false;
        instructions[1].startTime = -1;
    }
    else
    {
        instructions[1].nodeId = stoi(inputStrings[2]);
        instructions[1].fileName = inputStrings[3];
        instructions[1].isStart = true;
        instructions[1].startTime = stoi(inputStrings[5]);
    }


    for (int i = 0; i < 2; ++i)
    {
        CoordinatorMessage_Base *node = new CoordinatorMessage_Base("node");
        node->setConfigFileName(instructions[i].fileName.c_str());
        node->setIsStart(instructions[i].isStart);
        node->setStartTime(instructions[i].isStart ? instructions[i].startTime : 0);
        sendDelayed(node, instructions[i].startTime - simTime(), "outs", instructions[i].nodeId);
    }
}

void Coordinator::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}
