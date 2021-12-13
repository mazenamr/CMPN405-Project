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

#ifndef __CMPN405_PROJECT_COORDINATOR_H_
#define __CMPN405_PROJECT_COORDINATOR_H_

#include <omnetpp.h>
#include <vector>

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Coordinator : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
  public:
    const std::string fileName = "coordinator.txt";
    std::vector<Instruction> instructions;

    struct Instruction
    {
        int nodeId;
        std::string fileName;
        bool isStart = false;
        int startTime = -1;

        Instruction()
        {
        }

        Instruction(int id, std::string file, bool sender, int time)
        {
            this->nodeId = id;
            this->fileName = file;
            this->isStart = sender;
            this->startTime = time;
        }
    };
};

#endif
