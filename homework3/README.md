作業三

練習如何從 CA 傳入記憶體位址給 TA。
Hint: 可參考optee_examples/random如何將一陣列記憶體位址傳入 TA

1.排序
從 CA 傳入一亂數陣列記憶體位址給 TA，TA 利用指標去接陣列記憶體位置並完
成排序的工作。排序完成後 CA 再將該陣列內的值依序印出。

2.Base64編碼
實作base64編碼。
Encode: 將一記憶體位址和大小給 TA，TA根據內容進行編碼後CA將編碼內容
印出。
Decode: 將一記憶體位址和大小給 TA，TA根據內容進行解碼後CA將解碼內容
印出。

繳交截圖
1. 排序結果
2. 利用自己名字做base64 encode和decode

![homwork3](https://user-images.githubusercontent.com/81294928/136702590-430d9f5b-07c1-42d6-80f3-60a25411800c.png)
