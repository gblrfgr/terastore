#include <bits/chrono.h>
#include <chrono>
#include <cstdint>
#include <stdexcept>
#include <terastore/snowflake.hpp>

namespace tst {

template <typename Clock>
SnowflakeGenerator<Clock>::SnowflakeGenerator(
    std::chrono::time_point<Clock> epoch, uint_least16_t machine_id)
    : m_last_used(epoch), m_epoch(epoch), m_uses_this_millisecond(0),
      m_machine_id(machine_id) {
  if (machine_id > 0x3FF) {
    throw std::runtime_error("machine id too big");
  } else if (m_epoch > Clock::now()) {
    throw std::runtime_error("epoch must not be in the future");
  }
}

template <typename Clock>
snowflake_t SnowflakeGenerator<Clock>::next_snowflake() {
  uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
      Clock::now() - this->m_epoch);
  uint64_t last_used = std::chrono::duration_cast<std::chrono::milliseconds>(
      this->m_last_used - this->m_epoch);
  if (now != last_used) {
    this->m_last_used = Clock::now();
    this->m_uses_this_millisecond = 0;
  } else {
    this->m_uses_this_millisecond++;
  }
  return (snowflake_t)((now & 0x1FFFFFFFFFF) << 22) +
         (this->m_machine_id << 12) + (this->m_uses_this_millisecond & 0xFFF);
}

} // namespace tst