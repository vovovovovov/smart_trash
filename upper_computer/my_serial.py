from utils import Serial
import struct

ser = Serial('COM12', 115200, timeout=1)
def u8array_to_float(u8_array_0, u8_array_1, u8_array_2, u8_array_3):
    """
    将C函数float2u8Arry生成的字节数组还原为浮点数
    :param u8_array: 包含4个uint8整数的列表（值范围0-255）
    :return: 还原后的浮点数
    """
    # 反转字节顺序（因C函数中恒执行字节反转）
    reverted_bytes = bytes([u8_array_0, u8_array_1, u8_array_2, u8_array_3])
    
    # 将字节序列解析为浮点数（使用大端序格式）
    return struct.unpack('>f', reverted_bytes)[0]

# 构建发送帧
def build_packet(oled, motor_send_data):
    SOF = b'\xa5'
    packet = SOF
    for i in range (4):
        packet += b'\x00'

    # oled屏幕是否显示，1显示，0关闭
    tmp1 = oled
    packet += tmp1

    # 识别的垃圾种类，0无，1-4对应四种垃圾
    # tmp1 = b'\x04'
    tmp1 = motor_send_data
    packet += tmp1

    for i in range(56):
        packet += b'\x00'

    EOF = b'\xff'
    packet += EOF

    return packet

def send_packet(ser, packet):
    # 发送数据
    ser.write(packet)
    print(f"Packet sent: {packet}")


# if __name__ ==  "__main__":
#     pass
    # ser = Serial('COM11', 115200, timeout=1)  # 裁判系统规定的频率
    # buffer = b''  # 初始化缓冲区

    # import time
    # # 发送示例
    # while True:
    #     packet_data =  build_packet()
    #     send_packet(packet_data)
        

# 接收示例
# while True:
#     # 从串口读取数据
#     received_data = ser.read_all()  # 读取一秒内收到的所有串口数据
#     buffer += received_data

#     # 查找帧头（SOF）的位置
#     sof_index = buffer.find(b'\xa5')

#     while sof_index != -1:

#         print("我找到了")
#         packet_data = buffer[sof_index:]
#         next_sof_index = packet_data.find(b'\xa5', 1)
#         if next_sof_index != -1:
#             # 如果找到下一个帧头，说明当前帧头到下一个帧头之间是一个完整的数据包
#             packet_data = packet_data[:next_sof_index]
#             # print(packet_data)
#         else:
#             # 如果没找到下一个帧头，说明当前帧头到末尾不是一个完整的数据包
#             break
#         distance = u8array_to_float(packet_data[5], packet_data[6], packet_data[7], packet_data[8])
#         print(distance)

#         # 湿度
#         humi_int = packet_data[9]
#         # 温度
#         temp_int = packet_data[10]

#     time.sleep(2)
    
