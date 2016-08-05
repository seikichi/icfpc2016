# 雑スクプト集

## calc_all_resemblances.py

- 全問題でスコア計算して JSON 吐きます
- 第一引数に実行するスクリプトを指定して下さい

```
> ./tools/calc_all_resemblances.py rectangle/rectangle > rectangle.json
> ./tools/calc_all_resemblances.py multirectangle/multirectangle > multirectangle.json
```

## compare_resemblances.py

- `calc_all_resemblances.py` で作成した JSON を比較する

```
% ./tools/compare_resemblances.py rectangle.json multirectangle.json
multirectangle.json wins 100 / 101 problems to rectangle.json

multirectangle.json loses following problems:
problem 69: multirectangle.json resemblance = 0.438206 but rectangle.json resemblance = 0.442648
```
