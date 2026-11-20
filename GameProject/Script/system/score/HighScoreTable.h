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
    std::string name;
    Score score;
    std::chrono::system_clock::time_point timestamp;
};


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
        bool isNewRecord;           // 新記録かどうか
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
    /// 名前とスコアを登録します。
    /// </summary>
    /// <param name="name">登録する名前。</param>
    /// <param name="score">登録するスコア。</param>
    /// <returns>登録結果を表す32ビット符号なし整数。</returns>
    SubmitResult Submit(const std::string& name, const Score score);


    /// --- 配列インターフェース ---
    size_t size() const { return entries_.size(); }
    size_t capacity() const { return entries_.capacity(); }
    bool empty() const { return entries_.empty(); }
    void clear() { entries_.clear(); }


    /// ハイスコアの配列を取得します。
    const ScoreEntries& GetEntries() const { return entries_; }

    /// ベストスコアのエントリを取得します。
    const ScoreEntry& GetBestEntry() const;

    /// JSONに変換
    nlohmann::json to_json() const;

    /// JSONから構築
    void from_json(const nlohmann::json& j);


private:
    // ハイスコアの配列 (昇順ソート済み)
    ScoreEntries entries_;
    size_t capacity_;
};
