import streamlit as st
import time
import random
from PIL import Image, ImageDraw, ImageFont

# 垃圾分类知识库
GARBAGE_KNOWLEDGE = {
    "可回收物": ["纸张", "塑料瓶", "玻璃瓶", "金属罐", "衣服", "电子产品"],
    "厨余垃圾": ["剩饭菜", "果皮果核", "茶叶渣", "花卉植物", "动物饲料"],
    "有害垃圾": ["电池", "灯泡", "过期药品", "化妆品", "化学品", "温度计"],
    "其他垃圾": ["烟蒂", "用过的纸巾", "尿不湿", "陶瓷碎片", "一次性餐具"]
}

# 生成AI回复
def generate_ai_response(user_message):
    """根据用户输入生成AI回复"""
    # 简单关键词匹配
    user_message = user_message.lower()
    
    # 欢迎语
    if "帮助" in user_message or "介绍" in user_message or "功能" in user_message:
        return "您好！我是垃圾分类科普助手，可以解答以下问题：\n1. 某物品属于哪类垃圾？\n2. 垃圾分类小知识\n3. 当地回收点查询\n请告诉我您需要什么帮助?"
    
    # 具体物品分类
    for category, items in GARBAGE_KNOWLEDGE.items():
        for item in items:
            if item.lower() in user_message:
                return f"〖{item}〗属于【{category}】\n📝 分类理由：{generate_classification_reason(item, category)}"
    
    # 分类知识
    if "分类" in user_message or "知识" in user_message:
        return "【垃圾分类小知识】\n" + "\n".join(
            f"● {category}: {', '.join(items[:3])}..." 
            for category, items in GARBAGE_KNOWLEDGE.items()
        ) + "\n\n您想了解哪类垃圾的详细列表？"
    
    # 默认回复
    default_responses = [
        "抱歉，我不太明白您的问题。能否再详细描述一下？",
        "目前我主要擅长解答垃圾分类相关问题，您可以问我物品分类、垃圾分类知识等内容。",
        "您是想问某样物品属于哪类垃圾吗？请告诉我物品名称。"
    ]
    return random.choice(default_responses)

# 生成分类理由
def generate_classification_reason(item, category):
    """生成垃圾分类的理由解释"""
    reasons = {
        "可回收物": "可回收物指的是适宜回收利用和资源化利用的生活废弃物，如{}可以通过回收再利用减少资源浪费。",
        "厨余垃圾": "厨余垃圾是指易腐的生物质生活废弃物，如{}可以通过堆肥处理变成有机肥料。",
        "有害垃圾": "有害垃圾是指对人体健康或自然环境造成直接或潜在危害的生活废弃物，如{}需要特殊安全处理。",
        "其他垃圾": "其他垃圾是指除可回收物、有害垃圾、厨余垃圾以外的混杂、难以分类的生活垃圾，如{}需通过卫生填埋等方式处理。"
    }
    return reasons[category].format(item)

# 创建分类图标
def create_category_icon(category_name):
    """创建垃圾分类类别的视觉图标"""
    width, height = 120, 120
    img = Image.new('RGB', (width, height), color=(255, 255, 255))
    draw = ImageDraw.Draw(img)
    
    # 绘制图标
    colors = {
        "可回收物": (0, 123, 255),
        "厨余垃圾": (103, 194, 58),
        "有害垃圾": (255, 92, 92),
        "其他垃圾": (173, 181, 189)
    }
    
    # 圆形背景
    draw.ellipse([(10, 10), (110, 110)], fill=colors[category_name])
    
    # 绘制符号
    symbols = {
        "可回收物": "♻",
        "厨余垃圾": "🥦",
        "有害垃圾": "⚠",
        "其他垃圾": "🗑"
    }
    
    font = ImageFont.truetype("arial.ttf", 40) if category_name != "可回收物" else ImageFont.truetype("arial.ttf", 60)
    draw.text((60, 60), symbols[category_name], fill=(255, 255, 255), anchor="mm", font=font)
    
    return img

# 初始化会话
if "messages" not in st.session_state:
    st.session_state.messages = [
        {"role": "ai", "content": "您好！我是垃圾分类科普助手，请问有什么可以帮助您的？"}
    ]

# 显示标题 - 精确匹配图片中的文字和风格
st.markdown('<div class="header-title">AI垃圾分类科普助手</div>', unsafe_allow_html=True)

# 分类图标展示
st.markdown('<div class="category-showcase">', unsafe_allow_html=True)
for category in ["可回收物", "厨余垃圾", "有害垃圾", "其他垃圾"]:
    icon = create_category_icon(category)
    col1, col2, col3 = st.columns([1, 1, 1])
    with col2:
        st.image(icon, width=60)
        st.caption(f"{category}")
st.markdown('</div>', unsafe_allow_html=True)

# 显示对话历史
st.markdown('<div class="chat-container">', unsafe_allow_html=True)
for message in st.session_state.messages:
    css_class = "user-message" if message["role"] == "user" else "ai-message"
    st.markdown(f'<div class="{css_class}">{message["content"]}</div>', unsafe_allow_html=True)
st.markdown('</div>', unsafe_allow_html=True)

# 用户输入
input_container = st.container()
with st.container():
    st.markdown('<div class="input-area">', unsafe_allow_html=True)
    user_input = st.text_input("", key="user_input", placeholder="请输入您的垃圾分类问题...", label_visibility="collapsed")
    st.markdown('</div>', unsafe_allow_html=True)

# 处理用户输入
if user_input:
    # 添加用户消息
    st.session_state.messages.append({"role": "user", "content": user_input})
    
    # 生成AI回复
    ai_response = generate_ai_response(user_input)
    
    # 模拟AI思考时间
    with st.spinner("思考中..."):
        time.sleep(0.5)
        st.session_state.messages.append({"role": "ai", "content": ai_response})
    
    # 清空输入框并重新运行
    st.session_state.user_input = ""
    st.rerun()