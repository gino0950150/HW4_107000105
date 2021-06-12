# HW4_107000105 <br>

# 4_1 <br>
1.compile main.cpp 進入車子<br>
2.sudo python3 car_control.py /dev/ttyUSB0<br>
3.輸入d1(hw4範例裡的d2，與停車格x軸的距離，在停車格右邊是正值，左邊是負值)，按下enter<br>
4.輸入d2(hw4範例裡d1，與停車格y軸的距離)，按下enter<br>
5.輸入direction(由上往下看，右邊輸入e，左邊輸入w，下方輸入n)<br>，按下enter<br>

輸入完後，車子會根據輸入的值停入停車格<br>

#4_2<br>
1.compile main.cpp 進入車子<br>
2.燒 openmv.py 進入openmv<br>
3.將車子放在其視野看得到白線的位置<br>
4.按下reset <br>
5.車子會自動跟著白線走  <br>
補充： 主要是透過openmv寫判斷式決定當下要前進左轉或右轉，送給車子不同的char，code參考 https://book.openmv.cc/example/10-Color-Tracking/black-grayscale-line-following.html 做修正<br>


#4_3 <br>
1.compile main.cpp進車子
2.燒 openmv.py 進入openmv<br>
3.將車子放在其視野看得到tag的位置<br>
4.按下reset<br>
5.車子會自動調整位置<br>
補充:與上題一樣，判斷式寫在openmv.py<br>



