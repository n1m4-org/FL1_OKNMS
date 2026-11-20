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
    static ScoreEntry kInvalidEntry = { "Invalid Score", 0u, {} };
    if (entries_.empty()) return kInvalidEntry;
    return entries_.front();
}

nlohmann::json HighScoreTable::to_json() const
{
    nlohmann::json j;
    
    for (const auto& entry : entries_)
    {
        nlohmann::json entryJson;
        entryJson["name"] = entry.name;
        entryJson["score"] = entry.score;
        entryJson["timestamp"] = std::chrono::duration_cast<std::chrono::milliseconds>(entry.timestamp.time_since_epoch()).count();
        j.push_back(entryJson);
    }

    return j;
}

void HighScoreTable::from_json(const nlohmann::json& j)
{
    for (const auto& entryJson : j)
    {
        ScoreEntry entry;
        entry.name = entryJson.at("name").get<std::string>();
        entry.score = entryJson.at("score").get<Score>();
        auto timestampMs = entryJson.at("timestamp").get<int64_t>();
        entry.timestamp = std::chrono::system_clock::time_point(std::chrono::milliseconds(timestampMs));
        entries_.push_back(entry);
    }
}
