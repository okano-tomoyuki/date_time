# DateTime

DataTimeはC++で時刻情報の取り扱いを簡便にするために開発されました。

## 1. 準備

DataTimeはC++11以降の環境で動作するシングルヘッダーのライブラリとして
開発してされています。(依存ライブラリ等はなく、includeするだけで使えます。)

## 2. 使い方

メンバは以下の内容を個別に保持しています。
- 年
- 月
- 日
- 時
- 分
- 秒
- ミリ秒
- マイクロ秒

それぞれゲッターを設けているので個別で必要な場合は以下のように取り出してください。

``` cpp
// 現在の日時(UTC)でインスタンス化
auto dt    = DataFrame();

// メンバの取り出し
auto year        = df.year();
auto month       = df.month();
auto day         = df.day();  
auto hour        = df.month();  
auto minute      = df.minute();  
auto second      = df.second();
auto millisecond = df.millisecond();
auto microsecond = df.microsecond();  
```

日時を文字列として取り出したいケースも多いと思います。
その場合、to_strメソッドが使用できます。
置換対象の文字列の箇所をすべて日時情報として置換します。
(現状エスケープ機能は実装していないので利用の際は気を付けてください。)

|置換文字||
|--|--|
|yyyy|年を4桁の数字で置換|
|mm|月を2桁で0埋めした数字で置換|
|dd|日を2桁で0埋めした数字で置換|
|hh|時間(24時間表記)を2桁で0埋めした数字で置換|
|nn|分を2桁で0埋めした数字で置換|
|ss|秒を2桁で0埋めした数字で置換|
|zzz|ミリ秒を3桁で0埋めした数字で置換|
|zzzzzz|ミリ秒+マイクロ秒を0埋めした6桁の数字で置換|

なお、置換の優先順位はzzzzzz > zzz に設定されています。

``` cpp
// 現在の日時(UTC)でインスタンス化
auto dt    = DataFrame();

auto date       = dt.to_str("yyyy/mm/dd");      // 例 "2024/5/19"
auto time       = dt.to_str("hh:nn:ss.zzz");    // 例 "15:23:18.432"
auto iso_format = dt.to_str("yyyymmddThhnnssZ"); // 例 "2024/5/19T15:23:18"
```

文字列からDataTime型への変換用にfrom_strメソッドが用意されています。
staticメソッドなので利用方法を間違えないように注意してください。

``` cpp
auto iso_format = DateTime().to_str("yyyymmddThhnnssZ");
auto dt = DateTime::from_str(iso_format, "yyyymmddThhnnssZ");
```

時刻の加算操作をするためにはaddメソッドを使用します。
UTC -> JSTに変換する際などに利用できます。

``` cpp
auto utc = DateTime();
auto jst = DateTime::add(utc, 9, DateTime::HOUR);
```

加算値として与える値は負の値でも構いません。
加算単位の指定は以下のenum値から選択できます。

|Unit|加算単位|
|--|--|
|HOUR|時|
|MIN|分|
|SEC|秒|
|MSEC|ミリ秒|
|USEC|マイクロ秒|

時刻どうしの比較をする場合、diffメソッドが利用できます。
単位指定はaddメソッドの際と同様です。

``` cpp
auto utc = DateTime();
auto jst = DateTime::add(utc, 9, DateTime::HOUR);
auto diff_hour = DateTime::diff(jst, utc, DateTime::HOUR); // 9
```