#include <QDebug>
#include "RobotState.h"

std::map<int,QString> RobotState::statusNames;

RobotState::RobotState()
{
    initStatusNames();
}

RobotState::RobotState(Status status, qint64 timestamp,
    koord pos, float v, float a, qint8 light)
    : _status(status), _timestamp(timestamp), _pos(pos), _v(v), _a(a), _light(light)
{
    initStatusNames();
}

void RobotState::initStatusNames()
{
    if (statusNames.empty())
    {
        // Még nincsen inicializálva.
        statusNames[(int)Status::Accelerate] = QString("Gyorsítás");
        statusNames[(int)Status::Default] = QString("Alap");
        statusNames[(int)Status::Reset] = QString("Reset");
        statusNames[(int)Status::Stopping] = QString("Megállás");
        statusNames[(int)Status::Turn] = QString("Fordulás");
        statusNames[(int)Status::SelfTest] = QString("Önteszt");
    }
}

QString RobotState::getStatusName() const
{
    auto it = statusNames.find((int)_status);
    Q_ASSERT(it != statusNames.end());
    return it->second;
}

void RobotState::WriteTo(QDataStream& stream) const
{
    stream << (qint32)_status;
    stream << _timestamp;
    stream << _pos.x;
    stream << _pos.y;
    stream << _pos.orient;
    stream << _pos.turn;
    stream << _v;
    stream << _a;
    stream << _light;
}

void RobotState::ReadFrom(QDataStream& stream)
{
    qint32 tmpQint32;
    stream >> tmpQint32;
    _status = (Status)tmpQint32;
    stream >> _timestamp;
    stream >> _pos.x;
    stream >> _pos.y;
    stream >> _pos.orient;
    stream >> _pos.turn;
    stream >> _v;
    stream >> _a;
    stream >> _light;
}

void RobotState::CopyFrom(const RobotState &other)
{
    _status = other._status;
    _timestamp = other._timestamp;
    _pos = other._pos;
    _v = other._v;
    _a = other._a;
    _light = other._light;
}

QDataStream &operator<<(QDataStream& stream, const RobotState& state)
{
    state.WriteTo(stream);
    return stream;
}

QDataStream &operator>>(QDataStream& stream, RobotState& state)
{
    state.ReadFrom(stream);
    return stream;
}
