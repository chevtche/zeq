
/* Copyright (c) 2014-2015, Human Brain Project
 *                          Stefan.Eilemann@epfl.ch
 */

#ifndef ZEROEQ_RECEIVER_H
#define ZEROEQ_RECEIVER_H

#include <zeroeq/api.h>
#include <zeroeq/types.h>

#include <memory>

#include <vector>
#include <string>

namespace zeroeq
{
namespace detail { class Receiver; }
namespace connection { namespace detail{ class Broker; }}

/**
 * Base class for entities receiving data.
 *
 * Provides a receive() method, which demultiplexes data between multiple inputs
 * of multiple instances of receivers. Receivers form a shared group by linking
 * them at construction time.
 *
 * Not intended to be as a final class. Not thread safe.
 *
 * Example: @include tests/receiver.cpp
 */
class Receiver
{
public:
    /** Create a new standalone receiver. */
    ZEROEQ_API Receiver();

    /**
     * Create a shared receiver.
     *
     * All receivers sharing a group may receive data when receive() is called
     * on any of them.
     *
     * @param shared another receiver to form a simultaneous receive group with.
     */
    ZEROEQ_API explicit Receiver( Receiver& shared );

    /** Destroy this receiver. */
    ZEROEQ_API virtual ~Receiver();

    /**
     * Receive at least one event from all shared receivers.
     *
     * Using receive( 0 ) is equivalent to polling the receivers for data.
     *
     * @param timeout timeout in ms for poll, default blocking poll until at
     *                least one event is received
     * @return true if at least one event was received
     * @throw std::runtime_error when polling failed.
     */
    ZEROEQ_API bool receive( const uint32_t timeout = TIMEOUT_INDEFINITE );

protected:
    friend class detail::Receiver;

    /** Add this receiver's sockets to the given list */
    virtual void addSockets( std::vector< detail::Socket >& entries ) = 0;

    /**
     * Process data on a signalled socket.
     *
     * @param socket the socket provided from addSockets().
     */
    virtual void process( detail::Socket& socket ) = 0;

    /**
     * Update the internal connection list.
     *
     * Called on all members of a shared group regularly by receive() to update
     * their list of sockets.
     */
    virtual void update() {}

    /**
     * Add the given connection to the list of receiving sockets.
     *
     * @param uri the ZeroMQ address to connect to.
     */
    virtual void addConnection( const std::string& uri ) = 0;
    friend class connection::detail::Broker;

    void* getZMQContext(); //!< @internal returns the ZeroMQ context

private:
    Receiver& operator=( const Receiver& ) = delete;

    std::shared_ptr< detail::Receiver > const _impl;
};

}

#endif
