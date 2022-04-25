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
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <bitset>

#define GENERATOR 0b100000111

#define inc(X) X = (X + 1) % maxSequence

Define_Module(Node);

void Node::initialize()
{
    errorCheckingAlgorithm = par("errorCheckingAlgorithm").intValue();
    windowSize = par("windowSize").intValue();
    maxSequence = 2 * windowSize + 1;
    windowStart = 0;
    expectedFrame = 0;
    allSent = false;
    messageStatus = std::vector<bool>(maxSequence, true);
}

void Node::sendWindow(bool error)
{
    int counter = windowStart;
    for (int i = 0; i < windowSize; i++)
    {
        if (messageStatus[counter])
        {
            countTransmissions++;
            NodeMessage_Base* nmsg = new NodeMessage_Base(getName());
            messageHeader header;
            header.sendingTime = simTime().dbl();
            header.messageId = counter;

            Message message = messagesToSend[counter];
            double delay = message.delay ? par("delay").doubleValue() : 0.0;
            nmsg->setHeader(header);
            nmsg->setPiggybackingId(expectedFrame);
            nmsg->setPiggybacking(error ? NACK : ACK);

            if(errorCheckingAlgorithm == 0)
                nmsg->setTrailer(CRC(message.content, GENERATOR));
            else
                nmsg->setHammingParity(getHammingParity(message.content).c_str());

            if (message.modification)
            {
                int randomCharacterIndex = uniform(0, message.content.size());
                int randomBitIndex = uniform(0, 7);
                message.content[randomCharacterIndex] = message.content[randomCharacterIndex] ^ (1 << randomBitIndex);
            }
            nmsg->setPayload(message.content.c_str());

            if (messagesToSend[counter].loss)
            {
                std::cout << " - " << getName() << "[" << getIndex() << "]" << " LOSS message with id = " << nmsg->getHeader().messageId
                        << " and content = " << nmsg->getPayload() << " at t = " << simTime().dbl() + delay + i * 0.05 << "s" << (message.modification ? " with modification\n" : "")
                        << " and piggybacking: " << (nmsg->getPiggybacking() ? "ACK" : "NACK") << " number " << nmsg->getPiggybackingId() << "\n";

                EV << " - " << getName() << "[" << getIndex() << "]" << " LOSS message with id = " << nmsg->getHeader().messageId
                        << " and content = " << nmsg->getPayload() << " at t = " << simTime().dbl() + delay + i * 0.05 << "s" << (message.modification ? " with modification\n" : "")
                        << " and piggybacking: " << (nmsg->getPiggybacking() ? "ACK" : "NACK") << " number " << nmsg->getPiggybackingId() << "\n";

                std::ofstream myFile(outputPath, std::ios_base::app);
                myFile << " - " << getName() << "[" << getIndex() << "]" << " LOSS message with id = " << nmsg->getHeader().messageId
                        << " and content = " << nmsg->getPayload() << " at t = " << simTime().dbl() + delay + i * 0.05 << "s" << (message.modification ? " with modification\n" : "")
                        << " and piggybacking: " << (nmsg->getPiggybacking() ? "ACK" : "NACK") << " number " << nmsg->getPiggybackingId() << "\n";
                myFile.close();
            }


            sendDelayed(nmsg->dup(), delay + i * 0.05, "out");
            scheduleAt(simTime() + delay + par("timer").doubleValue() + 0.05 * i, nmsg->dup());
            messageStatus[counter] = false;

            std::cout << " - " << getName() << "[" << getIndex() << "]" << " sends " << (delay > 0.0 ? "DELAYED" : "") << " message with id = " << nmsg->getHeader().messageId
                    << " and content = " << nmsg->getPayload() << " at t = " << simTime().dbl() + delay + i * 0.05 << "s" << (message.modification ? " with modification\n" : "")
                    << " and piggybacking: " << (nmsg->getPiggybacking() ? "ACK" : "NACK") << " number " << nmsg->getPiggybackingId() << "\n";

            EV << " - " << getName() << "[" << getIndex() << "]" << " sends " << (delay > 0.0 ? "DELAYED" : "") << " message with id = " << nmsg->getHeader().messageId
                                << " and content = " << nmsg->getPayload() << " at t = " << simTime().dbl() + delay + i * 0.05 << "s" << (message.modification ? " with modification\n" : "")
                                << " and piggybacking: " << (nmsg->getPiggybacking() ? "ACK" : "NACK") << " number " << nmsg->getPiggybackingId() << "\n";

            std::ofstream myFile(outputPath, std::ios_base::app);
            myFile <<" - " << getName() << "[" << getIndex() << "]" << " sends message with id = " << nmsg->getHeader().messageId
                    << " and content = " << nmsg->getPayload() << " at t = " << simTime().dbl() + 0.01 + delay + i * 0.05 << "s" << (message.modification ? " with modification\n" : "")
                    << " and piggybacking: " << (nmsg->getPiggybacking() ? "ACK" : "NACK") << " number " << nmsg->getPiggybackingId() << "\n";
            myFile.close();

            if (message.duplicated)
            {
                countTransmissions++;
                sendDelayed(nmsg->dup(), 0.01 + delay + i * 0.05, "out");

                std::cout << " - " << getName() << "[" << getIndex() << "]" << " sends DUPLICATE message with id = " << nmsg->getHeader().messageId
                        << " and content = " << nmsg->getPayload() << " at t = " << simTime().dbl() + 0.01 + delay + i * 0.05 << "s" << (message.modification ? " with modification\n" : "")
                        << " and piggybacking: " << (nmsg->getPiggybacking() ? "ACK" : "NACK") << " number " << nmsg->getPiggybackingId() << "\n";

                EV << " - " << getName() << "[" << getIndex() << "]" << " sends DUPLICATE message with id = " << nmsg->getHeader().messageId
                                    << " and content = " << nmsg->getPayload() << " at t = " << simTime().dbl() + 0.01 + delay + i * 0.05 << "s" << (message.modification ? " with modification\n" : "")
                                    << " and piggybacking: " << (nmsg->getPiggybacking() ? "ACK" : "NACK") << " number " << nmsg->getPiggybackingId() << "\n";

                std::ofstream myFile(outputPath, std::ios_base::app);
                myFile <<" - " << getName() << "[" << getIndex() << "]" << " sends DUPLICATE message with id = " << nmsg->getHeader().messageId
                        << " and content = " << nmsg->getPayload() << " at t = " << simTime().dbl() + 0.01 + delay + i * 0.05 << "s" << (message.modification ? " with modification\n" : "")
                        << " and piggybacking: " << (nmsg->getPiggybacking() ? "ACK" : "NACK") << " number " << nmsg->getPiggybackingId() << "\n";
                myFile.close();

            }
        }
        inc(counter);
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
                message += (inputStrings[i++] + " ");

                if (i == inputStrings.size())
                    break;
            }
            message.pop_back();
            allMessages.push(Message(bits, message));
            if (i != inputStrings.size())
                bits = inputStrings[i];
        }
        stuffBytes();

        messagesToSend = std::vector<Message>(maxSequence);
        for (int i = 0; i < maxSequence; ++i)
        {
            messagesToSend[i] = allMessages.front();
            allMessages.pop();
        }

        if (getIndex() < 2)
            outputPath = "../outputs/pair01.txt";
        else if (getIndex() < 4)
            outputPath = "../outputs/pair23.txt";
        else
            outputPath = "../outputs/pair45.txt";

        if(isStarter)
        {
            std::ofstream myFile(outputPath);
            myFile.close();
            // send window size
            sendWindow(false);
        }

        return;
    }

    // Case 1: Message timeout
    if (msg->isSelfMessage())
    {
        NodeMessage_Base* received = check_and_cast<NodeMessage_Base*>(msg);

        if(!messageStatus[received->getHeader().messageId])
            return;

        sendWindow(false);

        return;
    }
    // Case 2: Normal Message to receive
    else
    {
        NodeMessage_Base* received = check_and_cast<NodeMessage_Base*>(msg);
        bool error = false;

        bool hasMessage = received->getHeader().messageId != -1;
        std::string message = received->getPayload();
        // check if the message is corrupted
        if (errorCheckingAlgorithm == 0)
        {
            std::string crc = std::string(received->getPayload()) + received->getTrailer();
            error = ((int)CRC(crc, GENERATOR) != 0);
        }
        else
        {
            std::string hammingParity = getHammingParity(message);
            int errorLocation = 0;
            int p = hammingParity.size() - 1;
            for (int i = 0; i < hammingParity.size() / 2; i++)
                std::swap(hammingParity[i], hammingParity[hammingParity.size() - i - 1]);

            std::string tempHamming = std::string(received->getHammingParity());
            for (int i = 0; i < tempHamming.size() / 2; i++)
                std::swap(tempHamming[i], tempHamming[tempHamming.size() - i - 1]);

            for (int i = 0; i < hammingParity.size(); i++)
            {
                tempHamming[i] = hammingParity[i] == tempHamming[i] ? '0' : '1';
            }
            // code word in tempHamming

            for (int i = 0; i < tempHamming.size(); ++i)
            {
                errorLocation += tempHamming[i] == '1' ? pow(2, p) : 0;
                p--;
            }

            int modifiedChar = errorLocation / 8;
            int modifiedBit = errorLocation % 8;
            if (errorLocation != 0)
            {
                std::string oldMessage = message;
                message[modifiedChar - 1] ^= (1 << modifiedBit);

                std::cout << " - " << getName() << "[" << getIndex() << "]" << " HAMMING CHECK FOUND ERROR received message with id = " << received->getHeader().messageId
                               << " and FAULTY content = " << oldMessage << " But corrected it to " << message
                               << " error at bit " << modifiedBit << "in character " << modifiedChar - 1 << " at t = " << simTime().dbl()
                               << "and piggybacking " << (received->getPiggybacking() ? "ACK" : "NACK") << " number " << received->getPiggybackingId() << "\n";
                EV << " - " << getName() << "[" << getIndex() << "]" << " HAMMING CHECK FOUND ERROR received message with id = " << received->getHeader().messageId
                        << " and FAULTY content = " << oldMessage << " But corrected it to " << message
                        << " error at bit " << modifiedBit << "in character " << modifiedChar - 1 << " at t = " << simTime().dbl()
                        << "and piggybacking " << (received->getPiggybacking() ? "ACK" : "NACK") << " number " << received->getPiggybackingId() << "\n";
                std::ofstream myFile(outputPath, std::ios_base::app);
                myFile << " - " << getName() << "[" << getIndex() << "]" << " HAMMING CHECK FOUND ERROR received message with id = " << received->getHeader().messageId
                            << " and FAULTY content = " << oldMessage << " But corrected it to " << message
                            << " error at bit " << modifiedBit << "in character " << modifiedChar - 1 << " at t = " << simTime().dbl()
                            << "and piggybacking " << (received->getPiggybacking() ? "ACK" : "NACK") << " number " << received->getPiggybackingId() << "\n";
                myFile.close();
            }
        }

        if (!error && hasMessage)
        {
            inc(expectedFrame);
        }
        destuffBytes(message);
        std::cout << " - " << getName() << "[" << getIndex() << "]" << " received message with id = " << received->getHeader().messageId
                << " and content = " << message << " at t = " << simTime().dbl() << "s" << (error ? " with modification, " : " ")
                << "and piggybacking " << (received->getPiggybacking() ? "ACK" : "NACK") << " number " << received->getPiggybackingId() << "\n";

        EV << " - " << getName() << "[" << getIndex() << "]" << " received message with id = " << received->getHeader().messageId
                << " and content = " << message << " at t = " << simTime().dbl() << "s" << (error ? " with modification, " : " ")
                << "and piggybacking " << (received->getPiggybacking() ? "ACK" : "NACK") << " number " << received->getPiggybackingId() << "\n";

        std::ofstream myFile(outputPath, std::ios_base::app);
        myFile << " - " << getName() << "[" << getIndex() << "]" << " received message with id = " << received->getHeader().messageId
                << " and content = " << message << " at t = " << simTime().dbl() << "s" << (error ? " with modification, " : " ")
                << "and piggybacking " << (received->getPiggybacking() ? "ACK" : "NACK") << " number " << received->getPiggybackingId() << "\n";
        myFile.close();

        if (hasMessage)
        {
            if (receivedStrings[message] > 0)
            {
                std::cout << " - " << getName() << "[" << getIndex() << "]" << " dropped duplicate message with id = " << received->getHeader().messageId
                        << " and content = " << message << " at t = " << simTime().dbl() << "\n";

                EV << " - " << getName() << "[" << getIndex() << "]" << " dropped duplicate message with id = " << received->getHeader().messageId
                        << " and content = " << message << " at t = " << simTime().dbl() << "\n";

                std::ofstream myFile(outputPath, std::ios_base::app);
                myFile << " - " << getName() << "[" << getIndex() << "]" << " dropped duplicate message with id = " << received->getHeader().messageId
                        << " and content = " << message << " at t = " << simTime().dbl() << "\n";
                myFile.close();

                return;
            }
            else
                receivedStrings[message]++;
        }

        int seq = received->getPiggybackingId();
        if (received->getPiggybacking() == ACK)
        {
            while (windowStart != seq)
            {
                if (allMessages.size() > 0)
                {
                    messagesToSend[windowStart] = allMessages.front();
                    messageStatus[windowStart] = true;
                    allMessages.pop();
                }
                correctTransmissions++;
                inc(windowStart);
            }
        }
        // if piggyback is NACK, set message status to true so the window sending function re-sends
        else
        {
            bool error = false;
            if(par("errorCheck").intValue() == 0)
            {
                std::string crc = std::string(receivedMsg->getPayload()) + receivedMsg->getTrailer();
                error = ((int)CRC(crc, GENERATOR) != 0);
            }
            else
            {
                // do hamming
            }
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
            messageStatus[seq] = true;
            messagesToSend[seq].delay = 0;
            messagesToSend[seq].duplicated = 0;
            messagesToSend[seq].modification = 0;
            messagesToSend[seq].loss = 0;
        }

        if (allMessages.size() == 0)
        {
            bool check = true;
            if (!allSent)
            {
                for (int i = 0; i < windowSize; ++i)
                {
                    if (messageStatus[i])
                    {
                        check = false;
                        break;
                    }
                }
            }

            if (check)
            {
                allSent = true;
                for(int i = 0; i < messageStatus.size(); ++i)
                    messageStatus[i] = false;
                if (!hasMessage)
                {
                    std::ofstream myFile;
                    myFile.open(outputPath, std::ios::app);
                    double totalTransmissionTime = simTime().dbl();
                    std::cout << " ----------------------------------------\n";
                    std::cout << " - " << getName() << "[" << getIndex() << "]" << " end of input file\n";
                    std::cout << " - " << "total transmission time = " << totalTransmissionTime << "\n";
                    std::cout << " - " << "total number of transmissions = " << countTransmissions << "\n";
                    std::cout << " - " << "the network throughput = " << (double)correctTransmissions / totalTransmissionTime << "\n";
                    myFile << " ----------------------------------------\n";
                    myFile << " - " << getName() << "[" << getIndex() << "]" << " end of input file\n";
                    myFile << " - " << "total transmission time = " << totalTransmissionTime << "\n";
                    myFile << " - " << "total number of transmissions = " << countTransmissions << "\n";
                    myFile << " - " << "the network throughput = " << (double)correctTransmissions / totalTransmissionTime << "\n";
                    myFile.close();
                    return;
                }


                NodeMessage_Base* nmsg = new NodeMessage_Base("NOPAYLOAD");
                messageHeader header;
                header.sendingTime = simTime().dbl();
                header.messageId = -1;
                nmsg->setPayload("$NOPAYLOAD$");
                nmsg->setPiggybacking(error ? NACK : ACK);
                nmsg->setPiggybackingId(expectedFrame);
                nmsg->setHeader(header);
                send(nmsg, "out");
            }
        }

        if (!allSent)
            sendWindow(error);

        return;
    }
}

std::string Node::getHammingParity(std::string message)
{
    std::string codeMessage = "";
   for (int i = 0; i < message.length(); i++)
   {
       std::bitset<8> bits(message[i]);
       codeMessage += bits.to_string();
   }
   //1000001
   //codeMessage = "1101001100110101";
   int r = 0;
   int m = codeMessage.size(); // number of bits in the mesaage
   int i = 0;
   while (pow(2, i) <= m + 1)
   {
       i++;
   }
   r = i;
   codeMessage.insert(0, "9");
   //cout << codeMessage << " " << r;
   // We now need to insert hamming bits at there correct posistions
   // 1000001 -> 0010001001
   std::vector<int> hammingBitsPositions;
   std::vector<bool> hammingBits(r, 0);
   for (int i = 0; i < r ;i++)
   {
       int x = pow(2, i);
       codeMessage.insert(x, "0");
       hammingBitsPositions.push_back(x);

   }
   for (int i = 0; i < hammingBits.size(); i++)
   {
       int stride = pow(2, i);
       int lastIndex = -1;
       //initial elements
       for (int j = stride; j < stride * 2; j++)
       {
           if (j < codeMessage.size())
           {
               hammingBits[i] = hammingBits[i] ^ (codeMessage[j] == '1' ? 1 : 0);
               lastIndex = j + 1;
           }
       }
       //alternating part
       lastIndex += stride;
       // if stride == 1
       if (stride == 1)
       {
           for (int k = lastIndex; k < codeMessage.size(); k++)
           {
               if (k % 2 != 0)
               {
                   hammingBits[i] = hammingBits[i] ^ (codeMessage[k] == '1' ? 1 : 0);
               }
           }
       }
       else
       {
           int flag = lastIndex;
           while (flag < codeMessage.size())
           {
               int x = lastIndex;
               int y = lastIndex + stride;
               for (int k = x; k < y; k++)
               {
                   if (k < codeMessage.size())
                   {
                       hammingBits[i] = hammingBits[i] ^ (codeMessage[k] == '1' ? 1 : 0);
                       lastIndex = k + 1;
                   }
               }
               lastIndex += stride;
               flag += stride;
           }
       }
   }
   //insert into message
   for (int i = 0; i < hammingBits.size(); i++)
   {
       codeMessage[hammingBitsPositions[i]] =  (hammingBits[i]? '1': '0');
   }
   codeMessage.erase(0, 1);
   std::string hammingBitsString = "";
   for (int i = 0; i < hammingBits.size(); i++)
       hammingBitsString += (hammingBits[i] ? '1' : '0');

   return hammingBitsString;
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

void Node::stuffBytes()
{
    for(int i = 0; i < allMessages.size(); ++i)
    {
        auto message = allMessages.front();
        allMessages.pop();
        for(int j = 0; j < message.content.size(); ++j)
        {
            if(message.content[j] == '$' || message.content[j] == '/')
            {
                message.content.insert(j++, "/");
            }
        }
        message.content.insert(0, "$");
        message.content.insert(message.content.size(), "$");
        allMessages.push(message);
    }
}

void Node::destuffBytes(std::string& message)
{
    // remove the first and last character of the first and last character of the message
    message.erase(0, 1);
    message.erase(message.size() - 1, 1);
    for(int i = 0; i < message.size() - 1; i++)
    {
        if(message[i] == '/' && (message[i + 1] == '/' || message[i + 1] == '$'))
        {
            message.erase(i--, 1);
        }
    }
}
