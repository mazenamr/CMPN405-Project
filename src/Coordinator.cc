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

using namespace std;

Define_Module(Coordinator);

void Coordinator::initialize()
{
    ifstream fin(fileName);
    vector<string> inputStrings(par("n").intValue()*3);

    for (auto&& i: inputStrings)
    {
        fin >> i;
    }

    vector<Instruction> instructions(par("n").intValue());
    instructions[0] = new instruction(stoi(inputStrings[0]), inputStrings[1], false, -1);

    if (inputStrings[2] == "start")
    {
        instructions[0].isStart = true;
        instructions[0].startTime = stoi(inputStrings[3]);
        instructions[1] = new instruction(stoi(inputStrings[4]), inputStrings[5], false, -1);
    }
    else
    {
        instructions[1] = new instruction(stoi(inputStrings[2]), inputStrings[3], true, stoi(inputStrings[5]));
    }
}

void Coordinator::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}
