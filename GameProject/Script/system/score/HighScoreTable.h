#pragma once

#include <Script.h>
#include <type/Score.h>
#include <chrono>
#include <vector>
#include <string>
#include <optional>
#include <json.hpp>

TSTRUCT()
struct ScoreEntry
{
    GENERATED_STRUCT_BODY()

    TPROPERTY(Visible)
    std::string name;

    TPROPERTY(Edit)
    Score score = 0;

    std::chrono::system_clock::time_point timestamp;
};

void to_json(nlohmann::json& j, const ScoreEntry& e);
void from_json(const nlohmann::json& j, ScoreEntry& e);


TCLASS()
class HighScoreTable : public Tako::Script
{
    GENERATED_BODY()

public:
    // ハイスコア配列型
    using ScoreEntries = std::vector<ScoreEntry>;

    struct SubmitResult
    {
        std::optional<size_t> rank; // 登録されたランキング (0-based)
        bool isNewRecord = false;   // 新記録かどうか
    };

    explicit HighScoreTable(size_t capacity = 999u);


    /// <summary>
    /// スコアが条件を満たすかどうかを判定します。
    /// </summary>
    /// <param name="score">判定するスコア。</param>
    /// <returns>スコアが条件を満たす場合はtrue、それ以外の場合はfalse。</returns>
    bool Qualifies(const Score score) const;

    /// <summary>
    /// もし登録したら何位になるかを返します。(Submit実行前を想定)
    /// </summary>
    /// <param name="score">順位を調べるスコア。</param>
    /// <returns>スコアの順位(0-based)。順位が存在しない場合は空のstd::optional。</returns>
    std::optional<size_t> RankOf(const Score score) const;

    /// <summary>
    /// 指定した名前とスコアで送信結果を作成します。
    /// </summary>
    /// <param name="name">送信対象の名前。</param>
    /// <param name="score">送信するスコア。</param>
    /// <returns>送信結果。</returns>
    SubmitResult Submit(const std::string& name, const Score score);


    /// --- 配列インターフェース ---
    size_t size() const { return entries_.size(); }
    size_t capacity() const { return capacity_; }
    bool empty() const { return entries_.empty(); }
    void clear() { entries_.clear(); }


    /// ハイスコアの配列を取得します。
    const ScoreEntries& GetEntries() const { return entries_; }

    /// ベストスコアのエントリを取得します。
    const ScoreEntry& GetBestEntry() const;

    /// ハイスコアの配列をJSON形式に変換します。
    nlohmann::json ToJSON() const;

    /// JSON形式のハイスコア配列を復元します。
    void FromJSON(const nlohmann::json& j);

private:
    // ハイスコアの配列 (降順ソート済み)
    ScoreEntries entries_;
    size_t capacity_;
};


