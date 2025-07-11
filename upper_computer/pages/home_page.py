import streamlit as st

st.set_page_config(
    page_title="智能垃圾桶管理平台",
    page_icon="🗑️",
    layout="wide"
)

st.title("🚮 智慧垃圾桶管理系统")
st.header("基于物联网与人工智能的垃圾分类解决方案", divider="rainbow")

# 核心功能展示
st.subheader("✨ 核心功能")
col1, col2, col3 = st.columns(3)
with col1:
    st.markdown("**🤖 AI识别分类**")  # 修改1：更新图标和标题
    st.caption("通过图像识别技术自动开启对应分类桶盖")  # 修改2：更新描述文本
with col2:
    st.markdown("**📊 智能满溢监测**")
    st.caption("实时监测垃圾容量，自动通知清运人员处理")
with col3:
    st.markdown("**🌱 环保积分系统**")
    st.caption("正确投放可获积分奖励，兑换环保礼品")

# 技术架构图（模拟）
st.subheader("🔧 技术架构")
st.image("https://via.placeholder.com/800x300/2F4F4F/FFFFFF?text=物联网+传感器+AI分析平台", 
         use_container_width =True)

# 应用场景
with st.expander("🏙️ 多场景应用案例", expanded=True):
    scenario_col1, scenario_col2, scenario_col3 = st.columns(3)
    with scenario_col1:
        st.markdown("**🏘️ 社区应用**")
        st.markdown("- xxx社区实现95%分类准确率")
        st.markdown("- 垃圾清运效率提升40%")
    with scenario_col2:
        st.markdown("**🏫 校园场景**")
        st.markdown("- 教育版垃圾桶带垃圾分类教学功能")
        st.markdown("- 学生参与率提升300%")
    with scenario_col3:
        st.markdown("**🏢 商业综合体**")
        st.markdown("- 自动压缩技术减少60%清运频次")
        st.markdown("- 异味控制改善购物环境")

# 数据看板
st.subheader("📈 环保成效数据")
metric1, metric2, metric3 = st.columns(3)
metric1.metric("垃圾分类准确率", "95%", "25%↑")
metric2.metric("垃圾回收利用率", "35%", "15%↑")
metric3.metric("有害垃圾减少量", "30%", "行业领先")

# 附加资源
st.divider()
st.subheader("📚 了解更多")
st.markdown("""
- [天津滨海科学城智能垃圾桶案例研究](https://www.tjbh.gov.cn/sitefiles/services/cms/page.aspx?s=11813&n=12163&c=591360)
- [智能垃圾桶商业计划书模板](https://max.book118.com/html/2025/0220/6144231155011043.shtm)
- [物联网垃圾桶技术白皮书](http://www.bbpei.com/shoplhsyh/news/1290383.html)
""")

# 演示入口
st.sidebar.success("⬆️ 选择功能演示")
st.sidebar.divider()
st.sidebar.markdown("### 系统演示模块")
st.sidebar.markdown("1. **实时监控平台**  \n查看垃圾桶状态地图")
st.sidebar.markdown("2. **数据分析中心**  \n垃圾分类成效统计")
st.sidebar.markdown("3. **设备管理系统**  \n远程配置与维护")

st.sidebar.divider()
st.sidebar.caption("v1.0 | © 2025 连理智造")