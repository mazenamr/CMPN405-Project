//
// Generated file, do not edit! Created by nedtool 5.7 from NodeMessage.msg.
//

#ifndef __NODEMESSAGE_M_H
#define __NODEMESSAGE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0507
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
    typedef struct MessageHeader {
        int messageId;
        int sendingTime;
    } messageHeader;
// }}

// cplusplus {{
    typedef enum Control {
        NACK,
        ACK
    } control;
// }}

/**
 * Class generated from <tt>NodeMessage.msg:35</tt> by nedtool.
 * <pre>
 * packet NodeMessage
 * {
 *     \@customize(true);  // see the generated C++ header for more info
 *     messageHeader header;
 *     string payload;
 *     char trailer;
 *     control piggybacking;
 *     int piggybackingId;
 *     string hammingParity;
 * }
 * </pre>
 *
 * NodeMessage_Base is only useful if it gets subclassed, and NodeMessage is derived from it.
 * The minimum code to be written for NodeMessage is the following:
 *
 * <pre>
 * class NodeMessage : public NodeMessage_Base
 * {
 *   private:
 *     void copy(const NodeMessage& other) { ... }

 *   public:
 *     NodeMessage(const char *name=nullptr, short kind=0) : NodeMessage_Base(name,kind) {}
 *     NodeMessage(const NodeMessage& other) : NodeMessage_Base(other) {copy(other);}
 *     NodeMessage& operator=(const NodeMessage& other) {if (this==&other) return *this; NodeMessage_Base::operator=(other); copy(other); return *this;}
 *     virtual NodeMessage *dup() const override {return new NodeMessage(*this);}
 *     // ADD CODE HERE to redefine and implement pure virtual functions from NodeMessage_Base
 * };
 * </pre>
 *
 * The following should go into a .cc (.cpp) file:
 *
 * <pre>
 * Register_Class(NodeMessage)
 * </pre>
 */
class NodeMessage_Base : public ::omnetpp::cPacket
{
  protected:
    messageHeader header;
    ::omnetpp::opp_string payload;
    char trailer;
    control piggybacking;
    int piggybackingId;
    ::omnetpp::opp_string hammingParity;

  private:
    void copy(const NodeMessage_Base& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const NodeMessage_Base&);
    // make constructors protected to avoid instantiation

    // make assignment operator protected to force the user override it
    NodeMessage_Base& operator=(const NodeMessage_Base& other);

  public:
    virtual ~NodeMessage_Base();
    NodeMessage_Base(const char *name=nullptr, short kind=0);
    NodeMessage_Base(const NodeMessage_Base& other);
    virtual NodeMessage_Base *dup() const override {return new NodeMessage_Base(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual messageHeader& getHeader();
    virtual const messageHeader& getHeader() const {return const_cast<NodeMessage_Base*>(this)->getHeader();}
    virtual void setHeader(const messageHeader& header);
    virtual const char * getPayload() const;
    virtual void setPayload(const char * payload);
    virtual char getTrailer() const;
    virtual void setTrailer(char trailer);
    virtual control& getPiggybacking();
    virtual const control& getPiggybacking() const {return const_cast<NodeMessage_Base*>(this)->getPiggybacking();}
    virtual void setPiggybacking(const control& piggybacking);
    virtual int getPiggybackingId() const;
    virtual void setPiggybackingId(int piggybackingId);
    virtual const char * getHammingParity() const;
    virtual void setHammingParity(const char * hammingParity);
};


#endif // ifndef __NODEMESSAGE_M_H

