#pragma once
#include <cmath>
#include <cstdint>
#include <stdexcept>

namespace simcore {

// Small, explicit duration wrapper (seconds).
struct Duration {
    double seconds{0.0};

    constexpr Duration() = default;
    constexpr explicit Duration(double s) : seconds(s) {}

    static constexpr Duration from_seconds(double s) { return Duration{s}; }
    static constexpr Duration from_minutes(double m) { return Duration{60.0 * m}; }
    static constexpr Duration from_hours(double h) { return Duration{3600.0 * h}; }
    static constexpr Duration from_days(double d) { return Duration{86400.0 * d}; }
};

// UTC-ish calendar time for convenience (no leap seconds handled here).
struct UtcCalendar {
    int year;      // e.g. 2026
    int month;     // 1-12
    int day;       // 1-31
    int hour;      // 0-23
    int minute;    // 0-59
    double second; // [0, 60) (no leap seconds modeled)
};

// Time stored as nanoseconds since a chosen epoch.
// For sim purposes, we often choose J2000 epoch: 2000-01-01 12:00:00 UTC-ish.
// We are NOT modeling leap seconds; treat this as a continuous "UTC-like"
// timescale.
class Time {
  public:
    // J2000 epoch: 2000-01-01 12:00:00
    // Julian Date at J2000 is exactly 2451545.0
    static constexpr double JD_J2000 = 2451545.0;

    constexpr Time() = default;

    // Construct from seconds since J2000
    static Time from_seconds_since_j2000(double sec) { return Time{to_ns(sec)}; }

    // Construct from nanoseconds since J2000
    static constexpr Time from_ns_since_j2000(std::int64_t ns) { return Time{ns}; }

    // Construct from Julian Date
    static Time from_julian_date(double jd) {
        const double days = jd - JD_J2000;
        const double sec = days * 86400.0;
        return from_seconds_since_j2000(sec);
    }

    // Construct from calendar date/time (UTC-like). Useful for scenarios.
    // Uses a standard Gregorian calendar to JD conversion (no leap seconds).
    static Time from_utc_calendar(const UtcCalendar& utc) {
        const double jd = utc_to_jd(utc);
        return from_julian_date(jd);
    }

    // Accessors
    constexpr std::int64_t ns_since_j2000() const { return ns_since_j2000_; }

    double seconds_since_j2000() const { return static_cast<double>(ns_since_j2000_) * 1e-9; }

    double julian_date() const { return JD_J2000 + (seconds_since_j2000() / 86400.0); }

    double modified_julian_date() const {
        // MJD = JD - 2400000.5
        return julian_date() - 2400000.5;
    }

    // Arithmetic
    friend Time operator+(Time t, Duration d) { return Time{t.ns_since_j2000_ + to_ns(d.seconds)}; }
    friend Time operator-(Time t, Duration d) { return Time{t.ns_since_j2000_ - to_ns(d.seconds)}; }
    friend Duration operator-(Time a, Time b) {
        return Duration{(static_cast<double>(a.ns_since_j2000_ - b.ns_since_j2000_) * 1e-9)};
    }

    // Comparisons
    friend constexpr bool operator==(Time a, Time b) {
        return a.ns_since_j2000_ == b.ns_since_j2000_;
    }
    friend constexpr bool operator!=(Time a, Time b) { return !(a == b); }
    friend constexpr bool operator<(Time a, Time b) {
        return a.ns_since_j2000_ < b.ns_since_j2000_;
    }
    friend constexpr bool operator<=(Time a, Time b) {
        return a.ns_since_j2000_ <= b.ns_since_j2000_;
    }
    friend constexpr bool operator>(Time a, Time b) { return b < a; }
    friend constexpr bool operator>=(Time a, Time b) { return !(a < b); }

  private:
    std::int64_t ns_since_j2000_{0};

    constexpr explicit Time(std::int64_t ns) : ns_since_j2000_(ns) {}

    static std::int64_t to_ns(double seconds) {
        // Round to nearest ns for determinism
        const double ns = seconds * 1e9;
        // guard against overflow in very long sims; you can relax if you want
        if (ns > static_cast<double>(INT64_MAX) || ns < static_cast<double>(INT64_MIN)) {
            throw std::overflow_error("Time overflow converting seconds to nanoseconds");
        }
        return static_cast<std::int64_t>(std::llround(ns));
    }

    static bool is_gregorian_valid(int y, int m, int d) {
        (void)y;
        if (m < 1 || m > 12)
            return false;
        if (d < 1 || d > 31)
            return false;
        return true;
    }

    // Gregorian calendar UTC -> Julian Date (UTC-like; no leap seconds).
    // Algorithm: Fliegel & Van Flandern / standard astronomical conversion.
    static double utc_to_jd(const UtcCalendar& utc) {
        if (!is_gregorian_valid(utc.year, utc.month, utc.day)) {
            throw std::invalid_argument("Invalid UTC calendar date");
        }
        if (utc.hour < 0 || utc.hour > 23 || utc.minute < 0 || utc.minute > 59 ||
            utc.second < 0.0 || utc.second >= 60.0) {
            throw std::invalid_argument("Invalid UTC calendar time");
        }

        int Y = utc.year;
        int M = utc.month;
        const int D = utc.day;

        if (M <= 2) {
            Y -= 1;
            M += 12;
        }

        const int A = Y / 100;
        const int B = 2 - A + (A / 4);

        const double day_fraction = (utc.hour + (utc.minute + utc.second / 60.0) / 60.0) / 24.0;

        // Note: using floor on doubles intentionally here.
        const double JD = std::floor(365.25 * (Y + 4716)) + std::floor(30.6001 * (M + 1)) +
                          static_cast<double>(D) + day_fraction + static_cast<double>(B) - 1524.5;

        return JD;
    }
};

} // namespace simcore
