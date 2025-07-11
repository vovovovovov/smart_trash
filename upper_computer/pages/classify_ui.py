import cv2
import streamlit as st
import time
from ultralytics import YOLO

# 页面设置
st.set_page_config(
    page_title="YOLOv8 实时目标检测",
    layout="wide",
    page_icon="🔍"
)

# 初始化YOLO模型（使用缓存避免重复加载）
@st.cache_resource
def load_model():
    return YOLO(r'D:\BaiduNetdiskDownload\product_class\product_class\upper_computer\pages\yolov8s-world.pt')

model = load_model()

# 侧边栏控制面板
with st.sidebar:
    st.header("检测控制")
    confidence_threshold = st.slider('置信度阈值', 0.0, 1.0, 0.25, 0.01)
    detect_button = st.button("开始实时检测")
    stop_button = st.button("停止检测")
    
    # 检测统计信息
    st.divider()
    stats_placeholder = st.empty()

# 初始化视频捕获
if 'cap' not in st.session_state:
    st.session_state.cap = None
    st.session_state.detection_active = False
    st.session_state.last_timer = time.time()
    st.session_state.detected_objects = {}

# 主界面布局
col1, col2 = st.columns([0.7, 0.3])

with col1:
    st.header("实时视频流")
    video_placeholder = st.empty()
    
    if detect_button and not st.session_state.detection_active:
        st.session_state.cap = cv2.VideoCapture(0)
        st.session_state.detection_active = True
        st.session_state.last_timer = time.time()
        st.session_state.detected_objects = {}  # 重置检测结果
        st.rerun()

with col2:
    st.header("检测结果")
    results_placeholder = st.empty()
    
    st.subheader("实时物体分类")
    class_cols = st.columns(2)

# 主检测循环
if st.session_state.detection_active and st.session_state.cap is not None:
    cap = st.session_state.cap
    
    # 重置检测结果容器
    st.session_state.detected_objects = {}
    
    while st.session_state.detection_active and not stop_button:
        ret, frame = cap.read()
        if not ret:
            st.warning("无法获取视频帧，请检查摄像头")
            break
        
        # 转换颜色空间
        rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        video_placeholder.image(rgb_frame, caption='实时视频流', use_container_width=True)
        
        # 实时检测逻辑
        current_time = time.time()
        if (current_time - st.session_state.last_timer) > 0.3:  # 提高检测频率至0.3秒
            # 执行YOLO推理
            results = model.predict(frame, conf=confidence_threshold)
            
            # 清空前次结果
            st.session_state.detected_objects = {}
            annotated_frame = frame.copy()
            
            for result in results:
                annotated_frame = result.plot(img=annotated_frame)
                
                # 仅记录当前帧结果
                for box in result.boxes:
                    cls_id = int(box.cls.item())
                    cls_name = result.names[cls_id]
                    st.session_state.detected_objects[cls_name] = st.session_state.detected_objects.get(cls_name, 0) + 1
            
            # 更新检测时间戳
            st.session_state.last_timer = current_time
            
            # 显示带标注的当前帧
            results_placeholder.image(
                cv2.cvtColor(annotated_frame, cv2.COLOR_BGR2RGB),
                caption=f'检测时间: {time.strftime("%H:%M:%S")}',
                use_container_width=True
            )
        
        # 实时更新分类结果
        with class_cols[0]:
            st.markdown("**当前检测物体:**")
            if st.session_state.detected_objects:
                for obj in list(st.session_state.detected_objects.keys())[:5]:  # 仅显示最新5个
                    st.markdown(f"- {obj}")
            else:
                st.info("检测中...")
                
        with class_cols[1]:
            st.markdown("**实时数量:**")
            if st.session_state.detected_objects:
                for obj, count in list(st.session_state.detected_objects.items())[:5]:
                    st.markdown(f"- {obj}: {count}个")
    
    # 资源释放
    if stop_button:
        st.session_state.detection_active = False
        if st.session_state.cap:
            st.session_state.cap.release()
        st.rerun()
else:
    st.info("点击「开始实时检测」启动摄像头")