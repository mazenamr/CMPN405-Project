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
      bool duplication;
      bool delay;
      std::string content;

      Message()
      {
      }

      Message(std::string bits, std::string message)
      {
        modification = bits[0] == '0' ? false : true;
        loss = bits[1] == '0' ? false : true;
        duplication = bits[2] == '0' ? false : true;
        delay = bits[3] == '0' ? false : true;
        content = message;
      }
    };

    std::vector<Message> messages;
};

#endif
