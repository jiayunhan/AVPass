/**
 * @file LogfileQueueReaderExample.hpp
 *
 * PUBLIC_HEADER
 */

#ifndef LOGFILEQUEUEREADERNODE_HPP
#define LOGFILEQUEUEREADERNODE_HPP


#include <PolySyncNode.hpp>


// Forward declaration
namespace polysync{
    class Logfile;
}


/**
 * @brief The LogfileQueueReaderNode class
 *
 * @li Implement an interface to polysync::Node
 * @li Open PolySync *.plog file using the PolySync C++ Logfile API.
 * @li Enable the Logfile object's message queue to receive data that has been
 * extracted from the logfile.
 */
class LogfileQueueReaderNode : public polysync::Node
{

public:

    /**
     * @brief Destructor
     *
     * @li Call polysync::Node::Node() -- Base class constructor
     */
    LogfileQueueReaderNode();

    /**
     * @brief Default Desctructor -- Does nothing
     *
     * @note Dynamically allocated resources are deleted in @ref
     * releaseStateEvent();
     */
    virtual ~LogfileQueueReaderNode() = default;

protected:

    /**
     * @li Set path to *.plog file
     * @li Enable message queue
     * @li Set Logfile mode to read
     * @li Set Logfile state to enabled
     */
    virtual void prepareLogfileToRead();

    /**
     * @li Dequeue PolySync message
     * @li Validate dequeued message
     * @li Increment message count
     * @li Print message to std out
     */
    virtual void readDequeuedMessage();

    /**
     * @li Output number of messages processed
     * @li Notify user that end of file was reached
     */
    virtual void printResults();

    /**
     * @note Called once after polysync::Node::connectPolySync() is called
     *
     * @li Allocate Logfile object
     * @li Call @ref prepareLogfileToRead()
     * @li If something went wrong, @ref polysync::Node::disconnectPolySync()
     */
    virtual void initStateEvent() override;

    /**
     * @note Called repeatedly during runtime
     *
     * @li Pop message from the queue
     * @li Print message data to std out
     * @li if end of logfile reached, @ref polysync::Node::disconnectPolySync()
     */
    virtual void okStateEvent() override;

    /**
     * @note Called once, after polysync::Node::disconnectPolySync()
     *
     * @li call @ref printResults()
     * @li Set Logfile mode to off
     * @li delete dynamically allocated memory
     */
    virtual void releaseStateEvent() override;


protected:

    // Compile-time constants
    // Default path assumes that the C or C++ Log Writer has been used.
    static constexpr auto DefaultLogfilePath = "/tmp/polysync_logfile.plog";

    static constexpr ps_timestamp FiveSeconds = 5000000;

    static constexpr ps_timestamp DefaultSessionId = 1;

    static constexpr ps_timestamp DefaultTimingOffset = 1;

    static constexpr uint Enable = 1;


protected:

    polysync::Logfile * _logFile;

    int _numMessagesRead;

}; // END LogFileTestNode


#endif // LOGFILEQUEUEREADERNODE_HPP
