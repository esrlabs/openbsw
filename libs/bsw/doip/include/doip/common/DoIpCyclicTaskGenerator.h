// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include <async/Types.h>

#include <estd/functional.h>

namespace doip
{
/**
 * Class the implements a simple cyclic task generator.
 */
class DoIpCyclicTaskGenerator : private ::async::RunnableType
{
public:
    using CyclicTaskType = ::estd::function<void()>;

    /**
     * Constructor.
     * \param cyclicTask function that will be called periodically
     * \param context asynchronous execution context
     * \param cyclicTaskDelta task delta (in ms) of cyclic tasks
     */
    DoIpCyclicTaskGenerator(
        CyclicTaskType cyclicTask, ::async::ContextType context, uint16_t cyclicTaskDelta);

    /**
     * Start calling the cyclic task periodically.
     */
    void start();

    /**
     * Stop calling the cyclic tasks.
     */
    void shutdown();

private:
    void execute() override;

    CyclicTaskType _cyclicTask;
    ::async::TimeoutType _runTimeout;
    uint16_t _cyclicTaskDelta;
    ::async::ContextType _context;
};

} // namespace doip
