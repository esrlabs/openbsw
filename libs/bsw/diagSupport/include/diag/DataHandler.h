// Copyright 2025 Accenture.

#ifndef GUARD_D10F7A84_08F8_4982_A6AC_7D3D5C274935
#define GUARD_D10F7A84_08F8_4982_A6AC_7D3D5C274935

#include "diag/IDataHandler.h"

#include <estd/forward_list.h>
#include <estd/object_pool.h>
#include <estd/slice.h>

namespace diag
{

template<typename R>
class DataHandler
: public IDataHandler<R>
, private ::async::RunnableType
{
public:
    using RequestType        = R;
    using HandleFunctionType = bool (*)(void*, RequestType&);

    struct IdInfo
    {
        HandleFunctionType handle;
        uint16_t dataId;
        uint8_t contextIdx;
        bool isAlwaysActive;
    };

    struct ComponentInfo
    {
        IdInfo const* firstIdInfo;
        size_t idInfoCount;
    };

    class ComponentHandler : public ::estd::forward_list_node<ComponentHandler>
    {
    public:
        ComponentHandler(ComponentInfo const& componentInfo)
        : _componentInfo(&componentInfo), _component(nullptr), _isActive(false)
        {}

        void initComponent(void* component) { _component = component; }

        void setActive(bool isActive) { _isActive = isActive; }

    private:
        friend class DataHandler;

        ComponentInfo const* _componentInfo;
        void* _component;
        bool _isActive;
    };

protected:
    struct Request : public ::estd::forward_list_node<Request>
    {
        Request(
            RequestType& request,
            ComponentHandler const& componentHandler,
            IdInfo const& idInfo,
            uint8_t priority)
        : _request(request)
        , _componentHandler(componentHandler)
        , _idInfo(idInfo)
        , _priority(priority)
        {}

        RequestType& _request;
        ComponentHandler const& _componentHandler;
        IdInfo const& _idInfo;
        uint8_t _priority;
    };

public:
    DataHandler(
        ::estd::slice<::async::ContextType const> const& contexts,
        ::estd::object_pool<Request>& requestPool)
    : _requestPool(requestPool)
    , _contexts(contexts)
    , _nextRequest(nullptr)
    , _nextRequestIt(_pendingRequests.end())
    {}

    void addComponentHandler(ComponentHandler& handler)
    {
        ::async::LockType lock;
        _componentHandlers.push_front(handler);
    }

    typename IDataHandler<R>::Result handleRequest(RequestType& request, uint8_t priority) override
    {
        ComponentHandler const* componentHandler = nullptr;
        IdInfo const* idInfo                     = lookupIdInfo(request.dataId(), componentHandler);
        if (idInfo == nullptr)
        {
            return IDataHandler<R>::Result::UNKNOWN_ID;
        }
        ::async::ModifiableLockType lock;
        if (_requestPool.empty())
        {
            return IDataHandler<R>::Result::BUSY;
        }
        Request& req
            = _requestPool.allocate().construct(request, *componentHandler, *idInfo, priority);
        typename ::estd::forward_list<Request>::iterator prevIt = _pendingRequests.before_begin();
        for (typename ::estd::forward_list<Request>::iterator it = _pendingRequests.begin();
             it != _pendingRequests.end();
             ++it)
        {
            if (priority > it->_priority)
            {
                break;
            }
            prevIt = it;
        }
        _pendingRequests.insert_after(prevIt, req);
        _nextRequestIt = _pendingRequests.begin();
        lock.unlock();
        handleNextRequest();
        return IDataHandler<R>::Result::OK;
    }

private:
    void execute() override
    {
        ::async::ModifiableLockType lock;
        if (_nextRequest == nullptr)
        {
            return;
        }
        Request& request = *_nextRequest;
        ++_nextRequestIt;
        _nextRequest = nullptr;
        lock.unlock();
        if (!request._idInfo.isAlwaysActive && !request._componentHandler._isActive)
        {
            request._request.sendResponse(RequestType::ResponseCode::INACTIVE);
        }
        else if (!request._idInfo.handle(request._componentHandler._component, request._request))
        {
            handleNextRequest();
            return;
        }
        lock.lock();
        if ((_nextRequestIt != _pendingRequests.end()) && (&(*_nextRequestIt) == &request))
        {
            ++_nextRequestIt;
        }
        _pendingRequests.remove(request);
        _requestPool.release(request);
        lock.unlock();
        handleNextRequest();
    }

    void handleNextRequest()
    {
        ::async::ModifiableLockType lock;
        if ((_nextRequest != nullptr) || (_nextRequestIt == _pendingRequests.end()))
        {
            return;
        }
        _nextRequest = &(*_nextRequestIt);
        lock.unlock();
        ::async::ContextType const context
            = _contexts[static_cast<size_t>(_nextRequest->_idInfo.contextIdx)];
        ::async::execute(context, *this);
    }

    IdInfo const* lookupIdInfo(uint16_t dataId, ComponentHandler const*& componentHandler) const
    {
        ::async::ModifiableLockType lock;
        typename ::estd::forward_list<ComponentHandler>::const_iterator it
            = _componentHandlers.begin();
        lock.unlock();
        while (it != _componentHandlers.end())
        {
            ::estd::slice<IdInfo const> idInfos = ::estd::slice<IdInfo const>::from_pointer(
                it->_componentInfo->firstIdInfo, it->_componentInfo->idInfoCount);
            for (auto& idInfo : idInfos)
            {
                if (idInfo.dataId == dataId)
                {
                    componentHandler = &(*it);
                    return &idInfo;
                }
            }
            ++it;
        }
        return nullptr;
    }

    ::estd::forward_list<ComponentHandler> _componentHandlers;
    ::estd::object_pool<Request>& _requestPool;
    ::estd::slice<::async::ContextType const> _contexts;
    ::estd::forward_list<Request> _pendingRequests;
    Request* _nextRequest;
    typename ::estd::forward_list<Request>::iterator _nextRequestIt;
};

namespace declare
{

template<typename R, size_t N>
class DataHandler : public ::diag::DataHandler<R>
{
public:
    static constexpr size_t REQUEST_POOL_SIZE = N;

    DataHandler(::estd::slice<::async::ContextType const> const& contexts)
    : ::diag::DataHandler<R>(contexts, _requestPool)
    {}

private:
    ::estd::declare::object_pool<typename ::diag::DataHandler<R>::Request, N> _requestPool;
};

} // namespace declare

} // namespace diag

#endif // GUARD_D10F7A84_08F8_4982_A6AC_7D3D5C274935
