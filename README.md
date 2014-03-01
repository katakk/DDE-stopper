DDE-stopper
===========

DDEしてかってにIEが起動するアプリがうざい。

DDEInitiate("IExplore", "WWW_OpenURL") とかしてるゴミアプリがいる。
![spy](https://raw.github.com/katakk/DDE-stopper/master/spy.png)  

アプリバイナリエディタで開いて www.mediaenco... とか調べたけど無かった。
とてもとてもうざいのでグローバルフックして、WM_DDE_ACKを適当に投げることにした。

     LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
     {
         if( pMsg->message == WM_DDE_INITIATE )  {
              SendMessage(hwndDDESender, WM_DDE_ACK, (WPARAM)hwndDDEReceiver, 0);   
     
     
