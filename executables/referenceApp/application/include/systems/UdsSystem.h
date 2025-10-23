// Copyright 2024 Accenture.

#pragma once

#include <async/Async.h>
#include <async/IRunnable.h>
#include <config/ConfigIds.h>
#include <diag/DataHandler.h>
#include <lifecycle/AsyncLifecycleComponent.h>
#include <uds/DiagDispatcher.h>
#include <uds/DummySessionPersistence.h>
#include <uds/ReadIdentifierPot.h>
#include <uds/UdsLifecycleConnector.h>
#include <uds/async/AsyncDiagHelper.h>
#include <uds/async/AsyncDiagJob.h>
// #include <uds/jobs/ReadIdentifierFromMemory.h>
#include <uds/services/communicationcontrol/CommunicationControl.h>
// #include <uds/services/readdata/ReadDataByIdentifier.h>
#include <uds/ReadDataById.h>
#include <uds/services/routinecontrol/RequestRoutineResults.h>
#include <uds/services/routinecontrol/RoutineControl.h>
#include <uds/services/routinecontrol/StartRoutine.h>
#include <uds/services/routinecontrol/StopRoutine.h>
#include <uds/services/sessioncontrol/DiagnosticSessionControl.h>
#include <uds/services/testerpresent/TesterPresent.h>
#include <uds/services/writedata/WriteDataByIdentifier.h>

namespace lifecycle
{
class LifecycleManager;
}

namespace config
{
class ITransportSystem;

class UdsSystem
: public ComponentBase<
      ScopeType,
      CtxId<Ctx::DIAG>,
      Types<
          Id<ITransportSystem>,
          Id<::lifecycle::ILifecycleManager>,
          typename ScopeType::ContextTableId>>
, private ::async::IRunnable
{
public:
    UdsSystem();

    void init();
    void start();
    void stop();

    static constexpr auto services()
    {
        return serviceAccessor<UdsSystem>()
            .member<ReadDataHandlerType, &UdsSystem::_readDataHandler>()
            .service<Id<::diag::DataHandler<::diag::IReadDataRequest>>>();
    }

    static constexpr auto diagnostics()
    {
        return all(
            diagAccessor<UdsSystem>().readData<&UdsSystem::readCf01>(0xcf01U),
            diagAccessor<UdsSystem>()
                .member<uds::ReadIdentifierPot, &UdsSystem::_read22Cf02>()
                .readData<uint32_t, &uds::ReadIdentifierPot::readAdcValue>(0xcf02U));
    }

private:
    using ReadDataHandlerType = ::diag::declare::DataHandler<::diag::IReadDataRequest, 4>;

    UdsSystem(::async::ContextType context);

    void do_init(bool const wakingUp);

    void addDiagJobs();
    void removeDiagJobs();
    void shutdownTimeoutManager();
    void shutdownComplete(transport::AbstractTransportLayer&);
    void processDiagCluster();
    void execute() override;

    bool readCf01(::diag::IReadDataRequest& request);

    uds::UdsLifecycleConnector _udsLifecycleConnector;

    uds::DummySessionPersistence _dummySessionPersistence;

    uds::DiagJobRoot _jobRoot;
    uds::DiagnosticSessionControl _diagnosticSessionControl;
    uds::CommunicationControl _communicationControl;
    uds::DiagnosisConfiguration<5, 1, 16> _udsConfiguration;
    uds::DiagDispatcher2 _udsDispatcher;
    uds::declare::AsyncDiagHelper<5> _asyncDiagHelper;

    ::diag::declare::DataHandler<::diag::IReadDataRequest, 4> _readDataHandler;
    ::uds::declare::ReadDataById<4> _readDataById;

    // ReadDataByIdentifier _readDataByIdentifier;
    uds::WriteDataByIdentifier _writeDataByIdentifier;
    uds::RoutineControl _routineControl;
    uds::StartRoutine _startRoutine;
    uds::StopRoutine _stopRoutine;
    uds::RequestRoutineResults _requestRoutineResults;
    // ReadIdentifierFromMemory _read22Cf01;
    uds::ReadIdentifierPot _read22Cf02;
    uds::TesterPresent _testerPresent;

    ::async::TimeoutType _timeout;
};
} // namespace config
