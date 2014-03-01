DDE-stopper
===========

DDEしてかってにIEが起動するアプリがうざい。

DDEInitiate("IExplore", "WWW_OpenURL") とかしてるゴミアプリがいる。
![spy](https://raw.github.com/katakk/DDE-stopper/master/spy.png)  

とてもとてもうざいのでグローバルフックして、WM_DDE_ACKを適当に投げることにした。

