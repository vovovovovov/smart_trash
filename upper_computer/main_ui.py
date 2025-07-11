import streamlit as st
from PIL import Image
import pandas as pd
import numpy as np
import time
import cv2
from ultralytics import YOLOWorld
from utils import Serial
import struct
from my_serial import *
import random

from enum import Enum
class Rubbish(Enum):
    idle = 0
    recyclable = 1
    hazardous = 2
    food_waste = 3
    residual_waste = 4

rubbish_flag = Rubbish.recyclable

idle_pic_path_list = [r'./data/pic/1.png', r'./data/pic/2.png', r'./data/pic/3.png']
idle_pic_path = r'./data/pic/1.png'

recyclable_pic_path = r'./data/pic/recyclable.png'
hazardous_pic_path = r'./data/pic/hazardous.png'
food_waste_pic_path = r'./data/pic/food_waste.png'
residual_waste = r'./data/pic/residual_waste.png'


st.title("📈 智慧垃圾桶实时数据监控系统")

sidebar = st.sidebar.title('导航')


with sidebar:
    st.text("导航栏")
    st.button('刷新数据')

    rubbish_class = st.selectbox(
    label = '输入垃圾类别',
    options = ('无垃圾', '有害垃圾', '可回收垃圾', '其他垃圾', '厨余垃圾'),
    index = 0,
    format_func = str,
    help = '选择一个垃圾类别吧'
    )

    print(rubbish_class)


st.set_page_config(layout="wide")

# 初始化多个变量（仅在会话首次运行时执行）
if 'rubbish_distance' not in st.session_state:
    st.session_state.rubbish_distance = [0,0,0,0,0,0,0,0,0,0]
if 'temperature_data' not in st.session_state: 
    st.session_state.temperature_data = [0,0,0,0,0,0,0,0,0,0]
if 'humidity_data' not in st.session_state:
    st.session_state.humidity_data = [0,0,0,0,0,0,0,0,0,0]
if 'rubbish_flag' not in st.session_state:
    st.session_state.rubbish_flag = Rubbish.idle


# 通用队列
def enqueue(origin_data, value, max_length=10):
    queue = origin_data.copy()
    queue.append(value)
    if len(queue) > max_length:
        queue.pop(0)
    origin_data = queue
    return origin_data

#串口数据更新(接收)
received_data = ser.read_all()  # 读取一秒内收到的所有串口数据
buffer = b''  # 初始化缓冲区
buffer += received_data
sof_index = buffer.find(b'\xa5')
serial_distance = None
serial_humi_int = None
serial_temp_int = None
if sof_index != -1:
    packet_data = buffer[sof_index:]
    next_sof_index = packet_data.find(b'\xa5', 1)
    if next_sof_index != -1:
        packet_data = packet_data[:next_sof_index]
    else:
        pass
    print(packet_data)
    # print(len(packet_data))
    try:
        # 距离
        serial_distance = u8array_to_float(packet_data[5], packet_data[6], packet_data[7], packet_data[8])
        # 湿度
        serial_humi_int = packet_data[9]
        # 温度
        serial_temp_int = packet_data[10]
    except:
        pass


for row in range(2):
    cols = st.columns(3)
    for col_index, col in enumerate(cols):
        with col:
            with st.container(border=True, height=400):
                

                # 根据行列索引分配不同组件
                if row == 0 and col_index == 0:
                    st.header(f"垃圾满载情况")
                    if serial_distance != None: # 更新数据
                        value = serial_distance
                        st.session_state.rubbish_distance = enqueue(st.session_state.rubbish_distance,value)
                    chart_data = pd.DataFrame(st.session_state.rubbish_distance, columns=["数据"])
                    st.area_chart(chart_data)

                elif row == 0 and col_index == 1:
                    st.header(f"温度检测")
                    if serial_temp_int != None:
                        value = serial_temp_int
                        st.session_state.temperature_data = enqueue(st.session_state.temperature_data,value)
                    chart_data = pd.DataFrame(st.session_state.temperature_data, columns=["数据"])
                    st.area_chart(chart_data)

                elif row == 0 and col_index == 2:  # humidity_data
                    st.header(f"湿度检测")
                    if serial_humi_int != None:
                        value = serial_humi_int
                        st.session_state.humidity_data = enqueue(st.session_state.humidity_data,value)
                    chart_data = pd.DataFrame(st.session_state.humidity_data, columns=["数据"])
                    st.area_chart(chart_data)


                # oled显示
                elif row == 1 and col_index == 0:
                    st.header(f"OLED显示屏状态")
                    st.markdown("### 📊 实时状态概览")
                    st.components.v1.html("""
                        <div id="realtime-clock" style="font-size:24px; font-weight:bold; text-align:center;"></div>
                        <script>
                            function updateTime() {
                                const now = new Date();
                                const timeStr = now.toLocaleTimeString('zh-CN', {hour12: false});
                                document.getElementById("realtime-clock").innerHTML = timeStr;
                            }
                            setInterval(updateTime, 1000);  // 每秒更新
                            updateTime();  // 立即显示
                        </script>
                        """, height=50)
                    st.success("状态正常")
                    oled_switch = st.checkbox('关闭',value=False)
                    if oled_switch:
                        st.write('已关闭')
                    else:
                        st.write('已开启')

                elif row == 1 and col_index == 1:
                    st.header(f"风扇/舵机状态")
                    st.success("🌀风扇状态正常")
                    st.success("⚙️舵机状态正常")

                elif row == 1 and col_index == 2:
                    st.header(f"识别垃圾类别")
                    rubbish_classify = Rubbish.idle

                    print(rubbish_class)
                    print(type(rubbish_class))
                    if rubbish_class == "无垃圾":
                        rubbish_classify = Rubbish.idle
                    elif rubbish_class == "可回收垃圾":
                        rubbish_classify = Rubbish.recyclable
                    elif rubbish_class == "有害垃圾":
                        rubbish_classify = Rubbish.hazardous
                    elif rubbish_class == "厨余垃圾":
                        rubbish_classify = Rubbish.food_waste
                    elif rubbish_class == "其他垃圾":
                        rubbish_classify = Rubbish.residual_waste

                    print(rubbish_classify)
                    if rubbish_classify == Rubbish.idle:
                        index_tmp =  random.randint(0, 2)
                        image = Image.open(idle_pic_path_list[index_tmp])
                        caption = '无垃圾'
                    elif rubbish_classify == Rubbish.recyclable:
                        image = Image.open(recyclable_pic_path)
                        caption = '可回收垃圾'
                    elif rubbish_classify == Rubbish.hazardous:
                        image = Image.open(hazardous_pic_path)
                        caption = '有害垃圾'
                    elif rubbish_classify == Rubbish.food_waste:
                        image = Image.open(food_waste_pic_path)
                        caption = '厨余垃圾'
                    elif rubbish_classify == Rubbish.residual_waste:
                        image = Image.open(residual_waste)
                        caption = '其他垃圾'
                    st.image(image, caption=caption, use_container_width=True)

                else:
                    st.header(f"控制面板 {col_index+1}")
                    if st.button("点击生成", key=f"btn_{row}_{col_index}"):
                        st.success(f"行{row+1}列{col_index+1}操作成功")

# 串口数据更新（发送）
buffer = b''  # 初始化缓冲区
if oled_switch:
    oled_send_data = b'\x00'
else:
    oled_send_data = b'\x01'

if rubbish_class == "无垃圾":
    motor_send_data = b'\x00'
elif rubbish_class == "可回收垃圾":
    motor_send_data = b'\x01'
elif rubbish_class == "有害垃圾":
    motor_send_data = b'\x02'
elif rubbish_class == "厨余垃圾":
    motor_send_data = b'\x03'
elif rubbish_class == "其他垃圾":
    motor_send_data = b'\x04'
packet_data_send =  build_packet(oled_send_data, motor_send_data)
send_packet(ser,packet_data_send)

