#ifndef MESSAGE_T_HPP
#define MESSAGE_T_HPP

#include "Messages/BackMessage.hpp"
#include "Messages/BatteryMessage.hpp"
#include "Messages/BatteryMessage.hpp"
#include "Messages/ClockwiseMessage.hpp"
#include "Messages/CommandMessage.hpp"
#include "Messages/CounterClockwiseMessage.hpp"
#include "Messages/DownMessage.hpp"
#include "Messages/FlipMessage.hpp"
#include "Messages/ForwardMessage.hpp"
#include "Messages/GoMessage.hpp"
#include "Messages/LandMessage.hpp"
#include "Messages/LeftMessage.hpp"
#include "Messages/MDirectionMessage.hpp"
#include "Messages/MoffMessage.hpp"
#include "Messages/MonMessage.hpp"
#include "Messages/RightMessage.hpp"
#include "Messages/SpeedMessage.hpp"
#include "Messages/TakeoffMessage.hpp"
#include "Messages/TimeMessage.hpp"
#include "Messages/UpMessage.hpp"

namespace messages
{
  typedef boost::variant<messages::BackMessage,
                         messages::BatteryMessage,
                         messages::ClockwiseMessage,
                         messages::CommandMessage,
                         messages::CounterClockwiseMessage,
                         messages::DownMessage,
                         messages::FlipMessage,
                         messages::ForwardMessage,
                         messages::GoMessage,
                         messages::LandMessage,
                         messages::LeftMessage,
                         messages::MDirectionMessage,
                         messages::MoffMessage,
                         messages::MonMessage,
                         messages::RightMessage,
                         messages::SpeedMessage,
                         messages::TakeoffMessage,
                         messages::TimeMessage,
                         messages::UpMessage>
    Message_t;
} // namespace messages

#endif