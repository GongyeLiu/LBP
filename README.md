# LBP
 LBP特征提取和可视化



包括圆形LBP和旋转不变LBP（最小LBP），采样半径和采样点数可调，图像大小不限。返回的是LBP特征图，尺寸较大可通过统计直方图的方式获取256维特征向量。

效果如下：

**原图像**

![1](README.assets/1.jpeg)

**圆形LBP（半径5，采样点12）**

![1_LBP](README.assets/1_LBP.jpg)

**旋转不变LBP**

![1_LBPMin](README.assets/1_LBPMin-1617620566632.jpg)