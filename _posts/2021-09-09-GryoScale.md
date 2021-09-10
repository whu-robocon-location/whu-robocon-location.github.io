---
title："陀螺仪标定实验"
tags：
- gyro
---

# 陀螺仪标定

## 目的

陀螺仪采集到的数据会随转速改变，需要测定陀螺仪输出和转速之间的关系，即积分系数。

## 原理

陀螺仪直接输出的是AD值，我们需要对AD值进行转换才能得到当前的角速度，进而积分得到角度。因此，我们定义积分系数

$$
Scale = \frac{AD}{\omega}
$$

实验发现，积分系数与角速度相关，需要在不同转速下测定积分系数与角速度的关系。在代码中，我们引入中间变量filted_rate

$$
filted\_rate = AD*Sample\_Time
$$

根据不同转速下测定积分系数与filted_rate的关系，并用二次曲线拟合，得到

$$
Scale = A*filted\_rate^2 + B*filted\_rate + C
$$

单片机运算能力不足，不能实时根据角速度计算积分系数，故我们采取牺牲空间的查表法,保证filted_rate从1~1500变化时都有对应的积分系数。于是有：

$$
AD = Scale*\omega\\
filted\_rate = AD*Sample\_Time\\
\theta = filted\_rate/Scale*Sample\_Time
$$


## 标定仪器

1. 陀螺仪标定专用转台带elmo

2. 陀螺仪及采集板

3. 驱动elmo所需的顶板、开关板、线等

4. 无线jlink

5. 小电池*2

6. 匝带 + 胶枪

   

## 标定流程

#### 1. 代码检查

1. 注释掉程序中位于positioncount.h中的scaleTab，将积分系数设置为任意常量
2. 检查数据传输方式。VG910用的SPI发送陀螺仪数据，VG103PT用的是USART

#### 2. 安装检查

1. 注意不要让机械组同学安装陀螺仪时损伤顶板
2. 检查放置转台的地面是否平坦
3. 无线jlink和电池是否用匝带和胶枪固定在转台上，不会给陀螺仪施加额外的力

#### 3. 接线检查

1. 检查电机使能后是否有轴向间隙
2. 检查接线。先用上位机驱动elmo转一圈，检查连线、转速、转角是否正常。注意实际转速和驱动转速不能差太多，3%以内。
3. 检查电池电压是否充足
4. 检查jlink是否有警告，否则可能出现陀螺仪转了半圈数据丢包情况

#### 4. 标定

1. 将Position中的AngleDeg加入监视，开启周期更新窗口
2. 打开elmo上位机，使电机以30°/s,60°/s,120°/s转动±360°，观察角度值是否逼近±360°，误差在0.1°以内代表数据正常，结束标定；
3. 否则，使电机按照表格中填写的数值转动。由于间隙的存在，我们需要让陀螺仪每次都转过间隙角，即开始和结束时都朝着同一方向把陀螺仪怼一下。电机转动之前保持陀螺仪静止3s，保证陀螺仪没有零漂
4. 将angleDeg的值填入[表格](https://github.com/whu-robocon-location/whu-robocon-location.github.io/tree/master/_posts/VG910ScaleTemplate.xlsx)中，继续下一组角速度实验，reset清空当前角度值
5. 每个速度做一遍，做完10个速度检查一下数据。使用VG910时角度一定会有明显的减小，VG103PT则基本平稳。VG910做到150°/s即可，VG103PT做到300°/s
6. 绘制角速度和积分系数的图，使用二次曲线拟合，若R²小于0.97，剔除异常数据，并在该角速度下重新做实验；
7. 当R²满足要求，计算filtedRate，拟合filtedRate和积分系数的关系，使filtedRate从0到1500变化，计算积分系数，然后导出[积分系数](https://github.com/whu-robocon-location/whu-robocon-location.github.io/tree/master/_posts/ScaleEG.txt)，填入代码中的ScaleTab
