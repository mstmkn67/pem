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

サンプルやマニュアルは、製作中です
マニュアル
https://github.com/mstmkn67/pem/blob/master/doc/manual.pdf
