<!--
 * @Author: Location Group
 * @Date: 2021-01-26 09:40:27
 * @LastEditTime: 2021-01-27 15:36:21
 * @LastEditors: Please set LastEditors
 * @Description: Simple process for location
-->
# 1.调车

## 改代码、接线
1.  <input type="checkbox" checked>position_now改在main里面</input>
2.  <input type="checkbox" checked> 检测直线度和圆弧阈值的 </input>
3. <input type="checkbox" checked> 圆弧解算代码 </input> 

## 组间配合
1. 机械组在装车的时候要在旁边看着
   （1) 稳定性。两片滑块板子之间的间距足够大，间距太小车容易左右晃

   （2）平行度。保证车的边缘和导轨边缘平行。有的限位板可能有很大的加工误差，比如孔打反了，这对限位有大影响。

   （3）原装钉子，别乱铣板子然后配上奇奇怪怪的螺钉。从透明小盒子里拿尼龙套和螺钉。

   (4) 夹子四个都要装
2. 量模型
   
   （1） 获取码盘中心到小车中心（几何中心）的连线距离L和夹角α；
         小车旋转180°后坐标改变量x，y，以及R = sqrt（x^2 + y^2)

   （2） 小车x轴与码盘x轴的夹角β；

   （3） 获取车和轴承的模型，算出舵轮需要转的角度。若为全向轮，可以省略。

   （4）α与β一般可以目测，最重要的是L.详细说明在"机械组准备"表格中给出

找主控，确定小车坐标轴

找机构组，把转轮子的代码准备好

问问定位组自己，准备代码、表格、示波器、matlab文件，并将这些放入一个文件夹，写上日期和标定流程。

* ### 1.0 陀螺仪标定

1. 这一步是最重要的，优先级最高。如果陀螺仪没标定，那就需要手动铝管限位。
   
   整理好有哪些陀螺仪是标好的，哪些没有标好，做好标记。（我对陀螺仪还不是很熟）

   准备好elmo上位机，调好pid，用hzj之前搓的ec 30 dat就可以了。

2. 使用陀螺仪拟合表开始做实验
   
   （1） 打开代码，找到代码使用的积分系数。理论上积分系数可以设置为1，只要保证调试的时候代码的积分系数和表格中使用的积分系数一致。

   （2） 由于上位机要求整数，电机可能转不到360°，需要算出电机能转的转角。如果用的是elmo上位机，转角在excel中已经写好。

   （3）利用散点图拟合转速和积分系数的关系，得到公式，并且将公式输入scale列中。

   （4）拉出角度分度尺，一般0.2就ok，保证filted_rate要从0取到150都有对应的积分系数值。可以利用wps数据栏中的删除重复数据去掉重复的filted_rate项。

   （5）将excel的1500个数据另存为txt，利用程序gryo_sort_scale即可整理出陀螺仪积分系数表。这里也可用excel处理，只是目前还不会。

   （6）测试陀螺仪性能。

* ### 1.1导轨直线度标定
***
#### 定义
1. 可用距离d：大于180cm，（对应小车脉冲数大于30000），起点和终点角度差小于0.1°的一段距离
2. 解算阈值angle_delta_max：小车在可用距离段行进时，陀螺仪输出的最大角度
#### 实验流程
1. 导轨检查：
   滑块必须配套原装钉，否则重装。
   小车推动顺滑，否则检查场地卫生、导轨上油、擦去灰尘、滑块上珠子。
2. 得出解算阈值
   将小车从导轨起点推到终点，得到解算阈值，加入代码。
3. 导轨直线度检验：
   （1）每36cm记录一次角度变化值，选出可用距离。若无可用距离，选出角度小于0.1°的一段。
   （2）记录始末点，重复实验一次精度。目前只有VG103不太稳定，实验时要注意输出的角度。
***

* ### 1.2 联合标定表格预备
***
#### 定义
1. 脉冲误差限：当码盘脉冲数为30000时，为保证实验精度，依据经验，码盘误差限为20。若码盘脉冲数不足30000，误差限可按比例缩放。
2. 联合标定表格：含有两组解、正反轮径系数、误差处理的表格
3. 有效数据：做四次实验取码盘脉冲数中位数，若四次实验中某一组的码盘脉冲数与中位数的差值在脉冲误差限内，则这一次实验数据有效。
#### 流程
1. 检查小车坐标系和对应参数是否正确。
2. 确定小车坐标系。先推单轴，注意限位，注意正反推都是实验数据，一定要做至少四组。推到点以后需要置零；单轴标完之后换边。
3. 检验β的值是否正确。将取整后的脉冲放入联合标定数据栏。
4. （可选） 得到系数以后，将其作为原始数据放入matlab程序line_grad.m中优化，将输出作为最终系数.matlab程序中β值也需要修改。
   积分步长和迭代次数根据收敛情况确定。积分步长与输入量数量级相当比较好，由于syms无法看到值，所以看到收敛要停掉程序修改迭代次数。
5. 检验。
   (优化方案)
   1. 寻找打断程序时可以自动输出现有值的办法；
   
   2. 利用matlab自带梯度下降函数。
   
   3. 限定脉冲数的数值波动，上下偏差5，最多偏差10
问题：如果要验证四个方向，会额外拆装一次车，是否有更好的替代方案？
***

* ### 1.3 圆弧标定

*** 
1. 检查程序中圆弧反算的部分：
   (1) K1 = arc_K1 K2 = arc_K2 ?
   (2) L = sqrt(K1*K1 + K2*K2)/2 ?
   (3) 直线更新是否被注释掉了（保证解算时不进直线更新即可）   * 待优化

2. 准备圆弧解算表格
   做四组数据，确保正转和反转的α和L均在某一个值上下偏移0.01左右（代表实验数据问题不大）即可

3. 实验
   先催促机械组装模型。由于轴承比较难装，大概会花半个小时。需要检查是否有轴向偏移量。
   由于转180°比较难控制，所以推的时候注意要尽量从远端推。
   jlink特别容易掉，目前没找到原因。所以要么换用他人的jlink，要么推的时候一定要慢，晃动一定要小，不然起个身的功夫就没了。

4. 将表格中绿色数据丢进matlab程序cir_grad.m里更新系数。
   需要修改的参数在matlab注释中均有说明。

5. 修改x，y值，重复步骤2，保证正转和反转α和L值不超过0.1和0.2（这个值目前还没有明确的标准）；否则，重复步骤2~3

6. 检验正转和反转时的实验效果，否则，重复步骤2~4，同时寻找正反转的阈值差。最多一米偏一毫米。

* 7. 待优化：
      1.（3）
      2. 圆弧解算的程序，将现在的转180度更改为转任意角度。(matlab迭代有难度)

问题：
陀螺仪不可能输出完美的180，假设每次转动180，但输出只有179，这个系统误差应该怎么办？
理论上讲，是修改x和y，这大概就是圆弧更新存在的意义。另一个方面，两边圆弧更新的x和y不一定一样，所以圆弧很难收敛。设想，我们有四个参数。
圆弧更新精度应该可以提上去。但是实际转角找不到，暂时用差值代替。毕竟陀螺仪是累计误差，问题不大？？？

#### 标准
0. 第一次做实验前先验证陀螺仪，保证正转和反转的精度

1. 禁止回程，但允许如下情况：179.99记一次，180.01记录一次。角度相差0.02只会影响到参数小数点后第三位，回程影响到第一位；
2. 每次实验AngleDegree必须在179.99到180.01之间，推远端以控制。对于全为正转或反转的数据集，解算出的α（注意弧度转角度）角度差值和L差值相对于中值的误差分别小于0.1%和0.05%
3. 每次实验做5组，方便取中位数。将各组数据平均数（因为各数据可信）放入matlab，对输出结果取均值。
4. 实验校核 正转反转，x，y相对误差必须小于0.1%左右
      验证时角度AngleDeg必须在179.99和180.01之间，正反各做两次，以防偶然情况。均值、正转、反转各做一组。                  
5. 如果未达到精度，且重复上述流程一次之后，仍然无法满足。考虑（1）提高角度精度到179.995~180.005.误差限二分法。
***


www 之前一直没意识到圆弧更新是我推导的所以就要对它负责任，真正使用的时候才知道自己错了那么多，忽略了那么多。

* ### 1.4 实验效果检验
***
1. ''如果不使用圆弧更新，想一想阈值设置方法''（输出直线运动时，angle_delta的最大值）
   
2. 推导轨运动，判断偏移量。采集四个方向的数据，丢进err_grad.m里。

3. 推导轨运动，再看偏移量。记录下最后一次优化后各个方向的误差，寻找是否有让梯度下降全局收敛的方法。
***

* ### 1.4 整理
***
1. 记录实验时间，实验流程以及代码修改人，保存好代码备份。
   
2. 收好器件。保护好导轨。
***