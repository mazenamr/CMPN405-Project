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

#ifndef __SECTION1_NODE_H_
#define __SECTION1_NODE_H_

#include "CoordinatorMessage_m.h"
#include "NodeMessage_m.h"
#include <omnetpp.h>
#include <vector>
#include <queue>
#include <bitset>
#include <unordered_map>
using namespace omnetpp;

class Node : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
  public:
    struct Message
    {
      bool modification;
      bool loss;
      bool duplicated;
      bool delay;
      std::string content;

      Message()
      {
      }

      Message(std::string bits, std::string message)
      {
        modification = bits[0] == '1';
        loss = bits[1] == '1';
        duplicated = bits[2] == '1';
        delay = bits[3] == '1';
        content = message;
      }
    };

    int maxSequence;
    int windowSize;
    int windowStart = 0;
    int expectedFrame = 0;
    bool allSent = false;

    std::queue<Message> allMessages;
    std::vector<Message> messagesToSend;
    std::unordered_map<std::string, int> receivedStrings;
    std::vector<bool> messageStatus;

    bool isStarter = false;
    int countTransmissions = 0;
    int correctTransmissions = 0;
    int sequenceNumber = 0;
    int errorCheckingAlgorithm = 0;
    std::string outputPath = "../outputs/pair01.txt";

    void stuffBytes();
    void destuffBytes(std::string& message);
    void sendWindow(bool error);
    char CRC(std::string, int generator);
    std::string getHammingParity(std::string message);
};

#endif
