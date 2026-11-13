#include "check.h"

#include <iostream>
#include <sstream>
#include <vector>

namespace dip {
namespace bad {

class ConsoleAlarm {
public:
    void send(int temperature) {
        std::cout << "Overheat: " << temperature << '\n';
    }
};

class TemperatureMonitor {
public:
    void checkTemperature(int temperature) {
        if (temperature > 80) {
            alarm_.send(temperature);
        }
    }

private:
    ConsoleAlarm alarm_;
};

} // namespace bad
namespace good {

// Потребность монитора: сообщить о перегреве, независимо от канала доставки.
class AlarmSink {
public:
    virtual ~AlarmSink() = default;
    virtual void notifyOverheat(int temperature) = 0;
};

class TemperatureMonitor {
public:
    explicit TemperatureMonitor(AlarmSink& alarm) : alarm_(alarm) {}

    void checkTemperature(int temperature) {
        if (temperature > 80) {
            alarm_.notifyOverheat(temperature);
        }
    }

private:
    AlarmSink& alarm_;
};

class StreamAlarm final : public AlarmSink {
public:
    explicit StreamAlarm(std::ostream& output) : output_(output) {}

    void notifyOverheat(int temperature) override {
        output_ << "Overheat: " << temperature << '\n';
    }

private:
    std::ostream& output_;
};

class RecordingAlarm final : public AlarmSink {
public:
    void notifyOverheat(int temperature) override {
        temperatures_.push_back(temperature);
    }

    const std::vector<int>& temperatures() const { return temperatures_; }

private:
    std::vector<int> temperatures_;
};

} // namespace good

void run() {
    std::cout << "== DIP ==\n";
    bad::TemperatureMonitor before;
    std::cout << "before: ";
    before.checkTemperature(90);

    std::ostringstream output;
    good::StreamAlarm streamAlarm(output);
    good::TemperatureMonitor after(streamAlarm);
    after.checkTemperature(80);
    check(output.str().empty(), "DIP: boundary does not trigger alarm");
    after.checkTemperature(90);
    check(output.str() == "Overheat: 90\n", "DIP: stream notification");
    std::cout << "after: " << output.str();

    good::RecordingAlarm recording;
    good::TemperatureMonitor tested(recording);
    tested.checkTemperature(79);
    tested.checkTemperature(80);
    tested.checkTemperature(81);
    check(recording.temperatures() == std::vector<int>{81}, "DIP: policy independent of output");
    std::cout << "recorded alarms: " << recording.temperatures().size() << '\n';
}

} // namespace dip
