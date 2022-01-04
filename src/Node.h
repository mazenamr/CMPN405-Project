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

#include <omnetpp.h>
#include <vector>
using namespace omnetpp;

/**
 * TODO - Generated class
 */
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

    std::vector<bool> acks;
    std::vector<bool> sent;
    std::vector<int> seq;
    std::vector<Message> messages;
    std::string outputPath = "../outputs/pair01.txt";
    int count = 0;
    bool isStarter = false;
    int index = 0;
    int countTransmissions = 0;
    int correctTransmissions = 0;
    int sequenceNumber = 0;
    void Node::sendSRUpdated(cMessage* msg, int piggyback, bool error = false);
    void sendSR(std::string messageName, int piggyback, bool error = false, int seq = 0);
    void sendMessage(std::string messageName);
    void byteStuffing();
    void byteDeStuffing(std::string message);
    char CRC(std::string, int generator);
    std::string generateCodeMessage(std::string message);
};

#endif
