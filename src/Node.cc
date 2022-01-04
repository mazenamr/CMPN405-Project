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
#include <cmath>

#define GENERATOR 0b100000111

Define_Module(Node);

void Node::initialize()
{
    // TODO - Generated method body
}

char Node::CRC(std::string message, int generator)
{
    int size = (int)std::log2(generator) + 1;
    int remainder = 0;
    for (int i = 0; i < message.length(); i++)
    {
        remainder ^= (message[i]);
        remainder <<= 1;
        if ((remainder >> (size - 1)) >= 1)
            remainder ^= generator;
    }
    return (char)remainder;
}

void Node::sendSRUpdated(cMessage* msg, int piggyback, bool error)
{
    for (int i = 0; i < messages.size(); ++i)
    {
        if (acks[i] == false)
        {
            index = i;
            break;
        }
    }

    if (index == messages.size())
    {
        // done sending
        NodeMessage_Base *nmsg = new NodeMessage_Base(getName());
        nmsg->setPiggybacking(error ? NACK : ACK);
        //nmsg->setPiggybackingId(piggyback);
        messageHeader header;
        header.messageId = -1;
        nmsg->setHeader(header);
        sendDelayed(nmsg, 0.2, "out");
        return;
    }

    for (int i = index; i < par("windowSize").intValue(); ++i)
    {
        if (!acks[i] && !sent[i])
        {
            sent[i] = true;
            NodeMessage_Base *nmsg = new NodeMessage_Base(getName());
            nmsg->setPiggybacking(error ? NACK : ACK);
            nmsg->setPiggybackingId(piggyback);
            nmsg->setTrailer(CRC(messages[i].content, GENERATOR));
            if (!messages[i].loss)
            {
                // modify bit block
                if (messages[i].modification)
                {
                    int randomCharacterIndex = uniform(0, messages[i].content.size());
                    int randomBitIndex = uniform(0, 7);
                    messages[i].content[randomCharacterIndex] = messages[i].content[randomCharacterIndex] ^ (1 << randomBitIndex);
                }

                double delay = messages[i].delay ? par("delay").doubleValue() : 0.0;
                nmsg->setPayload(messages[i].content.c_str());
                messageHeader header;
                header.messageId = index;
                header.sendingTime = simTime().dbl();
                nmsg->setHeader(header);

                if (messages[i].duplicated)
                {
                    countTransmissions++;
                    sendDelayed(nmsg->dup(), 0.01 + delay + i * 0.05, "out");
                }

                // normal send
                sendDelayed(nmsg->dup(), delay + i * 0.05, "out");
                std::cout << " - " << getName() << "[" << getIndex() << "]" << " sends message with id = " << nmsg->getHeader().messageId
                        << " and content = " << nmsg->getPayload() << " at t = " << simTime().dbl() + delay << "s" << (messages[index].modification ? " with modification\n" : "\n");
            }

            scheduleAt(simTime() + par("timer").doubleValue() + i * 0.05 , nmsg->dup());
        }
    }
}

void Node::sendSR(std::string messageName, int piggyback, bool error, int seq)
{

    if (index == messages.size())
    {
        // done sending
        NodeMessage_Base *nmsg = new NodeMessage_Base(getName());
        nmsg->setPiggybacking(error ? NACK : ACK);
        nmsg->setPiggybackingId(piggyback);
        messageHeader header;
        header.messageId = -1;
        nmsg->setHeader(header);
        sendDelayed(nmsg, 0.2, "out");
        return;
    }

    NodeMessage_Base *nmsg = new NodeMessage_Base(getName());
    nmsg->setPiggybacking(error ? NACK : ACK);
    nmsg->setPiggybackingId(piggyback);
    nmsg->setTrailer(CRC(messages[index].content, GENERATOR));
    if (!messages[index].loss)
    {
        // modify bit block
        if (messages[index].modification)
        {
            int randomCharacterIndex = uniform(0, messages[index].content.size());
            int randomBitIndex = uniform(0, 7);
            messages[index].content[randomCharacterIndex] = messages[index].content[randomCharacterIndex] ^ (1 << randomBitIndex);
        }

        double delay = messages[index].delay ? par("delay").doubleValue() : 0.0;
        nmsg->setPayload(messages[index].content.c_str());
        messageHeader header;
        header.messageId = index;
        header.sendingTime = simTime().dbl();
        nmsg->setHeader(header);

        if (messages[index].duplicated)
        {
            countTransmissions++;
            sendDelayed(nmsg->dup(), 0.01 + delay + seq * 0.5, "out");
        }

        // normal send
        sendDelayed(nmsg->dup(), delay + seq * 0.5, "out");
        scheduleAt(simTime() + par("timer").doubleValue() + seq * 0.5, nmsg->dup());
        index++;
        std::cout << " - " << getName() << "[" << getIndex() << "]" << " sends message with id = " << nmsg->getHeader().messageId
                << " and content = " << nmsg->getPayload() << " at t = " << simTime().dbl() + delay << "s" << (messages[index].modification ? " with modification\n" : "\n");
    }
    else
    {
        scheduleAt(simTime() + par("timer").doubleValue() , nmsg->dup());
    }
}

void Node::sendMessage(std::string messageName)
{
    std::ofstream log;
    log.open(outputPath, std::ios::app);

    if (index == messages.size())
    {
        double totalTransmissionTime = simTime().dbl();
        std::cout << " ----------------------------------------\n";
        std::cout << " - " << getName() << "[" << getIndex() << "]" << " end of input file\n";
        std::cout << " - " << "total transmission time = " << totalTransmissionTime << "\n";
        std::cout << " - " << "total number of transmissions = " << countTransmissions << "\n";
        std::cout << " - " << "the network throughput = " << (double)correctTransmissions / totalTransmissionTime << "\n";
        log << " ----------------------------------------\n";
        log << " - " << getName() << "[" << getIndex() << "]" << " end of input file\n";
        log << " - " << "total transmission time = " << totalTransmissionTime << "\n";
        log << " - " << "total number of transmissions = " << countTransmissions << "\n";
        log << " - " << "the network throughput = " << (double)correctTransmissions / totalTransmissionTime << "\n";
        return;
    }

    countTransmissions++;
    if (!messages[index].modification && !messages[index].loss)
        correctTransmissions++;

    NodeMessage_Base *nmsg = new NodeMessage_Base(messageName.c_str());
    nmsg->setTrailer(CRC(messages[index].content, GENERATOR));
    if (!messages[index].loss)
    {
        // modify bit block
        if (messages[index].modification)
        {
            int randomCharacterIndex = uniform(0, messages[index].content.size());
            int randomBitIndex = uniform(0, 7);
            messages[index].content[randomCharacterIndex] = messages[index].content[randomCharacterIndex] ^ (1 << randomBitIndex);
        }

        double delay = messages[index].delay ? par("delay").doubleValue() : 0.0;
        nmsg->setPayload(messages[index].content.c_str());
        messageHeader header;
        header.messageId = index;
        header.sendingTime = simTime().dbl();
        nmsg->setHeader(header);

        if (messages[index].duplicated)
        {
            countTransmissions++;
            sendDelayed(nmsg->dup(), 0.01 + delay, "out");
        }

        // normal send
        sendDelayed(nmsg->dup(), delay, "out");

        std::cout << " - " << getName() << "[" << getIndex() << "]" << " sends message with id = " << nmsg->getHeader().messageId
                << " and content = " << nmsg->getPayload() << " at t = " << simTime().dbl() + delay << "s" << (messages[index].modification ? " with modification\n" : "\n");
        log << " - " << getName() << "[" << getIndex() << "]" << " sends message with id = " << nmsg->getHeader().messageId
                << " and content = " << nmsg->getPayload() << " at t = " << simTime().dbl() + delay << "s" << (messages[index].modification ? " with modification\n" : "\n");
    }
    else
    {
        scheduleAt(simTime() + par("timer").doubleValue() , new cMessage(""));
    }

    log.close();
    cancelAndDelete(nmsg);
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
        messages[i].content.insert(0, "$");
        messages[i].content.insert(messages[i].content.size(), "$");
    }
}

void Node::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if (strcmp(msg->getName(), "coordinator") == 0)
    {
        CoordinatorMessage_Base *cmsg = check_and_cast<CoordinatorMessage_Base*>(msg);
        char path[] = "../inputs/";
        isStarter = cmsg->getIsStart();
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
        acks = std::vector<bool>(messages.size(), false);
        sent = std::vector<bool>(messages.size(), false);
        byteStuffing();

        if(isStarter)
        {

            std::ofstream myFile(outputPath);
            myFile.close();
            for (int i = 0; i < par("windowSize").intValue(); ++i)
                sendSR(msg->getName(), i, false, i);
        }

        cancelAndDelete(msg);
        return;
    }

    if (!msg->isSelfMessage())
    {

        std::ofstream log;
        log.open(outputPath, std::ios::app);

        NodeMessage_Base *receivedMsg = check_and_cast<NodeMessage_Base*>(msg);
        if (receivedMsg->getPiggybackingId() != 0)
        {
            if (receivedMsg->getPiggybacking() == ACK)
                acks[receivedMsg->getPiggybackingId() - 1] = ACK;
        }

        /*
        if (index == messages.size() && receivedMsg->getHeader().messageId == -1)
                    return;
        */

        std::string crc = std::string(receivedMsg->getPayload()) + receivedMsg->getTrailer();
        bool error = ((int)CRC(crc, GENERATOR) != 0);
        sendSR(getName(), receivedMsg->getPiggybackingId(), error);
        std::cout << " - " << getName() << "[" << getIndex() << "]" << " received message with id = " << receivedMsg->getHeader().messageId
                            << " and content = " << receivedMsg->getPayload() << " at t = " << simTime().dbl() << "s" << (error ? " with modification, " : " ")
                            << "and piggybacking " << (error ? "NACK" : "ACK") << " number " << (error ? NACK : ACK) << "\n";
        log.close();
    }

    if (msg->isSelfMessage())
    {
        NodeMessage_Base *receivedMsg = check_and_cast<NodeMessage_Base*>(msg);

        if (!acks[receivedMsg->getPiggybackingId()])
        {
            sendDelayed(msg->dup(), 0, "out");
            scheduleAt(simTime() + par("timer").doubleValue() , msg->dup());
        }
    }
    /*
    if (strcmp(msg->getName(), "starter") == 0)
    {
        std::ofstream log;
        log.open(outputPath, std::ios::app);

        NodeMessage_Base *receivedMsg = check_and_cast<NodeMessage_Base*>(msg);
        // Error detection and/or correction happen here.
        if (receivedMsg->getHeader().messageId < sequenceNumber)
        {
            std::cout << " - " << getName() << "[" << getIndex() << "]" << " drops message with id = " << receivedMsg->getHeader().messageId << "\n";
            log << " - " << getName() << "[" << getIndex() << "]" << " drops message with id = " << receivedMsg->getHeader().messageId << "\n";
        }
        else
        {
            std::string crc = std::string(receivedMsg->getPayload()) + receivedMsg->getTrailer();
            bool error = ((int)CRC(crc, GENERATOR) != 0);
            std::cout << " - " << getName() << "[" << getIndex() << "]" << " received message with id = " << receivedMsg->getHeader().messageId
                                << " and content = " << receivedMsg->getPayload() << " at t = " << simTime().dbl() << "s" << (error ? " with modification, " : " ")
                                << "and piggybacking " << (error ? "NACK" : "ACK") << " number " << (error ? NACK : ACK) << "\n";
            log << " - " << getName() << "[" << getIndex() << "]" << " received message with id = " << receivedMsg->getHeader().messageId
                    << " and content = " << receivedMsg->getPayload() << " at t = " << simTime().dbl() << "s" << (error ? " with modification, " : " ")
                    << "and piggybacking " << (error ? "NACK" : "ACK") << " number " << (error ? NACK : ACK) << "\n";

            NodeMessage_Base *nmsg = new NodeMessage_Base(error ? "NACK" : "ACK");
            nmsg->setPiggybacking(error ? NACK : ACK);
            nmsg->setPiggybackingId(error ? NACK : ACK);
            nmsg->setPayload(error ? "NACK" : "ACK");
            messageHeader header;
            header.messageId = index;
            header.sendingTime = simTime().dbl();
            nmsg->setHeader(header);
            nmsg->setTrailer(CRC(error ? "NACK" : "ACK", GENERATOR));
            sendDelayed(nmsg, 0.2, "out");
            index++;
            sequenceNumber++;
        }
        log.close();
    }
    else
    {
        if (msg->isSelfMessage())
        {
            std::ofstream log;
            log.open(outputPath, std::ios::app);
            std::cout << " - " << getName() << "[" << getIndex() << "]" << " timeout for message id = " << index << " at t = " << simTime().dbl() << "s\n";
            log << " - " << getName() << "[" << getIndex() << "]" << " timeout for message id = " << index << " at t = " << simTime().dbl() << "s\n";
            log.close();
        }
        NodeMessage_Base *receivedMsg = check_and_cast<NodeMessage_Base*>(msg);
        sendMessage("starter");
    }
    */
    cancelAndDelete(msg);

}



