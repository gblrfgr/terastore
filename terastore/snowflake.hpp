#ifndef TERASTORE_SNOWFLAKE_HPP
#define TERASTORE_SNOWFLAKE_HPP

#include <chrono>
#include <cstdint>

namespace tst {

typedef uint64_t snowflake_t;

template <typename Clock> class SnowflakeGenerator {
private:
  const std::chrono::time_point<Clock> m_epoch;
  std::chrono::time_point<Clock> m_last_used;
  uint_least16_t m_uses_this_millisecond;
  uint_least16_t m_machine_id;

public:
  SnowflakeGenerator(std::chrono::time_point<Clock> epoch,
                     uint_least16_t m_machine_id);
  SnowflakeGenerator(SnowflakeGenerator &) = delete;
  SnowflakeGenerator(SnowflakeGenerator &&) = default;

  snowflake_t next_snowflake();
};

} // namespace tst

#endif