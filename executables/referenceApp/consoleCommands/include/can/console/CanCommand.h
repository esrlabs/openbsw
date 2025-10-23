// Copyright 2024 Accenture.

#pragma once

#include <can/canframes/CANFrame.h>
#include <can/transceiver/ICanTransceiver.h>
#include <util/command/CommandContext.h>
#include <util/command/GroupCommand.h>
#include <util/format/SharedStringWriter.h>

namespace can
{
class CanCommand : public ::util::command::GroupCommand
{
public:
    CanCommand(ICanTransceiver& canTransceiver);

protected:
    enum Commands
    {
        CMD_INFO,
        CMD_SEND
    };

    DECLARE_COMMAND_GROUP_GET_INFO
    void executeCommand(::util::command::CommandContext& context, uint8_t idx) override;

private:
    void send(::util::command::CommandContext& context, ::util::format::SharedStringWriter& writer);

    ICanTransceiver& _canTransceiver;
    CANFrame _canFrame;
};

} // namespace can
