#include "HighScoreTable.h"

#include <cassert>
#include <algorithm>
#include <functional>

HighScoreTable::HighScoreTable(size_t capacity /*= 999u*/)
{
    assert(capacity > 0u && "capacity must be greater than 0");
    entries_.reserve(capacity);
    capacity_ = capacity;
}

bool HighScoreTable::Qualifies(const Score score) const
{
    // ハイスコアのサイズが上限に達していない場合は条件を満たす
    if (entries_.size() < capacity_) return true;

    // 最下位のスコアを取得
    const Score lowest = entries_.back().score;

    // > で比較することで、最下位と同点のスコアは条件を満たさないようにする (先に登録されたスコアを優先する)
    return score > lowest;
}

std::optional<size_t> HighScoreTable::RankOf(const Score score) const
{
    if (!Qualifies(score)) return std::nullopt;

    // 同点のスコアが存在する場合は、先に登録されたスコアを優先するようにする
    auto it = std::ranges::upper_bound(entries_, score, std::greater{}, &ScoreEntry::score);

    auto d = std::ranges::distance(entries_.begin(), it);
    return static_cast<size_t>(d);
}

HighScoreTable::SubmitResult HighScoreTable::Submit(const std::string& name, const Score score)
{
    const std::optional<size_t> offset = RankOf(score);
    if (!offset.has_value()) return { std::nullopt, false };

    // 上限に達している場合は、最下位のスコアを削除する
    if (entries_.size() >= capacity_)
    {
        assert(*offset < entries_.size());
        entries_.pop_back();
    }

    ScoreEntry newEntry{ name, score, std::chrono::system_clock::now() };
    entries_.insert(entries_.begin() + *offset, std::move(newEntry));
    return { *offset, *offset == 0u };
}

const ScoreEntry& HighScoreTable::GetBestEntry() const
{
    static const ScoreEntry kInvalidEntry = { "Invalid Score", 0u, {} };
    if (entries_.empty()) return kInvalidEntry;
    return entries_.front();
}

nlohmann::json HighScoreTable::ToJSON() const
{
    return entries_;
}

void HighScoreTable::FromJSON(const nlohmann::json& j)
{
    entries_.clear();
    if (!j.is_array()) return;

    j.get_to(entries_);

    // 降順ソート済みであることを保証する
    std::ranges::stable_sort(entries_, [](const ScoreEntry& a, const ScoreEntry& b)
    {
        if (a.score != b.score) return a.score > b.score;

        // 同点の場合は、先に登録されたスコアを優先する
        return a.timestamp < b.timestamp;
    });

    if (entries_.size() > capacity_)
    {
        entries_.resize(capacity_);
    }
}

void to_json(nlohmann::json& j, const ScoreEntry& e)
{
    j["name"] = e.name;
    j["score"] = e.score;
    j["timestamp"] = std::chrono::duration_cast<std::chrono::milliseconds>(e.timestamp.time_since_epoch()).count();
}

void from_json(const nlohmann::json& j, ScoreEntry& e)
{
    e.name = j.at("name").get<std::string>();
    e.score = j.at("score").get<Score>();
    e.timestamp = std::chrono::system_clock::time_point(std::chrono::milliseconds(j.at("timestamp").get<int64_t>()));
}
