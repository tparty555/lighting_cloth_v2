# lighting_cloth_v2
This is the project for lighting cloth for "gokilive" (lighting cloth version2)

衣装：白衣\
ライト：LEDテープ (ws2813b)\
LEDの配置：\
<img width=700 alt="全体図" src="/Users/yusuke/tparty/light_cloth_v2/lighting_cloth_v2/haiti.jpg">

2019/03/20段階での設計\
PC(Openframeworks)から有線で服のArduinoとシリアル通信をする\
このとき、lighting cloth formatにしたがって通信する\
LEDテープは全て直列に繋がっている\
電源は最終的にはモバイルバッテリーとする。とりあえずはACアダプター5V2Aを使用する\
ArduinoはformatにしたがってLEDを光らせる\

## Lighting cloth format
1~100 光の色相(0~360のMap)
101~200は予約色相
210~は色相以外おん