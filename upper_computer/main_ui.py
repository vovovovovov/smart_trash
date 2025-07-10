import streamlit as st
from PIL import Image
import pandas as pd
import numpy as np
import time

from enum import Enum
class Rubbish(Enum):
    idle = 0
    recyclable = 1
    hazardous = 2
    food_waste = 3
    residual_waste = 4

rubbish_flag = Rubbish.food_waste

idle_pic_path = r'./data/pic/1.png'
recyclable_pic_path = r'./data/pic/recyclable.png'
hazardous_pic_path = r'./data/pic/hazardous.png'
food_waste_pic_path = r'./data/pic/food_waste.png'
residual_waste = r'./data/pic/residual_waste.png'

print("update")


st.title("📈 智慧垃圾桶实时数据监控系统")

sidebar = st.sidebar.title('导航')



st.set_page_config(layout="wide")
for row in range(2):
    cols = st.columns(4)
    for col_index, col in enumerate(cols):
        with col:
            with st.container(border=True, height=400):
                # 根据行列索引分配不同组件
                if row == 0 and col_index == 0:
                    st.header(f"垃圾满载情况")
                    chart_data = pd.DataFrame(np.random.randn(20, 1), columns=["数据"])
                    st.area_chart(chart_data)
                elif row == 0 and col_index == 1:
                    st.header(f"温度检测")
                    chart_data = pd.DataFrame(np.random.randn(20, 1), columns=["数据"])
                    st.area_chart(chart_data)
                elif row == 0 and col_index == 2:
                    st.header(f"湿度检测")
                    chart_data = pd.DataFrame(np.random.randn(20, 1), columns=["数据"])
                    st.area_chart(chart_data)
                elif row == 0 and col_index == 3:
                    st.header(f"实时图像显示")
                    chart_data = pd.DataFrame(np.random.randn(20, 1), columns=["数据"])
                    st.area_chart(chart_data)

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
                    st.header(f"AI垃圾分类科普助手")

                    # 初始化会话状态
                    if 'chat_history' not in st.session_state:
                        st.session_state.chat_history = [
                            {"role": "ai", "content": "您好！我是垃圾分类科普助手，请问有什么可以帮助您的？"}
                        ]
                    
                    # 创建分类图标展示区
                    st.markdown('<div class="category-showcase">', unsafe_allow_html=True)
                    col1, col2, col3, col4 = st.columns(4)
                    with col1:
                        st.image(Image.open(recyclable_pic_path), width=40)
                        st.caption("可回收")
                    with col2:
                        st.image(Image.open(hazardous_pic_path), width=40)
                        st.caption("有害")
                    with col3:
                        st.image(Image.open(food_waste_pic_path), width=40)
                        st.caption("厨余")
                    with col4:
                        st.image(Image.open(residual_waste), width=40)
                        st.caption("其他")
                    st.markdown('</div>', unsafe_allow_html=True)
                    
                    # 显示对话历史
                    with st.container(height=200, border=False):
                        for message in st.session_state.chat_history:
                            if message["role"] == "user":
                                st.markdown(f"""
                                <div style="text-align: right; margin-bottom: 10px;">
                                    <div style="background-color: #e3f2fd; border-radius: 10px; padding: 8px 12px; display: inline-block;">
                                        {message["content"]}
                                    </div>
                                </div>
                                """, unsafe_allow_html=True)
                            else:
                                st.markdown(f"""
                                <div style="text-align: left; margin-bottom: 10px;">
                                    <div style="background-color: #f5f5f5; border-radius: 10px; padding: 8px 12px; display: inline-block;">
                                        {message["content"]}
                                    </div>
                                </div>
                                """, unsafe_allow_html=True)
                    
                    # 用户输入区域
                    if user_input := st.chat_input("输入问题...", key="chat_input"):
                        st.session_state.chat_history.append({"role": "user", "content": user_input})

                    uploaded_file = 0
                    # 处理用户输入
                    if user_input or uploaded_file:
                        # 添加用户消息
                        if uploaded_file:
                            user_content = "【图片识别】"
                            image = Image.open(uploaded_file)
                            st.session_state.uploaded_image = image  # 保存图片用于显示
                        else:
                            user_content = user_input
                        
                        st.session_state.chat_history.append({"role": "user", "content": user_content})
                        
                        # 生成AI回复 [1,4](@ref)
                        with st.spinner("识别中..."):
                            time.sleep(1)  # 模拟处理时间
                            
                            # 垃圾分类知识库 [1,2,3](@ref)
                            GARBAGE_KNOWLEDGE = {
                                "可回收物": ["塑料瓶", "纸张", "玻璃瓶", "金属罐", "衣服", "电子产品"],
                                "有害垃圾": ["电池", "灯泡", "过期药品", "化妆品", "化学品", "温度计"],
                                "厨余垃圾": ["果皮", "剩饭菜", "茶叶渣", "花卉植物", "动物饲料"],
                                "其他垃圾": ["烟蒂", "用过的纸巾", "尿不湿", "陶瓷碎片", "一次性餐具"]
                            }
                            
                            if uploaded_file:
                                # 模拟图像识别结果 [4](@ref)
                                detected_items = ["塑料瓶", "纸盒"]  # 实际中应使用AI模型
                                ai_response = "识别结果：\n"
                                for item in detected_items:
                                    for category, items in GARBAGE_KNOWLEDGE.items():
                                        if item in items:
                                            ai_response += f"- {item} → 【{category}】\n"
                                ai_response += "\n请将物品放入正确的分类垃圾桶！"
                            else:
                                # 文本查询处理
                                found = False
                                for category, items in GARBAGE_KNOWLEDGE.items():
                                    if user_input in items:
                                        ai_response = f"〖{user_input}〗属于【{category}】\n"
                                        ai_response += "📝 处理建议："
                                        if category == "可回收物":
                                            ai_response += "请清洁后投入蓝色回收箱"
                                        elif category == "有害垃圾":
                                            ai_response += "请投入红色专用回收箱"
                                        elif category == "厨余垃圾":
                                            ai_response += "请去除包装后投入绿色厨余桶"
                                        else:
                                            ai_response += "请投入灰色其他垃圾桶"
                                        found = True
                                        break
                                
                                if not found:
                                    ai_response = f"未找到'{user_input}'的分类信息\n"
                                    ai_response += "您可以尝试：\n"
                                    ai_response += "1. 使用更常见的名称查询\n"
                                    ai_response += "2. 拍照识别物品\n"
                                    ai_response += "3. 描述物品特征（如：'圆柱形金属容器'）"
                            
                            st.session_state.chat_history.append({"role": "ai", "content": ai_response})
                        
                        # 清空输入框并重新运行
                        st.session_state.user_input = ""
                        st.rerun()
                    
                    # 显示上传的图片（如果有）
                    if 'uploaded_image' in st.session_state:
                        st.image(st.session_state.uploaded_image, caption="识别物品", width=150)



                elif row == 1 and col_index == 3:
                    st.header(f"识别垃圾类别")
                    rubbish_classify = rubbish_flag
                    # print(rubbish_classify)

                    if rubbish_classify == Rubbish.idle:
                        image = Image.open(idle_pic_path)
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
                        image = Image.open(recyclable_pic_path)
                        caption = '其他垃圾'
                    # st.image(image, caption=caption, width = 500)  
                    st.image(image, caption=caption, use_container_width=True)
                else:
                    st.header(f"控制面板 {col_index+1}")
                    if st.button("点击生成", key=f"btn_{row}_{col_index}"):
                        st.success(f"行{row+1}列{col_index+1}操作成功")

