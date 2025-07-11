import streamlit as st
from openai import OpenAI

# 页面配置
st.set_page_config(
    page_title="垃圾分类测试助手",
    page_icon="🗑️",
    layout="centered"
)

# 初始化OpenAI客户端
def get_client():
    return OpenAI(
        api_key='TODO',  # 推荐使用st.secrets管理密钥
        base_url="https://dashscope.aliyuncs.com/compatible-mode/v1",
    )

# 预设回答规则
PRESET_RULES = {
    # 有害垃圾
    "电池": "🔋 电池属于有害垃圾！请放入红色垃圾桶，避免污染环境",
    "荧光灯管": "💡 荧光灯管含有汞，属于有害垃圾！",
    "药品": "💊 过期药品属于有害垃圾！请勿随意丢弃",
    # 可回收物
    "塑料瓶": "🧴 塑料瓶属于可回收物！请清空并压扁后放入蓝色垃圾桶",
    "纸张": "📄 废纸属于可回收物！请避免污染后放入蓝色垃圾桶",
    "玻璃": "🔮 玻璃制品属于可回收物！小心处理避免割伤",
    "金属": "🪙 金属制品属于可回收物！可再循环利用",
    # 厨余垃圾
    "果皮": "🍌 果皮属于厨余垃圾！请放入绿色垃圾桶",
    "剩饭": "🍚 剩饭剩菜属于厨余垃圾！可用于堆肥",
    "茶叶": "🍵 茶叶渣属于厨余垃圾！是天然肥料",
    # 其他垃圾
    "纸巾": "🧻 纸巾属于其他垃圾！请放入灰色垃圾桶",
    "尿不湿": "👶 尿不湿属于其他垃圾！卫生处理后丢弃",
    "陶瓷": "🏺 陶瓷碎片属于其他垃圾！请包裹后丢弃",
    # 特殊处理项
    "奶茶杯": "🧋 奶茶杯特殊处理：\n1. 倒掉液体→厨余垃圾\n2. 珍珠等配料→厨余垃圾\n3. 杯身、杯盖→其他垃圾\n4. 吸管→其他垃圾",
    "电池包装": "📦 电池包装处理：\n- 塑料外包装→可回收\n- 防伪标签→其他垃圾\n- 电池本身→有害垃圾",
}

# AI模型交互函数
def ai_classify(prompt):
    """使用大模型处理未覆盖的垃圾分类查询"""
    client = get_client()
    
    # 精心设计的系统提示词
    system_prompt = """
    你是一名专业的垃圾分类助手AI学海行，请严格按以下规则回答：
    1. 只能回答与垃圾分类相关的问题
    2. 回答需包含分类结果和简要解释
    3. 使用垃圾分类专业术语：可回收物、有害垃圾、厨余垃圾、其他垃圾
    4. 回答简洁明了，不超过50字
    5. 格式范例：【分类结果】厨余垃圾 → 果皮可自然降解
    """
    
    response = client.chat.completions.create(
        model="qwen-plus",
        messages=[
            {'role': 'system', 'content': system_prompt},
            {'role': 'user', 'content': prompt}
        ],
        temperature=0.1  # 降低随机性
    )
    return response.choices[0].message.content

# 处理用户查询的统一函数
def classify_waste(prompt):
    """处理垃圾分类查询：优先使用预设规则，未覆盖时调用AI"""
    # 检查预设规则
    for item, response in PRESET_RULES.items():
        if item in prompt:
            return response
    
    # 调用AI处理未覆盖的查询
    return ai_classify(prompt)

# 初始化对话
if "messages" not in st.session_state:
    st.session_state.messages = [
        {"role": "assistant", "content": "🔍 垃圾分类测试助手已启动！请提问：\n例：电池是什么垃圾？奶茶杯如何处理？"}
    ]

# 主界面
st.title("♻️ 垃圾分类测试助手")
st.caption("AI学海行 | 专业垃圾分类指导")

# 显示历史对话
for msg in st.session_state.messages:
    with st.chat_message(msg["role"]):
        st.markdown(msg["content"])

# 用户输入处理
if prompt := st.chat_input("输入垃圾名称或处理问题..."):
    # 添加用户消息
    st.session_state.messages.append({"role": "user", "content": prompt})
    
    try:
        # 处理用户查询
        response = classify_waste(prompt)
        
        # 添加AI回复
        st.session_state.messages.append({"role": "assistant", "content": response})
    except Exception as e:
        # API错误处理
        error_msg = f"⚠️ 服务暂时不可用，请稍后再试\n（错误代码：{str(e)}）"
        st.session_state.messages.append({"role": "assistant", "content": error_msg})
    
    # 刷新界面
    st.rerun()

# 侧边栏说明
st.sidebar.title("🗂️ 分类说明")
st.sidebar.subheader("🟥 有害垃圾")
st.sidebar.caption("含化学品/重金属物品 如电池、药品")
st.sidebar.divider()

st.sidebar.subheader("🟦 可回收物")
st.sidebar.caption("清洁可再利用物品 如塑料、纸张")
st.sidebar.divider()

st.sidebar.subheader("🟩 厨余垃圾")
st.sidebar.caption("易腐烂食品类废物 如果皮、剩菜")
st.sidebar.divider()

st.sidebar.subheader("⬜ 其他垃圾")
st.sidebar.caption("除前三类外的垃圾 如纸巾、一次性用品")
st.sidebar.divider()

st.sidebar.caption("功能说明:\n- 优先匹配预设规则\n- 未覆盖时调用AI分析\n- 处理复杂垃圾分类问题")