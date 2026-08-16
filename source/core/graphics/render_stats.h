#ifndef CORE_GRAPHICS_RENDER_STATS_H_
#define CORE_GRAPHICS_RENDER_STATS_H_

#include <cstdint>
#include <chrono>
#include <memory>
#include <sstream>
#include <string>

#include "core/common/perf_stats.h"
#include "core/graphics_common/render_primitives.h"

namespace amit::graphics
{
    using amit::common::CounterStat;
    using amit::common::IStatsBucket;
    using amit::common::StatsCollector;
    using amit::common::TimerStat;

    enum class RenderStatCounter : std::uint8_t
    {
        kRasterizedPixelCount
    };

    enum class StatsCollectionLevel : std::uint8_t
    {
        kNone,
        kFrame,
        kDraw,
        kPrimitive
    };

    class RenderFrameStats
    {
    public:
        void RecordDrawCall(std::int64_t vertex_count, std::int64_t index_count, std::int64_t primitive_count)
        {
            ++draw_call_count_;
            vertex_count_ += vertex_count;
            index_count_ += index_count;
            primitive_count_ += primitive_count;
        }

        void IncrementRasterizedPixelCount(std::int64_t count = 1)
        {
            rasterized_pixel_count_ += count;
        }

        void AddDrawDuration(double elapsed_ms)
        {
            total_draw_duration_ms_ += elapsed_ms;
        }

        std::string ToOverlayString() const
        {
            std::ostringstream stream;
            stream << "Frame Stats\n"
                   << "Draw Calls: " << draw_call_count_ << "\n"
                   << "Vertices: " << vertex_count_ << "\n"
                   << "Indices: " << index_count_ << "\n"
                   << "Primitives: " << primitive_count_ << "\n"
                   << "Rasterized Pixels: " << rasterized_pixel_count_ << "\n"
                   << "Draw Time: " << total_draw_duration_ms_ << " ms\n";
            return stream.str();
        }

    private:
        std::int64_t draw_call_count_         = 0;
        std::int64_t vertex_count_            = 0;
        std::int64_t index_count_             = 0;
        std::int64_t primitive_count_         = 0;
        std::int64_t rasterized_pixel_count_  = 0;
        double       total_draw_duration_ms_  = 0.0;
    };

    class DrawCallStats : public IStatsBucket
    {
    public:
        void Init() override
        {
            draw_timer_               = GetOrCreateTimerStat("Duration");
            vertex_counter_           = GetOrCreateCounterStat("VertexCount");
            index_counter_            = GetOrCreateCounterStat("IndexCount");
            primitive_counter_        = GetOrCreateCounterStat("PrimitiveCount");
            rasterized_pixel_counter_ = GetOrCreateCounterStat("RasterizedPixelCount");
        }

        ~DrawCallStats() override = default;

        std::shared_ptr<TimerStat>   draw_timer_;
        std::shared_ptr<CounterStat> vertex_counter_;
        std::shared_ptr<CounterStat> index_counter_;
        std::shared_ptr<CounterStat> primitive_counter_;
        std::shared_ptr<CounterStat> rasterized_pixel_counter_;

        friend class amit::common::StatsCollector;

    private:
        DrawCallStats(const std::string& name)
            : IStatsBucket(name)
        {
        }
    };

    class ScopedDrawCallStats
    {
    public:
        template <RenderPrimitiveType PrimitiveType>
        ScopedDrawCallStats(RenderFrameStats&                     render_frame_stats,
                            StatsCollectionLevel                  stats_collection_level,
                            const RenderPrimitive<PrimitiveType>& primitive,
                            const std::string&                    name = "DrawCallStats")
            : render_frame_stats_(render_frame_stats)
            , stats_collection_level_(stats_collection_level)
        {
            if (stats_collection_level_ == StatsCollectionLevel::kNone)
            {
                return;
            }

            const std::int64_t vertex_count    = GetVertexCount(primitive);
            const std::int64_t index_count     = GetIndexCount(primitive);
            const std::int64_t primitive_count = GetPrimitiveCount(primitive);

            render_frame_stats_.RecordDrawCall(vertex_count, index_count, primitive_count);
            draw_start_time_ = std::chrono::high_resolution_clock::now();

            if (stats_collection_level_ >= StatsCollectionLevel::kDraw)
            {
                draw_call_stats_ =
                    StatsCollector::GetInstance().GetOrCreateStatsBucket<DrawCallStats>(GetUniqueDrawCallName(name));
                draw_call_stats_->vertex_counter_->Increment(vertex_count);
                draw_call_stats_->index_counter_->Increment(index_count);
                draw_call_stats_->primitive_counter_->Increment(primitive_count);
                draw_call_stats_->draw_timer_->StartTimer();
            }
        }

        void IncrementStatCount(RenderStatCounter counter, std::int64_t count = 1)
        {
            if (stats_collection_level_ == StatsCollectionLevel::kNone)
            {
                return;
            }

            switch (counter)
            {
            case RenderStatCounter::kRasterizedPixelCount:
                render_frame_stats_.IncrementRasterizedPixelCount(count);
                if (draw_call_stats_)
                {
                    draw_call_stats_->rasterized_pixel_counter_->Increment(count);
                }
                break;
            }
        }

        ScopedDrawCallStats(const ScopedDrawCallStats&)            = delete;
        ScopedDrawCallStats& operator=(const ScopedDrawCallStats&) = delete;

        ~ScopedDrawCallStats()
        {
            EndScope();
        }

    private:
        static std::string GetUniqueDrawCallName(const std::string& name)
        {
            static std::uint64_t draw_call_counter = 0;
            return name + " " + std::to_string(++draw_call_counter);
        }

        template <RenderPrimitiveType PrimitiveType>
        static std::int64_t GetVertexCount(const RenderPrimitive<PrimitiveType>& primitive)
        {
            (void)primitive;

            if constexpr (PrimitiveType == RenderPrimitiveType::kPoint)
            {
                return 1;
            }
            else if constexpr (PrimitiveType == RenderPrimitiveType::kLine)
            {
                return 2;
            }
            else if constexpr (PrimitiveType == RenderPrimitiveType::kTriangle)
            {
                return 3;
            }

            return 0;
        }

        template <RenderPrimitiveType PrimitiveType>
        static std::int64_t GetIndexCount(const RenderPrimitive<PrimitiveType>& primitive)
        {
            (void)primitive;
            return 0;
        }

        template <RenderPrimitiveType PrimitiveType>
        static std::int64_t GetPrimitiveCount(const RenderPrimitive<PrimitiveType>& primitive)
        {
            (void)primitive;
            return 1;
        }

        void EndScope()
        {
            if (stats_collection_level_ == StatsCollectionLevel::kNone)
            {
                return;
            }

            const auto end_time   = std::chrono::high_resolution_clock::now();
            const auto elapsed_ms = std::chrono::duration<double, std::milli>(end_time - draw_start_time_).count();
            render_frame_stats_.AddDrawDuration(elapsed_ms);

            if (draw_call_stats_)
            {
                draw_call_stats_->draw_timer_->EndTimer();
            }
        }

        RenderFrameStats&              render_frame_stats_;
        std::shared_ptr<DrawCallStats> draw_call_stats_;
        StatsCollectionLevel           stats_collection_level_;
        std::chrono::high_resolution_clock::time_point draw_start_time_;
    };

}  // namespace amit::graphics

#endif  // CORE_GRAPHICS_RENDER_STATS_H_
