# pem

(磁石)振り子のプログラムです。
[OCTA](http://octa.jp/)のインターフェースであるGOURMETで動きます。
GOURMETのメニューから、
Tool>Environments Setup
から、ダウンロードしたディレクトリを指定して使ってください。

実行ファイルは、Ubuntu18.04.5でコンパイルしています。
pem -I inputfile -O outputfile
で実行してください。
たとえば、
pem -I pendulum.udf -O pendulum_o.udf
のようにです。

udfのデータ名は変えたほうが良いかなと思っています。

サンプルやマニュアルは、製作中です。

## マニュアル
https://github.com/mstmkn67/pem/blob/master/doc/manual.pdf

## サンプル
0. チュートリアル https://github.com/mstmkn67/pem/blob/master/sample/0_tutorial/doc/tutorial.pdf
- 楕円体の自由回転　https://youtu.be/UQbKPZzIAbQ
- 磁石振り子(軸対称) https://youtu.be/UBWoQCETfzA
- 磁石振り子(軸非対称) https://youtu.be/dPGkkhcUG7M
- 磁石振り子(摩擦あり) https://youtu.be/dPGkkhcUG7M

1. 単振り子 https://github.com/mstmkn67/pem/blob/master/sample/1_simplePendulum/doc/simplePendulum.pdf
- 単振り子 https://youtu.be/rppyDI8gqTg

2. 振り子の波(Pendulum Wave)
- 線形解で考えた初期角度(30度) https://youtu.be/z2s-MZl5GoI
- 厳密解で考えた初期角度(30度) https://youtu.be/wYixgBzghbw
- 線形解で考えた初期角度(150度) https://youtu.be/lLCJf_7ZDrs
- 厳密解で考えた初期角度(150度) https://youtu.be/3Yz_H9489Q0

3. 振り子の錘

4. 球面振り子

5. 差分方法

6. 外磁場によるダイポール https://github.com/mstmkn67/pem/blob/master/sample/6_externalField/doc/externalField.pdf
- 外磁場によるダイポール　https://youtu.be/LGii4xLuqyU
