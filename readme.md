# c_lifegame

"c_lifegame"は、趣味で作ったC言語で動作するライフゲーム関係のゲームです。  
ターミナル上で動作します。

# 概要

## lifegame

一般的なライフゲームです。

## lifeterritory

ライフゲームを応用した陣取りゲームです。

# 使い方

## lifegame

`lifegame_field.csv`を編集してフィールドを設定します。  
「２」の場所が塗りつぶされて表示されます。  
フィールドの幅は`lifegame.c`の`FIELD_X`と`FIELD_Y`と同じ幅にしてください。

## lifeterritory

`lifeterritory_field.csv`を編集してフィールドを設定します。  
「１」「２」がそれぞれの陣地のマス、「３」が塗りつぶせない壁マスです。  
フィールドの幅は`lifeterritory.c`の`FIELD_X`と`FIELD_Y`と同じ幅にしてください。  
`MAGNIFICATION`は塗りつぶされやすさを表します。