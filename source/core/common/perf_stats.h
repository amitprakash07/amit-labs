#ifndef CORE_COMMON_PERF_STATS_H
#define CORE_COMMON_PERF_STATS_H

#include <chrono>
#include <iostream>
#include <unordered_map>

#include "core.h"

namespace amit::common
{
    enum class StatType : uint32_t
    {
        kStatBucket = 0,
        kTimer      = 0x10000000,
        kCounter    = 0x20000000,
    };

    inline std::string_view EnumTypeToString(StatType stat_type)
    {
        switch (stat_type)
        {
        case StatType::kStatBucket:
            return "Stat Bucket";
        case StatType::kTimer:
            return "Timer";
        case StatType::kCounter:
            return "Counter";
        }

        return "";
    }

    using StatId = uint64_t;

    class Stat
    {
    public:
        Stat() = delete;

        StatId GetId() const
        {
            return stat_id_;
        }

        std::string_view GetName() const
        {
            return name_;
        }

    protected:
        Stat(StatType type, const std::string& name);

    private:
        StatType        type_;
        StatId          stat_id_;
        std::string     name_;
        static uint64_t stat_timer_id_counter_;
        static uint64_t stat_counter_id_counter_;
    };

    class TimerStat : public Stat
    {
    public:
        TimerStat(const std::string& name)
            : Stat(StatType::kTimer, name)
            , elapsed_ms_(0.0)
        {
        }

        void StartTimer()
        {
            start_ = std::chrono::high_resolution_clock::now();
        }

        void EndTimer()
        {
            std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
            elapsed_ms_ = std::chrono::duration<double, std::milli>(end - start_).count();
        }

        double GetElapsedMilliseconds() const
        {
            return elapsed_ms_;
        }

    private:
        std::chrono::high_resolution_clock::time_point start_;
        double                                         elapsed_ms_;
    };

    class CounterStat : public Stat
    {
    public:
        CounterStat(const std::string& name)
            : Stat(StatType::kCounter, name)
            , count_(0)
        {
        }

        void Increment(int64_t value = 1)
        {
            count_ += value;
        }

        int64_t GetCount() const
        {
            return count_;
        }

    private:
        int64_t count_;
    };

    using StatBucketId = uint64_t;

    class IStatsBucket
    {
    public:
        StatBucketId GetId() const
        {
            return bucket_id_;
        }

        virtual void Init() = 0;

        const std::string& GetName() const
        {
            return name_;
        }

        std::string ToString() const;

        virtual ~IStatsBucket() = default;

    protected:
        IStatsBucket(const std::string& name);

        std::shared_ptr<TimerStat>   GetOrCreateTimerStat(const std::string& name);
        std::shared_ptr<CounterStat> GetOrCreateCounterStat(const std::string& name);

        ObjectLabel<StatType> stat_bucket_label_;

    private:
        std::string                                                   name_;
        std::unordered_map<std::string, std::shared_ptr<TimerStat>>   timer_stats_;
        std::unordered_map<std::string, std::shared_ptr<CounterStat>> counter_stats_;
        StatBucketId                                                  bucket_id_;
        static uint64_t                                               stats_bucket_id_counter_;
    };

    class StatsCollector
    {
    public:
        static StatsCollector& GetInstance()
        {
            static StatsCollector instance;
            return instance;
        }

        StatsCollector(const StatsCollector&) = delete;

        template <class StatBucketType>
            requires std::derived_from<StatBucketType, IStatsBucket>
        std::shared_ptr<StatBucketType> GetOrCreateStatsBucket(const std::string& name);

        std::string GetAllStatsAsString() const
        {
            std::string ret_string;

            for (const auto& [id, bucket] : stats_buckets_)
            {
                ret_string += bucket->ToString();
            }

            return ret_string;
        }

    private:
        StatsCollector() = default;
        std::unordered_map<StatBucketId, std::shared_ptr<IStatsBucket>> stats_buckets_;
    };

    template <class StatBucketType>
        requires std::derived_from<StatBucketType, IStatsBucket>
    std::shared_ptr<StatBucketType> StatsCollector::GetOrCreateStatsBucket(const std::string& name)
    {
        for (const auto& [id, bucket] : stats_buckets_)
        {
            if (bucket->GetName() == name)
            {
                return std::reinterpret_pointer_cast<StatBucketType>(bucket);
            }
        }

        std::shared_ptr<StatBucketType> new_bucket = std::shared_ptr<StatBucketType>(new StatBucketType(name));
        stats_buckets_[new_bucket->GetId()]        = new_bucket;
        new_bucket->Init();
        return new_bucket;
    }
}  // namespace amit::common

#endif