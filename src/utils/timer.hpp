#ifndef UTILS_TIMER_HPP
#define UTILS_TIMER_HPP
#include <chrono>
#include <mutex>

namespace Utils {
using namespace std::chrono;
class Timer {
    typedef time_point<system_clock, milliseconds> TimePoint;

   public:
    Timer() { update_time_point(); }
    milliseconds elapsed_ms() {
        std::scoped_lock lck(mtx);
        auto delta = time_point_cast<TimePoint::duration>(
                         system_clock::time_point(system_clock::now())) -
                     saved_time_point;

        return milliseconds(delta);
    }
    TimePoint get_saved_time_point() {
        std::scoped_lock lck(mtx);
        return saved_time_point;
    }
    TimePoint get_current_time_point() {
        return time_point_cast<TimePoint::duration>(
            system_clock::time_point(system_clock::now()));
    }
    void update_time_point() {
        std::scoped_lock lck(mtx);
        saved_time_point = time_point_cast<TimePoint::duration>(
            system_clock::time_point(system_clock::now()));
    }

    void operator+=(milliseconds time) {
        std::scoped_lock lck(mtx);
        saved_time_point += time;
    }

    void operator-=(milliseconds time) {
        std::scoped_lock lck(mtx);
        saved_time_point -= time;
    }

   private:
    std::mutex mtx;
    TimePoint saved_time_point;
};
}

#endif