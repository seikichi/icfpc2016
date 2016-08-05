# 雑スクプト集

## calc_all_resemblances.py

- 全問題でスコア計算して JSON 吐きます
- 第一引数に実行するスクリプトを指定して下さい

```
> ./tools/calc_all_resemblances.py rectangle/rectangle > rectangle.json
> ./tools/calc_all_resemblances.py multirectangle/multirectangle > multirectangle.json
```

## compare_resemblances.py

- `calc_all_resemblances.py` で作成した JSON を比較します
- ベースライン的な方を第一引数にしてね!

```
% ./tools/compare_resemblances.py rectangle.json multirectangle.json
multirectangle.json wins 100 / 101 problems to rectangle.json

multirectangle.json loses following problems:
problem 69: multirectangle.json resemblance = 0.438206 but rectangle.json resemblance = 0.442648
```

## show_rankings.py

- ICFPC の API を実行して各問題のランキング上位 5 スコアを見ます

```
% ./tools/show_rankings.py | tail -5
problem 97: 0.846153, 0.846153, 0.6875, 0.68402, 0.68402 ...
problem 98: 1.0, 0.75, 0.75, 0.666666, 0.375 ...
problem 99: 0.776397, 0.776397, 0.447993, 0.438126, 0.383172 ...
problem 100: 1.0, 0.954545, 0.954545, 0.875, 0.857843 ...
problem 101: 0.437443, 0.308181, 0.308181, 0.308181, 0.305258 ...
```
