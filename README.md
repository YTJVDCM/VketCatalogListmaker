# VketCatalogListMakerについて

このソフトはバーチャルマーケット(以後Vケット)3以降のワールド内でカタログを開くボタンを押すと、該当サークルの情報がJSONファイルとして出力されるソフトになっています。  
現在  
カタログURL、サークル名、Vケットのワールド、場所(設定されているもののみ)、該当カタログを開いたVケットの種類(3,4等)、サークルアイコンURL、サークルヘッダーURL(存在する場合のみ)  
の7(8)つが取得できます。  

## 注意事項

**本ソフトウェアは現段階で未完成です。**  
バグ等で仮にWindowsが破損しても責任は取れません。  
本ソフトウェアはVケット非公式のアプリです。  
その他本ソフトウェアの基本的なライセンスはGPL v3に準拠します。  
(ライセンスファイルはインストーラーに同梱されています。)  
また、ソフトに同梱されているソフトウェア等ついてはソフトウェアごとのライセンスに従い使用しています。  

また、一人でソフトを作るのは骨が折れるのでIssueでもPull Requestでもどんどん送ってください  

## 更新履歴

- 1.0.0: 初期リリース
- 1.0.1: 使用ソフトの変更、Vケット4対応に向けた仕様変更(Vケット4には非対応)
- 1.1.0: Vケット4対応、出力したJSONをHTML化するソフトを実装、ソフト名をVketJSONに変更


## 使用(同梱)ソフト、ライブラリ一覧

- [MinGW](http://www.mingw.org/)

- [Curl](https://curl.haxx.se/download.html)

- [Cygwin](https://www.cygwin.com/)

- [Xmllint](http://xmlsoft.org/index.html)

- [zlib](https://www.zlib.net/)

- [PicoJSON](https://github.com/kazuho/picojson)

- [WiX Toolset](https://wixtoolset.org/)

- [Ocra](https://github.com/larsch/ocra)